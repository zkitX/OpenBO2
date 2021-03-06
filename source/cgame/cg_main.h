#pragma once

#include "cg_hudelem.h"
#include "cg_visionsets.h"
#include "cg_vehicle.h"
#include <bgame/bg_faction.h>
#include <client/cl_main.h>
#include <game/g_weapon.h>

enum objectiveState_t
{
	OBJST_EMPTY = 0x0,
	OBJST_ACTIVE = 0x1,
	OBJST_INVISIBLE = 0x2,
	OBJST_NUMSTATES = 0x3,
};

struct playerEntity_t
{
	float fLastWeaponPosFrac;
	int bPositionToADS;
	vec3_t vPositionLastOrg;
	float fLastIdleFactor;
	vec3_t baseMoveOrigin;
	vec3_t baseMoveAngles;
};

struct objective_t
{
	objectiveState_t state;
	vec3_t origin;
	__int16 entNum;
	vec2_t size;
	int icon;
	__int16 ownerNum;
	unsigned __int16 name;
	__int16 teamMask;
	char progress;
	int clientUseMask[1];
	char gamemodeFlags;
	char flags;
	char teamNum;
};

struct playerState_s
{
	int commandTime;
	int pm_type;
	int bobCycle;
	int pm_flags;
	__int64 weapFlags;
	int otherFlags;
	int pm_time;
	unsigned int loopSoundId;
	int loopSoundFade;
	vec3_t origin;
	vec3_t velocity;
	int remoteEyesEnt;
	int remoteEyesTagname;
	int remoteControlEnt;
	int weaponTime;
	int weaponDelay;
	int weaponTimeLeft;
	int weaponDelayLeft;
	int weaponIdleTime;
	int grenadeTimeLeft;
	int throwBackGrenadeOwner;
	int throwBackGrenadeTimeLeft;
	int weaponRestrictKickTime;
	bool mountAvailable;
	bool bRunLeftGun;
	bool bCarryingTurret;
	vec3_t mountPos;
	float mountDir;
	bool bThirdPerson;
	int foliageSoundTime;
	int gravity;
	float leanf;
	int speed;
	vec3_t delta_angles;
	int groundEntityNum;
	int moverEntityNum;
	int moverTimestamp;
	int groundType;
	vec3_t vLadderVec;
	int jumpTime;
	float jumpOriginZ;
	int slideTime;
	int moveType;
	int legsTimer;
	int torsoTimer;
	__int16 legsAnim;
	__int16 torsoAnim;
	int legsAnimDuration;
	int torsoAnimDuration;
	int damageTimer;
	int damageDuration;
	int dmgDirection;
	int dmgType;
	int corpseIndex;
	int movementDir;
	int eFlags;
	int eFlags2;
	PlayerVehicleState vehicleState;
	__int16 predictableEventSequence;
	__int16 predictableEventSequenceOld;
	int predictableEvents[4];
	unsigned int predictableEventParms[4];
	__int16 unpredictableEventSequence;
	__int16 unpredictableEventSequenceOld;
	int unpredictableEvents[4];
	unsigned int unpredictableEventParms[4];
	ClientNum_t clientNum;
	Weapon offHandWeapon;
	OffhandSecondaryClass offhandSecondary;
	OffhandPrimaryClass offhandPrimary;
	renderOptions_s renderOptions;
	int momentum;
	Weapon weapon;
	Weapon lastStandPrevWeapon;
	Weapon lastWeaponAltModeSwitch;
	Weapon stowedWeapon;
	char unusedCompatibilityPadding;
	Weapon meleeWeapon;
	int weaponstate;
	int weaponstateLeft;
	unsigned int weaponShotCount;
	unsigned int weaponShotCountLeft;
	float fWeaponPosFrac;
	int adsDelayTime;
	int spreadOverride;
	int spreadOverrideState;
	float weaponSpinLerp;
	int viewmodelIndex;
	vec3_t viewangles;
	int viewHeightTarget;
	float viewHeightCurrent;
	int viewHeightLerpTime;
	int viewHeightLerpTarget;
	int viewHeightLerpDown;
	vec2_t viewAngleClampBase;
	vec2_t viewAngleClampRange;
	int damageEvent;
	int damageYaw;
	int damagePitch;
	int damageCount;
	int stats[4];
	PlayerHeldWeapon heldWeapons[15];
	AmmoPool ammoNotInClip[15];
	AmmoClip ammoInClip[15];
	float proneDirection;
	float proneDirectionPitch;
	float proneTorsoPitch;
	ViewLockTypes viewlocked;
	__int16 viewlocked_entNum;
	int vehiclePos;
	int vehicleType;
	int vehicleAnimBoneIndex;
	int linkFlags;
	vec3_t linkAngles;
	int cursorHint;
	int cursorHintString;
	int cursorHintEntIndex;
	int cursorHintWeapon;
	int iCompassPlayerInfo;
	unsigned int spyplaneTypeEnabled;
	unsigned int satelliteTypeEnabled;
	int locationSelectionInfo;
	locSel_t locationSelectionType;
	SprintState sprintState;
	int lastDtpEnd;
	float fTorsoPitch;
	float fWaistPitch;
	float holdBreathScale;
	int holdBreathTimer;
	int chargeShotTimer;
	unsigned int chargeShotLevel;
	unsigned int shotsFiredFromChamber;
	float quickScopeScale;
	int quickScopeTimer;
	unsigned int clientFields;
	unsigned int clientFields2;
	unsigned int clientFields3;
	unsigned int entityStateClientFields;
	float moveSpeedScaleMultiplier;
	MantleState mantleState;
	int vehicleAnimStage;
	int vehicleEntryPoint;
	unsigned int scriptedAnim;
	int scriptedAnimTime;
	int meleeChargeEnt;
	int meleeChargeDist;
	int meleeChargeTime;
	int weapLockFlags;
	int weapLockedEntnum;
	unsigned int airburstMarkDistance;
	unsigned int perks[2];
	ActionSlotType actionSlotType[4];
	ActionSlotParam actionSlotParam[4];
	Weapon inventoryWeapon;
	__int16 wiiumoteAimX;
	__int16 wiiumoteAimY;
	char wiiuControllerType;
	char vehicleDefIndex;
	int entityEventSequence;
	int weapAnim;
	int weapAnimLeft;
	float aimSpreadScale;
	int shellshockIndex;
	int shellshockTime;
	int shellshockDuration;
	float dofNearStart;
	float dofNearEnd;
	float dofFarStart;
	float dofFarEnd;
	float dofNearBlur;
	float dofFarBlur;
	float dofViewmodelStart;
	float dofViewmodelEnd;
	int waterlevel;
	int smokeColorIndex;
	int hudElemLastAssignedSoundID;
	int adsZoomSelect;
	int adsZoomLatchTime;
	bool adsZoomLatchState;
	int adsPrevZoomSelect;
	int adsPrevZoomSelectTime;
	int artilleryInboundIconLocation;
	float visionSetLerpRatio;
	int poisoned;
	int binoculars;
	int scriptCursorHintString;
	objective_t objective[32];
	int deltaTime;
	int killCamEntity;
	int killCamTargetEntity;
	int introShotsFired;
	struct {
		hudelem_s current[31];
		hudelem_s archival[31];
	} hud;
};

