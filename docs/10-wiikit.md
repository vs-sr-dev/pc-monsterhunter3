# This port and wiikit

This port takes [wiikit](https://github.com/vs-sr-dev/wiikit) as a
submodule at `wiikit/`, like the five ports before it. While this port is
private, wiikit's code, comments and commits do not name it ("a stripped
2010 RSO game" stands for it).

## What this port used as it is (session 1)

`wiikit.disc` (the RVZ read directly), `dol`, `ppc`, discovery and the
recompiler, and the whole runtime: threads, interrupts, IOS, GX and the
renderer, AX, the Classic Controller on SDL gamepads.

## What it gave wiikit (session 1, not yet committed there)

| Change | Layer | Why it is not game knowledge |
|---|---|---|
| **RSO modules**: `wiikit/rso.py` (the format, the `.sel`; `attach` links modules at virtual addresses into the executable's image), `recomp --sel --rso`, relocated immediates read from the loaded module, switch addresses made virtual, `runtime/rso.cpp` (`RSOLinkListFixed` / `RSOUnLinkList` hooks, run-time -> virtual addresses in dispatch and in names) | 2, 4, 5 | any game built with the SDK's RSO library |
| the recompiler writes `symbols.tsv` (every unit, named) for `wiiboot --symbols` | 4 | a stripped executable's names in the runtime's logs and crash reports |
| WPAD: `WPADGetDataFormat` / `WPADSetDataFormat`, `WPADControlDpd`, `WPADIsDpdEnabled`, `WPADGetDpdSensitivity`, `WPADGetAccGravityUnit`, `WPADSetSamplingCallback`; request callbacks called at the next delivery | 5 | games that read WPAD themselves and clamp its sticks |
| `/dev/usb/hid` (version 4, never a device attached) | 5 | the SDK's HID/KBD libraries (USB keyboards) fail without it |
| the CPU's EFB reads (`GXPeekARGB`/`GXPeekZ` at `0xC8000000`): the renderer reads the EFB back after what was submitted, peeks read that copy until more is recorded; CPU writes ignored | 5 | any game that reads its picture back (brightness, picking) |
| the AI clock: a frame is ready once the guest has taken the DSP's interrupt for it; after a block given up on, the schedule restarts instead of catching up; `WIIKIT_AUDIODBG` reports each block given up on, with the chain's times and the guest's call chain | 5 | AX skips a frame when the AI interrupt comes before the DSP's |

A copy of the uncommitted diff is kept in `build/wiikit-session1.patch`.

Before these go in, the routine: recompile and boot the four Wii ports and
Mega Man X: Command Mission with them (the same screens as before; the
self-test of Victorious), then commit in wiikit and bump every port.
