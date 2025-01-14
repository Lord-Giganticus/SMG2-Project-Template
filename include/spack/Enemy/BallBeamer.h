#pragma once

#include "LiveActor.h"
#include "Enemy/StateController.h"
#include "Enemy/RingBeam.h"
#include "Util/Functor.h"

class BallBeamer : public LiveActor {
public:
	BallBeamer(const char *pName);

	virtual void init(const JMapInfoIter &rIter);
	virtual void control();
	virtual void attackSensor(HitSensor *pSender, HitSensor *pReceiver);
	virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor *pSender, HitSensor *pReceiver);

	void syncSwitchOnA();
	void syncSwitchOffA();
	void syncSwitchOnB();
	void setupAttack();
	void exeWait();
	void exeAttack();
	void exeInter();
	void exeBind();

	WalkerStateBindStarPointer* mBindStarPointer;
	ActorStateParamScale* mParamScale;
	RingBeam** mBeams;
	s32 mNumMaxBeams;
	s32 mAttackDuration;
	f32 mActiveRange;
	bool mActive;
	Mtx mHeadMtx;
};

namespace NrvBallBeamer {
	NERVE(NrvWait);
	NERVE(NrvAttack);
	NERVE(NrvInter);
	NERVE(NrvBind);
};