struct cg_t
{
	ClientNum_t clientNum;
	LocalClientNum_t localClientNum;
	DemoType demoType;
	CubemapShot cubemapShot;
	int cubemapSize;
	int renderScreen;
	int latestSnapshotNum;
	int latestSnapshotTime;
	snapshot_s* snap;
	snapshot_s* nextSnap;
	snapshot_s activeSnapshots[2];
	bool slowMotionModified;
	float frameInterpolation;
	int frametime;
	int time;
	int oldTime;
	int physicsTime;
	int mapRestart;
	bool bThirdPerson;
	thirdPersonType renderingThirdPerson;
	playerState_s predictedPlayerState;
	int lastPlayerStateOverride;
	centity_t predictedPlayerEntity;
	playerEntity_t playerEntity;
	int predictedErrorTime;
	vec3_t predictedError;
	playerState_s lastUIPlayerState;
	float landChange;
	int landTime;
	float heightToCeiling;
	int heightToCeilingTS;
	refdef_t refdef;
	vec3_t refdefViewAngles;
	Camera cameraData;
	float lastPelvisCameraOffset;
	vec3_t baseGunAngles;
	vec3_t swayViewAngles;
	vec3_t swayAngles;
	vec3_t swayOffset;
	vec3_t recoilAngles;
	vec3_t recoilSpeed;
	vec3_t damageKickAngles;
	int previousRecoilTime;
	float previousRecoilRatio;
	vec3_t lastStandSwayAngles;
	vec3_t lastStandSwayAVel;
	vec3_t lastStandSwayTarget;
	vec3_t radiantCameraOrigin;
	vec3_t radiantCameraAngles;
	int radiantCamInUse;
	int iEntityLastType[1792];
	XModel* pEntityLastXModel[1792];
	float zoomSensitivity;
	bool isLoading;
	char objectiveText[1024];
	team_indicator teamIndicator;
	bool colorBlindAssist;
	int teamPlayerMask[10][1];
	int vehicleInitView;
	vec3_t prevVehicleInvAxis[3];
	bool vehicleViewLocked;
	vec3_t vehicleViewLockedAngles;
	int missileInitView;
	vec3_t storedRemoteCameraAngles;
	vec3_t storedRemoteCameraOrigin;
	char scriptMainMenu[256];
	bool initialPlayersConnected;
	bool initialPlayersConnectedRespondedTo;
	int numScores;
	int teamScores[10];
	unsigned int teamHasMeat[10];
	int teamPings[10];
	int teamPlayers[10];
	ClientNum_t scoreOrder[18];
	scoreboardColumnType_t scoreboardColumnTypes[5];
	int matchUIVisibilityFlags;
	unsigned int talkFlags;
	objectiveMapInfo_t objectiveMapInfo[64][2];
	objective_t clientObjectives[32];
	int bombTimer[2];
	int showScores;
	int scoreFadeTime;
	int scoresTop;
	int scoresOffBottom;
	int scoresBottom;
	int selectedGamerIndex;
	int bannerLines[10];
	unsigned __int64 selectedGamerXuid;
	int selectedGamerClientNum;
	int drawHud;
	int timeScaleTimeStart;
	int timeScaleTimeEnd;
	float timeScaleStart;
	float timeScaleEnd;
	ClientNum_t crosshairClientNum;
	int crosshairClientLastTime;
	int crosshairClientStartTime;
	unsigned int crosshairTraceDistance;
	unsigned int airburstMarkDistance;
	int airburstMarkStartTime;
	int airburstMarkEndTime;
	int airburstMarkInProgress;
	int gpsJammerLastInactive;
	int bulletsFiredContinuously;
	int bulletsFiredContinuouslyLeft;
	int bulletsFiredStopTime;
	int cursorHintIcon;
	int cursorHintTime;
	int cursorHintFade;
	int cursorHintString;
	int vehicleHintTime;
	int lastVehicleSeat;
	int driverHintShown;
	int gunnerHintShown;
	int lastClipFlashTime;
	InvalidCmdHintType invalidCmdHintType;
	int invalidCmdHintTime;
	SuccessfulCmdHintType successfulCmdHintType;
	int successfulCmdHintTime;
	int lastHealthPulseTime;
	int lastHealthLerpDelay;
	int lastHealthClient;
	float lastHealth;
	float healthOverlayFromAlpha;
	float healthOverlayToAlpha;
	int healthOverlayPulseTime;
	int healthOverlayPulseDuration;
	int healthOverlayPulsePhase;
	bool healthOverlayHurt;
	int healthOverlayLastHitTime;
	float healthOverlayOldHealth;
	int healthOverlayPulseIndex;
	int proneBlockedEndTime;
	int lastStance;
	int lastStanceChangeTime;
	int lastStanceFlashTime;
	int voiceTime;
	Weapon weaponSelect;
	int weaponSelectTime;
	bool weaponForcedSelect;
	Weapon weaponLatestPrimary;
	Weapon lastWeaponForAlt[15];
	unsigned int viewModelFx;
	Weapon prevViewmodelWeapon;
	bool prevViewmodelWeaponInASD;
	Weapon equippedOffHand;
	viewDirectionalHitIndicator_t directionalHitIndicator[4];
	viewDamage_t viewDamage[8];
	int damageTime;
	float damageX;
	float damageY;
	float damageValue;
	float viewFade;
	int waterDropCount;
	int waterDropStartTime;
	int waterDropStopTime;
	int weaponIdleTime;
	int nomarks;
	int clearMarks;
	int newPlayerViewmodel;
	bool weaponLaserOff;
	int v_dmg_time;
	float v_dmg_pitch;
	float v_dmg_roll;
	float v_dmg_flash;
	float fBobCycle;
	float xyspeed;
	vec3_t kickAVel;
	vec3_t kickAngles;
	vec3_t offsetAngles;
	float gunPitch;
	float gunYaw;
	float gunXOfs;
	float gunYOfs;
	float gunZOfs;
	vec3_t flamethrowerKickOffset;
	float minigunSpinAngle;
	vec3_t viewModelAxis[4];
	float rumbleScale;
	float compassNorthYaw;
	vec2_t compassNorth;
	Material* compassMapMaterial;
	Material* compassMapScrambledMaterial;
	vec2_t compassMapUpperLeft;
	vec2_t compassMapWorldSize;
	int compassLastTime;
	float compassYaw;
	float compassSpeed;
	int compassFadeTime;
	int healthFadeTime;
	int ammoFadeTime;
	int stanceFadeTime;
	int sprintFadeTime;
	int offhandFadeTime;
	int offhandFlashTime;
	int waypointFadeTime;
	int vehicleControlsFadeTime;
	shellshock_t shellshock;
	struct {
		int time;
		int duration;
	} testShock;
	int shellshockSoundActive;
	int holdBreathTime;
	int holdBreathInTime;
	int holdBreathDelay;
	float holdBreathFrac;
	float radarProgress;
	float satelliteProgress;
	int radarType;
	vec2_t selectedLocation;
	vec2_t selectedYaw;
	int typewriterStartTime;
	vec2_t selectedLocations[3];
	int lastTimeSelected;
	SprintState sprintStates;
	int adsViewErrorDone;
	int inKillCam;
	int wasInKillCam;
	bool killCamFirstFrameRan;
	int killCamEntity;
	int invalidKillCamEntity;
	KillCamEntityType killCamEntityType;
	int killCamLastEntityNum;
	vec3_t killCamLastEntityOrg;
	vec3_t killCamLastEntityAngles;
	KillCamEntityRestState killCamEntityRestState;
	int killCamLookAtEntity;
	vec3_t killCamLookAt;
	vec3_t killCamHelicopterOffset;
	__int16 killCamLastVehicleDefIndex;
	int killCamStoppedTime;
	float killCamStoppedDecelTime;
	vec3_t killCamStoppedPos;
	vec3_t killCamStoppedVel;
	vec3_t killCamPrevBombOrigin;
	int killCamLerpEndTime;
	vec3_t killCamOldViewAngles;
	vec3_t killCamOldViewOrg;
	bgs_t bgs;
	cpose_t viewModelPose;
	visionSetVars_t visionSetPreLoaded[8];
	char visionSetPreLoadedName[8][64];
	visionSetVars_t visionSetFrom[8];
	visionSetVars_t visionSetTo[8];
	visionSetVars_t visionSetCurrent[8];
	visionSetLerpData_t visionSetLerpData[8];
	char visionNameNaked[64];
	char visionNameNight[64];
	char visionNameLastStand[64];
	char visionNameFlare[64];
	char visionNameInfrared[64];
	char visionNameTvguided[64];
	char visionNameExtraCam[64];
	bool drewReticle;
	bitarray<64> extraButton_bits;
	bool breathPressedLastFrame;
	int lastActionSlotTime;
	bool playerTeleported;
	float flareRatioLastFrame;
	int stepViewStart;
	float stepViewChange;
	struct {
		float aimSpreadScale;
	} lastFrame;
	int oldCommandTime;
	vec3_t oldOrigin;
	vec3_t oldViewangles;
	transPlayerState_t oldTransPlayerState;
	hudElemSoundInfo_t hudElemSound[32];
	int nextRippleTime;
	unsigned int impactEffectsNext;
	unsigned int impactEffects[256];
	float warDesiredMomentumProgress;
	float warCurrentMomentumProgress;
	int warMomentumMultiplier;
	int warMomentumBlitzkriegTime;
	int warMomentumFlashTime;
	float gunnerPitchOffset;
	unsigned int centInNextSnapshot[32];
	centity_t* slideFakeEnt;
	int cameraLinkedEntitiesCount;
	int cameraLinkedEntities[4];
	link_type_e cameraLinkedEntitiesType[4];
	vec3_t cameraLinkedEntitiesOffset[4];
	int groundTiltEntNum;
	int poisoned;
	Material* genericMaterialMap[64];
	bool genericFilterPassEnabled[8][4];
	Material* genericFilterPassMaterial[8][4];
	int genericFilterPassTarget[8][4];
	int genericFilterPassSampler0[8][4];
	int genericFilterPassSampler1[8][4];
	float genericFilterPassParam[8][4][16];
	int genericFilterPassQuads[8][4];
	int genericFilterBitFlag[8];
	bool genericOverlayEnabled[3];
	Material* genericOverlayMaterial[3];
	float genericOverlayParam[3][16];
	int genericOverlaySortIndex[3];
	float genericOverlayParamADS[8];
	bool genericOverlayEnableADS;
	int swimming;
	int swimmingStartTime;
	int missileViewWobbleLastTime;
	vec3_t missileViewWobbleFreq;
	vec3_t missileViewWobbleAmp;
	int counterSpinTime;
	float counterSpinTarget;
	float counterSpinAngle;
	int extraCamEntity;
	int extraCamActive;
	int extraCamStatic;
	vec3_t extraCamOrigin;
	vec3_t extraCamAngles;
	int hasLocalRadar;
	vec2_t localRadarPos;
	float localRadarProgress;
	int globalScramblerActive;
	float scramblerEnemyAlpha;
	float scramblerFriendlyAlpha;
	int popUpSwayStartTime;
	float popupRotationAngle;
	float popUpAngleDelta;
	CompassScrambler compassScrambler[16];
	centity_t* closestEnemyScrambler;
	int alliesFlagAway;
	int axisFlagAway;
	int resetWeaponStateFlag;
	int delayedShieldHitEventsCount;
	ShieldHitDelayedEvent delayedShieldHitEvents[2];
	int recentKilledByClientNum;
	int recentKilledByTime;
	int recentVictimClientNums[8];
	int recentVictimTimes[8];
	int recentVictimIndex;
	bool talkingToEnemy;
	struct FogSettings {
		float start;
		float r;
		float g;
		float b;
		float density;
		float heightDensity;
		float baseHeight;
		float fogColorScale;
		float sunFogColorR;
		float sunFogColorG;
		float sunFogColorB;
		float sunFogDirX;
		float sunFogDirY;
		float sunFogDirZ;
		float sunFogStartAng;
		float sunFogEndAng;
		float fogMaxOpacity;
	} serverFogSettings;
	bool forceInfraredMaterialsStreaming;
	bool sonarAttachmentOn;
	bool hudOutlineOn;
	bool sonarAttachmentFullscreenOn;
	DroppedWeaponDobjInfo droppedWeaponDobjInfo[64];
	vec3_t traceHitPos;
	int traceHitDistance;
	bool traceHitEnemy;
	int rangeFinderStartTime;
	bool rangeFinderTimerActive;
	bool rangeFinderPerformTrace;
	bool rangeFinderPerformFxVisTest;
	int playerVehicleEntity;
	bool clientCollisionLinked;
	float altScopeFrac;
	int lastAltScopeFracUpdateTime;
	bool useAlternateAimAssistParams;
	bool useFlashLight;
	int empStartTime;
	int lastEmpTime;
	footstepEvent_t footstepEvents[16];
	unsigned int numFootstepEvents;
	int movers[64];
	int moversCount;
	bullet_hit_info_t hit_infos[12];
	int num_hit_infos;
	int hit_charge;
	bool hit_possible;
	bool bClientNeedsConnectCallback;
	targetInfo_t targets[32];
	snd_ambient_script_data soundAmbientData[32];
	int numAmbientDataStructs;
	snd_ambient_script_data* defaultAmbientRoom;
	snd_ambient_collider soundAmbientColliderData[64];
	int currentAmbientRoom;
	int currentAmbientPackage;
	unsigned int ambientRoomOverride;
	unsigned int ambientPackageOverride;
	int shoutcastListenInBits;
	Weapon selectedClassWeapon;
	int gumpTransitionState;
	vec3_t gumpTransitionColor;
	float gumpTransitionAlpha;
	int gumpTransitionTime;
	int hudDpadShown;
	int resetLuiNeeded;
	int hostMigrationOccured;
	int safeSpectatorCamTime;
	vec3_t safeSpectatorCamPos;
	vec3_t safeSpectatorCamOffset;
	team_t team;
};


class cg_main
{
};
