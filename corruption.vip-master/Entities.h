
#pragma once

#include "MiscDefinitions.h"
#include "ClientRecvProps.h"
#include "offsets.h"
#include "Vector.h"
extern float intervalPerTick;
#define TEAM_CS_T 2
#define TEAM_CS_CT 3
extern Vector LastAngleAA;
#define BONE_USED_BY_HITBOX			0x00000100
#define DO_ONCE                                       \
    for (static std::atomic<int> _run_already_(false); \
         !_run_already_.fetch_or(true);)               \

#define IMPLEMENT_SINGLETON(classname)\
public:\
	static std::shared_ptr<classname> GetInstance() {\
		static std::shared_ptr<classname> instance(new classname);\
		return instance;\
	}\
private:\
	classname() {}\
	classname(classname const&) = delete;\
	void operator=(classname const&) = delete;

#define IMPLEMENT_SINGLETON_INITIALIZED(classname)\
public:\
	static std::shared_ptr<classname> GetInstance() {\
		static std::shared_ptr<classname> instance(new classname);\
		return instance;\
	}\
private:\
	classname() {Initialize();}\
	classname(classname const&) = delete;\
	void operator=(classname const&) = delete;

#define ptr( x, x1, x2 ) *(x*)( (DWORD)x1 + (DWORD)x2 )

class IClientRenderable;
class IClientNetworkable;
class IClientUnknown;
class IClientThinkable;
class IClientEntity;
class CSWeaponInfo;
struct WeaponInfo_t;


class CHudTexture
{
public:
	char type[64]; //0x0000
	char subtype[64]; //0x0040
	char unknowndata00[2]; //0x0080
	char charinFont; //0x0082
	char unknowndata01[1]; //0x0083
};//Size=0x00AC

class CSWeaponInfo
{
public:
	virtual ~CSWeaponInfo() {};
	char*		consoleName;			// 0x0004
	char		pad_0008[12];			// 0x0008
	int			iMaxClip1;				// 0x0014
	int			iMaxClip2;				// 0x0018
	int			iDefaultClip1;			// 0x001C
	int			iDefaultClip2;			// 0x0020
	char		pad_0024[8];			// 0x0024
	char*		szWorldModel;			// 0x002C
	char*		szViewModel;			// 0x0030
	char*		szDroppedModel;			// 0x0034
	char		pad_0038[4];			// 0x0038
	char*		N0000023E;				// 0x003C
	char		pad_0040[56];			// 0x0040
	char*		szEmptySound;			// 0x0078
	char		pad_007C[4];			// 0x007C
	char*		szBulletType;			// 0x0080
	char		pad_0084[4];			// 0x0084
	char*		szHudName;				// 0x0088
	char*		szWeaponName;			// 0x008C
	char		pad_0090[56];			// 0x0090
	int 		WeaponType;				// 0x00C8
	int			iWeaponPrice;			// 0x00CC
	int			iKillAward;				// 0x00D0
	char*		szAnimationPrefix;		// 0x00D4
	float		flCycleTime;			// 0x00D8
	float		flCycleTimeAlt;			// 0x00DC
	float		flTimeToIdle;			// 0x00E0
	float		flIdleInterval;			// 0x00E4
	bool		bFullAuto;				// 0x00E8
	char		pad_0x00E5[3];			// 0x00E9
	int			iDamage;				// 0x00EC
	float		flArmorRatio;			// 0x00F0
	int			iBullets;				// 0x00F4
	float		flPenetration;			// 0x00F8
	float		flFlinchVelocityModifierLarge;	// 0x00FC
	float		flFlinchVelocityModifierSmall;	// 0x0100
	float		flRange;				// 0x0104
	float		flRangeModifier;		// 0x0108
	float		flThrowVelocity;		// 0x010C
	char		pad_0x010C[12];			// 0x0110
	bool		bHasSilencer;			// 0x011C
	char		pad_0x0119[3];			// 0x011D
	char*		pSilencerModel;			// 0x0120
	int			iCrosshairMinDistance;	// 0x0124
	int			iCrosshairDeltaDistance;// 0x0128 - iTeam?
	float		flMaxPlayerSpeed;		// 0x012C
	float		flMaxPlayerSpeedAlt;	// 0x0130
	float		flSpread;				// 0x0134
	float		flSpreadAlt;			// 0x0138
	float		flInaccuracyCrouch;		// 0x013C
	float		flInaccuracyCrouchAlt;	// 0x0140
	float		flInaccuracyStand;		// 0x0144
	float		flInaccuracyStandAlt;	// 0x0148
	float		flInaccuracyJumpInitial;// 0x014C
	float		flInaccuracyJump;		// 0x0150
	float		flInaccuracyJumpAlt;	// 0x0154
	float		flInaccuracyLand;		// 0x0158
	float		flInaccuracyLandAlt;	// 0x015C
	float		flInaccuracyLadder;		// 0x0160
	float		flInaccuracyLadderAlt;	// 0x0164
	float		flInaccuracyFire;		// 0x0168
	float		flInaccuracyFireAlt;	// 0x016C
	float		flInaccuracyMove;		// 0x0170
	float		flInaccuracyMoveAlt;	// 0x0174
	float		flInaccuracyReload;		// 0x0178
	int			iRecoilSeed;			// 0x017C
	float		flRecoilAngle;			// 0x0180
	float		flRecoilAngleAlt;		// 0x0184
	float		flRecoilAngleVariance;	// 0x0188
	float		flRecoilAngleVarianceAlt;	// 0x018C
	float		flRecoilMagnitude;		// 0x0190
	float		flRecoilMagnitudeAlt;	// 0x0194
	float		flRecoilMagnitudeVariance;	// 0x0198
	float		flRecoilMagnitudeVarianceAlt;	// 0x019C
	float		flRecoveryTimeCrouch;	// 0x01A0
	float		flRecoveryTimeStand;	// 0x01A4
	float		flRecoveryTimeCrouchFinal;	// 0x01A8
	float		flRecoveryTimeStandFinal;	// 0x01AC
	int			iRecoveryTransitionStartBullet;// 0x01B0 
	int			iRecoveryTransitionEndBullet;	// 0x01B4
	bool		bUnzoomAfterShot;		// 0x01B8
	bool		bHideViewModelZoomed;	// 0x01B9
	char		pad_0x01B5[2];			// 0x01BA
	char		iZoomLevels[3];			// 0x01BC
	int			iZoomFOV[2];			// 0x01C0
	float		fZoomTime[3];			// 0x01C4
	char*		szWeaponClass;			// 0x01D4
	float		flAddonScale;			// 0x01D8
	char		pad_0x01DC[4];			// 0x01DC
	char*		szEjectBrassEffect;		// 0x01E0
	char*		szTracerEffect;			// 0x01E4
	int			iTracerFrequency;		// 0x01E8
	int			iTracerFrequencyAlt;	// 0x01EC
	char*		szMuzzleFlashEffect_1stPerson; // 0x01F0
	char		pad_0x01F4[4];			 // 0x01F4
	char*		szMuzzleFlashEffect_3rdPerson; // 0x01F8
	char		pad_0x01FC[4];			// 0x01FC
	char*		szMuzzleSmokeEffect;	// 0x0200
	float		flHeatPerShot;			// 0x0204
	char*		szZoomInSound;			// 0x0208
	char*		szZoomOutSound;			// 0x020C
	float		flInaccuracyPitchShift;	// 0x0210
	float		flInaccuracySoundThreshold;	// 0x0214
	float		flBotAudibleRange;		// 0x0218
	char		pad_0x0218[8];			// 0x0220
	char*		pWrongTeamMsg;			// 0x0224
	bool		bHasBurstMode;			// 0x0228
	char		pad_0x0225[3];			// 0x0229
	bool		bIsRevolver;			// 0x022C
	bool		bCannotShootUnderwater;	// 0x0230
};


