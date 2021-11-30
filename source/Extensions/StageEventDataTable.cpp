#include "spack/Extensions/StageEventDataTable.h"
#include "Util.h"
#include "spack/Util/ActorUtil.h"

/*
* Authors: Evanbowl
* 
* StageEventDataTable:
*
* Code optimation help by Galaxy Master
*
* These functions un-hardcode certain options a stage can have. Those being:
* Chimp, Glider, Disable Pause, Play Star Chance, and Purple Coin Gearmo.
*/

namespace SPack {

	const char *typestr = 0;

	void* SEDTbcsv = loadArcAndFile("/SystemData/PTSystemData.arc", "/System/StageEventDataTable.bcsv");

    //StageEventDataTable Parser
	bool StageEventDataTable(const char* value) {

		JMapInfo* exceptTable = new JMapInfo();
		exceptTable->attach(SEDTbcsv);
		s32 numEntries = MR::getCsvDataElementNum(exceptTable);

		const char *currentStage = MR::getCurrentStageName();
		s32 currentScenario = MR::getCurrentScenarioNo();

		for (s32 i = 0; i < numEntries; i++) {
			const char *exceptStage = 0;
			s32 exceptScenario = 0;
			MR::getCsvDataStr(&exceptStage, exceptTable, "StageName", i);
			if (!exceptStage)
				continue;
			MR::getCsvDataStr(&typestr, exceptTable, "Flags", i);
			if (!typestr)
				continue;
			MR::getCsvDataS32(&exceptScenario, exceptTable, "ScenarioNo", i);

			if ((exceptScenario == 0 || exceptScenario == currentScenario) &&
			    MR::isEqualStringCase(currentStage, exceptStage) && strstr(typestr, value)) {
				//OSReport("(StageEventDataTable) Stage: %s, Scenario: %d, Flags: %s\n", exceptStage, currentScenario, typestr);
				return true;
				}
		}
		return false;
	}

	bool isChimp() {
		return SPack::StageEventDataTable("Chimp"); //Makes the set galaxy a Chimp stage. If you apply this to a non-chimp star, the high scores will not work.
	}

	bool isPauseDisabled() {
		return SPack::StageEventDataTable("DisablePause"); //Completely disables the pause menu in the set galaxy.
	}

	bool isGlider() {
		return SPack::StageEventDataTable("Glider"); //Makes the set galaxy a Glider stage.
	}

	bool isDisableStarChanceBGM() {
		return !SPack::StageEventDataTable("DisableStarChanceBGM"); //Makes the set galaxy not set the BGM to star chance after a star spawn.
	}

	bool isPurpleCoinCaretaker() {
		return SPack::StageEventDataTable("PurpleCoinsOver100"); //Makes purple coins in the set galaxy behave like Tall Trunk/Rolling Coaster.
	}
	
	bool isStoryBook() {
    if (MR::isEqualStageName("FileSelect")) //FileSelect is "special", and will crash if it returns true on this stage.
        return false;
    return SPack::StageEventDataTable("StoryBook"); //Makes the set galaxy a Story Book stage, adding a story book border and preventing return to the Starship.
	}
	
    kmBranch(0x800568F0, isChimp);
	
	kmBranch(0x80056B40, isPauseDisabled);
	
	kmBranch(0x800570C0, isGlider);

	kmBranch(0x80056DF0, isDisableStarChanceBGM);
	
	kmBranch(0x80056D70, isPurpleCoinCaretaker);

    kmBranch(0x80056BE0, isStoryBook);
}