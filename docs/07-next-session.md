# TODO — session 2

The game plays into the village and a quest's map at 30 frames a second,
with the pad (session 1). The sound stutters.

1. **The stutter.** Short gaps everywhere in game, one every 0.3-1 s
   (`WIIKIT_AUDIODBG=1`: 168 blocks given up on in a few minutes of play;
   each report says when the AI interrupt was raised and acknowledged, when
   the last command list came, and where the guest was). The pattern: the
   guest takes the AI interrupt at once, but AX sends no command list, so no
   frame comes for 50 ms. AX's AI callback (`__AXOutAiCallback`, 80470700)
   mixes only if its "DSP ready" flag (`-0x3EA8(r13)`) is 1; otherwise it
   sets 2 and asserts the DSP task, and the frame waits for the task's
   resume callback (`804707C0`, registered by `__AXOutInitDSP`), which then
   mixes it (2) or sets the flag to 1. Lead: on a console, after AX's yield
   mail (`0xDCD10002`) the task manager answers "carry on" (`0xCDD10003`)
   and the micro-code replies `0xDCD10001` (resume, with an interrupt): the
   resume callback runs there. wiikit ignores `0xCDD10003` and never
   resumes. Check it against Dolphin's DSP HLE (`UCodes.cpp`, the task mails)
   and the SDK's task manager (`__DSPHandler`), then answer the resume and
   play in game with `WIIKIT_AUDIODBG=1`. Tried at the end of session 1 in
   attract mode only (120 s, no window): 3 blocks given up on with the
   resume answered, 5 without: too few to judge; test in game. Other suspects: the game's own AX frame
   callback (`-0x3EAC(r13)`, Capcom's sound engine) running long inside the
   AI interrupt.
2. **Play on**: a hunt, a carve, back to the village, the quest's rewards;
   the other maps (each loads its `mapNN.rso`); the arena.
3. **Saving**: the game saves to the NAND; write a save, load it.
4. The CPU's EFB writes (`GXPokeARGB`, logged once): what the game draws so,
   and whether it shows.
5. The Home Button menu (`hbm_data.rso`, through KPAD): open it with Home.

Build and run:

    sh build/rebuild.sh                     # recompile with the modules, ninja
    cd build/run1 && ../recomp-build/wiiboot ../extract --symbols ../recomp/symbols.tsv

(`build/rebuild.sh` holds the recompiler's command line of the README, with
`PATH=/c/msys64/mingw64/bin`.) `build/fonts` holds the boot ROM's fonts and
`dsp_coef.bin` (copied from Arc Rise Fantasia's build). Without input the
game runs its opening movie, its title and its attract movies; Begin Game
needs the pad (wiikit's `WIIKIT_PAD` presses Remote buttons, which a game
played with the Classic does not see).
