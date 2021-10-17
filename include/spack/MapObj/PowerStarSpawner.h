#pragma once

#include "LiveActor.h"
#include "MapObj/PowerStar.h"

class ActorCameraInfo;
class PowerStar;

class PowerStarSpawner : public LiveActor {
public:
	PowerStarSpawner(const char *);

	virtual void init(JMapInfoIter const &);
	virtual void movement();
	virtual void spawnAtMario(f32 offset);
	virtual void createDisplayStar();

	ActorCameraInfo* mCamInfo;
	s32 mScenario;
	s32 mDelay;
	s32 mElapsed;
	s32 mSpawnMode;
	s32 GroupID;
	s32 mFromMario;
	bool mUseSuccessSE;
	bool arg1;
	bool arg2;
	bool arg3;
	s32 mUseDisplayModel;
	ModelObj* DisplayStar;
	TVec3f upVec;
	Mtx DisplayStarMtx;
	f32 YOffset;
};
