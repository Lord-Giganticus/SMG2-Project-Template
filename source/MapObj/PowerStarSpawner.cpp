#include "spack/MapObj/PowerStarSpawner.h"
#include "Util.h"
#include "Player/MarioAccess.h"
#include "Player/MarioActor.h"
#include "spack/Extensions/PowerStarColors.h"
/*
* Created by Aurum, Evanbowl & Someone, with Group ID support thanks to Zyphro.
* 
* Spawns the specified Power Star if its SW_A is activated. The delay until the star spawns as well
* as the position from where the star spawns can be specified as well.
*
* This object is a bit overdeveloped but is one of my favorites.
*
* One of my favorite objects!
*
*/

PowerStarSpawner::PowerStarSpawner(const char* pName) : LiveActor(pName) {
	mScenario = 1;
	mDelay = 0;
	mElapsed = 0;
    mCamInfo = 0;
	mFromMario = -1;
    mSpawnMode = -1;
    arg1 = 0;
    arg2 = 0;
    arg3 = 0;
    mUseSuccessSE = -1;
    mUseDisplayModel = -1;
    GroupID = -1;
	upVec = (TVec3f(0.0f, 1.0f, 0.0f));
    YOffset = 300.0f;
}

void PowerStarSpawner::init(JMapInfoIter const& rIter) {
    MR::initDefaultPos(this, rIter);

    MR::processInitFunction(this, "StarPiece", false);
    MR::hideModel(this); //A model is specified then hidden since it is not neccessary, or else the ModelObj will crash the game.

	MR::connectToSceneMapObj(this);
	MR::invalidateClipping(this); //This object will never unload when offscreen.

	MR::useStageSwitchReadA(this, rIter); //Reads SW_A.
    MR::calcGravity(this);

	MR::getJMapInfoArg0NoInit(rIter, &mScenario); //Star ID
	MR::getJMapInfoArg1NoInit(rIter, &mSpawnMode); //Time Stop/Instant Appear/Squizzard Spawn
	MR::getJMapInfoArg2NoInit(rIter, &mDelay); //Delay before spawn.
    MR::getJMapInfoArg3NoInit(rIter, &mUseSuccessSE); //Play a sound when activated?
	MR::getJMapInfoArg4NoInit(rIter, &mFromMario); //Should the Star start it's spawn path at Mario?
    MR::getJMapInfoArg5NoInit(rIter, &mUseDisplayModel); //Show display model?
    MR::getJMapInfoArg6NoInit(rIter, &YOffset); //Y Offset if "Spawn At Mario" is used.

    MR::getJMapInfoGroupID(rIter, &GroupID); //This will cause the PowerStarSpawner to start and end the Power Star's spawn path at Mario.

    initSound(1, "PowerStarSpawner", false, mTranslation); //Initializes Sound
    MR::declarePowerStar(this, mScenario); //Declares the star determined by mScenario.
    makeActorAppeared();

    if (GroupID >= 0)
    MR::joinToGroupArray(this, rIter, "Star", 0x10), MR::initActorCamera(this, rIter, &mCamInfo);
    //Joins the group array to the star. This allows the PowerStarSpawner to behave like a PowerStarAppearPoint.

    if (mUseDisplayModel >= 0)
    PowerStarSpawner::createDisplayStar();
}


void PowerStarSpawner::createDisplayStar() {
    DisplayStar = new ModelObj("パワースター", "PowerStar", mUseDisplayModel ? (MtxPtr)DisplayStarMtx : NULL, -2, -2, -2, false);

    MR::setMtxTrans((MtxPtr)DisplayStarMtx, mTranslation); //Set the mtx translation to the PowerStarSpawner's mTranslation.

    MR::emitEffect(DisplayStar, "Light"); //Starts the PowerStar effect "Light" on the DisplayStar.
    MR::invalidateShadowAll(DisplayStar); //Shadows are not needed so they are hidden.

    //Start custom SetupColor function
    if (MR::hasPowerStarInCurrentStage(mScenario)) //Checks if you have the specified star.
	MR::startBva(DisplayStar, "PowerStarColor");
    //If you have the specified star, ignore setting the color up and just set to clear.
	else {
    s32 frame = SPack::getPowerStarColor(MR::getCurrentStageName(), mScenario);
	MR::startBtp(DisplayStar, "PowerStarColor");
    MR::startBrk(DisplayStar, "PowerStarColor");
    MR::startBtk(DisplayStar, "PowerStarColor");
	MR::startBva(DisplayStar, "PowerStarColor");

	MR::setBtpFrameAndStop(DisplayStar, frame);
	MR::setBrkFrameAndStop(DisplayStar, frame);
	MR::setBtkFrameAndStop(DisplayStar, frame);
	MR::setBvaFrameAndStop(DisplayStar, 0);
    //If you do not have the specified star, start setting up the color animations and setting up the animation frames.
    }
    //End custom SetupColor function

    if (mUseDisplayModel == 1)
	upVec.set<f32>(-mGravity), //Sets the up vector to what the gravity is. This allows the DisplayStar to calculate it's gravity, like the normal PowerStar.
	MR::makeMtxUpFront((TPositionMtx*)&DisplayStarMtx, upVec, mTranslation);

    DisplayStar->appear();
}

void PowerStarSpawner::spawnAtMario(f32 offset) {
    MR::setPosition(this, *MR::getPlayerPos()); //Teleports the PowerStarSpawner to Mario

    MarioActor* playeractor = MarioAccess::getPlayerActor();
    TVec3f gravityvec = *playeractor->MarioActor::getGravityVec();
    JMAVECScaleAdd((Vec*)&gravityvec, (Vec*)&mTranslation, (Vec*)&mTranslation, offset*-1);
    }

void PowerStarSpawner::movement() {
    if (mFromMario == 1 && GroupID < 0)
    PowerStarSpawner::spawnAtMario(250); //This is used to bypass an issue where if a star starts it's spawn path at the player, it de-rails the player off of launch star rails.

    if (GroupID >= 0)
    PowerStarSpawner::spawnAtMario(YOffset); //This function moves the Power Star Spawner to Mario and also puts it above him relative to the current gravity, only if a Group ID is set.

    if (mUseDisplayModel == 1)
    MR::rotateMtxLocalYDegree((MtxPtr)&DisplayStarMtx, 3),
    MR::setMtxTrans((MtxPtr)&DisplayStarMtx, mTranslation);

    if (mUseDisplayModel == 0)
    upVec.set<f32>(-mGravity),
    DisplayStar->mRotation.set(upVec);

    DisplayStar->mTranslation.set(mTranslation);

	if (MR::isOnSwitchA(this)) {
		mElapsed++;

		if (mElapsed == 1 && mUseSuccessSE)
        MR::startLevelSound(this, "OjPSSOn", -1, -1, -1); //Plays sound.

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
