#include "spack/Extensions/WarpAreaParser.h"
#include "Util.h"
#include "Util/StageUtil.h"
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
* 
*
* WarpAreaStageTable:
*
* This function is a list of stages that the WarpArea can take the player to.
*
*/

namespace SPack {

	bool warpareaused = 0;
	const char *destStage = 0;
	s32 destScenario = 0;
	s32 destGreenStarScenario = -1;
	s32 CSVFadeInType = 0;
	s32 FadeInType = 0;
	s32 CSVFadeInTime = 0;
	s32 FadeInTime = 45;
	s32 bcsvIndex = 0;

	void* WASTbcsv = loadArcAndFile("/SystemData/PTSystemData.arc", "/System/WarpAreaStageTable.bcsv");
	
	void WarpAreaParser(s32 selectedindex) {

		JMapInfo* StageTable = new JMapInfo();
		StageTable->attach(WASTbcsv);
		s32 entries = MR::getCsvDataElementNum(StageTable);

		for (s32 i = 0; i < entries; i++) {

			MR::getCsvDataStr(&destStage, StageTable, "StageName", i);
			MR::getCsvDataS32(&destScenario, StageTable, "ScenarioNo", i);
			MR::getCsvDataS32(&destGreenStarScenario, StageTable, "GreenStarScenarioNo", i);
			MR::getCsvDataS32(&CSVFadeInType, StageTable, "WipeType", i);
			MR::getCsvDataS32(&CSVFadeInTime, StageTable, "WipeTime", i);
            MR::getCsvDataS32(&bcsvIndex, StageTable, "Index", i);

        if (selectedindex == bcsvIndex) {
		    if (destScenario < 1 || destScenario > 8)
            OSReport("(WarpAreaParser) %d is not a valid scenario number. Skipping.\n", destScenario);

			if (destGreenStarScenario != -1 || destGreenStarScenario > 3 || destGreenStarScenario == 0)
            OSReport("(WarpAreaParser) %d is not a valid green star scenario number. Skipping.\n", destGreenStarScenario);
			
            FadeInType = CSVFadeInType; //Separate variables are used to prevent the needed values from being overwritten by the next row in the BCSV.
			FadeInTime = CSVFadeInTime; //Awful and janky, but it works.

			OSReport("(WarpAreaParser) Going to %s %d, Green Star %d, Wipe Type: %d, Wipe Time: %d, BCSV Index: %d\n", destStage, destScenario, destGreenStarScenario, FadeInType, FadeInTime, bcsvIndex);

            if (destGreenStarScenario > 0)
			destGreenStarScenario += 3;

            MR::goToGalaxy(destStage);
            MR::goToGalaxyNoSelection(destStage, destScenario, destGreenStarScenario, 0);
		    warpareaused = true;
		   }
		}

	}

	void selectWipeClose(s32 type, s32 fadeTime)
	{
    if (fadeTime == -1)
	fadeTime = 45;
	    if (type < 1)
	    MR::closeSystemWipeCircle(fadeTime);
	    if (type == 1)
	    MR::closeSystemWipeFade(fadeTime);
	    if (type == 2)
	    MR::closeSystemWipeWhiteFade(fadeTime);
	    if (type == 3)
	    MR::closeSystemWipeCircleWithCaptureScreen(fadeTime);
	    if (type == 4)
	    MR::closeSystemWipeFadeWithCaptureScreen(fadeTime);
	    if (type == 5)
	    MR::closeSystemWipeMario(fadeTime);
	}

	void selectWipeOpen(s32 type, s32 fadeTime)
	{
    if (fadeTime == -1)
	fadeTime = 45;

		if (type < 1)
	    MR::openSystemWipeCircle(fadeTime);
		if (type == 1)
	    MR::openSystemWipeFade(fadeTime);
		if (type == 2)
	    MR::openSystemWipeWhiteFade(fadeTime);
		if (type == 3)
	    MR::openSystemWipeMario(fadeTime);
	}

	void useWarpAreaFadeIn() {
        if (warpareaused == true) {
		selectWipeOpen(FadeInType, FadeInTime);
		warpareaused = false;
		}
        else
        MR::openSystemWipeWhiteFade(90);
	}

kmCall(0x804B44D0, useWarpAreaFadeIn);

}