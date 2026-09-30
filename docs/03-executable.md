# The executable and its modules

## main.dol

Stripped, 6.7 MB, entry `80006310`; `.text` at `8003F260`-`8056FAA0`. The
libraries it reports:

* RVL SDK `0x4302_145`, built February 27 2009 (KPAD and WPAD June 22 2009);
  DWC, NHTTP, SSL, NCD, NWC24, SO (the online service, now closed) of May
  and June 2009; HID, KBD, KPR (USB keyboards); THP; PAD.
* NW4R G3D and EF of February 1 2010.
* The Home Button menu (`homebuttonLib`), mostly in `hbm_data.rso`.

Names (`tools/names.py` -> `build/names.tsv`), most trusted first:

* `tools/names-manual.tsv`: 23 by hand, each with its evidence (KPAD's init
  and reset, WPAD's read, allocator, status, data format, DPD, the RSO
  library, `RealMode`, `PPCHalt`, `__VIRetraceHandler`). `WPADInit` is the
  0x70-byte function at `804EDBB0`; the one that prints "WPADInit()" is
  `__wpadInitSub`.
* `mh3.sel`: 758 functions, the link names of what the modules import.
* Signatures (`tools/sigmatch.py`): Dolphin's `totaldb.dsy` and
  Victorious's ELF, 1 288 kept.
* The SDK's own debug strings: 36.

The game reads its controllers itself: `WPADRead` into its own samples, then
`WPADClampStick`, per channel (`fn_80042088`); an extension callback per
channel (`8004378C`) sets the data format and the pointing camera. KPAD
serves only the Home Button menu. `fn_800411AC` and its like are the
linker's "keep" stubs: they call hundreds of functions with zeros so that
the modules can import them; they never run.

## The RSO modules

RSO is the SDK's relocatable-module library (`RSOLinkList`, `RSOUnLinkList`,
`RSOStaticLocateObject`...): unlike a GameCube REL, a module keeps its
symbols as names. The game (`800405FC`) reads a module into a buffer it
chooses, puts its bss after it, and calls `RSOLinkList`; then it calls the
module's `_prolog` through the header. `mh3.sel` (read by
`RSOStaticLocateObject`, `80040710`) lists 919 of the executable's symbols
(section 1 `.init`, 2 `.text`, the data sections unplaced). Every import of
every module resolves there: the modules do not import from each other.

| Module | Code | Data | What |
|---|---|---|---|
| `em_data` | 0x4F650 | 0x6FB98 | the monsters |
| `quest_data` | 0x33A68 | 0x2EA20 | quests |
| `hbm_data` | 0x2AC28 | 0x4EB4 | the Home Button menu |
| `lobby_data` | 0x18924 | 0x678B4 | village, town, lobby |
| `demo_data` | 0x167A4 | 0x1FB68 | title, movies, credits |
| `net_data` | 0x4C70 | 0x8794 | online |
| `com_data` | 0x1144 | 0x45830 | shared tables |
| `map00`-`map11`, `map_town`, `map_village` | 0x184-0x18C each | | a map's tables |

Relocations: ADDR32 in data only; ADDR16_HA/LO always on an instruction's
immediate; REL24 on branches (12 865 of `em_data`'s 12 963 external ones
are calls into the executable).

### How they are recompiled (wiikit)

`wiikit/rso.py` links each module at a virtual address (from `81800000`,
past MEM1, within a `bl`'s reach of the executable) into the executable's
image, as segments of it; `python -m wiikit.recomp ... --sel mh3.sel --rso
MODULE.rso ...`. Discovery, switch tables and direct calls then work as for
the executable's own code. The generated code does not depend on where the
game loads a module: an instruction whose immediate is relocated reads it
from the loaded module, which the SDK's own linker has patched
(`ld16(g_rso_base[k] + offset)`); a switch's run-time case address is made
virtual. The runtime (`runtime/rso.cpp`) learns where each module is from
`RSOLinkListFixed` and `RSOUnLinkList`, and turns a run-time address in a
loaded module's code (its virtual methods, its `_prolog`) into the virtual
one. Return addresses stay virtual. Seen at run time: `hbm_data` at boot;
`com_data`, `lobby_data`, `demo_data`, `quest_data` at the title;
`map_village`; `quest_data`, `em_data` and `map01` in a quest, relinked at
other addresses as the game moves between them.
