#pragma once

#include "GUI.h"
#include "Controls.h"

class CRageBotTab : public CTab
{
public:
	void Setup();

	CGroupBox AimbotGroup;
	CCheckBox AimbotEnable;
	CSeperate  AimbotSelection1;
	CComboBox AimbotSelection;
	CCheckBox AimbotFriendlyFire;
	CSeperate AimbotHitbox1;
	CComboBox AimbotHitbox;
	CCheckBox AimbotMultipoint;

	CSeperate AimbotHitscan1;
	CComboBox AimbotHitscan;
	CSlider   AimbotPointscale;
	CCheckBox AimbotAutoFire;
	CCheckBox AimbotAutoWall;
	CCheckBox AimbotSilentAim;
	CSeperate   AimbotHitchance1;
	CSlider2   AimbotHitchance;
	CSeperate  AimbotFov1;
	CSlider3  AimbotFov;
	CSeperate   AimbotMinimumDamage1;
	CSlider AimbotMinimumDamage;
	CCheckBox AimbotAutoScope;
	CCheckBox PVSFix;
	CCheckBox FakeLagFix;
	CKeyBind SomeShit;
	CCheckBox EnginePrediction;
	CCheckBox AimbotAimStep;
	CCheckBox DisableInterp;
	CCheckBox AimbotPerfectSilentAim;
	CSeperate AimbotResolver1;
	CComboBox AimbotResolver;
	CCheckBox PitchCorrection;
	CCheckBox AutoRevolver;
	CSlider AddFakeYaw;
	CSlider CustomPitch;
	CCheckBox LBY;
	CSeperate FakeYaw1;
	CComboBox FakeYaw;
	CCheckBox LowerbodyFix;
	CCheckBox PreferBodyAim;
	CSlider BaimIfUnderXHealth;
	CCheckBox AWPAtBody;
	CSlider XShots;
	CCheckBox AimbotKeyPress;
	CKeyBind  AimbotKeyBind;
	CKeyBind  AimbotStopKey;

	CGroupBox BindAA;
	CCheckBox EnabledBindAA;
	CKeyBind KeyBindAA;
	CComboBox ComboKeybind;


	CGroupBox FakeLagGroup;
	CCheckBox FakeLagEnable;
	CSlider   FakeLagChoke;
	CSeperate FakeLagChoke1;
	CComboBox FakeLagAir;
	CComboBox FakeLagType;
	CCheckBox OtherAimbotDebug;


	CGroupBox AccuracyGroup;
	CCheckBox AccuracyRecoil;
	CCheckBox AccuracyAutoStop;
	CCheckBox AimbotAutoPistol;
	CCheckBox AccuracyAutoCrouch;
	CCheckBox ResolverDebug;
	CComboBox AccuracyResolver;
	CCheckBox AccuracyPositionAdjustment;

	CGroupBox AntiAimGroup;
	CCheckBox AntiAimEnable;
	CSeperate AntiAimPitch1;
	CComboBox AntiAimPitch;
	CSeperate AntiAimYaw1;
	CComboBox AntiAimYaw;
	CCheckBox FlipAA;
	CSeperate AntiAimEdge1;
	CComboBox AntiAimEdge;
	CSlider AntiAimOffset;
	CCheckBox AntiAimKnife;
	CCheckBox AccuracyResolver2;
	CKeyBind AccuracyAntifakewalk;
	CCheckBox AntiAimTarget;
};

class CLegitBotTab : public CTab
{
public:
	void Setup();

	CGroupBox AimbotGroup;
	CCheckBox FakeLagFix;
	CCheckBox AimbotEnable;
	CCheckBox AimbotAutoFire;
	CCheckBox AimbotFriendlyFire;
	CCheckBox AimbotKeyPress;
	CKeyBind  AimbotKeyBind;
	CCheckBox AimbotSmokeCheck;

	CCheckBox OtherAimbotDebug;

	CSlider4   Testslide1;


	CSeperate LegitSpace;
	CComboBox LegitChoice;


	CGroupBox TriggerGroup;
	CCheckBox TriggerEnable;
	CCheckBox TriggerKeyPress;
	CCheckBox AimbotAutoPistol;
	CKeyBind  TriggerKeyBind;
	CCheckBox TriggerHitChance;
	CSlider   TriggerHitChanceAmmount;
	CSlider3   TriggerDelay;
	CSeperate TriggerDelay1;
	CSlider   TriggerBurst;
	CSlider   TriggerBreak;
	CCheckBox TriggerRecoil;


