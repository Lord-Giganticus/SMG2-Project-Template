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
    void isWarpArea();
};