enum class CSGOClassID
{
	CAK47 = 1,
	CBaseAnimating = 2,
	CBaseAnimatingOverlay = 3,
	CBaseAttributableItem = 4,
	CBaseButton = 5,
	CBaseCombatCharacter = 6,
	CBaseCombatWeapon = 7,
	CBaseCSGrenade = 8,
	CBaseCSGrenadeProjectile = 9,
	CBaseDoor = 10,
	CBaseEntity = 11,
	CBaseFlex = 12,
	CBaseGrenade = 13,
	CBaseParticleEntity = 14,
	CBasePlayer = 15,
	CBasePropDoor = 16,
	CBaseTeamObjectiveResource = 17,
	CBaseTempEntity = 18,
	CBaseToggle = 19,
	CBaseTrigger = 20,
	CBaseViewModel = 21,
	CBaseVPhysicsTrigger = 22,
	CBaseWeaponWorldModel = 23,
	CBeam = 24,
	CBeamSpotlight = 25,
	CBoneFollower = 26,
	CBreakableProp = 27,
	CBreakableSurface = 28,
	CC4 = 29,
	CCascadeLight = 30,
	CChicken = 31,
	CColorCorrection = 32,
	CColorCorrectionVolume = 33,
	CCSGameRulesProxy = 34,
	CCSPlayer = 35,
	CCSPlayerResource = 36,
	CCSRagdoll = 37,
	CCSTeam = 38,
	CDeagle = 39,
	CDecoyGrenade = 40,
	CDecoyProjectile = 41,
	CDynamicLight = 42,
	CDynamicProp = 43,
	CEconEntity = 44,
	CEconWearable = 45,
	CEmbers = 46,
	CEntityDissolve = 47,
	CEntityFlame = 48,
	CEntityFreezing = 49,
	CEntityParticleTrail = 50,
	CEnvAmbientLight = 51,
	CEnvDetailController = 52,
	CEnvDOFController = 53,
	CEnvParticleScript = 54,
	CEnvProjectedTexture = 55,
	CEnvQuadraticBeam = 56,
	CEnvScreenEffect = 57,
	CEnvScreenOverlay = 58,
	CEnvTonemapController = 59,
	CEnvWind = 60,
	CFEPlayerDecal = 61,
	CFireCrackerBlast = 62,
	CFireSmoke = 63,
	CFireTrail = 64,
	CFish = 65,
	CFlashbang = 66,
	CFogController = 67,
	CFootstepControl = 68,
	CFunc_Dust = 69,
	CFunc_LOD = 70,
	CFuncAreaPortalWindow = 71,
	CFuncBrush = 72,
	CFuncConveyor = 73,
	CFuncLadder = 74,
	CFuncMonitor = 75,
	CFuncMoveLinear = 76,
	CFuncOccluder = 77,
	CFuncReflectiveGlass = 78,
	CFuncRotating = 79,
	CFuncSmokeVolume = 80,
	CFuncTrackTrain = 81,
	CGameRulesProxy = 82,
	CHandleTest = 83,
	CHEGrenade = 84,
	CHostage = 85,
	CHostageCarriableProp = 86,
	CIncendiaryGrenade = 87,
	CInferno = 88,
	CInfoLadderDismount = 89,
	CInfoOverlayAccessor = 90,
	CItem_shot = 91,
	CItemDogtags = 92,
	CKnife = 93,
	CKnifeGG = 94,
	CLightGlow = 95,
	CMaterialModifyControl = 96,
	CMolotovGrenade = 97,
	CMolotovProjectile = 98,
	CMovieDisplay = 99,
	CParticleFire = 100,
	CParticlePerformanceMonitor = 101,
	CParticleSystem = 102,
	CPhysBox = 103,
	CPhysBoxMultiplayer = 104,
	CPhysicsProp = 105,
	CPhysicsPropMultiplayer = 106,
	CPhysMagnet = 107,
	CPlantedC4 = 108,
	CPlasma = 109,
	CPlayerResource = 110,
	CPointCamera = 111,
	CPointCommentaryNode = 112,
	CPointWorldText = 113,
	CPoseController = 114,
	CPostProcessController = 115,
	CPrecipitation = 116,
	CPrecipitationBlocker = 117,
	CPredictedViewModel = 118,
	CProp_Hallucination = 119,
	CPropDoorRotating = 120,
	CPropJeep = 121,
	CPropVehicleDriveable = 122,
	CRagdollManager = 123,
	CRagdollProp = 124,
	CRagdollPropAttached = 125,
	CRopeKeyframe = 126,
	CSCAR17 = 127,
	CSceneEntity = 128,
	CSensorGrenade = 129,
	CSensorGrenadeProjectile = 130,
	CShadowControl = 131,
	CSlideshowDisplay = 132,
	CSmokeGrenade = 133,
	CSmokeGrenadeProjectile = 134,
	CSmokeStack = 135,
	CSpatialEntity = 136,
	CSpotlightEnd = 137,
	CSprite = 138,
	CSpriteOriented = 139,
	CSpriteTrail = 140,
	CStatueProp = 141,
	CSteamJet = 142,
	CSun = 143,
	CSunlightShadowControl = 144,
	CTeam = 145,
	CTeamplayRoundBasedRulesProxy = 146,
	CTEArmorRicochet = 147,
	CTEBaseBeam = 148,
	CTEBeamEntPoint = 149,
	CTEBeamEnts = 150,
	CTEBeamFollow = 151,
	CTEBeamLaser = 152,
	CTEBeamPoints = 153,
	CTEBeamRing = 154,
	CTEBeamRingPoint = 155,
	CTEBeamSpline = 156,
	CTEBloodSprite = 157,
	CTEBloodStream = 158,
	CTEBreakModel = 159,
	CTEBSPDecal = 160,
	CTEBubbles = 161,
	CTEBubbleTrail = 162,
	CTEClientProjectile = 163,
	CTEDecal = 164,
	CTEDust = 165,
	CTEDynamicLight = 166,
	CTEEffectDispatch = 167,
	CTEEnergySplash = 168,
	CTEExplosion = 169,
	CTEFireBullets = 170,
	CTEFizz = 171,
	CTEFootprintDecal = 172,
	CTEFoundryHelpers = 173,
	CTEGaussExplosion = 174,
	CTEGlowSprite = 175,
	CTEImpact = 176,
	CTEKillPlayerAttachments = 177,
	CTELargeFunnel = 178,
	CTEMetalSparks = 179,
	CTEMuzzleFlash = 180,
	CTEParticleSystem = 181,
	CTEPhysicsProp = 182,
	CTEPlantBomb = 183,
	CTEPlayerAnimEvent = 184,
	CTEPlayerDecal = 185,
	CTEProjectedDecal = 186,
	CTERadioIcon = 187,
	CTEShatterSurface = 188,
	CTEShowLine = 189,
	CTesla = 190,
	CTESmoke = 191,
	CTESparks = 192,
	CTESprite = 193,
	CTESpriteSpray = 194,
	CTest_ProxyToggle_Networkable = 194,
	CTestTraceline = 196,
	CTEWorldDecal = 197,
	CTriggerPlayerMovement = 198,
	CTriggerSoundOperator = 199,
	CVGuiScreen = 200,
	CVoteController = 201,
	CWaterBullet = 202,
	CWaterLODControl = 203,
	CWeaponAug = 204,
	CWeaponAWP = 205,
	CWeaponBaseItem = 206,
	CWeaponBizon = 207,
	CWeaponCSBase = 208,
	CWeaponCSBaseGun = 209,
	CWeaponCycler = 210,
	CWeaponElite = 211,
	CWeaponFamas = 212,
	CWeaponFiveSeven = 213,
	CWeaponG3SG1 = 214,
	CWeaponGalil = 215,
	CWeaponGalilAR = 216,
	CWeaponGlock = 217,
	CWeaponHKP2000 = 218,
	CWeaponM249 = 219,
	CWeaponM3 = 220,
	CWeaponM4A1 = 221,
	CWeaponMAC10 = 222,
	CWeaponMag7 = 223,
	CWeaponMP5Navy = 224,
	CWeaponMP7 = 225,
	CWeaponMP9 = 226,
	CWeaponNegev = 227,
	CWeaponNOVA = 228,
	CWeaponP228 = 229,
	CWeaponP250 = 230,
	CWeaponP90 = 231,
	CWeaponSawedoff = 232,
	CWeaponSCAR20 = 233,
	CWeaponScout = 234,
	CWeaponSG550 = 235,
	CWeaponSG552 = 236,
	CWeaponSG556 = 237,
	CWeaponSSG08 = 238,
	CWeaponTaser = 239,
	CWeaponTec9 = 240,
	CWeaponTMP = 241,
	CWeaponUMP45 = 242,
	CWeaponUSP = 243,
	CWeaponXM1014 = 244,
	CWorld = 245,
	DustTrail = 246,
	MovieExplosion = 247,
	ParticleSmokeGrenade = 248,
	RocketTrail = 249,
	SmokeTrail = 250,
	SporeExplosion = 251,
	SporeTrail = 252
};


