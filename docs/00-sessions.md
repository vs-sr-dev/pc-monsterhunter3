# Session log

## Session 1 — from the disc to a quest's map

Goal: set the port up on wiikit, see what the disc holds, boot. It went past
that: the game plays into the village and a quest, with a pad.

Results:

* **The disc** (`01-disc-layout.md`): RMHP08, the RVZ read by `wiikit.disc`
  directly, 4 743 files, 2.7 GB: NW4R models and effects, THP movies,
  Capcom's own sound banks, and **21 RSO modules** with `mh3.sel`.
* **The executable** (`03-executable.md`): stripped; the RVL SDK of February
  2009 (KPAD and WPAD of June 2009), NW4R of February 2010, DWC and the
  network libraries, and HID/KBD for USB keyboards. 2 105 names: 758 from
  `mh3.sel` (the executable's exports to its modules: link names), 1 288
  by signature (Dolphin's database and Victorious's ELF), 36 from the SDK's
  own strings, 23 by hand.
* **The RSO modules**: most of the game's code (monsters, quests, the lobby,
  the movies, the Home Button menu, the maps) is loaded at run time into
  the heap and linked by the SDK. wiikit learnt to recompile them with the
  executable, at virtual addresses, running wherever the game loads them.
  Recompiled at the first try: 22 822 units, 1.59 M instructions, 0 gaps.
* **The boot**, step by step: `RealMode` in a form the known pattern missed;
  `WPADInit` named by the string pass where its sub-function prints the
  name (as in Arc Rise Fantasia); seven WPAD functions the game calls that
  wiikit lacked (the data format, the pointing camera, the accelerometer's
  gravity unit, the sampling callback); `/dev/usb/hid`, which the game opens
  at Begin Game (USB keyboards for the chat) and which is always there on a
  console; the CPU's reads of the EFB (`GXPeekARGB`), after file selection.
* **It plays.** The strap screen, the Capcom logo, the opening movie with
  sound, the title, file selection, character creation, the village, a
  quest's map (Deserted Island, `map01` and `em_data`), with the user's
  Xbox One pad, at a steady 30 frames a second with rare small dips.
* **The sound stutters** everywhere, briefly. Found and fixed on the way (in
  wiikit): the AI clock started a block as soon as a frame was mixed,
  before the game had taken the DSP's interrupt, and AX then skipped the
  next frame; after a gap, the clock's catch-up gave up on a burst of blocks.
  149 dropped blocks in two minutes of attract mode became 5, and bursts are
  gone, but in game isolated gaps remain (`07-next-session.md`).

wiikit's changes of this session are not committed there yet: they wait for
the check on the other five ports (`10-wiikit.md`).
