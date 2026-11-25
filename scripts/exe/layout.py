"""How the linked exe is laid out next to the target: where the functions the database names landed.

A byte-identical exe needs every function at the target's address. That cannot be asked of one
function at a time (a function of the wrong length moves all that follow it), so it is measured
over the whole link and may only get better:

  same address     functions at the target's address
  same distance    functions whose distance to the next named function is the target's (a stretch
                   laid out like the target's, wherever it starts)
  in order         the longest run of functions that come in the target's order, not necessarily
                   next to each other

Usage: python scripts/exe/layout.py [--map build/exe/Ragexe.map] [--json out.json] [--gate | --record]
  --gate    exit 1 when a number is lower than in data/layout.json (the last recorded link)
  --record  write the numbers to data/layout.json
Needs .td/symbols.json (td sync) and a map from scripts/exe/build_exe.py. As a gate before a merge:
  python scripts/exe/build_exe.py && python scripts/exe/layout.py --gate
"""

import argparse
import bisect
import json
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
RECORD = ROOT / "data" / "layout.json"
KEYS = (("sameAddress", "at the target's address"), ("sameDistance", "as far from the next as in the target"), ("inOrder", "in the target's order (longest run)"))


def map_symbols(path):
    line = re.compile(r"^\s*[0-9a-f]{4}:[0-9a-f]{8}\s+(\S+)\s+([0-9a-f]{8})\s", re.I)
    out = {}
    for text in path.read_text(encoding="utf-8", errors="replace").splitlines():
        match = line.match(text)
        if match and int(match.group(2), 16):
            out.setdefault(match.group(1), int(match.group(2), 16))
    return out


def target_functions():
    database = json.loads((ROOT / ".td" / "symbols.json").read_text(encoding="utf-8"))
    column = {name: index for index, name in enumerate(database["columns"])}
    return {row[column["decorated"]]: row[column["addr"]] for row in database["symbols"]
            if row[column["kind"]] == "function" and row[column["decorated"]] and row[column["addr"]]}


def longest_increasing(values):
    tails = []
    for value in values:
        at = bisect.bisect_left(tails, value)
        if at == len(tails):
            tails.append(value)
        else:
            tails[at] = value
    return len(tails)


def measure(ours, theirs):
    common = sorted((theirs[name], ours[name]) for name in theirs if name in ours)
    # One name per target address: aliases of a folded function would count twice.
    pairs = [pair for i, pair in enumerate(common) if i == 0 or pair[0] != common[i - 1][0]]
    same_distance = sum(1 for a, b in zip(pairs, pairs[1:]) if b[0] - a[0] == b[1] - a[1])
    return {
        "functions": len(pairs),
        "sameAddress": sum(1 for there, here in pairs if there == here),
        "sameDistance": same_distance,
        "inOrder": longest_increasing([here for _, here in pairs]),
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    parser.add_argument("--map", type=Path, default=ROOT / "build" / "exe" / "Ragexe.map")
    parser.add_argument("--json", type=Path)
    parser.add_argument("--gate", action="store_true", help="fail when a number dropped since data/layout.json")
    parser.add_argument("--record", action="store_true", help="write the numbers to data/layout.json")
    args = parser.parse_args()
    result = measure(map_symbols(args.map), target_functions())
    if args.json:
        args.json.write_text(json.dumps(result, indent=1) + "\n", encoding="utf-8")
    n = max(1, result["functions"])
    print(f"{result['functions']} functions of the database are in the link")
    before = json.loads(RECORD.read_text(encoding="utf-8")) if RECORD.exists() else {}
    dropped = []
    for key, label in KEYS:
        change = result[key] - before[key] if key in before else None
        print(f"  {result[key]:>6}  {100 * result[key] / n:5.1f}%  {label}" + (f"  ({change:+d})" if change else ""))
        if change is not None and change < 0:
            dropped.append(label)
    if args.record:
        RECORD.write_text(json.dumps(result, indent=1) + "\n", encoding="utf-8")
    if args.gate and dropped:
        raise SystemExit("the layout moved away from the target's: fewer functions " + "; ".join(dropped))


if __name__ == "__main__":
    main()
