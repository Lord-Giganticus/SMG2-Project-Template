#include "spack/AreaObj/WarpArea.h"
#include "spack/Extensions/WarpAreaStageTable.h"
#include "Util.h"
/*
* Created by Evanbowl with help from Aurum, AwesomeTMC, Galaxy Master, and Zyphro.
* 
* An area that takes Mario to a specified General Position with the name "WarpAreaDestPos".
*/

s32 mode;

WarpArea::WarpArea(const char* pName) : AreaObj(pName) {
	hasEnteredArea = 0; 
	AlexSus = 0; 
    /*I was told to name this AlexSus so I did. It's real purpose is a counter to allow the WarpArea to properly time its actions.
    (Closing wipes, warping the player at the right time, etc)
    */
	pos = (TVec3f(0.0f, 0.0f, 0.0f));
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
	AlexSus++;
}

	if (AlexSus == 10) { //Phase 1: Start a circle wipe and lock player control.
		SPack::selectWipeClose(mObjArg1, mObjArg2);
		MR::offPlayerControl();
	
	if (mObjArg0 < 0) {
		MR::stopSubBGM(45);
		MR::stopStageBGM(45);
	}
}

if (AlexSus == mObjArg2 + 60) { //Phase 2: Set the player state to Wait to prevent animation issues and then warp Mario to the specified GeneralPos. Or, go to a galaxy.
	MR::setPlayerStateWait();

	if (mObjArg0 >= 0) {

		char WarpAreaDestPos[0xF];
	
		sprintf(WarpAreaDestPos, "WarpAreaPos%03d", mObjArg0);
		MR::setPlayerPosAndWait(WarpAreaDestPos);
		OSReport("(WarpArea) Warping player to WarpArea %s\n", WarpAreaDestPos);
	
		if (MR::findNamePos(WarpAreaDestPos, &pos, &pos))
		OSReport("(WarpArea) WarpArea position %s does exist! Warping...\n", WarpAreaDestPos);
		else
		OSReport("(WarpArea) WarpArea position %s does not exist! Skipping.\n", WarpAreaDestPos);
	
		}
		else 
			SPack::WarpAreaStageTable(mObjArg5);
		}

	if (AlexSus == mObjArg2 + 90) { //Phase 3: Open the circle wipe, restore player control, and reset both "AlexSus" and "hasEnteredArea" vars.
		SPack::selectWipeOpen(mObjArg3, mObjArg4); //The "hasEnteredArea" var is used to prevent a softlock caused by leaving the WarpArea during the circle wipe closure.
		MR::onPlayerControl(1);
		AlexSus = 0;
		hasEnteredArea = 0;
		}
	}

const char* WarpArea::getManagerName() const {
	return "SwitchArea";
}