enum moveTypes
{
	MOVETYPE_NONE = 0,			// never moves
	MOVETYPE_ISOMETRIC,					// For players -- in TF2 commander view, etc.
	MOVETYPE_WALK,						// Player only - moving on the ground
	MOVETYPE_STEP,						// gravity, special edge handling -- monsters use this
	MOVETYPE_FLY,						// No gravity, but still collides with stuff
	MOVETYPE_FLYGRAVITY,				// flies through the air + is affected by gravity
	MOVETYPE_VPHYSICS,					// uses VPHYSICS for simulation
	MOVETYPE_PUSH,						// no clip to world, push and crush
	MOVETYPE_NOCLIP,					// No gravity, no collisions, still do velocity/avelocity
	MOVETYPE_LADDER,					// Used by players only when going onto a ladder
	MOVETYPE_OBSERVER,					// Observer movement, depends on player's observer mode
	MOVETYPE_CUSTOM,					// Allows the entity to describe its own physics
	MOVETYPE_LAST = MOVETYPE_CUSTOM,	// should always be defined as the last item in the list
	MOVETYPE_MAX_BITS = 4
};


enum class CSGOHitboxID
{
	Head = 0,
	Neck,
	NeckLower,
	Pelvis,
	Stomach,
	LowerChest,
	Chest,
	UpperChest,
	RightThigh,
	LeftThigh,
	RightShin,
	LeftShin,
	RightFoot,
	LeftFoot,
	RightHand,
	LeftHand,
	RightUpperArm,
	RightLowerArm,
	LeftUpperArm,
	LeftLowerArm,
	Max,
};

