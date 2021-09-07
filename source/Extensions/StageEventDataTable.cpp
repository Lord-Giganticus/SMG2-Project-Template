#include "spack/Extensions/StageEventDataTable.h"
#include "Util.h"
#include "spack/Util/LoadResource.h"
#include "nw4rtest.h"
#include "Util/FileUtil.h"
/*
* Authors: Evanbowl
* 
* Code optimation help by Galaxy Master
*
* These functions un-hardcode certain options a stage can have. Those being:
* Chimp, Glider, Disable Pause, Play Star Chance, and Purple Coin Gearmo.
* 
*/

namespace SPack {

    void* SEDTarc = Syati::loadArchive("/SystemData/StageEventDataTable.arc");
    void* SEDTbcsv = Syati::loadResourceFromArchive("/SystemData/StageEventDataTable.arc", "StageEventDataTable.bcsv");

	bool StageEventDataTable(const char* value) {

		JMapInfo* exceptTable = new JMapInfo();
		exceptTable->attach(SEDTbcsv);
		s32 numEntries = MR::getCsvDataElementNum(exceptTable);

		const char *currentStage = MR::getCurrentStageName();
		s32 currentScenario = MR::getCurrentScenarioNo();

		for (s32 i = 0; i < numEntries; i++) {
			const char *exceptStage = 0;
			s32 exceptScenario = 0;
			const char *typestr = 0;

			MR::getCsvDataStr
			(&exceptStage, exceptTable, "StageName", i);
			if (!exceptStage)
				continue;
			MR::getCsvDataStr(&typestr, exceptTable, "Type", i);
			if (!typestr)
				continue;
			MR::getCsvDataS32(&exceptScenario, exceptTable, "ScenarioNo", i);

			if ((exceptScenario == 0 || exceptScenario == currentScenario) &&
			    MR::isEqualStringCase(currentStage, exceptStage)
				&& strstr(typestr, value)) {
				OSReport("(StageEventDataTable) Type: %s, Stage: %s, Scenario: %d\n", value, currentStage, currentScenario);
				return true;
				}
		}
		return false;
	}

	bool isChimp() {
		return SPack::StageEventDataTable("Chimp");
	}

	bool isPauseDisabled() {
		return SPack::StageEventDataTable("DisablePause");
	}

	bool isGlider() {
		return SPack::StageEventDataTable("Glider");
	}

	bool isDisableStarChanceBGM() {
		return !SPack::StageEventDataTable("DisableStarChanceBGM");
	}

	bool isPurpleCoinCaretaker() {
		return SPack::StageEventDataTable("PurpleCoinsOver100");
	}
	
	bool isStoryBook() {
    if (MR::isEqualStageName("FileSelect")) //FileSelect is "special", and will crash if it returns true on this stage.
        return false;
    return SPack::StageEventDataTable("StoryBook");
	}

    kmBranch(0x800568F0, isChimp);
	
	kmBranch(0x80056B40, isPauseDisabled);
	
	kmCall(0x803592C0, isGlider);

	kmBranch(0x80056DF0, isDisableStarChanceBGM);
	
	kmBranch(0x80056D70, isPurpleCoinCaretaker);

    kmBranch(0x80056BE0, isStoryBook);
}
