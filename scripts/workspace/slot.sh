#!/usr/bin/env bash
# Create or refresh private clones (slots) under workers/<name>.
# Usage: bash scripts/workspace/slot.sh w1 [w2 ...]
# A dirty slot is never touched.
set -euo pipefail

# Layout: <root>/<main clone>/scripts/workspace/<this>, or <root>/workers/<slot>/scripts/workspace/<this>.
# Native path (C:/...) on Git Bash/MSYS so Windows tools accept it; POSIX path elsewhere.
CLONE="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && { pwd -W 2>/dev/null || pwd; })"
if [ "$(basename "$(dirname "$CLONE")")" = workers ]; then
    ROOT="$(dirname "$(dirname "$CLONE")")"; MAIN="${RODECOMP_MAIN:-$ROOT/rodecomp}"
else
    ROOT="$(dirname "$CLONE")"; MAIN="${RODECOMP_MAIN:-$CLONE}"
fi
[ $# -gt 0 ] || { echo "usage: $0 <slot>..." >&2; exit 2; }
[ -d "$MAIN/.git" ] || { echo "main clone not found: $MAIN" >&2; exit 1; }
URL="$(git -C "$MAIN" remote get-url origin)"

copy_td() {
    local dst="$1/.td"
    mkdir -p "$dst"
    for f in config.json symbols.json symbols.json.etag target.exe; do
        if [ -f "$MAIN/.td/$f" ]; then cp -f "$MAIN/.td/$f" "$dst/$f"; fi
    done
    # Keep .td/ out of `git status`, as in the main clone.
    local ex="$1/.git/info/exclude"
    grep -qxF '.td/' "$ex" 2>/dev/null || echo '.td/' >> "$ex"
}

rc=0
for name in "$@"; do
    case "$name" in */*|*\\*|..|.|"") echo "[$name] invalid slot name" >&2; rc=1; continue;; esac
    slot="$ROOT/workers/$name"
    mkdir -p "$ROOT/workers" "$ROOT/_work/$name"
    if [ ! -d "$slot/.git" ]; then
        if ! { git clone -q "$MAIN" "$slot" &&
               git -C "$slot" remote set-url origin "$URL" &&
               git -C "$slot" fetch -q origin &&
               git -C "$slot" checkout -q -B main origin/main; }; then
            rm -rf -- "$slot"
            echo "[$name] creation failed, removed partial clone" >&2; rc=1; continue
        fi
        copy_td "$slot"
        echo "[$name] created at $(git -C "$slot" rev-parse --short HEAD)"
        continue
    fi
    if [ -n "$(git -C "$slot" status --porcelain)" ]; then
        echo "[$name] dirty, skipped" >&2; rc=1; continue
    fi
    git -C "$slot" fetch -q origin
    git -C "$slot" checkout -q -B main origin/main
    copy_td "$slot"
    echo "[$name] refreshed at $(git -C "$slot" rev-parse --short HEAD)"
done
exit $rc