	CGroupBox TriggerFilterGroup;
	CCheckBox TriggerHead;
	CCheckBox TriggerChest;
	CCheckBox TriggerStomach;
	CCheckBox TriggerArms;
	CCheckBox TriggerLegs;
	CCheckBox TriggerTeammates;
	CCheckBox TriggerFlashCheck;
	CCheckBox TriggerSmokeCheck;

	CGroupBox WeaponMainGroup;
	CSlider   WeaponMainSpeed;
	CSeperate WeaponMainSpeed1;
	CSeperate WeaponMainFoV1;
	CSlider3   WeaponMainFoV;
	CSeperate WeaponMainRecoil1;
	CSlider WeaponMainRecoil;
	CSeperate WeaponMainHitbox1;
	CComboBox WeaponMainHitbox;
	CSlider WeaponMainAimtime;
	CSlider WeaoponMainStartAimtime;

	CGroupBox WeaponPistGroup;
	CSlider   WeaponPistSpeed;
	CSeperate   WeaponPistSpeed1;
	CSlider3   WeaponPistFoV;
	CSeperate   WeaponPistFoV1;
	CSlider WeaponPistRecoil;
	CSeperate WeaponPistRecoil1;
	CComboBox WeaponPistHitbox;
	CSeperate WeaponPistHitbox1;
	CSlider WeaponPistAimtime;
	CSlider WeaoponPistStartAimtime;

	CGroupBox WeaponSnipGroup;
	CSeperate WeaponSnipGroup1;
	CSlider   WeaponSnipSpeed;
	CSeperate WeaponSnipSpeed1;
	CSlider3   WeaponSnipFoV;
	CSeperate WeaponSnipFoV1;
	CSlider WeaponSnipRecoil;
	CSeperate WeaponSnipRecoil1;
	CComboBox WeaponSnipHitbox;
	CSeperate WeaponSnipHitbox1;
	CSlider WeaponSnipAimtime;
	CSlider WeaoponSnipStartAimtime;

	CGroupBox WeaponOtherGroup;
	CSlider   WeaponOtherSpeed;
	CSeperate WeaponOtherSpeed1;
	CSlider3   WeaponOtherFoV;
	CSeperate WeaponOtherFoV1;
	CSlider WeaponOtherRecoil;
	CSeperate WeaponOtherRecoil1;
	CComboBox WeaponOtherHitbox;
	CSeperate WeaponOtherHitbox1;
	CSlider WeaponOtherAimtime;
	CSlider WeaoponOtherStartAimtime;

	/*	CGroupBox WeaponShotgunGroup;
	CSlider   WeaponOtherSpeed;
	CSlider3   WeaponOtherFoV;
	CSlider WeaponOtherRecoil;
	CComboBox WeaponOtherHitbox;
	CSlider WeaponOtherAimtime;
	CSlider WeaoponOtherStartAimtime;

	CGroupBox WeaponMGGroup;
	CSlider   WeaponOtherSpeed;
	CSlider3   WeaponOtherFoV;
	CSlider WeaponOtherRecoil;
	CComboBox WeaponOtherHitbox;
	CSlider WeaponOtherAimtime;
	CSlider WeaoponOtherStartAimtime; */
	CSlider   WeaponMGSpeed;
	CSlider3   WeaponMGFoV;
	CSlider WeaponMGRecoil;
	CComboBox WeaponMGHitbox;
	CSlider WeaponMGAimtime;
	CSlider WeaoponMGStartAimtime;

	CSlider   AimbotInaccuracy;
	CSeperate WeaponMainInacc1;
	CSlider   WeaponMainInacc;
	CSeperate WeaponOtherInacc1;
	CSlider WeaponOtherInacc;
	CSeperate WeaponPistInacc1;
	CSlider   WeaponPistInacc;
	CSeperate WeaponSnipInacc1;
	CSlider   WeaponSnipInacc;
};

class CVisualTab : public CTab
{
public:
	void Setup();

	// Master Switch
	CLabel ActiveLabel;
	CCheckBox DrawLinesAA;

	// Options Settings
	CGroupBox OptionsGroup;
	CCheckBox Active;
	CCheckBox OptionsBox;
	CCheckBox OptionsFilled;
	CCheckBox OptionsName;
	CCheckBox OptionsHealth;
	CCheckBox OptionsGray;
	CCheckBox OptionsWeapon;
	CCheckBox OptionsArmor;
	CCheckBox OptionsInfo;
	CCheckBox DrawDistance;
	CCheckBox OptionsMoney;
	CCheckBox HitmarkerSound;
	CCheckBox NoHitmarker;
	CCheckBox OptionsHelmet;
	CCheckBox OptionsBarrels;
	CCheckBox OptionsKit;
	CCheckBox OptionsDefuse;
	CCheckBox OptionsGlow;
	CComboBox OptionsChams;
	CSeperate OptionsChams1;
	CCheckBox OptionsChamsLegit;
	CComboBox OptionsDroppedc;
	CSeperate OptionsDroppedc1;
	CCheckBox OptionsSkeleton;
	CCheckBox OptionsAimSpot;
	CCheckBox OptionsCompRank;
	CCheckBox lbyidicador;
	CCheckBox backtrackskele;
	CCheckBox LegitMode1;
	CGroupBox ModelG;

