# Source material

`sonicmania-rgds-source.tar.xz` contains the complete modified engine/game source,
recursive dependencies and SDL2 headers. It is a source snapshot, not an executable or game datapack.

- Start with the repository's `BUILD.md` for the final public release.
- `source-revisions.json` records upstream revisions before platform modifications.
- `build-theora.py` and the original Xiph tarball support the static decoder build.
- `qa_*` are development validation harnesses.
- `licenses/` retains third-party licence notices.
- `history/` contains old build/validation reports; version-specific statements there are historical.
- Final shell-launcher source is directly under the repository's `APPS` folder.

The ARM64 cross SDK and original game assets are not included.
