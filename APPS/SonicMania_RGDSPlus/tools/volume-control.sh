#!/bin/bash
# Preserve the stock console level across a game session.
rgds_volume_node=/sys/class/anbernic_misc/openbor_volume
rgds_volume_saved=''
rgds_volume_save() {
    local value
    [ -r "$rgds_volume_node" ] && [ -w "$rgds_volume_node" ] || { echo 'RGDS volume: system node unavailable; no snapshot'; return 0; }
    value=$(cat "$rgds_volume_node") || return 0
    case "$value" in ''|*[!0-9]*) return 0;; esac
    [ "${#value}" -le 2 ] && [ "$value" -le 10 ] || return 0
    rgds_volume_saved=$value
    printf 'RGDS volume: saved system level=%s\n' "$value"
}
rgds_volume_restore() {
    [ -n "$rgds_volume_saved" ] || return 0
    if printf '%s\n' "$rgds_volume_saved" > "$rgds_volume_node"; then
        printf 'RGDS volume: restored system level=%s\n' "$rgds_volume_saved"
    fi
}