	// Filters Settings
	CGroupBox FiltersGroup;
	CCheckBox FiltersAll;
	CCheckBox FiltersPlayers;
	CCheckBox DLight;
	CCheckBox OptionsTeammates;
	CCheckBox FiltersWeapons;
	CKeyBind  ThirdPersonKeyBind;
	CSeperate ThirdPersonKeyBind1;
	CComboBox ThirdpersonAngle;
	CSeperate ThirdpersonAngle1;
	CCheckBox OtherNades;
	CCheckBox Filtersselfesp;
	CCheckBox FiltersChickens;
	CCheckBox FiltersC4;
	CCheckBox OptionsPlant;
	CCheckBox Granades;
	CCheckBox Grenades2;

	// Other Settings
	CCheckBox SniperCrosshair;
	CGroupBox OtherGroup;
	CSeperate OtherCrosshair1;
	CComboBox OtherCrosshair;
	CCheckBox Clock;
	CComboBox OtherRecoilCrosshair;
	CCheckBox OtherHitmarker;
	CCheckBox OtherRadar;
	CComboBox AmbientSkybox;
	CSeperate AmbientSkybox1;
	CCheckBox OtherNoVisualRecoil;
	CCheckBox Grenades;
	CCheckBox GrenadePrediction;
	CCheckBox OtherNoSky;
	CCheckBox OtherNoFlash;
	CCheckBox DisablePostProcess;
	CSlider AmbientRed;
	CSlider AmbientGreen;
	CSlider AmbientBlue;
	CCheckBox OtherSpectators;
	CCheckBox OtherThirdperson;
	CCheckBox OtherNoScope;
	CCheckBox OtherNoSmoke;
	CCheckBox OtherAsusWalls;
	CComboBox OtherNoHands;
	CSeperate OtherNoHands1;
	CSlider OtherViewmodelFOV;
	CCheckBox SightLines;
	CSlider SightLinesLength;
};

class CMiscTab : public CTab
{
public:
	void Setup();

	CGroupBox Gloves;
	CCheckBox EnableGloves;
	CComboBox GloveSkin;
	CComboBox GloveModel;
	CButton GlovesApply;

	CSeperate word1;
	CSeperate word2;
	CSeperate word3;
	CSeperate word4;

	CGroupBox KnifeGroup;
	CComboBox KnifeModel;
	CTextField KnifeSkin;
	CCheckBox SkinEnable;


	// Other Settings
	CGroupBox OtherGroup;
	CCheckBox TrashTalk;
	CCheckBox OtherAutoJump;
	CCheckBox OtherEdgeJump;
	CComboBox OtherAutoStrafe;
	CSeperate OtherAutoStrafe1;
	CGroupBox FakeLagGroup;
	CCheckBox OtherSafeMode;
	CComboBox OtherChatSpam;
	CCheckBox OtherTeamChat;
	CSlider	  OtherChatDelay;
	CCheckBox EnableBuyBot;
	CCheckBox Watermark;
	CComboBox BuyBot;
	CGroupBox BuyBotGroup;
	CSeperate BuyBotGrenades1;
	CComboBox BuyBotGrenades;
	CCheckBox BuyBotKevlar;
	CCheckBox BuyBotDefuser;
	CKeyBind  OtherAirStuck;
	CSeperate OtherAirStuck1;
	CKeyBind  OtherLagSwitch;
	CCheckBox ThirdpersonGhost;
	CCheckBox OtherAutoAccept;
	//CCheckBox OtherAutoAccept;
	CCheckBox OtherWalkbot;
	CComboBox OtherClantag;
	CKeyBind OtherSlowMotion;
	CKeyBind OtherFakeWalk;
	CSeperate OtherFakeWalk1;
	CCheckBox OtherCircleStrafe;
	CKeyBind OtherCircleStrafeKey;
	CSeperate OtherFOV1;
	CSlider3 OtherFOV;

	// Fake Lag Settings
	//CCheckBox FakeLagWhileShooting;