// Weapon IDs
enum ItemDefinitionIndex : int
{
	WEAPON_DEAGLE = 1,
	WEAPON_ELITE = 2,
	WEAPON_FIVESEVEN = 3,
	WEAPON_GLOCK = 4,
	WEAPON_AK47 = 7,
	WEAPON_AUG = 8,
	WEAPON_AWP = 9,
	WEAPON_FAMAS = 10,
	WEAPON_G3SG1 = 11,
	WEAPON_GALILAR = 13,
	WEAPON_M249 = 14,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10 = 17,
	WEAPON_P90 = 19,
	WEAPON_UMP45 = 24,
	WEAPON_XM1014 = 25,
	WEAPON_BIZON = 26,
	WEAPON_MAG7 = 27,
	WEAPON_NEGEV = 28,
	WEAPON_SAWEDOFF = 29,
	WEAPON_TEC9 = 30,
	WEAPON_TASER = 31,
	WEAPON_HKP2000 = 32,
	WEAPON_MP7 = 33,
	WEAPON_MP9 = 34,
	WEAPON_NOVA = 35,
	WEAPON_P250 = 36,
	WEAPON_SCAR20 = 38,
	WEAPON_SG556 = 39,
	WEAPON_SSG08 = 40,
	WEAPON_KNIFE_CT = 42,
	WEAPON_FLASHBANG = 43,
	WEAPON_HEGRENADE = 44,
	WEAPON_SMOKEGRENADE = 45,
	WEAPON_MOLOTOV = 46,
	WEAPON_DECOY = 47,
	WEAPON_INCGRENADE = 48,
	WEAPON_C4 = 49,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER = 60,
	WEAPON_USP_SILENCER = 61,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER = 64,
	WEAPON_KNIFE_BAYONET = 500,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT = 506,
	WEAPON_KNIFE_KARAMBIT = 507,
	WEAPON_KNIFE_M9_BAYONET = 508,
	WEAPON_KNIFE_TACTICAL = 509,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY = 515,
	WEAPON_KNIFE_PUSH = 516
};


class ScriptCreatedItem
{
public:
	CPNETVAR_FUNC(int, GetItemDefinitionIndex, 0xE67AB3B8); //m_iItemDefinitionIndex
	CPNETVAR_FUNC(int*, ItemDefinitionIndex, 0xE67AB3B8); //m_iItemDefinitionIndex
	CPNETVAR_FUNC(int*, ItemIDHigh, 0x714778A); //m_iItemIDHigh
	CPNETVAR_FUNC(int*, ItemIDLow, 0x3A3DFC74); //m_iItemIDLow
	CPNETVAR_FUNC(int*, AccountID, 0x24abbea8); //m_iAccountID
	CPNETVAR_FUNC(int*, EntityQuality, 0x110be6fe); //m_iEntityQuality
};

class AttributeContainer
{
public:
	CPNETVAR_FUNC(ScriptCreatedItem*, m_Item, 0x7E029CE5);
};



class CBaseCombatWeapon
{
public:
	bool IsGun() {
		int id = this->GetWeaponID();
		switch (id) {
		case WEAPON_DEAGLE:
		case WEAPON_ELITE:
		case WEAPON_FIVESEVEN:
		case WEAPON_GLOCK:
		case WEAPON_AK47:
		case WEAPON_AUG:
		case WEAPON_AWP:
		case WEAPON_FAMAS:
		case WEAPON_G3SG1:
		case WEAPON_GALILAR:
		case WEAPON_M249:
		case WEAPON_M4A1:
		case WEAPON_MAC10:
		case WEAPON_P90:
		case WEAPON_UMP45:
		case WEAPON_XM1014:
		case WEAPON_BIZON:
		case WEAPON_MAG7:
		case WEAPON_NEGEV:
		case WEAPON_SAWEDOFF:
		case WEAPON_TEC9:
			return true;
		case WEAPON_TASER:
			return true;
		case WEAPON_HKP2000:
		case WEAPON_MP7:
		case WEAPON_MP9:
		case WEAPON_NOVA:
		case WEAPON_P250:
		case WEAPON_SCAR20:
		case WEAPON_SG556:
		case WEAPON_SSG08:
			return true;
		case WEAPON_KNIFE_CT:
		case WEAPON_FLASHBANG:
		case WEAPON_HEGRENADE:
		case WEAPON_SMOKEGRENADE:
		case WEAPON_MOLOTOV:
		case WEAPON_DECOY:
		case WEAPON_INCGRENADE:
		case WEAPON_C4:
		case WEAPON_KNIFE_T:
			return false;
		case WEAPON_M4A1_SILENCER:
		case WEAPON_USP_SILENCER:
		case WEAPON_CZ75A:
		case WEAPON_REVOLVER:
			return true;
		default:
			return false;
		}
	}
	//CNETVAR_FUNC(float, GetPostponeFireReadyTime, 0xDB7B106E); //m_flPostponeFireReadyTime
	//CNETVAR_FUNC(float, GetNextPrimaryAttack, 0x000032B0); //m_flNextPrimaryAttack
	//CNETVAR_FUNC(int, GetAmmoInClip, 0x97B6F70C); //m_iClip1
	CNETVAR_FUNC(HANDLE, GetOwnerHandle, 0xC32DF98D); //m_hOwner
	//CNETVAR_FUNC(Vector, GetOrigin, 0x1231CE10); //m_vAbsOrigin
	//CPNETVAR_FUNC(int*, FallbackPaintKit, 0xADE4C870); // m_nFallbackPaintKit
	//CPNETVAR_FUNC(int*, FallbackSeed, 0xC2D0683D); // m_nFallbackSeed
	//CPNETVAR_FUNC(float*, FallbackWear, 0xA263576C); //m_flFallbackWear
	//CPNETVAR_FUNC(int*, FallbackStatTrak, 0x1ED78768); //m_nFallbackStatTrak
	//CPNETVAR_FUNC(int*, OwnerXuidLow, 0xAD8D897F);
	//CPNETVAR_FUNC(int*, OwnerXuidHigh, 0x90511E77);
	CPNETVAR_FUNC(int*, ViewModelIndex, 0x7F7C89C1);
	CPNETVAR_FUNC(int*, ModelIndex, 0x27016F83);
	CPNETVAR_FUNC(int*, WorldModelIndex, 0x4D8AD9F3);
	CPNETVAR_FUNC(char*, szCustomName, 0x0);
	CNETVAR_FUNC(int, GetZoomLevel, 0x26553F1A);
	CPNETVAR_FUNC(AttributeContainer*, m_AttributeManager, 0xCFFCE089);

