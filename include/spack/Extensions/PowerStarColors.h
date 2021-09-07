#pragma once

#include "syati.h"

namespace SPack {
	s32 getPowerStarColor(const char *pStage, s32 scenarioId);
	s32 getPowerStarColorCurrentStage(s32 scenarioId);

	wchar_t *getStarIcon(s32 startype);

	wchar_t *starIcon();
	wchar_t *cometStarIcon();
	wchar_t *hiddenStarIcon();
};