	//presets 
	CButton LoadButton;
	CButton SaveButton;
	CComboBox ConfigBox;
	CButton UnloadButton;
	CButton SkinApply;
	CSeperate ConfigSpacing1;
	CSeperate ConfigSpacing2;
	CSeperate ConfigSpacing3;
};

class ColorsTab : public CTab
{
public:
	void Setup();
	// Box ESP Colour
	CGroupBox ESPColorBox;
	CSlider   RBoxCT;
	CSlider   GBoxCT;
	CSlider   BBoxCT;
	CSlider   ABoxCT;
	CSlider   RBoxCTV;
	CSlider   GBoxCTV;
	CSlider   BBoxCTV;
	CSlider   ABoxCTV;
};

class CSkinchangerTab : public CTab
{
public:
	void Setup();

	CLabel SkinActive;
	CButton   SkinApply;

	CCheckBox GG;

	CGroupBox PistolGroup;
	CComboBox GLOCKSkin;
	CComboBox USPSSkin;
	CComboBox DEAGLESkin;
	CComboBox MAGNUMSkin;
	CComboBox DUALSSkin;
	CComboBox RevolverSkin;
	CComboBox FIVESEVENSkin;
	CComboBox TECNINESkin;
	CComboBox P2000Skin;
	CComboBox P250Skin;

	CGroupBox MPGroup;
	CComboBox MAC10Skin;
	CComboBox P90Skin;
	CComboBox UMP45Skin;
	CComboBox BIZONSkin;
	CComboBox MP7Skin;
	CComboBox MP9Skin;

	CGroupBox Riflegroup;
	CComboBox M41SSkin;
	CComboBox M4A4Skin;
	CComboBox AK47Skin;
	CComboBox AUGSkin;
	CComboBox FAMASSkin;
	CComboBox GALILSkin;
	CComboBox SG553Skin;

	CGroupBox MachinegunsGroup;
	CComboBox NEGEVSkin;
	CComboBox M249Skin;

	CGroupBox Snipergroup;
	CComboBox SCAR20Skin;
	CComboBox G3SG1Skin;
	CComboBox SSG08Skin;
	CComboBox AWPSkin;

	CGroupBox Shotgungroup;
	CComboBox MAG7Skin;
	CComboBox XM1014Skin;
	CComboBox SAWEDOFFSkin;
	CComboBox NOVASkin;

	CGroupBox SkinsettingsGroup;
	CCheckBox StatTrakEnable;
	CTextField StatTrackAmount;
	CTextField SkinName;
	CTextField KnifeName;
};

class CColorsTab : public CTab
{
public:
	void Setup();
	CGroupBox ControlGroup;
	CComboBox ControlColor;

	// CT IS VISUAL ESP
	CGroupBox CTVisESP;
	CSlider CTVisESPR; //red
	CSlider CTVisESPG; //green
	CSlider CTVisESPB; //blue

					   // T IS VISUAL ESP
	CGroupBox TVisESP;
	CSlider TVisESPR; //red
	CSlider TVisESPG; //green
	CSlider TVisESPB; //blue

					  // CT NOT VISIBLE ESP
	CGroupBox CTInvisESP;
	CSlider CTInvisESPR; //red
	CSlider CTInvisESPG; //green
	CSlider CTInvisESPB; //blue

						 // T NOT VISIBLE ESP
	CGroupBox TInvisESP;
	CSlider TInvisESPR; //red
	CSlider TInvisESPG; //green
	CSlider TInvisESPB; //blue

						// CT IS VISUAL CHAMS
	CGroupBox CTVisChams;
	CSlider CTVisChamsR; //red
	CSlider CTVisChamsG; //green
	CSlider CTVisChamsB; //blue

						 // T IS VISUAL CHAMS
	CGroupBox TVisChams;
	CSlider TVisChamsR; //red
	CSlider TVisChamsG; //green
	CSlider TVisChamsB; //blue

						// CT NOT VISIBLE CHAMS
	CGroupBox CTInvisChams;
	CSlider CTInvisChamsR; //red
	CSlider CTInvisChamsG; //green
	CSlider CTInvisChamsB; //blue

						   // T NOT VISIBLE CHAMS
	CGroupBox TInvisChams;
	CSlider TInvisChamsR; //red
	CSlider TInvisChamsG; //green
	CSlider TInvisChamsB; //blue
};



class CorruptionWindow : public CWindow
{
public:
	void Setup();

	CRageBotTab RageBotTab;
	CLegitBotTab LegitBotTab;
	CVisualTab VisualsTab;
	CMiscTab MiscTab;
	CColorsTab ColorsTab;
	CSkinchangerTab SkinchangerTab;
};

namespace Menu
{
	void SetupMenu();
	void DoUIFrame();

	extern CorruptionWindow Window;
};