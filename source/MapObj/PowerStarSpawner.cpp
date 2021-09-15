#include "spack/MapObj/PowerStarSpawner.h"
#include "Util.h"
#include "Player/MarioAccess.h"
#include "Player/MarioActor.h"

/*
* Created by Aurum, Evanbowl & Someone, with Group ID support thanks to Zyphro.
* 
* Spawns the specified Power Star if its SW_A is activated. The delay until the star spawns as well
* as the position from where the star spawns can be specified as well.
*
* This object is a bit overdeveloped but is one of my favorites.
*
* One of my favorite objects!
*/

PowerStarSpawner::PowerStarSpawner(const char* pName) : LiveActor(pName) {
	mScenario = 1;
	mDelay = 0;
	mElapsed = 0;
    mCamInfo = 0;
	mFromMario = -1;
    mSpawnMode = -1;
    mDebug = -1;
    arg1 = 0;
    arg2 = 0;
    arg3 = 0;
    YOffset = 300.0f;
    mUseSuccessSE = -1;
    mUseDisplayModel = -1;
    GroupID = -1;
}

void PowerStarSpawner::init(JMapInfoIter const& rIter) {
    MR::initDefaultPos(this, rIter);

    MR::processInitFunction(this, "StarPiece", false); //Shows a star piece where the star will spawn when activated.
    MR::hideModel(this);

    if (MR::getJMapInfoArg7NoInit(rIter, &mDebug) == 1)
    MR::processInitFunction(this, "StarPiece", false); //Shows a star piece where the star will spawn when activated.

	MR::connectToSceneMapObj(this);
	MR::invalidateClipping(this);

	MR::useStageSwitchReadA(this, rIter); //Reads SW_A.

    //This uses a LOT of args.
	MR::getJMapInfoArg0NoInit(rIter, &mScenario);
	MR::getJMapInfoArg1NoInit(rIter, &mDelay); //Delay before spawn.
	MR::getJMapInfoArg2NoInit(rIter, &mFromMario); //Should the Star start it's spawn path at Mario?
	MR::getJMapInfoArg3NoInit(rIter, &mUseSuccessSE); //Play a sound when activated?
	MR::getJMapInfoArg4NoInit(rIter, &mSpawnMode); //Time Stop/Instant Appear/Squizzard Spawn
    MR::getJMapInfoArg5NoInit(rIter, &YOffset); //This arg determines the Y offset if the Spawn At Mario functionality is used.
    MR::getJMapInfoArg6NoInit(rIter, &mUseDisplayModel);

    //Note: There WAS an obj_arg to allow for a Power Star display model to be shown, but was removed.
    //It was removed because the DummyDisplayModel code reads obj_arg7 no matter what, so it had to go.

    MR::getJMapInfoGroupID(rIter, &GroupID); //This will cause the PowerStarSpawner to start and end the Power Star's spawn path at Mario.

    initSound(1, "PowerStarSpawner", false, TVec3f(0.0f, 0.0f, 0.0f)); //Initializes Sound
    MR::declarePowerStar(this, mScenario); //Declares the star determined by mScenario.
    makeActorAppeared();

    if (GroupID >= 0) {
    MR::joinToGroupArray(this, rIter, "パワースター出現ポイントグループ", 0x10); //Joins the Power Star to the Group Array.
	MR::initActorCamera(this, rIter, &mCamInfo); //The bane of my existence.\
    This is used to initialize an actor camera.
    }

    //Display Star code
    if (mUseDisplayModel >= 0) {
    DisplayStar = new ModelObj("パワースター", "PowerStar", NULL, -2, -2, -2, false);
    DisplayStar->mTranslation.set(mTranslation);
    DisplayStar->mRotation.set(mRotation);
    MR::emitEffect(DisplayStar, "Light");

    if (MR::hasPowerStarInCurrentStage(mScenario))
    MR::startBva(DisplayStar, "PowerStarColor");
    else
    DisplayStar->setupColor(DisplayStar, mScenario);
    //This is so that if you already have the star being displayed, it just displays the clear texture instead of setting up the color.
    //This also fixes an issue that collected bronze stars display gold.

	DisplayStar->appear();
    }
}


void PowerStarSpawner::getStarSpawnPos(f32 offset) {
    MR::setPosition(this, *MR::getPlayerPos()); //Teleports the PowerStarSpawner to Mario

    MarioActor* playeractor = MarioAccess::getPlayerActor(); //Gets the player actor.
    TVec3f gravityvec = *playeractor->MarioActor::getGravityVec(); //Gets the gravity vector of the player.
    JMAVECScaleAdd((Vec*)&gravityvec, (Vec*)&mTranslation, (Vec*)&mTranslation, offset*-1); //This function always moves the Power Star Spawner to above Mario relative to the current gravity.\
    Var "offset" is multiplied by -1 because a negative offset means up.
    }

void PowerStarSpawner::movement() {
    if (mFromMario == 1 && GroupID <= 0)
    PowerStarSpawner::getStarSpawnPos(250); //This is used to bypass an issue where if a star starts it's spawn path at the player, it de-rails the player off of launch star rails.

    if (GroupID >= 0)
    PowerStarSpawner::getStarSpawnPos(YOffset); //This function moves the Power Star Spawner to Mario and also puts it above him relative to the current gravity, only if a Group ID is set.
    
    if (mUseDisplayModel == 1)
    DisplayStar->mRotation.add(TVec3f(0.0f, 3.0f, 0.0f));

    if (mUseDisplayModel >= 0)
    DisplayStar->mTranslation.set(mTranslation);

	if (MR::isOnSwitchA(this)) {
		mElapsed++;

		if (mElapsed == 1 && mUseSuccessSE)
        MR::startLevelSound(this, "OjPowerStarSpawnerSuccess", -1, -1, -1); //Plays sound.

		if (mElapsed >= mDelay) {

            switch (mSpawnMode) {
            case 0: //time continues during demo
            arg1 = 1;
            break;
            case 1: //star appears instantly
            arg2 = 1;
            break;
            case 2: //squizzard spawn
            arg3 = 1;
            break;
            case 3: //time continues during demo
            arg1 = 1;
            arg3 = 1;
            break;
            }

            MR::appearEventPowerStar("PowerStarSpawner", mScenario, &mTranslation, arg1, arg2, arg3);

            if (mUseDisplayModel >= 0)
            DisplayStar->kill();

            makeActorDead();
               }
       }
}