	int& GetWeaponID() {
		return *(int*)((uintptr_t)this + 0x00000034);
	}

	Vector GetOrigin() // GetVecOrigin
	{
		return *(Vector*)((uintptr_t)this + 0x00000134);
	}

	int GetAmmoInClip()
	{
		return *(int*)((uintptr_t)this + 0x00003204);
	}
	int GetMaxAmmoInClip()
	{
		return *(int*)((uintptr_t)this + 0x00003208);
	}

	float GetNextPrimaryAttack()
	{
		return *(float*)((uintptr_t)this + 0x000031D8);
	}

	float m_flPostponeFireReadyTime()
	{
		return *(float*)((uintptr_t)this + 0x000032C8);
	}

	int* FallbackPaintKit()
	{
		return (int*)((uintptr_t)this + 0x00003170);
	}

	int* FallbackSeed()
	{
		return (int*)((uintptr_t)this + 0x00003174);
	}

	float* FallbackWear()
	{
		return (float*)((uintptr_t)this + 0x00003178);
	}

	int* FallbackStatTrak()
	{
		return (int*)((uintptr_t)this + 0x0000317C);
	}
	int* OwnerXuidLow()
	{
		return (int*)((uintptr_t)this + 0x00003168);
	}
	int* OwnerXuidHigh()
	{
		return (int*)((uintptr_t)this + 0x0000316C);
	}
	int* ItemDefinitionIndex()
	{
		return (int*)((uintptr_t)this + 0x1D8);
	}
	int GetItemDefinitionIndex()
	{
		return *(int*)((uintptr_t)this + 0x1D8);
	}
	int* ItemIDHigh()
	{
		return (int*)((uintptr_t)this + 0x1F0);
	}
	int* ItemIDLow()
	{
		return (int*)((uintptr_t)this + 0x1F4);
	}

	float GetInaccuracy()
	{
		if (!this) return 0;
		typedef float(__thiscall* oInaccuracy)(void*);
		return call_vfunc< oInaccuracy >(this, 470)(this);
	}
	float GetSpread()
	{
		if (!this) return 0;
		typedef float(__thiscall* oInaccuracy)(void*);
		return call_vfunc< oInaccuracy >(this, 439)(this);
	}

	int GetAmmoInClip2() {
		return *(int*)((DWORD)this + 0x000031F4);
	}

	int* GetEntityQuality()
	{
		return (int*)((DWORD)this + 0x2D70 + 0x40 + 0x1DC);
	}

	void SetModelIndex(int modelIndex) {
		typedef void(__thiscall* OriginalFn)(PVOID, int);
		return call_vfunc<OriginalFn>(this, 75)(this, modelIndex);
	}

	void PreDataUpdate(int updateType)
	{
		PVOID pNetworkable = (PVOID)((DWORD)(this) + 0x8);
		typedef void(__thiscall* OriginalFn)(PVOID, int);
		return call_vfunc<OriginalFn>(pNetworkable, 6)(pNetworkable, updateType);
	}
   
	float GetInnacc()
	{
		typedef float(__thiscall *OrigFn)(void *);
		return call_vfunc<OrigFn>(this, 470)(this);
	}

	void UpdateAccPenalty() 
	{
		typedef void(__thiscall *OrigFn)(void *);
		return call_vfunc<OrigFn>(this, 470)(this);
	}
	void UpdateAccuracyPenalty(CBaseCombatWeapon *pWeapon) {
		DWORD dwUpdateVMT = (*reinterpret_cast< PDWORD_PTR* >(pWeapon))[471];
		return ((void(__thiscall*)(CBaseCombatWeapon*)) dwUpdateVMT)(pWeapon);
	}

	bool IsScoped(int x = 0)
	{
		return GetZoomLevel() > 0;
	}

	CSWeaponInfo* GetCSWpnData()
	{
		if (!this) return nullptr;

		typedef CSWeaponInfo*(__thiscall* OriginalFn)(void*);
		return  call_vfunc<OriginalFn>(this, 447)(this);
	}
};

class CCSBomb
{
public:
	//CNETVAR_FUNC(HANDLE, GetOwnerHandle, 0xC32DF98D); //m_hOwner 0x29BC
	//CNETVAR_FUNC(float, GetC4BlowTime, 0x297C); //m_flC4Blow 0x297C
	//CNETVAR_FUNC(float, GetC4DefuseCountDown, 0x2994); //m_flDefuseCountDown 0x2994
	HANDLE GetOwnerHandle()
	{
		return *(HANDLE*)((uintptr_t)this + 0x000031D0);
	}

	float GetC4BlowTime()
	{
		return *(float*)((uintptr_t)this + 0x0000297C);
	}

	float GetC4DefuseCountDown()
	{
		return *(float*)((uintptr_t)this + 0x00002994);
	}

	int GetBombDefuser()
	{
		return *(int*)((uintptr_t)this + 0x000017B8);
	}

	bool IsBombDefused()
	{
		return *(bool*)((uintptr_t)this + 0x00002998);
	}
};

class CCSGrenade
{
public:

	float GetThrowTime()
	{
		return *(float*)((uintptr_t)this + 0x00003344);
	}
};

class CLocalPlayerExclusive
{
public:
	CNETVAR_FUNC(Vector, GetViewPunchAngle, 0x68F014C0);//m_viewPunchAngle
	CNETVAR_FUNC(Vector, GetAimPunchAngle, 0xBF25C290);//m_aimPunchAngle
	CNETVAR_FUNC(Vector, GetAimPunchAngleVel, 0x8425E045);//m_aimPunchAngleVel
};

class CollisionProperty
{
public:
	CNETVAR_FUNC(Vector, GetMins, 0xF6F78BAB);//m_vecMins
	CNETVAR_FUNC(Vector, GetMaxs, 0xE47C6FC4);//m_vecMaxs
	CNETVAR_FUNC(unsigned char, GetSolidType, 0xB86722A1);//m_nSolidType
	CNETVAR_FUNC(unsigned short, GetSolidFlags, 0x63BB24C1);//m_usSolidFlags
	CNETVAR_FUNC(int, GetSurroundsType, 0xB677A0BB); //m_nSurroundType

