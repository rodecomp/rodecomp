#!/usr/bin/env python3
"""Offline lookup in the main clone's .td/symbols.json (or a slot's, with --slot).

Usage: sym.py <fn_00123 | name substring | 0xaddr> [--slot wN] [--limit N]
An address matches the symbol that contains it.
"""
import argparse
import json
import os
import sys
from pathlib import Path

CLONE = Path(__file__).resolve().parents[2]
ROOT = CLONE.parent.parent if CLONE.parent.name == "workers" else CLONE.parent
MAIN = Path(os.environ["RODECOMP_MAIN"]) if os.environ.get("RODECOMP_MAIN") else (
    ROOT / "rodecomp" if CLONE.parent.name == "workers" else CLONE)


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("query")
    ap.add_argument("--slot", help="read the slot's snapshot instead of the main clone's")
    ap.add_argument("--limit", type=int, default=20)
    a = ap.parse_args()

    base = ROOT / "workers" / a.slot if a.slot else MAIN
    path = base / ".td" / "symbols.json"
    data = json.loads(path.read_text(encoding="utf-8"))
    cols = data["columns"]
    rows = [dict(zip(cols, r)) for r in data["symbols"]]

    q = a.query
    if q.lower().startswith("0x"):
        addr = int(q, 16)
        hits = [r for r in rows if r["addr"] is not None and r["addr"] <= addr < r["addr"] + max(r["size"] or 1, 1)]
    elif any(r["id"] == q for r in rows):
        hits = [r for r in rows if r["id"] == q]
    else:
        ql = q.lower()
        hits = [r for r in rows if ql in (r["name"] or "").lower()
                or ql in (r["decorated"] or "").lower()]

    for r in hits[: a.limit]:
        print(f'{r["id"]:<10} {r["kind"]:<9} {("0x%08X" % r["addr"]) if r["addr"] is not None else "-":<10} size={r["size"] if r["size"] is not None else "-":<6} '
              f'{r["name"] or "-"}  unit={r["unit"] or "-"}')
    if len(hits) > a.limit:
        print(f"... {len(hits) - a.limit} more", file=sys.stderr)
    return 0 if hits else 1


if __name__ == "__main__":
    sys.exit(main())
