#pragma once

#include "syati.h"

namespace SPack {
    bool StageEventDataTable(const char* value);
	
	bool isPauseDisabled();
	bool isChimp();
	bool isDisableStarChanceBGM();
	bool isPurpleCoinCaretaker();
	bool isStoryBook();
	
	void WarpAreaParser(s32 selectedindex);
	void selectWipeClose(s32 type, s32 fadeTime);
	void selectWipeOpen(s32 type, s32 fadeTime);
    void useWarpAreaFadeIn();
};
