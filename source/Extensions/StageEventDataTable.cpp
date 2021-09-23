#include "spack/Extensions/StageEventDataTable.h"
#include "Util.h"
#include "spack/Util/LoadResource.h"
#include "Util/FileUtil.h"
/*
* Authors: Evanbowl
* 
* StageEventDataTable:
*
* Code optimation help by Galaxy Master
*
* These functions un-hardcode certain options a stage can have. Those being:
* Chimp, Glider, Disable Pause, Play Star Chance, and Purple Coin Gearmo.
* 
*
* WarpAreaStageTable:
*
* This function is a list of stages that the WarpArea can take the player to.
*
*/

namespace SPack {

	const char *typestr = 0;

    void* SEDTarc = Syati::loadArchive("/SystemData/StageEventDataTable.arc");
    void* SEDTbcsv = Syati::loadResourceFromArchive("/SystemData/StageEventDataTable.arc", "StageEventDataTable.bcsv");
    void* WASTbcsv = Syati::loadResourceFromArchive("/SystemData/StageEventDataTable.arc", "WarpAreaStageTable.bcsv");

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

    //WarpAreaStageTable Parser
	bool warpareaused = 0;

	void WarpAreaParser(s32 selectedindex) {
		JMapInfo* StageTable = new JMapInfo();
		StageTable->attach(WASTbcsv);
		s32 numEntries = MR::getCsvDataElementNum(StageTable);

		const char *destStage = 0;
		s32 destScenario = 0;
		s32 bcsvIndex = 0;

		for (s32 i = 0; i < numEntries; i++) {

			MR::getCsvDataStr(&destStage, StageTable, "StageName", i);
			MR::getCsvDataS32(&destScenario, StageTable, "ScenarioNo", i);
            MR::getCsvDataS32(&bcsvIndex, StageTable, "Index", i);

        if (selectedindex == bcsvIndex) {
           MR::goToGalaxy(destStage);
           MR::goToGalaxyNoSelection(destStage, destScenario, -1, 0);
		   OSReport("(WarpAreaStageTable) Stage: %s, Scenario: %d, Index: %d\n", destStage, destScenario, bcsvIndex);
		   warpareaused = true;
		   }
		}
	}

	void isWarpArea() {
        if (warpareaused == true)
        MR::openWipeCircle(45),
		warpareaused = false;
        else
        MR::openSystemWipeWhiteFade(90);
	}

kmCall(0x804B44D0, isWarpArea);
}
