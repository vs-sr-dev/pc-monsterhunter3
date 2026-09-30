# The disc

`Monster Hunter Tri (Europe) (En,Fr,De,Es,It).rvz`: RMHP08, a dual-layer
Wii disc (an UPDATE partition and a 4.4 GB DATA partition), read by
`wiikit.disc` directly. The DATA partition: 4 743 files, 2.7 GB. The
directories are numbered, not named:

| Dir | Files | Size | What |
|---|---|---|---|
| `00` | 42 | 13 MB | system: Capcom logo, save icon, loading screens, the Home Button menu's data, network error messages, a software keyboard (`sofkeybd_eur.arc`), per language |
| `01` | 21 | 3.7 MB | **the RSO modules** (`03-executable.md`) |
| `04` | 168 | 168 MB | NW4R models (`.brres`): the arena, player equipment |
| `05` | 799 | 4.7 MB | quests and monster sets (`.esd`, `.esp`, `questNN_lang.bin`) |
| `06` | 43 | 0.2 MB | `mNNN_..._pop.dat`: monster placement per map |
| `07` | 49 | 1.3 MB | `dcmNNN.bin` |
| `08` | 688 | 71 MB | player models and motions |
| `09` | 225 | 27 MB | weapons (`axe001.brres`...) |
| `10` | 175 | 60 MB | monsters (`em001`...: models, `.breft`/`.breff` effects) |
| `11` | 36 | 8.7 MB | common effects |
| `12`-`14` | 34 | 5 MB | Felyne masks, town and village NPCs |
| `15` | 1 459 | 496 MB | maps: models, collision (`mNN_atari.bin`), effects, `.sch` |
| `16` | 983 | 605 MB | sound: Capcom's banks (`.whd`, `.srt`, `.tsb`) and streams (`.ssd`, Dolby Pro Logic II) |
| `17` | 13 | 1.2 GB | THP movies (`demo39.thp`...) |

At the root: `mh3.sel` (the static module's export list), `opening.bnr`,
and the strap screens in six languages.
