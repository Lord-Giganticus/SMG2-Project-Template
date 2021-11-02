#include "spack/Util/ActorUtil.h"
#include "Enemy/StateController/AnimScaleController.h"
#include "LiveActor/LiveActor.h"
#include "Util/LiveActorUtil.h"
#include "Util/MtxUtil.h"
#include "Util.h"

/*
* Authors: Aurum
*/
namespace SPack {
	void initPosRotFromRailRider(LiveActor *pActor) {
		RailRider* railRider = pActor->mRailRider;

		f32 coord = railRider->calcNearestPos(pActor->mTranslation);
		railRider->setCoord(coord);

		updatePosRotFromRailRider(pActor);
	}

	void updatePosRotFromRailRider(LiveActor *pActor) {
		RailRider* railRider = pActor->mRailRider;

		// Calculate up vector
		TVec3f upVec(0.0f, 1.0f, 0.0f);

		if (MR::isCalcGravity(pActor))
			upVec.set<f32>(-pActor->mGravity);

		// Calculate rotation matrix from gravity (up) and rail direction (front)
		Mtx mtx;
		TVec3f euler;
		MR::makeMtxUpFront((TPositionMtx*)&mtx, upVec, railRider->mCurrentDirection);

		// Extract euler angles and convert them to degrees
		((TRotationMtx&)mtx).getEulerXYZ(euler);
		euler *= RAD2DEG;

		// Update actor's position and rotation
		pActor->mTranslation.set<f32>(railRider->mCurrentPos);
		pActor->mRotation.set<f32>(euler);
	}

	AnimScaleController* createSamboAnimScaleController(AnimScaleParam *pAnimScaleParam) {
		pAnimScaleParam->_10 = 4.0f;
		pAnimScaleParam->_14 = 1.0f;
		pAnimScaleParam->_18 = 0.06f;
		pAnimScaleParam->_1C = 2.0f;
		pAnimScaleParam->_20 = 0.06f;
		pAnimScaleParam->_24 = 0.4f;
		pAnimScaleParam->_28 = 0.8f;

		return new AnimScaleController(pAnimScaleParam);
	}

	void useTimerSE(LiveActor* pActor, s32 pTime) {

	s32 step = pActor->getNerveStep();

    if (step <= pTime) {
        if (!MR::isPlayerDead() && !MR::isPowerStarGetDemoActive()) {
            if (pTime == step)
                MR::startSystemSE("SE_SY_TIMER_A_0", -1, -1);
            else if ((step % 60) == 0) {
                if (step >= (pTime - 120))
                    MR::startSystemSE("SE_SY_TIMER_A_1", -1, -1);
                else
                    MR::startSystemSE("SE_SY_TIMER_A_2", -1, -1);
                }
           }
        }
	}
}
