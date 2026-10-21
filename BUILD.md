# Building the RG DS Plus target

These are developer instructions. Playing the supplied release does not require compiling.
The release ships the exact modified source snapshot; the SDK/sysroot is not bundled.

## Source layout

Extract `Sources/sonicmania-rgds-source.tar.xz` into a clean working directory.
It contains `sonic-mania/`, recursive dependencies and `SDL2-headers/`.
Do not replace the vendored engine tree with an unmodified upstream checkout.
Runtime scripts under `APPS` are the final single-launcher version and accompany the executable.

## Build environment used

- Linux x86_64 host, Ubuntu 24.04 userspace.
- CMake 3.28.3 and Ubuntu GCC/G++ 13 aarch64 cross compiler.
- Cortex-A55 flags: `-mcpu=cortex-a55 -fno-strict-aliasing`, Release `-O3`.
- `PORT_SDK` points to an extracted SDK tree with compiler executables in `usr/bin`,
  headers/sysroot in `usr`, and ARM64 libraries in `usr/lib/aarch64-linux-gnu`.
- `SDL_HEADERS` points to the extracted `SDL2-headers` directory.
- Link-time SDL2: Ubuntu ARM64 `libSDL2-2.0.so.0.3000.0`, 2.30.0+dfsg-1ubuntu3.1.
- libogg-dev 1.3.5-3build1 supplies the static ARM64 `libogg.a`.
- Theora decoder 1.1.1 built from the original archive included in `Sources`.
- The actual console uses its Stock SDL2 (reported 2.32.10), Wayland and ALSA.

The supplied toolchain expects this extracted layout. An ordinary compiler installation
with a different layout needs an adapted toolchain; this is not an automatic SDK installer.

## Theora decoder

Extract `Sources/libtheora-1.1.1.tar.bz2`. Set `THEORA_SOURCE` to the extracted directory,
export `PORT_SDK`, then run `python3 Sources/build-theora.py` from a clean working directory.
The helper builds only the portable decoder and applies the transposed qi/qti index correction.
It writes the static archive to `$PORT_SDK/usr/lib/aarch64-linux-gnu/libtheoradec.a`.

## Engine build

After extracting the source, set your actual absolute paths (examples below are placeholders):

```bash
export PORT_SDK=/absolute/path/to/extracted-sdk
export SDL_HEADERS=/absolute/path/to/SDL2-headers
export PATH="$PORT_SDK/usr/bin:$PATH"
export LD_LIBRARY_PATH="$PORT_SDK/usr/lib/x86_64-linux-gnu${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
cmake -S sonic-mania -B build-arm \
  -DCMAKE_TOOLCHAIN_FILE="$PWD/sonic-mania/rgds-toolchain.cmake" \
  -DPLATFORM=RGDS -DRETRO_REVISION=2 -DRETRO_MOD_LOADER=OFF \
  -DGAME_STATIC=ON -DGAME_INCLUDE_EDITOR=OFF -DRETRO_DISABLE_PLUS=ON \
  -DCMAKE_BUILD_TYPE=Release
cmake --build build-arm -j4
```

The output is `build-arm/dependencies/RSDKv5/RSDKv5`. Strip it with the ARM64 strip tool
and install it as `APPS/SonicMania_RGDSPlus/SonicMania`.
Rebuild `sonic-mania/rgds-calibrate.c` separately if changing the optional calibration tool.

The distributed engine must keep DLC disabled by default. Do not remove `RETRO_DISABLE_PLUS=ON`.
The single Plus launcher performs an explicit runtime opt-in; it does not change the binary default.

## Relevant modifications

- `dependencies/RSDKv5/platforms/RGDS.cmake` and `sonic-mania/rgds-toolchain.cmake`.
- Engine-side `rgds-hardware.cpp`, `rgds-render-utils.hpp`, `rgds-frame-clock.hpp`, `rgds-audio-utils.hpp`.
- SDL2 rendering/audio, Scene3D, rotozoom drawing and DummyCore integration within the snapshot.
- Launcher, volume/backlight restore and watcher scripts under `APPS`.

## Verification

Tests in `Sources/qa_*` are developer harnesses, not game launchers. Native host dummy-SDL
runs can check behaviour and pixel equality, but cannot establish console frame rate.
Some historical shell tests expect the pre-single-launcher staging layout; adapt paths to
the current APPS directory when running them. Do not preload QA libraries during normal play.
See `docs/VALIDATION.md` and `Sources/history` for the recorded checks and their limits.
