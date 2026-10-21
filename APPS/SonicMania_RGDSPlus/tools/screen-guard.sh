#!/bin/bash
# Independent child-lifetime watcher: restore screen and pre-game system volume.
set -u
game_pid=${1:-}
saved=${2:-}
node=${3:-/sys/class/backlight/backlight/brightness}
volume_saved=${4:-}
volume_node=${5:-/sys/class/anbernic_misc/openbor_volume}
case "$game_pid" in ''|*[!0-9]*) exit 1;; esac
[ "$game_pid" -gt 1 ] || exit 1
case "$saved" in *[!0-9]*) exit 1;; esac
case "$volume_saved" in *[!0-9]*) exit 1;; esac
if [ -n "$saved" ]; then
    [ "${#saved}" -le 5 ] && [ "$saved" -le 65535 ] || exit 1
fi
if [ -n "$volume_saved" ]; then
    [ "${#volume_saved}" -le 2 ] && [ "$volume_saved" -le 10 ] || exit 1
fi
[ -n "$saved$volume_saved" ] || exit 1
sleeper=''
restore() {
    trap - TERM INT HUP
    if [ -n "$sleeper" ]; then kill -TERM "$sleeper" 2>/dev/null || :; wait "$sleeper" 2>/dev/null || :; fi
    if [ -n "$saved" ]; then printf '%s\n' "$saved" > "$node"; fi
    if [ -n "$volume_saved" ]; then printf '%s\n' "$volume_saved" > "$volume_node"; fi
}
trap 'restore; exit 0' TERM INT
trap '' HUP
while kill -0 "$game_pid" 2>/dev/null; do
    sleep 1 & sleeper=$!
    wait "$sleeper" || :
    sleeper=''
done
restore
