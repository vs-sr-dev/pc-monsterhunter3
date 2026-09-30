// Monster Hunter Tri — the port's own layer over the wiikit runtime.
//
// Linked into wiiboot by mh3.cmake. What belongs here is what only this game
// needs.
#include "rt.h"

namespace {

// The game reads the Remote, its Nunchuk and the Classic Controller from
// WPAD's own samples (WPADRead, then WPADClampStick); KPAD serves only the
// Home Button menu. The port plays with the Classic Controller.
void install() {
    wpad_set_classic(true);
}

RtGameLayer layer("Monster Hunter Tri", install);

}  // namespace
