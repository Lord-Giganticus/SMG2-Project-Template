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
*
* Custom symbols: 1
* setupColor__8ModelObjFPC7NameObjl=0x802DFA20
* Reason: PowerStar isn't documented.
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
    YOffset = 300.0f;
    mUseSuccessSE = -1;
    mUseDisplayModel = -1;
    GroupID = -1;
}

void PowerStarSpawner::init(JMapInfoIter const& rIter) {
    MR::initDefaultPos(this, rIter);

    MR::processInitFunction(this, "StarPiece", false);
    MR::hideModel(this); //A model is specified then hidden since it is not neccessary, or else the ModelObj will crash the game.

	MR::connectToSceneMapObj(this);
	MR::invalidateClipping(this); //This object will never unload when offscreen.

	MR::useStageSwitchReadA(this, rIter); //Reads SW_A.

	MR::getJMapInfoArg0NoInit(rIter, &mScenario); //Star ID
	MR::getJMapInfoArg1NoInit(rIter, &mDelay); //Delay before spawn.
	MR::getJMapInfoArg2NoInit(rIter, &mFromMario); //Should the Star start it's spawn path at Mario?
	MR::getJMapInfoArg3NoInit(rIter, &mUseSuccessSE); //Play a sound when activated?
	MR::getJMapInfoArg4NoInit(rIter, &mSpawnMode); //Time Stop/Instant Appear/Squizzard Spawn
    MR::getJMapInfoArg5NoInit(rIter, &YOffset); //This arg determines the Y offset if the Spawn At Mario functionality is used.
    MR::getJMapInfoArg6NoInit(rIter, &mUseDisplayModel); //Show display model?

    MR::getJMapInfoGroupID(rIter, &GroupID); //This will cause the PowerStarSpawner to start and end the Power Star's spawn path at Mario.

    initSound(1, "PowerStarSpawner", false, TVec3f(0.0f, 0.0f, 0.0f)); //Initializes Sound
    MR::declarePowerStar(this, mScenario); //Declares the star determined by mScenario.
    makeActorAppeared();

    if (GroupID >= 0) {
    MR::joinToGroupArray(this, rIter, "パワースター出現ポイントグループ", 0x10); //Joins the Power Star to.
	MR::initActorCamera(this, rIter, &mCamInfo); //This is used to initialize an actor camera.
    }

    //Display Star code
    if (mUseDisplayModel >= 0) {
    DisplayStar = new ModelObj("パワースター", "PowerStar", NULL, -2, -2, -2, false);
    DisplayStar->mTranslation.set(mTranslation); //Moves the DisplayStar to where the PowerStarSpawner is.
    DisplayStar->mRotation.set(mRotation); //Sets the DisplayStar's rotation to what the PowerStarSpawner's rotation is.
    MR::emitEffect(DisplayStar, "Light"); //Emits the PowerStar effect "Light" on the DisplayStar.

    if (MR::hasPowerStarInCurrentStage(mScenario)) //Checks if you have the displayed star.
    MR::startBva(DisplayStar, "PowerStarColor"); //This line starts the PowerStarColor BVA animation. I used it to set the star color to clear.
    else
    DisplayStar->setupColor(DisplayStar, mScenario); //Checks what color the Star ID set to mScenario is. It then sets the color on the DisplayStar accordingly.

    //This is so that if you already have the star that is being displayed, it just displays the clear texture instead of setting up the color.
    //This also fixes an issue that collected bronze stars display gold.

	DisplayStar->appear(); //Makes the DisplayStar visible.
    MR::invalidateShadowAll(DisplayStar); //Shadows are not needed so they are hidden.
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
    DisplayStar->mRotation.add(TVec3f(0.0f, 3.0f, 0.0f)); //Start adding 3 rotation units per frame.

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