	bool IsSolid()
	{
		return (GetSolidType() != SOLID_NONE) && ((GetSolidFlags() & FSOLID_NOT_SOLID) == 0);
	}
};

class IClientRenderable
{
public:
	//virtual void*					GetIClientUnknown() = 0;
	virtual Vector const&			GetRenderOrigin(void) = 0;
	virtual Vector const&			GetRenderAngles(void) = 0;
	virtual bool					ShouldDraw(void) = 0;
	virtual bool					IsTransparent(void) = 0;
	virtual bool					UsesPowerOfTwoFrameBufferTexture() = 0;
	virtual bool					UsesFullFrameBufferTexture() = 0;
	virtual void					GetShadowHandle() const = 0;
	virtual void*					RenderHandle() = 0;
	virtual const model_t*				GetModel() const = 0;
	virtual int						DrawModel(int flags, uint8_t alpha) = 0;
	virtual int						GetBody() = 0;
	virtual void					ComputeFxBlend() = 0;
	bool SetupBones(matrix3x4_t *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
	{
		typedef bool(__thiscall* oSetupBones)(PVOID, matrix3x4_t*, int, int, float);
		return call_vfunc< oSetupBones>(this, 13)(this, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
	}
};
class AnimationLayer
{
public:
	char  pad_0000[20];
	uint32_t m_nOrder; //0x0014
	uint32_t m_nSequence; //0x0018
	float_t m_flPrevCycle; //0x001C
	float_t m_flWeight; //0x0020
	float_t m_flWeightDeltaRate; //0x0024
	float_t m_flPlaybackRate; //0x0028
	float_t m_flCycle; //0x002C
	void *m_pOwner; //0x0030 // player's thisptr
	char  pad_0038[4]; //0x0034
}; //Size: 0x0038
class IClientNetworkable
{
public:
	virtual IClientUnknown*	GetIClientUnknown() = 0;
	virtual void			Release() = 0;
	virtual ClientClass*	GetClientClass() = 0;// FOR NETVARS FIND YOURSELF ClientClass* stuffs
	virtual void			NotifyShouldTransmit( /* ShouldTransmitState_t state*/) = 0;
	virtual void			OnPreDataChanged( /*DataUpdateType_t updateType*/) = 0;
	virtual void			OnDataChanged( /*DataUpdateType_t updateType*/) = 0;
	virtual void			PreDataUpdate( /*DataUpdateType_t updateType*/) = 0;
	virtual void			PostDataUpdate( /*DataUpdateType_t updateType*/) = 0;
	virtual void			unknown();
	virtual bool			IsDormant(void) = 0;
	virtual int				GetIndex(void) const = 0;
	virtual void			ReceiveMessage(int classID /*, bf_read &msg*/) = 0;
	virtual void*			GetDataTableBasePtr() = 0;
	virtual void			SetDestroyedOnRecreateEntities(void) = 0;
};

class IClientUnknown
{
public:
	virtual void*		GetCollideable() = 0;
	virtual IClientNetworkable*	GetClientNetworkable() = 0;
	virtual IClientRenderable*	GetClientRenderable() = 0;
	virtual IClientEntity*		GetIClientEntity() = 0;
	virtual IClientEntity*		GetBaseEntity() = 0;
	virtual IClientThinkable*	GetClientThinkable() = 0;
};

class IClientThinkable
{
public:
	virtual IClientUnknown*		GetIClientUnknown() = 0;
	virtual void				ClientThink() = 0;
	virtual void*				GetThinkHandle() = 0;
	virtual void				SetThinkHandle(void* hThink) = 0;
	virtual void				Release() = 0;
};
#include "oofsetts.h"


class VarMapEntry_t {
public:
	unsigned short type;
	unsigned short m_bNeedsToInterpolate; // Set to false when this var doesn't
										  // need Interpolate() called on it anymore.
	void *data;
	void *watcher;
};

struct VarMapping_t {
	VarMapping_t() {
		m_nInterpolatedEntries = 0;
	}

	VarMapEntry_t* m_Entries;
	int m_nInterpolatedEntries;
	float m_lastInterpolationTime;
};

class __declspec (novtable)IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable
{
public:
public:
	virtual void			Release(void) = 0;
	virtual void			blahblahpad(void) = 0;
	virtual Vector&	GetAbsOrigin(void) const = 0;//in broken place use GetOrigin Below
	virtual const Vector&	GetAbsAngles(void) const = 0;

	//---                 NetVars                  ---//

	Vector reset = Vector(0, 0, 0);
	float resolved = 0.f;
	float flLastPelvisAng = 0.f;
	float flEyeAng = 0.f;
	float Backtrack[360];



	int GetGlowIndex()
	{
		return *(int*)(this + 0xA310);
	}
	bool IsAlive()
	{
		return ( GetHealth() > 0);
	}
	CPNETVAR_FUNC(CLocalPlayerExclusive*, localPlayerExclusive, 0x7177BC3E);// m_Local
	CPNETVAR_FUNC(CollisionProperty*, collisionProperty, 0xE477CBD0);//m_Collision
	CNETVAR_FUNC(float, GetLowerBodyYaw, 0xE6996CCF); //m_flLowerBodyYawTarget
	CNETVAR_FUNC(int, GetFlags, 0xE456D580); //m_fFlags
	CNETVAR_FUNC(float, GetTargetYaw, 0xE6996CCF)
	CNETVAR_FUNC(Vector, GetOrigin, 0x1231CE10); //m_vAbsOrigin
	CPNETVAR_FUNC(Vector*, GetOriginPtr, 0x1231CE10); //m_vAbsOrigin
	CNETVAR_FUNC(Vector, GetRotation, 0x6BEA197A); //m_angRotation
	CNETVAR_FUNC(int, GetTeamNum, 0xC08B6C6E); //m_iTeamNum
	CNETVAR_FUNC(int, GetMoney, 0xF4B3E183); //m_iAccount
	CNETVAR_FUNC(int, GetMaxHealth, 0xC52E1C28); //m_iMaxHealth
	//CNETVAR_FUNC(int, GetHealth, 0xA93054E3); //m_iHealth ///get health is crashing?
	CNETVAR_FUNC(float, GetFlashDuration, 0x4B5938D5); //m_flFlashDuration
	CNETVAR_FUNC(float , GetFlashAlpha, 0xFE79FB98); //m_flFlashMaxAlpha
	//CNETVAR_FUNC(unsigned char, GetLifeState, 0xD795CCFC); //m_lifeState
	CNETVAR_FUNC(HANDLE, GetActiveWeaponHandle, 0xB4FECDA3); //m_hActiveWeapon
	CNETVAR_FUNC(int, GetTickBase, 0xD472B079); //m_nTickBase
	CNETVAR_FUNC(Vector, GetViewOffset, 0xA9F74931); //m_vecViewOffset[0]
	CNETVAR_FUNC(Vector, GetViewPunch, 0x68F014C0);
	CNETVAR_FUNC(Vector, GetPunch, 0xBF25C290);
	CNETVAR_FUNC(Vector, GetVelocity, 0x40C1CA24); //m_vecVelocity[0]
	CNETVAR_FUNC(bool, HasGunGameImmunity, 0x6AD6FA0D); //m_bGunGameImmunity
	CNETVAR_FUNC(int, ArmorValue, 0x3898634); //m_ArmorValue
	//CNETVAR_FUNC(bool, HasHelmet, 0x7B97F18A); //m_bHasHelmet
	CNETVAR_FUNC(int, GetObserverMode, 0x2441D093); // m_iObserverMode
	CNETVAR_FUNC(HANDLE, GetObserverTargetHandle, 0x8660FD83); //m_hObserverTarget
	CNETVAR_FUNC(int, GetShotsFired, 0x3F2F6C66); //m_nTickBase
	CNETVAR_FUNC(float, GetSimulationTime, 0xC4560E44); //m_flSimulationTime
	CNETVAR_FUNC(float, GetAnimTime, 0xD27E8416);
	CNETVAR_FUNC(bool, IsScoped, 0x61B9C22C); //m_bIsScoped
	//CNETVAR_FUNC(bool, IsDefusing, 0xA2C14106); //m_bIsDefusing
	//CNETVAR_FUNC(int, GetPlayerCompRank, 0x75671F7F);
	// ----------------------------------------------//
#define TICK_INTERVAL			(  )
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / intervalPerTick ) )

