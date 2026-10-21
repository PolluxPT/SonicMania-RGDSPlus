#!/bin/bash
tools_dir=$(CDPATH= cd -P "$(dirname "$0")" 2>/dev/null && pwd) || exit 1
export SONICMANIA_VSYNC=0
exec /bin/bash "$tools_dir/../../SonicMania_PLUS.sh"
