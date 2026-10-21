#!/bin/bash
set -u
app_parent=$(CDPATH= cd -P "$(dirname "$0")" 2>/dev/null && pwd) || exit 1
app_dir="$app_parent/SonicMania_RGDSPlus"
mkdir -p "$app_dir" || exit 1
exec > "$app_dir/SonicMania_Arranque.txt" 2>&1
printf 'Sonic Mania RGDS+ 1.0 - 60 FPS e reposicao do volume do sistema\n'
date
printf 'Arquitetura: '; uname -m
case "$(uname -m)" in aarch64) ;; *) echo 'Requer Linux ARM64.'; exit 1;; esac
if [ ! -s "$app_dir/Data.rsdk" ]; then
    echo 'Falta Data.rsdk. Copia o teu ficheiro para APPS/SonicMania_RGDSPlus/Data.rsdk.'
    exit 1
fi
if [ ! -s "$app_dir/SonicMania" ]; then echo 'Falta o executavel SonicMania.'; exit 1; fi
chmod +x "$app_dir/SonicMania" || exit 1
unset LD_PRELOAD
unset RGDS_BENCH_FRAMES
export LD_LIBRARY_PATH="$app_dir/lib:/usr/lib:/lib:/mnt/vendor/lib"
export XDG_RUNTIME_DIR="${XDG_RUNTIME_DIR:-/var/run}"
export WAYLAND_DISPLAY="${WAYLAND_DISPLAY:-wayland-0}"
export SDL_VIDEODRIVER=wayland
export SDL_VIDEO_DOUBLE_BUFFER=0
export SDL_AUDIODRIVER=alsa
export SDL_JOYSTICK_ALLOW_BACKGROUND_EVENTS=1
export SDL_MOUSE_TOUCH_EVENTS=0
export SDL_TOUCH_MOUSE_EVENTS=0
export RGDS_STOCK=1
export RGDS_DISPLAY="${RGDS_DISPLAY:-0}"
export RGDS_MANIA_PLUS=1
export RGDS_VSYNC="${SONICMANIA_VSYNC:-0}"
if [ "${SONICMANIA_COMPATIBILIDADE:-0}" = 1 ]; then
    export SDL_RENDER_DRIVER=software
    export RGDS_VSYNC=0
else
    unset SDL_RENDER_DRIVER
fi
printf 'Ecra: %s; video: %s; audio: %s; renderer: %s\n' "$RGDS_DISPLAY" "$SDL_VIDEODRIVER" "$SDL_AUDIODRIVER" "${SDL_RENDER_DRIVER:-automatico}"
printf 'Volume: 0.35 * (nivel/10)^2; niveis 0..10\n'
printf 'Plus solicitado: %s; sincronizacao solicitada: %s\n' "$RGDS_MANIA_PLUS" "$RGDS_VSYNC"
if [ ! -S "$XDG_RUNTIME_DIR/$WAYLAND_DISPLAY" ]; then echo 'O menu nao disponibilizou Wayland.'; exit 1; fi
cd "$app_dir" || exit 1
# Keep one Plus entry at the APPS level; archive the former base launcher.
mkdir -p tools/atalhos-anteriores || exit 1
for old_name in SonicMania.sh SonicMania_Compatibilidade.sh SonicMania_CalibrarAnalogico.sh SonicMania_TesteSemVSync.sh; do
    old_path="$app_parent/$old_name"
    if [ -f "$old_path" ]; then
        old_backup="$app_dir/tools/atalhos-anteriores/$old_name"
        [ -e "$old_backup" ] && old_backup="$old_backup.$$"
        mv "$old_path" "$old_backup" || exit 1
    fi
done
if [ ! -f gamecontrollerdb.txt ]; then cp gamecontrollerdb.inicial.txt gamecontrollerdb.txt || exit 1; fi
chmod +x ./rgds-calibrate || exit 1
if ! grep -q 'leftx:' gamecontrollerdb.txt; then
    if [ ! -f gamecontrollerdb.sem-analogico.txt ]; then cp gamecontrollerdb.txt gamecontrollerdb.sem-analogico.txt || exit 1; fi
    cp gamecontrollerdb.inicial.txt gamecontrollerdb.txt.new || exit 1
    mv gamecontrollerdb.txt.new gamecontrollerdb.txt || exit 1
fi
if [ "${SONICMANIA_CALIBRAR:-0}" = 1 ]; then
    ./rgds-calibrate
    calibration_result=$?
    [ "$calibration_result" = 0 ] || exit "$calibration_result"
    [ "${SONICMANIA_CALIBRAR:-0}" = 1 ] && exit 0
fi
if [ ! -f Settings.ini ]; then cp Settings.inicial.ini Settings.ini || exit 1; fi
if [ -x /lib/ld-linux-aarch64.so.1 ]; then
    /lib/ld-linux-aarch64.so.1 --list ./SonicMania || exit 1
fi
if [ -f log.txt ]; then mv log.txt log.anterior.txt || exit 1; fi
. "$app_dir/tools/screen-control.sh" || exit 1
. "$app_dir/tools/volume-control.sh" || exit 1
rgds_screen_save
rgds_volume_save
game_pid=''
screen_guard=''
rgds_cleanup() {
    trap - EXIT INT TERM HUP
    if [ -n "$game_pid" ]; then
        kill -TERM "$game_pid" 2>/dev/null || :
        for attempt in {1..50}; do kill -0 "$game_pid" 2>/dev/null || break; sleep 0.1; done
        kill -KILL "$game_pid" 2>/dev/null || :
        wait "$game_pid" 2>/dev/null || :
    fi
    rgds_screen_restore
    rgds_volume_restore
    if [ -n "$screen_guard" ]; then kill -TERM "$screen_guard" 2>/dev/null || :; wait "$screen_guard" 2>/dev/null || :; fi
}
trap rgds_cleanup EXIT
trap 'exit 130' INT
trap 'exit 143' TERM HUP
./SonicMania &
game_pid=$!
if [ -n "$rgds_lower_saved$rgds_volume_saved" ]; then
    if command -v setsid >/dev/null 2>&1; then
        setsid /bin/bash "$app_dir/tools/screen-guard.sh" "$game_pid" "$rgds_lower_saved" "$rgds_lower_node" "$rgds_volume_saved" "$rgds_volume_node" &
    else
        /bin/bash "$app_dir/tools/screen-guard.sh" "$game_pid" "$rgds_lower_saved" "$rgds_lower_node" "$rgds_volume_saved" "$rgds_volume_node" &
    fi
    screen_guard=$!
fi
wait "$game_pid"
result=$?
game_pid=''
printf '\nCodigo de saida: %s\n' "$result"
exit "$result"