	inline Vector ExtrapolateTick(Vector p0, Vector v0)
{
	return p0 + (v0 * intervalPerTick);
}

Vector IClientEntity::GetPredicted(Vector p0)
{
	return ExtrapolateTick(p0, this->GetVelocity());
}

	int GetHealth()
	{
			return *(int*)((uintptr_t)this + 0x000000FC);
	}

	matrix3x4_t* BoneMatrix() { return *(matrix3x4_t**)((uintptr_t)this + 0x2698); }

	Vector GetBonePos(int Bone)
	{
		const matrix3x4_t* BoneMat = BoneMatrix();
		matrix3x4_t BoneTransform = BoneMat[Bone];
		return Vector(BoneTransform[0][3], BoneTransform[1][3], BoneTransform[2][3]);
	}

	int getIdx()
	{
		void *netwable = (void*)((DWORD)(this) + 0x8);
		typedef int(__thiscall *o_GetIdx)(void*);
		return call_vfunc<o_GetIdx>(netwable, 10)(netwable);
	}
	bool IsDefusing()
	{
		return *(bool*)((uintptr_t)this + 0x00003888);
	}
	float &getSimulTime()
	{
		return *(float*)((DWORD)this + 0x00000264);
	}
	float &getOldSimulTime()
	{

		static int offs = GET_NETVAR("CBaseEntity", "m_flSimulationTime") + 0x4;
		return *(float*)((DWORD)this + offs);
	}

	VarMapping_t* GetVarMap(void* pBaseEntity) {
		return reinterpret_cast<VarMapping_t*>((DWORD)pBaseEntity + 0x24); //0x4C );
	}
	int &getFlags()
	{
		return *(int*)((DWORD)this + 0x00000100);
	}
	int getChockedTicks()
	{
		if (this->GetSimulationTime() > this->getOldSimulTime())
			return TIME_TO_TICKS(fabsf(this->GetSimulationTime() - this->getOldSimulTime()));
		return 0;
	}/*
	int getChockedPackets()
	{
		if (this->GetSimulationTime() > Global::oldSimulTime[this->getIdx()])
			return TIME_TO_TICKS(fabs(this->GetSimulationTime() - Global::oldSimulTime[this->getIdx()]));
		return 0;
	}*/

	void updateClientSideAnimation()
	{
		typedef void(__thiscall *o_updateClientSideAnimation)(void*);
		call_vfunc<o_updateClientSideAnimation>(this, 218)(this);
	}

	//m_flPoseParameter
	float &getPoseParams(int idx)
	{
		return *reinterpret_cast<float*>(reinterpret_cast<uintptr_t>(this) + 0x00002764 + sizeof(float) * idx);
	}

	
	float &getLowBodYtarg()
	{
		return *reinterpret_cast<float*>((DWORD)this + 0x000039DC);
	}
	//m_flLowerBodyYawTarget
	IClientRenderable* GetClientRenderable()
	{
		return reinterpret_cast<IClientRenderable*>(this + 0x4);
	}


	void setAbsOriginal(Vector origin) // 55 8B EC 83 E4 F8 51 53 56 57 8B F1
	{
		using SetAbsOriginFn = void(__thiscall*)(void*, const Vector &origin);
		static SetAbsOriginFn SetAbsOrigin;

		if (!SetAbsOrigin)                                                           // ("client.dll", (PBYTE)"\x55\x8B\xEC\x83\xE4\xF8\x51\x53\x56\x57\x8B\xF1\xE8\x00\x00", "xxxxxxxxxxxxx??");
			SetAbsOrigin = (SetAbsOriginFn)(Utilities::Memory::FindPattern("client.dll", (PBYTE)"\x55\x8B\xEC\x83\xE4\xF8\x51\x53\x56\x57\x8B\xF1\xE8\x00\x00", "xxxxxxxxxxxxx??"));

		SetAbsOrigin(this, origin);
	}

