#!/usr/bin/env bash
# Quick test runner - automatically finds minishell binary

SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)

# Common locations to search for minishell binary
SEARCH_PATHS=(
    "../minishell/minishell"
    "../../minishell/minishell"
    "./minishell"
    "../minishell"
)

MS_BIN=""

# If binary path provided as argument, use it
if [ $# -gt 0 ] && [ -f "$1" ] && [ -x "$1" ]; then
    MS_BIN="$1"
    shift
else
    # Search for binary
    for path in "${SEARCH_PATHS[@]}"; do
        if [ -f "$path" ] && [ -x "$path" ]; then
            MS_BIN="$path"
            break
        fi
    done
fi

if [ -z "$MS_BIN" ]; then
    echo "Error: Could not find minishell binary"
    echo ""
    echo "Searched in:"
    for path in "${SEARCH_PATHS[@]}"; do
        echo "  - $path"
    done
    echo ""
    echo "Usage: $0 [path_to_minishell] [options]"
    exit 2
fi

# Get absolute path
MS_BIN=$(cd "$(dirname "$MS_BIN")" && pwd)/$(basename "$MS_BIN")

echo "Found minishell: $MS_BIN"
echo ""

# Run the tests
exec "$SCRIPT_DIR/run_tests.sh" "$MS_BIN" "$@"
