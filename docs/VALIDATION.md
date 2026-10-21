# Validation record

## Console

Tested device: Anbernic RG DS Plus, RK3568 aarch64, Stock Linux, SDL2/Wayland,
OpenGL ES 2 renderer, ALSA. Display 0 is the upper display on the tested system.

The RC2 executable processing/pacing is retained in the final engine; only its version
label changed. Special Stage 1 steady report FPS were:

`59.93, 60.07, 59.97, 60.01, 59.98, 60.04, 60.01`.

These are seven ten-second measurement windows, excluding the window containing stage loading.
Eleven intervals exceeded 25 ms across these windows; the largest was 27.09 ms.
This confirms near-60 average FPS in that test, not perfectly uniform frame delivery or all-scene performance.
Normal stages tested also remained near 60 FPS.
The previous VSync build had sustained UFO rates around 49–52 FPS.

Recorded audio late-interval, processing-over-budget, clipped-sample and invalid-sample
counters were zero. Scheduling indicators are not a complete ALSA underrun count or a listening-quality score.
Lower-backlight brightness was recorded/restored at 70 and exit status was zero.
The maintainer confirmed the final single-launcher setup was working.
Raw personal gameplay logs are not included in this public repository.

## Local checks during development

- ARM64 Release build and ELF imports; no new dynamic dependency introduced by the final update.
- Stable face sorting and rotozoom equality tests, including boundary/random cases.
- Nine baseline/optimised image comparisons earlier in development; three additional
  frame-120 exact comparisons for UFO1, CPZ1 and Menu after pacing changes.
- Deadline clock jitter/stall tests and native capped timing checks.
- Audio finite/headroom/invalid sample and volume gain tests.
- Simulated backlight/system-volume restoration on normal exit, error, signal and forced child death.
- Single-launcher integration with a simulated game and hardware files; the unchanged Wayland
  availability check was bypassed only in the sandbox test copy because socket creation was blocked.
- Shell syntax, ZIP integrity, checksums, source licence retention and excluded user/game files.

No physical battery-life measurement, broad firmware compatibility matrix, or exhaustive
all-stage benchmark is claimed. This publication package does not modify the tested executable.
