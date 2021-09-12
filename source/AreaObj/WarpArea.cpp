#include "spack/AreaObj/WarpArea.h"
#include "Util.h"
/*
* Created by Evanbowl with help from Aurum, AwesomeTMC, and Zyphro.
* 
* An area that takes Mario to a specified General Position with the name "WarpAreaDestPos".
*
* Code is NOT final. This will be cleaned up soon!
*
*/

s32 mode;
const char* StageList[4] = {"TemplateTestGalaxy", "GalaxyNameHere", "TemplateTestGalaxy", "DimensionBigCastleGalaxy"};

WarpArea::WarpArea(const char* pName) : AreaObj(pName) {
hasEnteredArea = 0; 
mElapsed = 0;
}

void WarpArea::init(const JMapInfoIter& rIter) {
    AreaObj::init(rIter);
    MR::connectToSceneAreaObj(this);
}

void WarpArea::movement() {
if (isInVolume(* MR::getPlayerPos()) || hasEnteredArea == 1) {
hasEnteredArea = 1;
mElapsed++;
}

      if (mElapsed == 10) { //Phase 1: Start a circle wipe and lock player control.
MR::closeWipeCircle(45);
MR::offPlayerControl();
}

if (mElapsed == 60) { //Phase 2: Set the player state to Wait to prevent animation issues and then warp Mario to the specified GeneralPos.
MR::setPlayerStateWait();

if (mObjArg0 >= 0) {
//Somehow this works?!

snprintf("WarpAreaDestPos###", 0x60, "%s%03d", "WarpAreaDestPos", mObjArg0);
MR::setPlayerPosAndWait("WarpAreaDestPos###");
}
else {
MR::goToGalaxy(StageList[mObjArg1]); //Credit to Shibbo's SceneChangeArea for the list!
MR::goToGalaxyNoSelection(StageList[mObjArg1], mObjArg2, -1, 0);
    }
}

if (mElapsed == 90) { //Phase 3: Open the circle wipe, restore player control, and reset both "mElapsed" and "hasEnteredArea" vars.
MR::openWipeCircle(45); //The "hasEnteredArea" var is used to prevent a softlock caused by leaving the WarpArea during the circle wipe closure.
MR::onPlayerControl(1);
mElapsed = 0;
hasEnteredArea = 0;
    }
}

const char* WarpArea::getManagerName() const {
    return "SwitchArea";
}

