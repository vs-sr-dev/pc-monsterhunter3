# pc-monsterhunter3

Toward a native PC port of **Monster Hunter Tri** (Wii, Capcom, 2009; Europe
and North America 2010), known in Japan as Monster Hunter 3. The Wii
version was never re-released (Monster Hunter 3 Ultimate, on 3DS and Wii U,
is a different game). The goal is the game running natively on PC, played
with a gamepad as the Classic Controller.

The route is static recompilation: the game's own PowerPC code, translated
to C++ and built for the PC, running on a replacement for the Wii's
hardware and system software. Nothing is emulated at the instruction
level, and nothing of the game is rewritten.

This repository documents the disc and its code, and holds the port's own
tools and layer. It is the sixth port built on
**[wiikit](https://github.com/vs-sr-dev/wiikit)**, the game-agnostic Wii
toolkit, taken here as a submodule at `wiikit/`. Where this game needs
something every Wii game would need, it goes into wiikit, not here.

## Where it stands

After one session the game **boots, plays its movies, and plays into the
village and a first quest**: the strap screen, the Capcom logo, the opening
movie with its sound, the title, file selection, character creation, the
village, a quest's map with its monsters, at 16:9 and a steady 30 frames a
second (the game's rate), played with an Xbox One pad as the Classic
Controller. **The sound stutters**: short gaps, everywhere, the open issue
(see [docs/07-next-session.md](docs/07-next-session.md)). Nothing has been
saved or played through yet.

Most of the game's code is not in its executable: 21 **RSO modules** (the
SDK's relocatable code: monsters, quests, the lobby, the movies, the Home
Button menu, the maps) are loaded at run time. wiikit now recompiles them
with the executable (see [docs/03-executable.md](docs/03-executable.md)).

## BYOA — Bring Your Own Assets

This repository contains **documentation and tools only**. No game data, no
executables, no assets. You need your own original disc. The work is done on
the European release, RMHP08; the addresses in `tools/` and `docs/` are that
executable's.

## Layout

    docs/     disc, code and module analysis, the plan, the session log
    tools/    Monster Hunter Tri-specific tools, the port's layer (mh3.cpp)
    wiikit/   game-agnostic Wii toolkit (submodule: github.com/vs-sr-dev/wiikit)
    build/    (not in git) the disc, everything derived from it, the build

## Building

As for the other wiikit ports: Python 3.8+ (3.14 for zstd RVZ images),
CMake, Ninja, clang (MSYS2) and SDL3; OpenGL 4.5 to run. From the root:

```sh
python -m wiikit.disc GAME.rvz --extract build/extract
python tools/sigmatch.py build/extract/sys/main.dol --dsy <Dolphin>/Sys/totaldb.dsy \
    --elf <Victorious>/Oscar_wii_final_versioned.elf --out build/sig_guess.tsv
python tools/names.py build/extract/sys/main.dol          # -> build/names.tsv
python -m wiikit.recomp build/extract/sys/main.dol --out build/recomp \
    --symbols build/names.tsv --hooks tools/mh3-hooks.txt \
    --sel build/extract/files/mh3.sel $(for f in build/extract/files/01/*.rso; do echo --rso $f; done)
cmake -S build/recomp -B build/recomp-build -G Ninja -DCMAKE_CXX_COMPILER=clang++ \
    -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS_RELEASE=-O1 -DWIIKIT_EXTRA=$PWD/tools/mh3.cmake
ninja -C build/recomp-build
cd build/run1 && ../recomp-build/wiiboot ../extract --symbols ../recomp/symbols.tsv
```

`build/fonts` holds the boot ROM's fonts and `dsp_coef.bin` (Dolphin's
`Sys/GC` has free ones). The names need Dolphin's `totaldb.dsy`; Victorious's
symbolised ELF adds the SDK functions the database lacks (sigmatch's
`--elf`); `mh3.sel` on the disc names the executable's exports to its
modules. `tools/look.py` is `wiikit.ppc` on the stripped DOL.

Playing: wiikit's Classic Controller mapping (the pad's buttons by
position; `build/keys.txt` for the keyboard). F11 fullscreen, F12 a GX
trace.

## Documentation

    00-sessions.md            progress log
    01-disc-layout.md         what is on the disc
    03-executable.md          the stripped DOL, mh3.sel, the RSO modules and how they are recompiled
    07-next-session.md        the plan for the next session
    10-wiikit.md              how this port uses and grows wiikit

## Licence

MIT. This covers the documentation and tools in this repository only. It
says nothing about Monster Hunter Tri, which remains the property of its
rights holders.
