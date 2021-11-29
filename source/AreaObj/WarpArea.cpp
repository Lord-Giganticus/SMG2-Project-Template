#include "spack/AreaObj/WarpArea.h"
#include "spack/Extensions/WarpAreaParser.h"
#include "Util.h"
/*
* Created by Evanbowl with help from Aurum, AwesomeTMC, Galaxy Master, and Zyphro.
* 
* An area that takes Mario to a specified General Position with the name "WarpAreaDestPos".
*/

s32 mode;

WarpArea::WarpArea(const char* pName) : AreaObj(pName) {
hasEnteredArea = 0; 
mElapsed = 0;
}

void WarpArea::init(const JMapInfoIter& rIter) {
    AreaObj::init(rIter);
    MR::connectToSceneAreaObj(this);
    if (mObjArg2 < 0)
    mObjArg2 = 45;
}

void WarpArea::movement() {
if (hasEnteredArea == 1 || isInVolume(* MR::getPlayerPos())) {
hasEnteredArea = 1;
mElapsed++;
}

    if (mElapsed == 10) { //Phase 1: Start a circle wipe and lock player control.
    SPack::selectWipeClose(mObjArg1, mObjArg2);
    MR::offPlayerControl();
    
    if (mObjArg0 < 0) {
    MR::stopSubBGM(45);
    MR::stopStageBGM(45);
    }
}

if (mElapsed == mObjArg2 + 60) { //Phase 2: Set the player state to Wait to prevent animation issues and then warp Mario to the specified GeneralPos.
MR::setPlayerStateWait();

if (mObjArg0 >= 0) {

char WarpAreaDestPos[0x60];
snprintf(WarpAreaDestPos, 0x10, "WarpAreaPos%03d", mObjArg0);

MR::setPlayerPosAndWait(WarpAreaDestPos);
OSReport("(WarpArea) Warping player to WarpArea position %03d\n", mObjArg0);
if (*MR::getPlayerPos() == TVec3f(0.0f, 0.0f, 0.0f))
OSReport("(WarpArea) WarpArea position %03d doesn't exist!\n", mObjArg0);
else
OSReport("(WarpArea) Warp successful!\n");
}
else {
SPack::WarpAreaParser(mObjArg5);
}
}

if (mElapsed == mObjArg2 + 90) { //Phase 3: Open the circle wipe, restore player control, and reset both "mElapsed" and "hasEnteredArea" vars.
SPack::selectWipeOpen(mObjArg3, mObjArg4); //The "hasEnteredArea" var is used to prevent a softlock caused by leaving the WarpArea during the circle wipe closure.
MR::onPlayerControl(1);
mElapsed = 0;
hasEnteredArea = 0;
    }
}

const char* WarpArea::getManagerName() const {
    return "SwitchArea";
}

