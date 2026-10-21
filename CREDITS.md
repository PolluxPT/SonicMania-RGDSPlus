# Credits

## Game and engine

Sonic Mania, Sonic Mania Plus, their characters, music and game resources belong to SEGA
and the respective authors. The original RSDK engine is by Evening Star and its respective authors.
No claim of authorship over the original game or engine is made by this port.

## Decompilation projects

- Sonic Mania: https://github.com/RSDKModding/Sonic-Mania-Decompilation
- RSDKv5: https://github.com/Rubberduckycooly/RSDKv5-Decompilation
- RSDK decompilation authors: Rubberduckycooly and chuliRMG, with the upstream contributors.
- Exact source revisions: `Sources/source-revisions.json`.

## Platform adaptation

RG DS Plus platform integration, hardware testing and release packaging: Filipe Baeta.
Changes include ARM64 target integration, controls, volume curve, lower-backlight handling,
frame pacing, rendering optimisations and audio telemetry. These changes do not transfer
ownership of upstream code or game assets to the port maintainer.

## Icon

- Creator: **hatemtiger**.
- Work: **Sonic Mania Plus icon (.ico)**.
- Original page: https://www.deviantart.com/hatemtiger/art/Sonic-Mania-Plus-icon-ico-945862352
- Included file: `APPS/Imgs/SonicMania_PLUS.png`.
- The maintainer supplied the PNG; it is copied byte-for-byte, without alteration.
- Artwork is credited separately and is not relicensed under the source-code licences.

## Libraries

- SDL2: Simple DirectMedia Layer contributors; the console provides the runtime.
- Ogg and Theora: Xiph.Org Foundation and contributors; decoder sources/notices are included.
- GCC runtimes: GNU project and contributors. Included runtime libraries were retained from
  the previously working ARM64 package. See `Sources/licenses/GCC-copyright.txt` for notices
  and the GCC Runtime Library Exception; upstream sources: https://gcc.gnu.org/releases/.
- tinyxml2 and stb_vorbis: their respective authors; notices remain in the complete source snapshot.

Full licence texts are retained in `Sources/licenses` and in the source snapshot.
