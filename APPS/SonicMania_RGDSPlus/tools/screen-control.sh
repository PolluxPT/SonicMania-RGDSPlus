#!/bin/bash
# Stock mapping verified on the user's RGDS+: backlight=lower, backlight1=upper.
rgds_lower_node=/sys/class/backlight/backlight/brightness
rgds_lower_saved=''
rgds_screen_save() {
    local value
    [ -r "$rgds_lower_node" ] && [ -w "$rgds_lower_node" ] || { echo 'RGDS screen: lower backlight node unavailable'; return 0; }
    value=$(cat "$rgds_lower_node") || return 0
    case "$value" in ''|*[!0-9]*) return 0;; esac
    [ "$value" -le 65535 ] || return 0
    rgds_lower_saved=$value
    export RGDS_LOWER_SAVED=$value
    printf 'RGDS screen: saved lower brightness=%s\n' "$value"
}
rgds_screen_restore() {
    [ -n "$rgds_lower_saved" ] || return 0
    if printf '%s\n' "$rgds_lower_saved" > "$rgds_lower_node"; then
        printf 'RGDS screen: restored lower brightness=%s\n' "$rgds_lower_saved"
    fi
}
