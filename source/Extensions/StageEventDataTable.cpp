#include "spack/Extensions/StageEventDataTable.h"
#include "Util.h"
#include "custom/archive.h"
#include "Util/StageUtil.h"
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

	void* loadPTPictureFont() {
	Syati::loadArchive("/SystemData/PTPictureFont.arc");
	return Syati::loadResourceFromArchive("/SystemData/PTPictureFont.arc", "PTPictureFont.brfnt");
	}

	kmCall(0x804B8048, loadPTPictureFont);

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
		return SPack::StageEventDataTable("Chimp"); //Makes the set galaxy a Chimp stage. If you apply this to a non-chimp star, the high scores will not work. This should be fixed soon.
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

    //WarpAreaStageTable Parser
	bool warpareaused = 0;
	const char *destStage = 0;
	s32 destScenario = 0;
	s32 CSVFadeInType = 0;
	s32 FadeInType = 0;
	s32 CSVFadeInTime = 0;
	s32 FadeInTime = 45;
	s32 bcsvIndex = 0;

	void WarpAreaParser(s32 selectedindex) {
		JMapInfo* StageTable = new JMapInfo();
		StageTable->attach(WASTbcsv);
		s32 numEntries = MR::getCsvDataElementNum(StageTable);

		for (s32 i = 0; i < numEntries; i++) {

			MR::getCsvDataStr(&destStage, StageTable, "StageName", i);
			MR::getCsvDataS32(&destScenario, StageTable, "ScenarioNo", i);
			MR::getCsvDataS32(&CSVFadeInType, StageTable, "FadeInType", i);
			MR::getCsvDataS32(&CSVFadeInTime, StageTable, "FadeInTime", i);
            MR::getCsvDataS32(&bcsvIndex, StageTable, "Index", i);

        if (selectedindex == bcsvIndex) {
		    if (destScenario < 1 || destScenario > 8)
            OSReport("(WarpAreaStageTable) %d is not a valid scenario number. Skipping.\n", destScenario);

            FadeInType = CSVFadeInType; //Separate variables are used to prevent the needed values from being overwritten by the next row in the BCSV.
			FadeInTime = CSVFadeInTime; //Weird solution, but it works.

            MR::goToGalaxy(destStage);
            MR::goToGalaxyNoSelection(destStage, destScenario, -1, 0);

		    OSReport("(WarpAreaStageTable) Going to %s %d, BCSV Index: %d\n", destStage, destScenario, bcsvIndex);
			OSReport("Fade Type: %d, Fade Time: %d\n", FadeInType, FadeInTime);
		    warpareaused = true;
		    
		   }
		}
	}

	void selectWipeClose(s32 type, s32 fadeTime) // mode 0 = Close, mode 1 = Open
	{
    if (type == -1)
	type = 0;
    if (fadeTime == -1)
	fadeTime = 45;
	    if (type == 0)
	    MR::closeSystemWipeCircle(fadeTime);
	    else if (type == 1)
	    MR::closeSystemWipeFade(fadeTime);
	    else if (type == 2)
	    MR::closeSystemWipeWhiteFade(fadeTime);
	    else if (type == 3)
	    MR::closeSystemWipeCircleWithCaptureScreen(fadeTime);
	    else if (type == 4)
	    MR::closeSystemWipeFadeWithCaptureScreen(fadeTime);
	    else if (type == 5)
	    MR::closeSystemWipeMario(fadeTime);
	}

	void selectWipeOpen(s32 type, s32 fadeTime) // mode 0 = Close, mode 1 = Open
	{
	if (type == -1)
	type = 0;
    if (fadeTime == -1)
	fadeTime = 45;
	    if (type == 0)
	    MR::openSystemWipeCircle(fadeTime);
	    else if (type == 1)
	    MR::openSystemWipeFade(fadeTime);
	    else if (type == 2)
	    MR::openSystemWipeWhiteFade(fadeTime);
	    else if (type == 3)
	    MR::openSystemWipeMario(fadeTime);
	}

	void useWarpAreaFadeIn() {
        if (warpareaused == true) {
		OSReport("Fade Type: %d Fade Time: %d\n", FadeInType, FadeInTime);
		selectWipeOpen(FadeInType, FadeInTime);
		warpareaused = false;
		}
        else
        MR::openSystemWipeWhiteFade(90);
	}

kmCall(0x804B44D0, useWarpAreaFadeIn);
}