	void InvalidateBoneCache() {
		static DWORD dwBoneCache = Utilities::Memory::FindPattern("client.dll", (BYTE*)"\x80\x3D\x00\x00\x00\x00\x00\x74\x16\xA1\x00\x00\x00\x00\x48\xC7\x81", "xx?????xxx????xxx");
		static DWORD dwForceBone = 0x267C;
		unsigned long iModelBoneCounter = **(unsigned long**)(dwBoneCache + 10);

		*(int*)((DWORD)this + dwForceBone + 0x20) = 0;
		*(unsigned int*)((DWORD)this + 0x2914) = 0xFF7FFFFF;
		*(unsigned int*)((DWORD)this + 0x2680) = (iModelBoneCounter - 1);
	}

	void setAbsAechse(Vector aechse)
	{
		using SetAbsAnglesFn = void(__thiscall*)(IClientEntity*, const Vector &angles);
		static SetAbsAnglesFn SetAbsAngles;

		if (!SetAbsAngles)
			SetAbsAngles = (SetAbsAnglesFn)(Utilities::Memory::FindPattern("client.dll", (BYTE*)"\x55\x8B\xEC\x83\xE4\xF8\x83\xEC\x64\x53\x56\x57\x8B\xF1\xE8", "xxxxxxxxxxxxxxx"));

		SetAbsAngles(this, aechse);
	}


	int GetMoveType()
	{
		if (!this)
			return 0;

		return ptr(int, this, 0x258);
	}
	bool m_bHasHeavyArmor()
	{
		return *(bool*)((DWORD)this + 0x0000B22D);
	}
	//unsigned char GetLifeState()
	//{
		//return *(unsigned char*)((uintptr_t)this + offsetz.DT_BasePlayer.m_lifeState);
	//}
	//int ArmorValue()
	//{
	//	return *(int*)((uintptr_t)this + offsetz.DT_CSPlayer.m_ArmorValue);
	//}

	bool HasHelmet()
	{
		return *(bool*)((uintptr_t)this + 0x0000B22C);
	}

	QAngle* GetEyeAnglesPointer()
	{
		return reinterpret_cast<QAngle*>((DWORD)this + (DWORD)0x528C);
	}

	QAngle GetEyeAngles()
	{
		return *reinterpret_cast<QAngle*>((DWORD)this + (DWORD)0x528C);
	}

	float GetCycle() {
		return *(float*)(uintptr_t(this) + 0x00000A14);
	}

	int GetSequence() {
		return *(int*)((uintptr_t)this + 0x000028AC);
	}
	float GetDuckSpeed() {
		return *(float*)((uintptr_t)this + 0x00002FA0);
	}

	float GetDuckAmount() {
		return *(float*)((uintptr_t)this + 0x00002F9C);
	}
	void SetAngle2(Vector wantedang)
	{
		typedef void(__thiscall* SetAngleFn)(void*, const Vector &);
		static SetAngleFn SetAngle = (SetAngleFn)((DWORD)GetModuleHandle("client.dll") + 0x1C2AC0);
		SetAngle(this, wantedang);
	} // paste it on your Entity class // paste it on your Entity class

	QAngle* GetEyeAnglesXY()
	{
		return (QAngle*)((DWORD)this + GET_NETVAR("DT_CSPlayer", "m_angEyeAngles"));
	}
	int GetHitboxSet()
	{
		return *(int*)((DWORD)this + 0x000009FC);
	}

	Vector GetHeadPos()
	{
		return this->GetBonePos(8);
	}

	Vector GetHeadAngle()
	{
		return GetAbsOrigin() + GetViewOffset();
	}

	int GetId()
	{
		typedef int(__thiscall* tGetId)(void*);
		return call_vfunc<tGetId>(this, 458)(this);
	}

	float* GetLowerBodyYawTarget() /*m_flLowerBodyYawTarget*/
	{
		return (float*)((DWORD)this + GET_NETVAR("DT_CSPlayer", "m_flLowerBodyYawTarget"));
	}

	bool IsPlayer()
	{
		return GetClientClass()->m_ClassID == (int)CSGOClassID::CCSPlayer;
	}

	Vector GetOrigin2() {
		return *(Vector*)((DWORD)this + 0x00000134);
	}

	Vector GetViewAngles2() {
		return *(Vector*)((DWORD)this + 0x00000104);
	}

	Vector GetAbsOrigin2() {
		__asm {
			MOV ECX, this
			MOV EAX, DWORD PTR DS : [ECX]
			CALL DWORD PTR DS : [EAX + 0x28]
		}
	}
	Vector GetAbsAngles2() {
		__asm {
			MOV ECX, this;
			MOV EAX, DWORD PTR DS : [ECX];
			CALL DWORD PTR DS : [EAX + 0x2C]
		}
	}

	Vector GetEyePosition() {
		Vector Origin = *(Vector*)((DWORD)this + 0x00000134);
		Vector View = *(Vector*)((DWORD)this + 0x00000104);
		return(Origin + View);
	}
	Vector GetAimPunch() {
		return *(Vector*)((DWORD)this + 0x00003018);
	}
	bool IsImmune() {
		return *(bool*)((DWORD)this + 0x000038A0);
	}
	ClientClass *GetClientClass2() {
		PVOID Networkable = (PVOID)((DWORD)(this) + 0x8);
		using Original = ClientClass*(__thiscall*)(PVOID);
		return call_vfunc<Original>(Networkable, 2)(Networkable);
	}
	HANDLE GetWeaponHandle() {
		return *(HANDLE*)((DWORD)this + 0x00002EE8);
	}
};
class CBaseAttributableItem
{
public:

	int* ItemDefinitionIndex()
	{
		return (int*)((uintptr_t)this + 0x1D8);
	}
	int GetItemDefinitionIndex()
	{
		return *(int*)((uintptr_t)this + 0x1D8);
	}
	int* ItemIDHigh()
	{
		return (int*)((uintptr_t)this + 0x1F0);
	}
	int* ItemIDLow()
	{
		return (int*)((uintptr_t)this + 0x1F4);
	}
	int* GetEntityQuality() {
		// DT_BaseAttributableItem -> m_AttributeManager -> m_Item -> m_iEntityQuality
		return (int*)((DWORD)this + 0x2D70 + 0x40 + 0x1DC);
	}

};