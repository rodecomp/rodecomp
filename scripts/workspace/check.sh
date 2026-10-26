#!/usr/bin/env bash
# Back up a slot's edits, then run `td check` from that slot.
# Usage: bash scripts/workspace/check.sh <slot> <file>... [--includers] [--full]
#   --full   print td's whole output instead of one line per function
set -euo pipefail

# Layout: <root>/<main clone>/scripts/workspace/<this>, or <root>/workers/<slot>/scripts/workspace/<this>.
# Native path (C:/...) on Git Bash/MSYS so Windows tools accept it; POSIX path elsewhere.
CLONE="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && { pwd -W 2>/dev/null || pwd; })"
if [ "$(basename "$(dirname "$CLONE")")" = workers ]; then
    ROOT="$(dirname "$(dirname "$CLONE")")"; MAIN="${RODECOMP_MAIN:-$ROOT/rodecomp}"
else
    ROOT="$(dirname "$CLONE")"; MAIN="${RODECOMP_MAIN:-$CLONE}"
fi
[ $# -ge 2 ] || { echo "usage: $0 <slot> <file>... [--includers] [--full]" >&2; exit 2; }
name="$1"; shift
slot="$ROOT/workers/$name"
[ -d "$slot/.git" ] || { echo "no slot: $slot (run slot.sh $name)" >&2; exit 1; }

full=0; args=()
for a in "$@"; do
    if [ "$a" = "--full" ]; then full=1; else args+=("$a"); fi
done

work="$ROOT/_work/$name"; mkdir -p "$work"
stamp="$(date +%Y%m%d-%H%M%S)"
patch="$work/pre-check-$stamp.patch"
git -C "$slot" diff HEAD --binary > "$patch"
before="$(git -C "$slot" diff HEAD --binary | git hash-object --stdin)"

log="$work/check-$stamp.log"
set +e
(cd "$slot" && td check "${args[@]}") > "$log" 2>&1
rc=$?
set -e

if [ $full -eq 1 ]; then
    cat "$log"
else
    # One line per function (stable ID lines) plus errors and the unannotated list.
    grep -E '^\s*(fn|gl|vt|st)_[0-9]+ |error|unannotated' "$log" || cat "$log"
fi

after="$(git -C "$slot" diff HEAD --binary | git hash-object --stdin)"
if [ "$before" != "$after" ]; then
    echo "WARNING: working tree changed during td check; edits saved in $patch" >&2
fi
echo "log: $log  backup: $patch  exit: $rc"
exit $rc
