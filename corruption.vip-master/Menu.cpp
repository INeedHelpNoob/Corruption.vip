#include "Menu.h"
#include "Controls.h"
#include "Hooks.h" // for the unload meme
#include "Interfaces.h"
#include "CRC32.h"

#define WINDOW_WIDTH 580
#define WINDOW_HEIGHT 580

CorruptionWindow Menu::Window;

void SaveCallbk()
{
	switch (Menu::Window.MiscTab.ConfigBox.GetIndex())
	{
	case 0:
		GUI.SaveWindowState(&Menu::Window, "CorruptionLegit.cfg");
		break;
	case 1:
		GUI.SaveWindowState(&Menu::Window, "CorruptionLegit2.cfg");
		break;
	case 2:
		GUI.SaveWindowState(&Menu::Window, "CorruptionHVH1.cfg");
		break;
	case 3:
		GUI.SaveWindowState(&Menu::Window, "CorruptionHVH2.cfg");
		break;
	case 4:
		GUI.SaveWindowState(&Menu::Window, "CorruptionHVH3.cfg");
		break;
	case 5:
		GUI.SaveWindowState(&Menu::Window, "CorruptionNSHvH.cfg");
		break;
	}
}

// Load Config from CSGO directory
void LoadCallbk()
{
	switch (Menu::Window.MiscTab.ConfigBox.GetIndex())
	{
	case 0:
		GUI.LoadWindowState(&Menu::Window, "CorruptionLegit.cfg");
		break;
	case 1:
		GUI.LoadWindowState(&Menu::Window, "CorruptionLegit2.cfg");
		break;
	case 2:
		GUI.LoadWindowState(&Menu::Window, "CorruptionHVH1.cfg");
		break;
	case 3:
		GUI.LoadWindowState(&Menu::Window, "CorruptionHVH2.cfg");
		break;
	case 4:
		GUI.LoadWindowState(&Menu::Window, "CorruptionHVH3.cfg");
		break;
	case 5:
		GUI.LoadWindowState(&Menu::Window, "CorruptionNSHvH.cfg");
		break;
	}
}

void UnLoadCallbk()
{
	DoUnload = true;
}

void KnifeApplyCallbk()
{
	Interfaces::Engine->ClientCmd_Unrestricted("record Corruption;stop");
}

void GlovesApplyCallbk()
{
	static ConVar* Meme = Interfaces::CVar->FindVar("cl_fullupdate");
	Meme->nFlags &= ~FCVAR_CHEAT;
	Interfaces::Engine->ClientCmd_Unrestricted("cl_fullupdate");
	bGlovesNeedUpdate = true;
}

void CorruptionWindow::Setup()
{
	SetPosition(300, 50);
	SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	RegisterTab(&RageBotTab);
	RegisterTab(&LegitBotTab);
	RegisterTab(&VisualsTab);
	RegisterTab(&MiscTab);
	RegisterTab(&SkinchangerTab);
	//RegisterTab(&Playerlist);

	RECT Client = GetClientArea();
	Client.bottom -= 29;

	RageBotTab.Setup();
	LegitBotTab.Setup();
	VisualsTab.Setup();
	MiscTab.Setup();
	SkinchangerTab.Setup();
	//Playerlist.Setup();

}

void CRageBotTab::Setup()
{
	SetTitle("A");

#pragma region Aimbot

	AimbotGroup.SetPosition(16 + 85, 22);
	AimbotGroup.SetText("Aimbot");
	AimbotGroup.SetSize(240, 370);//idk what spacing you use but this is 20 from the fakelag.. 422 is where fakelag is
	RegisterControl(&AimbotGroup);

	AimbotEnable.SetFileId("aim_enable");
	AimbotGroup.PlaceLabledControl("Enabled", this, &AimbotEnable);
	/*Removed because i think its useless*/
	// re-adding because skeet has it lol, but yeah its a bit useless unless you wanna kill some1 with lower health first etc
	AimbotFriendlyFire.SetFileId("aim_friendlyfire");
	AimbotGroup.PlaceLabledControl("Friendly Fire", this, &AimbotFriendlyFire);

	AimbotSelection1.SetFileId("aim_selection1");
	AimbotGroup.PlaceLabledControl("Target Selection", this, &AimbotSelection1);

	AimbotSelection.SetFileId("aim_selection");
	AimbotSelection.AddItem("Threat");
	AimbotSelection.AddItem("Distance");
	AimbotSelection.AddItem("Lowest Health");
	AimbotGroup.PlaceLabledControl("", this, &AimbotSelection);


	AimbotHitbox1.SetFileId("aim_hitbox1");
	AimbotGroup.PlaceLabledControl("Target Hitbox", this, &AimbotHitbox1);

	AimbotHitbox.SetFileId("aim_hitbox");
	AimbotHitbox.AddItem("Head");
	AimbotHitbox.AddItem("Neck");
	AimbotHitbox.AddItem("Chest");
	AimbotHitbox.AddItem("Stomach");
	AimbotHitbox.AddItem("Feet");
	AimbotGroup.PlaceLabledControl("", this, &AimbotHitbox);

	AimbotHitscan1.SetFileId("aim_hitscan1");
	AimbotGroup.PlaceLabledControl("Hitscan", this, &AimbotHitscan1);

	AimbotHitscan.SetFileId("aim_hitscan");
	AimbotHitscan.AddItem("Off");
	AimbotHitscan.AddItem("Low");
	AimbotHitscan.AddItem("Medium");
	AimbotHitscan.AddItem("High");
	AimbotGroup.PlaceLabledControl("", this, &AimbotHitscan); // everything+???? that sounds legit retarded lol

	AimbotMultipoint.SetFileId("aim_multipoint");
	AimbotGroup.PlaceLabledControl("Multi-point", this, &AimbotMultipoint);

	AimbotPointscale.SetFileId("aim_pointscale");//this will be aimheight
	AimbotPointscale.SetBoundaries(0, 5);//0 is center 5 is very top of head
	AimbotPointscale.SetValue(0);
	AimbotGroup.PlaceLabledControl("", this, &AimbotPointscale);

	AimbotAutoFire.SetFileId("aim_autofire");
	AimbotGroup.PlaceLabledControl("Automatic  Fire", this, &AimbotAutoFire);

	AimbotAutoWall.SetFileId("acc_awall");
	AimbotGroup.PlaceLabledControl("Automatic  Penetration", this, &AimbotAutoWall);

	AimbotSilentAim.SetFileId("aim_silent");
	AimbotGroup.PlaceLabledControl("Silent Aim", this, &AimbotSilentAim);

	//IF I HAVE TO REMOVE THIS FUCKING PSILENT THAT SHOOTS AT THE FLOOR ONE MORE TIME I SWEAR TO GOD
	// srry lol
	AimbotHitchance1.SetFileId("acc_chance1");
	AimbotGroup.PlaceLabledControl("Minimum Hitchance", this, &AimbotHitchance1);

	AimbotHitchance.SetFileId("acc_chance");
	AimbotHitchance.SetBoundaries(0, 100);
	AimbotHitchance.SetValue(0);
	AimbotGroup.PlaceLabledControl("", this, &AimbotHitchance);

	AimbotFov1.SetFileId("aim_fov1");
	AimbotGroup.PlaceLabledControl("Maximum FOV", this, &AimbotFov1);

	AimbotFov.SetFileId("aim_fov");
	AimbotFov.SetBoundaries(0, 180);
	AimbotFov.SetValue(180);
	AimbotGroup.PlaceLabledControl("", this, &AimbotFov);

	AimbotMinimumDamage1.SetFileId("acc_mindmg1");
	AimbotGroup.PlaceLabledControl("Minimum Damage", this, &AimbotMinimumDamage1);

	AimbotMinimumDamage.SetFileId("acc_mindmg");
	AimbotMinimumDamage.SetBoundaries(0, 100);
	AimbotMinimumDamage.SetValue(0);
	AimbotGroup.PlaceLabledControl("", this, &AimbotMinimumDamage);

	AimbotAutoScope.SetFileId("acc_scope");
	AimbotGroup.PlaceLabledControl("Automatic  Scope", this, &AimbotAutoScope);

	AimbotAimStep.SetFileId("aim_aimstep");
	AimbotGroup.PlaceLabledControl("Aimstep", this, &AimbotAimStep);

	//DisableInterp.SetFileId("aim_interp");
	//AimbotGroup.PlaceLabledControl("Disable Interpolation", this, &DisableInterp); //THIS NEEDS TO JUST AUTOMATICALLY ACTIVATE WITH BACKTRACKING


	/*ADDING NEW BIND AA BOX HERE TILL WE CAN FIX THE SPACE FOR AA AND OTHER ON RAGEBOT*/
	/*ADDING NEW BIND AA BOX HERE TILL WE CAN FIX THE SPACE FOR AA AND OTHER ON RAGEBOT*/
	/*	BindAA.SetPosition(16+85,411+11+15);
	BindAA.SetText("Bind Flip AA On Key");
	BindAA.SetSize(240,85);
	RegisterControl(&BindAA);

	EnabledBindAA.SetFileId("bind_enab");
	BindAA.PlaceLabledControl("Enabled",this,&EnabledBindAA);

	KeyBindAA.SetFileId("keeybind_enab");
	BindAA.PlaceLabledControl("Key For Switch",this,&KeyBindAA);

	ComboKeybind.SetFileId("combo_bindAA");
	ComboKeybind.AddItem("Real Flip 180");
	ComboKeybind.AddItem("Real Back");
	BindAA.PlaceLabledControl("AA Type",this,&ComboKeybind); */
	/*ADDING NEW BIND AA BOX HERE TILL WE CAN FIX THE SPACE FOR AA AND OTHER ON RAGEBOT*/
	/*ADDING NEW BIND AA BOX HERE TILL WE CAN FIX THE SPACE FOR AA AND OTHER ON RAGEBOT*/

#pragma endregion Aimbot Controls Get Setup in here

#pragma region fakelag memes

	RegisterControl(&FakeLagGroup);
	FakeLagGroup.SetPosition(16 + 85, 370 + 45);
	FakeLagGroup.SetText("Fake Lag");
	FakeLagGroup.SetSize(240, 105);

	FakeLagEnable.SetFileId("fakelag_enable");
	FakeLagGroup.PlaceLabledControl("Fake Lag", this, &FakeLagEnable);

	FakeLagType.SetFileId("fakelag_type");
	FakeLagType.AddItem("Off");
	FakeLagType.AddItem("Normal");
	FakeLagType.AddItem("Adaptive");
	FakeLagType.AddItem("Break Lag  Compensation");
	FakeLagGroup.PlaceLabledControl("", this, &FakeLagType);

	FakeLagChoke1.SetFileId("fakelag_choke1");
	FakeLagGroup.PlaceLabledControl("Choke Factor", this, &FakeLagChoke1);

	FakeLagChoke.SetFileId("fakelag_choke");
	FakeLagChoke.SetBoundaries(0, 14);
	FakeLagChoke.SetValue(0);
	FakeLagGroup.PlaceLabledControl("", this, &FakeLagChoke);

	OtherAimbotDebug.SetFileId("otr_aimdebug");
	FakeLagGroup.PlaceLabledControl("Fake Ping Exploit", this, &OtherAimbotDebug);

#pragma endregion fakelag xd

#pragma region Accuracy
	AccuracyGroup.SetPosition(285 + 85, 22);
	AccuracyGroup.SetText("Other");
	AccuracyGroup.SetSize(240, 270 - 45);
	RegisterControl(&AccuracyGroup);

	AccuracyRecoil.SetFileId("acc_norecoil");
	AccuracyGroup.PlaceLabledControl("Remove Recoil", this, &AccuracyRecoil);

	/*	AccuracyAutoCrouch.SetFileId("acc_crouch");
	AccuracyGroup.PlaceLabledControl("Auto Crouch", this, &AccuracyAutoCrouch); */

	AccuracyAutoStop.SetFileId("acc_stop");
	AccuracyGroup.PlaceLabledControl("Quick  Stop", this, &AccuracyAutoStop);

//	AccuracyAutoCrouch.SetFileId("acc_stop");
//	AccuracyGroup.PlaceLabledControl("Quick Stop + Duck", this, &AccuracyAutoCrouch);

	AccuracyPositionAdjustment.SetFileId("acc_posadj");
	AccuracyGroup.PlaceLabledControl("Position Adjustment", this, &AccuracyPositionAdjustment);

//	LowerbodyFix.SetFileId("acc_lowerbodyfix");
//	AccuracyGroup.PlaceLabledControl("LBY Correction", this, &LowerbodyFix); //should be done with aa correction lmao

	PitchCorrection.SetFileId("acc_pitchcorrection");
	AccuracyGroup.PlaceLabledControl("Anti-Aim Correction", this, &PitchCorrection);//prob useless in Spread HVH's 

	AimbotResolver1.SetFileId("acc_aaa");
	AccuracyGroup.PlaceLabledControl("Anti-Aim Resolver", this, &AimbotResolver1);

	AimbotResolver.SetFileId("acc_aaa");
	AimbotResolver.AddItem("Off");
	AimbotResolver.AddItem("On");
	AimbotResolver.AddItem("On-Bruteforce");
	AccuracyGroup.PlaceLabledControl("", this, &AimbotResolver);

	PreferBodyAim.SetFileId("acc_preferbaim");
	AccuracyGroup.PlaceLabledControl("Prefer Body-Aim", this, &PreferBodyAim);

	AWPAtBody.SetFileId("aim_awpatbody");
	AccuracyGroup.PlaceLabledControl("AWP Body-Aim", this, &AWPAtBody);

	AimbotResolver1.SetFileId("acc_aaa");
	AccuracyGroup.PlaceLabledControl("Baim if HP is under X", this, &AimbotResolver1);

	BaimIfUnderXHealth.SetFileId("acc_BaimIfUnderXHealth");
	BaimIfUnderXHealth.SetBoundaries(0, 100);
	BaimIfUnderXHealth.SetValue(0);
	AccuracyGroup.PlaceLabledControl("", this, &BaimIfUnderXHealth);

	FakeLagFix.SetFileId("fakelagfix1");//this is my fakelagfix //bt dont fucking touch
	AccuracyGroup.PlaceLabledControl("Backtrack", this, &FakeLagFix);

	SomeShit.SetFileId("someshit1");//this is my fakelagfix //bt dont fucking touch
	AccuracyGroup.PlaceLabledControl("Override", this, &SomeShit);


#pragma endregion  Accuracy controls get Setup in here

#pragma region AntiAim
	AntiAimGroup.SetPosition(285 + 85, 270);
	AntiAimGroup.SetText("Anti-Aimbot");
	AntiAimGroup.SetSize(240, 250);
	RegisterControl(&AntiAimGroup);

	AntiAimPitch1.SetFileId("aa_x1");
	AntiAimGroup.PlaceLabledControl("Pitch", this, &AntiAimPitch1);

	AntiAimPitch.SetFileId("aa_x");
	AntiAimPitch.AddItem("Off");
	AntiAimPitch.AddItem("Emotion");
	AntiAimPitch.AddItem("Bitwise Down");
	AntiAimPitch.AddItem("HalfDown");
	AntiAimPitch.AddItem("Jitter");
	AntiAimPitch.AddItem("Up");
	AntiAimPitch.AddItem("Zero");
	AntiAimPitch.AddItem("No-Spread");
	AntiAimGroup.PlaceLabledControl("", this, &AntiAimPitch);

	FakeYaw1.SetFileId("fakeaa1");
	AntiAimGroup.PlaceLabledControl("Yaw", this, &FakeYaw1);

	FakeYaw.SetFileId("fakeaa");
	FakeYaw.AddItem("Off");
	FakeYaw.AddItem("Fast Spin");
	FakeYaw.AddItem("Slow Spin");
	FakeYaw.AddItem("Jitter");
	FakeYaw.AddItem("18O Jitter");
	FakeYaw.AddItem("Backwards");
	FakeYaw.AddItem("Backwards Jitter");
	FakeYaw.AddItem("Sideways Switch");
	FakeYaw.AddItem("LBY");
	FakeYaw.AddItem("Fake Jitter");
	FakeYaw.AddItem("LBY Sideways");
	FakeYaw.AddItem("LBY Spin");
	AntiAimGroup.PlaceLabledControl("", this, &FakeYaw);//this is real dont let the fakeyaw thing fool you // @wrapzii ik lmao

	AntiAimEdge1.SetFileId("aa_edge1");
	AntiAimGroup.PlaceLabledControl("Edge", this, &AntiAimEdge1);

	AntiAimEdge.SetFileId("aa_edge");
	AntiAimEdge.AddItem("Off");
	AntiAimEdge.AddItem("Normal");
	AntiAimEdge.AddItem("Fake Out");
	AntiAimGroup.PlaceLabledControl("", this, &AntiAimEdge);

	AntiAimYaw1.SetFileId("aa_y");
	AntiAimGroup.PlaceLabledControl("Fake", this, &AntiAimYaw1);

	AntiAimYaw.SetFileId("aa_y");
	AntiAimYaw.AddItem("Off");
	AntiAimYaw.AddItem("Fast Spin");
	AntiAimYaw.AddItem("Slow Spin");
	AntiAimYaw.AddItem("Jitter");
	AntiAimYaw.AddItem("18O Jitter");
	AntiAimYaw.AddItem("Backwards");
	AntiAimYaw.AddItem("Backwards Jitter");
	AntiAimYaw.AddItem("Sideways Switch");
	AntiAimYaw.AddItem("LBY");
	AntiAimYaw.AddItem("LBY Jitter");
	AntiAimYaw.AddItem("LBY Sideways");
	AntiAimYaw.AddItem("LBY Spin");
	AntiAimGroup.PlaceLabledControl("", this, &AntiAimYaw);

	LBY.SetFileId("lby_nomove");
	AntiAimGroup.PlaceLabledControl("Non-Moving  LBY  Breaker", this, &LBY);

	AccuracyResolver2.SetFileId("acc_fag");
	AntiAimGroup.PlaceLabledControl("Anti FakeWalk", this, &AccuracyResolver2);

	AntiAimKnife.SetFileId("aa_knife");
	AntiAimGroup.PlaceLabledControl("Knife AA", this, &AntiAimKnife);

	AntiAimTarget.SetFileId("aa_target");
	AntiAimGroup.PlaceLabledControl("At Targets", this, &AntiAimTarget);
#pragma endregion  AntiAim controls get setup in here

}
void CLegitBotTab::Setup()
{
	SetTitle("B");

#pragma region Aimbot

/*	Testslide1.SetPosition(16+85+177,22);
	Testslide1.SetFileId("config_shit1");
	Testslide1.SetValue(10.f);
	Testslide1.SetBoundaries(0,15);
	Testslide1.SetSize(100, 11);
	RegisterControl(&Testslide1); */ // its ugly seeing it just sit in the menu :/


	AimbotGroup.SetPosition(16 + 85, 22);
	AimbotGroup.SetText("Legit Aimbot");
	AimbotGroup.SetSize(240, 170);
	RegisterControl(&AimbotGroup);

	AimbotEnable.SetFileId("aim_enable");
	AimbotGroup.PlaceLabledControl("Enabled", this, &AimbotEnable);

	AimbotAutoFire.SetFileId("aim_autofire");
	AimbotGroup.PlaceLabledControl("Auto Fire", this, &AimbotAutoFire);

	AimbotFriendlyFire.SetFileId("aim_friendfire");
	AimbotGroup.PlaceLabledControl("Friendly Fire", this, &AimbotFriendlyFire);

	AimbotSmokeCheck.SetFileId("otr_smokecheck");
	AimbotGroup.PlaceLabledControl("Check on Smoke", this, &AimbotSmokeCheck);
	/*some shit*/
	FakeLagFix.SetFileId("legt_bt");
	AimbotGroup.PlaceLabledControl("Backtrack-beta",this,&FakeLagFix);

	AimbotKeyBind.SetFileId("aim_key");
	AimbotGroup.PlaceLabledControl("", this, &AimbotKeyBind);

	OtherAimbotDebug.SetFileId("otr_aimdebug");
	AimbotGroup.PlaceLabledControl("Fake Ping Exploit", this, &OtherAimbotDebug);
#pragma endregion Aimbot shit

#pragma region Triggerbot
	TriggerGroup.SetPosition(285 + 85, 22);
	TriggerGroup.SetText("Triggerbot");
	TriggerGroup.SetSize(240, 170);
	RegisterControl(&TriggerGroup);

	TriggerEnable.SetFileId("trig_enable");
	TriggerGroup.PlaceLabledControl("Enabled", this, &TriggerEnable);

	TriggerKeyBind.SetFileId("trig_key");
	TriggerGroup.PlaceLabledControl("", this, &TriggerKeyBind);

	TriggerRecoil.SetFileId("trig_recoil");
	TriggerGroup.PlaceLabledControl("Recoil Control", this, &TriggerRecoil);

	TriggerSmokeCheck.SetFileId("acc_aaa1");
	TriggerGroup.PlaceLabledControl("Smoke Check", this, &TriggerSmokeCheck);

	TriggerDelay1.SetFileId("acc_aaa");
	TriggerGroup.PlaceLabledControl("Triggerbot Delay", this, &TriggerDelay1);

	TriggerDelay.SetFileId("trig_time");
	TriggerDelay.SetBoundaries(0, 100);
	TriggerDelay.SetValue(1);
	TriggerGroup.PlaceLabledControl("", this, &TriggerDelay);

	TriggerGroup.PlaceLabledControl("Choose your AA", this, &LegitSpace);

	LegitChoice.SetFileId("legit_aa");
	LegitChoice.AddItem("None");
	LegitChoice.AddItem("Backwards");
	LegitChoice.AddItem("Sideways");
	LegitChoice.AddItem("Adaptive Side");
	TriggerGroup.PlaceLabledControl("Legit AA", this, &LegitChoice);

#pragma endregion TriggerbotFilter stuff

#pragma region Main Weapon
	WeaponMainGroup.SetPosition(16 + 85, 208);
	WeaponMainGroup.SetText("Rifles");
	WeaponMainGroup.SetSize(240, 150);
	RegisterControl(&WeaponMainGroup);

	WeaponMainHitbox1.SetFileId("main_hitbox1");
	WeaponMainGroup.PlaceLabledControl("Hitbox", this, &WeaponMainHitbox1);

	WeaponMainHitbox.SetFileId("main_hitbox");
	WeaponMainHitbox.AddItem("Head");
	WeaponMainHitbox.AddItem("Neck");
	WeaponMainHitbox.AddItem("Chest");
	WeaponMainHitbox.AddItem("Stomach");
	WeaponMainHitbox.AddItem("Multihitbox");
	WeaponMainGroup.PlaceLabledControl("", this, &WeaponMainHitbox);

	WeaponMainSpeed1.SetFileId("main_speed1");
	WeaponMainGroup.PlaceLabledControl("Aimbot Speed", this, &WeaponMainSpeed1);

	WeaponMainSpeed.SetFileId("main_speed");
	WeaponMainSpeed.SetBoundaries(0.f, 10.f);
	WeaponMainSpeed.SetValue(1.f);
	WeaponMainGroup.PlaceLabledControl("", this, &WeaponMainSpeed);

	WeaponMainFoV1.SetFileId("main_fov1");
	WeaponMainGroup.PlaceLabledControl("Field of View", this, &WeaponMainFoV1);

	WeaponMainFoV.SetFileId("main_fov");
	WeaponMainFoV.SetBoundaries(0.f, 20.f);
	WeaponMainFoV.SetValue(3.5f);
	WeaponMainGroup.PlaceLabledControl("", this, &WeaponMainFoV);

/*	WeaponMainRecoil1.SetFileId("main_recoil1");
	WeaponMainGroup.PlaceLabledControl("Recoil Control", this, &WeaponMainRecoil1);

	WeaponMainRecoil.SetFileId("main_recoil");
	WeaponMainRecoil.SetBoundaries(0.f, 2.f);
	WeaponMainRecoil.SetValue(1.f);
	WeaponMainGroup.PlaceLabledControl("", this, &WeaponMainRecoil); */

	WeaponMainInacc1.SetFileId("main_inacc1");
	WeaponMainGroup.PlaceLabledControl("Weapon Inaccuracy", this, &WeaponMainInacc1);

	WeaponMainInacc.SetFileId("main_inacc");
	WeaponMainInacc.SetBoundaries(0.f, 15.f);
	WeaponMainGroup.PlaceLabledControl("", this, &WeaponMainInacc);

	WeaponMainAimtime.SetValue(0);
	WeaoponMainStartAimtime.SetValue(0);

#pragma endregion

#pragma region Pistols
	WeaponPistGroup.SetPosition(285 + 85, 208);
	WeaponPistGroup.SetText("Pistols");
	WeaponPistGroup.SetSize(240, 150);
	RegisterControl(&WeaponPistGroup);

	WeaponPistHitbox1.SetFileId("main_hitbox1");
	WeaponPistGroup.PlaceLabledControl("Hitbox", this, &WeaponPistHitbox1);

	WeaponPistHitbox.SetFileId("pist_hitbox");
	WeaponPistHitbox.AddItem("Head");
	WeaponPistHitbox.AddItem("Neck");
	WeaponPistHitbox.AddItem("Chest");
	WeaponPistHitbox.AddItem("Stomach");
	WeaponPistHitbox.AddItem("Multihitbox");
	WeaponPistGroup.PlaceLabledControl("", this, &WeaponPistHitbox);

	WeaponPistSpeed1.SetFileId("pist_speed1");
	WeaponPistGroup.PlaceLabledControl("Aimbot Speed", this, &WeaponPistSpeed1);

	WeaponPistSpeed.SetFileId("pist_speed");
	WeaponPistSpeed.SetBoundaries(0.f, 10.f);
	WeaponPistSpeed.SetValue(1.0f);
	WeaponPistGroup.PlaceLabledControl("", this, &WeaponPistSpeed);

	WeaponPistFoV1.SetFileId("pist_fov1");
	WeaponPistGroup.PlaceLabledControl("Field of View", this, &WeaponPistFoV1);

	WeaponPistFoV.SetFileId("pist_fov");
	WeaponPistFoV.SetBoundaries(0.f, 20.f);
	WeaponPistFoV.SetValue(3.f);
	WeaponPistGroup.PlaceLabledControl("", this, &WeaponPistFoV);

/*	WeaponPistRecoil1.SetFileId("pist_recoil1");
	WeaponPistGroup.PlaceLabledControl("Recoil Control", this, &WeaponPistRecoil1);

	WeaponPistRecoil.SetFileId("pist_recoil");
	WeaponPistRecoil.SetBoundaries(0.f, 2.f);
	WeaponPistRecoil.SetValue(1.f);
	WeaponPistGroup.PlaceLabledControl("", this, &WeaponPistRecoil); */

	WeaponPistInacc1.SetFileId("pist_inacc1");
	WeaponPistGroup.PlaceLabledControl("Weapon Inaccuracy", this, &WeaponPistInacc1);

	WeaponPistInacc.SetFileId("pist_inacc");
	WeaponPistInacc.SetBoundaries(0.f, 15.f);
	WeaponPistInacc.SetValue(0.f);
	WeaponPistGroup.PlaceLabledControl("", this, &WeaponPistInacc);

	WeaponPistAimtime.SetValue(0);
	WeaoponPistStartAimtime.SetValue(0);

#pragma endregion

#pragma region Snipers
	WeaponSnipGroup.SetPosition(16 + 85, 375);
	WeaponSnipGroup.SetText("Snipers");
	WeaponSnipGroup.SetSize(240, 150);
	RegisterControl(&WeaponSnipGroup);

	WeaponSnipHitbox1.SetFileId("main_hitbox1");
	WeaponSnipGroup.PlaceLabledControl("Hitbox", this, &WeaponSnipHitbox1);

	WeaponSnipHitbox.SetFileId("snip_hitbox");
	WeaponSnipHitbox.AddItem("Head");
	WeaponSnipHitbox.AddItem("Neck");
	WeaponSnipHitbox.AddItem("Chest");
	WeaponSnipHitbox.AddItem("Stomach");
	WeaponSnipHitbox.AddItem("Multihitbox");
	WeaponSnipGroup.PlaceLabledControl("", this, &WeaponSnipHitbox);

	WeaponSnipSpeed1.SetFileId("snip_speed1");
	WeaponSnipGroup.PlaceLabledControl("Max Speed", this, &WeaponSnipSpeed1);

	WeaponSnipSpeed.SetFileId("snip_speed");
	WeaponSnipSpeed.SetBoundaries(0.f, 10.f);
	WeaponSnipSpeed.SetValue(1.5f);
	WeaponSnipGroup.PlaceLabledControl("", this, &WeaponSnipSpeed);

	WeaponSnipFoV1.SetFileId("snip_fov1");
	WeaponSnipGroup.PlaceLabledControl("Field of View", this, &WeaponSnipFoV1);

	WeaponSnipFoV.SetFileId("snip_fov");
	WeaponSnipFoV.SetBoundaries(0.f, 20.f);
	WeaponSnipFoV.SetValue(2.f);
	WeaponSnipGroup.PlaceLabledControl("", this, &WeaponSnipFoV);

	/*	WeaponSnipRecoil1.SetFileId("snip_recoil1");
	WeaponSnipGroup.PlaceLabledControl("Recoil Control", this, &WeaponSnipRecoil1);

	WeaponSnipRecoil.SetFileId("snip_recoil");
	WeaponSnipRecoil.SetBoundaries(0.f, 2.f);
	WeaponSnipRecoil.SetValue(1.f);
	WeaponSnipGroup.PlaceLabledControl("", this, &WeaponSnipRecoil); */

	WeaponSnipInacc1.SetFileId("wconf_inacc_ak1");
	WeaponSnipGroup.PlaceLabledControl("Weapon Inaccuracy", this, &WeaponSnipInacc1);

	WeaponSnipInacc.SetFileId("wconf_inacc_ak");
	WeaponSnipInacc.SetBoundaries(0.f, 15.f);
	WeaponSnipGroup.PlaceLabledControl("", this, &WeaponSnipInacc);

	WeaponSnipAimtime.SetValue(0);
	WeaoponSnipStartAimtime.SetValue(0);

#pragma region MPs
	WeaponOtherGroup.SetPosition(285 + 85, 375);
	WeaponOtherGroup.SetText("Others");
	WeaponOtherGroup.SetSize(240, 150);
	RegisterControl(&WeaponOtherGroup);

	WeaponOtherHitbox1.SetFileId("other_hitbox1");
	WeaponOtherGroup.PlaceLabledControl("Hitbox", this, &WeaponOtherHitbox1);

	WeaponOtherHitbox.SetFileId("other_hitbox");
	WeaponOtherHitbox.AddItem("Head");
	WeaponOtherHitbox.AddItem("Neck");
	WeaponOtherHitbox.AddItem("Chest");
	WeaponOtherHitbox.AddItem("Stomach");
	WeaponOtherHitbox.AddItem("Multihitbox");
	WeaponOtherGroup.PlaceLabledControl("", this, &WeaponOtherHitbox);

	WeaponOtherSpeed1.SetFileId("other_speed1");
	WeaponOtherGroup.PlaceLabledControl("Max Speed", this, &WeaponOtherSpeed1);

	WeaponOtherSpeed.SetFileId("other_speed");
	WeaponOtherSpeed.SetBoundaries(0.f, 10.f);
	WeaponOtherSpeed.SetValue(1.0f);
	WeaponOtherGroup.PlaceLabledControl("", this, &WeaponOtherSpeed);

	WeaponOtherFoV1.SetFileId("other_speed1");
	WeaponOtherGroup.PlaceLabledControl("Field of View", this, &WeaponOtherFoV1);

	WeaponOtherFoV.SetFileId("other_fov");
	WeaponOtherFoV.SetBoundaries(0.f, 20.f);
	WeaponOtherFoV.SetValue(4.f);
	WeaponOtherGroup.PlaceLabledControl("", this, &WeaponOtherFoV);

/*	WeaponOtherRecoil1.SetFileId("other_recoil1");
	WeaponOtherGroup.PlaceLabledControl("Recoil Control", this, &WeaponOtherRecoil1); 

	WeaponOtherRecoil.SetFileId("other_recoil");
	WeaponOtherRecoil.SetBoundaries(0.f, 2.f);
	WeaponOtherRecoil.SetValue(1.f);
	WeaponOtherGroup.PlaceLabledControl("", this, &WeaponOtherRecoil); */

	WeaponOtherInacc1.SetFileId("other_inacc1");
	WeaponOtherGroup.PlaceLabledControl("Weapon Inaccuracy", this, &WeaponOtherInacc1);

	WeaponOtherInacc.SetFileId("other_inacc");
	WeaponOtherInacc.SetBoundaries(0.f, 15.f);
	WeaponOtherGroup.PlaceLabledControl("", this, &WeaponOtherInacc);

	WeaponOtherAimtime.SetValue(0);
	WeaoponOtherStartAimtime.SetValue(0);
#pragma endregion
	/*
	#pragma region Shotguns
	WeaponShotgunGroup.SetPosition(222 + 85, 365);
	WeaponShotgunGroup.SetText("Shotguns");
	WeaponShotgunGroup.SetSize(200, 136);
	RegisterControl(&WeaponShotgunGroup);

	WeaponOtherHitbox.SetFileId("shotgun_hitbox");
	WeaponOtherHitbox.AddItem("Head");
	WeaponOtherHitbox.AddItem("Neck");
	WeaponOtherHitbox.AddItem("Chest");
	WeaponOtherHitbox.AddItem("Stomach");
	WeaponOtherHitbox.AddItem("Multihitbox");
	WeaponShotgunGroup.PlaceLabledControl("Hitbox", this, &WeaponOtherHitbox);

	WeaponOtherSpeed.SetFileId("shotgun_speed");
	WeaponOtherSpeed.SetBoundaries(0.f, 100.f);
	WeaponOtherSpeed.SetValue(1.0f);
	WeaponShotgunGroup.PlaceLabledControl("Max Speed", this, &WeaponOtherSpeed);

	WeaponOtherFoV.SetFileId("shotgun_fov");
	WeaponOtherFoV.SetBoundaries(0.f, 20.f);
	WeaponOtherFoV.SetValue(3.f);
	WeaponShotgunGroup.PlaceLabledControl("Field of View", this, &WeaponOtherFoV);

	WeaponOtherRecoil.SetFileId("snip_recoil");
	WeaponOtherRecoil.SetBoundaries(0.f, 2.f);
	WeaponOtherRecoil.SetValue(1.f);
	WeaponShotgunGroup.PlaceLabledControl("Recoil Control", this, &WeaponOtherRecoil);

	WeaponOtherAimtime.SetValue(0);
	WeaoponOtherStartAimtime.SetValue(0);

	#pragma endregion

	#pragma region Machineguns
	WeaponMGGroup.SetPosition(428 + 85, 365);
	WeaponMGGroup.SetText("Heavy");
	WeaponMGGroup.SetSize(200, 136);
	RegisterControl(&WeaponMGGroup);

	WeaponOtherHitbox.SetFileId("mg_hitbox");
	WeaponOtherHitbox.AddItem("Head");
	WeaponOtherHitbox.AddItem("Neck");
	WeaponOtherHitbox.AddItem("Chest");
	WeaponOtherHitbox.AddItem("Stomach");
	WeaponOtherHitbox.AddItem("Multihitbox");
	WeaponMGGroup.PlaceLabledControl("Hitbox", this, &WeaponOtherHitbox);

	WeaponOtherSpeed.SetFileId("mg_speed");
	WeaponOtherSpeed.SetBoundaries(0.f, 100.f);
	WeaponOtherSpeed.SetValue(1.0f);
	WeaponMGGroup.PlaceLabledControl("Max Speed", this, &WeaponOtherSpeed);

	WeaponOtherFoV.SetFileId("mg_fov");
	WeaponOtherFoV.SetBoundaries(0.f, 20.f);
	WeaponOtherFoV.SetValue(4.f);
	WeaponMGGroup.PlaceLabledControl("Field of View", this, &WeaponOtherFoV);

	WeaponOtherRecoil.SetFileId("mg_recoil");
	WeaponOtherRecoil.SetBoundaries(0.f, 2.f);
	WeaponOtherRecoil.SetValue(1.f);
	WeaponMGGroup.PlaceLabledControl("Recoil Control", this, &WeaponOtherRecoil);

	WeaponOtherAimtime.SetValue(0);
	WeaoponOtherStartAimtime.SetValue(0);

	#pragma endregion */
}

void CVisualTab::Setup()
{
	SetTitle("C");

#pragma region Options
	OptionsGroup.SetText("Player ESP");
	OptionsGroup.SetPosition(16 + 85, 22);
	OptionsGroup.SetSize(240, 320);
	RegisterControl(&OptionsGroup);

	Active.SetFileId("opt_enable");
	OptionsGroup.PlaceLabledControl("Enabled", this, &Active);

	OptionsTeammates.SetFileId("opt_enemyonly");
	OptionsGroup.PlaceLabledControl("Teammates", this, &OptionsTeammates);

	OptionsBox.SetFileId("opt_box");
	OptionsGroup.PlaceLabledControl("Bounding Box", this, &OptionsBox);

	OptionsHealth.SetFileId("opt_hp");
	OptionsGroup.PlaceLabledControl("Health Bar", this, &OptionsHealth);

	OptionsArmor.SetFileId("opt_armor");
	OptionsGroup.PlaceLabledControl("Armor Bar", this, &OptionsArmor);

	OptionsName.SetFileId("opt_name");
	OptionsGroup.PlaceLabledControl("Name", this, &OptionsName);

	OptionsInfo.SetFileId("opt_info");
	OptionsGroup.PlaceLabledControl("Info", this, &OptionsInfo);

	DrawDistance.SetFileId("opt_drawdist");
	OptionsGroup.PlaceLabledControl("Distance", this, &DrawDistance);

	OptionsWeapon.SetFileId("opt_weapon");
	OptionsGroup.PlaceLabledControl("Weapon", this, &OptionsWeapon);

	OptionsMoney.SetFileId("opt_money");
	OptionsGroup.PlaceLabledControl("Money", this, &OptionsMoney);

	DLight.SetFileId("d_light");
	OptionsGroup.PlaceLabledControl("Dlight", this, &DLight);

	OptionsGlow.SetFileId("opt_glow");
	OptionsGroup.PlaceLabledControl("Glow", this, &OptionsGlow);

	OtherHitmarker.SetFileId("otr_hitmarkersound");
	OptionsGroup.PlaceLabledControl("Hitmarker", this, &OtherHitmarker);

	OptionsSkeleton.SetFileId("opt_bone");
	OptionsGroup.PlaceLabledControl("Skeleton", this, &OptionsSkeleton);

	//OptionsGray.SetFileId("opt_gray");
	//OptionsGroup.PlaceLabledControl("Gray Mode", this, &OptionsGray);

	lbyidicador.SetFileId("opt_lbyidicador");
	OptionsGroup.PlaceLabledControl("LBY ", this, &lbyidicador);

	backtrackskele.SetFileId("opt_btskele");
	OptionsGroup.PlaceLabledControl("Backtrack Esp", this, &backtrackskele);

	LegitMode1.SetFileId("opt_legitmode");
	OptionsGroup.PlaceLabledControl("Legit Mode ", this, &LegitMode1);
#pragma endregion Setting up the Options controls

#pragma region models

	/*i forgot what to put in the group with them... and its 4:20 am*/
	// i gotchu fam

	ModelG.SetText("Colored Models");
	ModelG.SetPosition(16 + 85, 320 + 45);
	ModelG.SetSize(240, 155);
	RegisterControl(&ModelG);

	OptionsChams1.SetFileId("opt_chams1");
	ModelG.PlaceLabledControl("Colored Model", this, &OptionsChams1);

	OptionsChams.SetFileId("opt_chams");
	OptionsChams.AddItem("Off");
	OptionsChams.AddItem("Normal");
	OptionsChams.AddItem("Flat");
	OptionsChams.AddItem("Visible Only");
	ModelG.PlaceLabledControl("", this, &OptionsChams);

	OtherNoHands1.SetFileId("otr_hands1");
	ModelG.PlaceLabledControl("Colored Hands", this, &OtherNoHands1);

	OtherNoHands.SetFileId("otr_hands");
	OtherNoHands.AddItem("Off");
	OtherNoHands.AddItem("Glass");
	OtherNoHands.AddItem("Transparent");
	OtherNoHands.AddItem("Chams");
	ModelG.PlaceLabledControl("", this, &OtherNoHands);

	OptionsDroppedc1.SetFileId("otr_hands1");
	ModelG.PlaceLabledControl("Colored Weapons", this, &OptionsDroppedc1);

	OptionsDroppedc.SetFileId("otr_hands1");
	OptionsDroppedc.AddItem("Off");
	OptionsDroppedc.AddItem("Chams");
	OptionsDroppedc.AddItem("Chams Colored");
	OptionsDroppedc.AddItem("Glass");
	ModelG.PlaceLabledControl("", this, &OptionsDroppedc);

#pragma endregion models memes

#pragma region Other
	OtherGroup.SetText("Other ESP");
	OtherGroup.SetPosition(285 + 85, 22);
	OtherGroup.SetSize(240, 230 - 45);
	RegisterControl(&OtherGroup);

	OtherRadar.SetFileId("otr_radar");
	OtherGroup.PlaceLabledControl("Radar", this, &OtherRadar);

	FiltersWeapons.SetFileId("otr_weaps");
	OtherGroup.PlaceLabledControl("Weapons", this, &FiltersWeapons);

	OtherNades.SetFileId("otr_nades");
	OtherGroup.PlaceLabledControl("Grenades", this, &OtherNades);

//	SniperCrosshair.SetFileId("eff_snipercrosshair"); 
//	OtherGroup.PlaceLabledControl("Crosshair on Sniper", this, &SniperCrosshair);

	FiltersC4.SetFileId("ftr_c4");
	OtherGroup.PlaceLabledControl("Bomb", this, &FiltersC4);

	Grenades2.SetFileId("otr_backtomexico");
	OtherGroup.PlaceLabledControl("Show Grenade Trajectory", this, &Grenades2);

	OtherSpectators.SetFileId("otr_speclist");
	OtherGroup.PlaceLabledControl("Spectators", this, &OtherSpectators);

	OptionsCompRank.SetFileId("opt_comprank");
	OtherGroup.PlaceLabledControl("Comp Rank", this, &OptionsCompRank);

	DrawLinesAA.SetFileId("otr_aalines");
	OtherGroup.PlaceLabledControl("Draw AA Lines", this, &DrawLinesAA);

	/* Clock.SetFileId("otr_clock");
	Clock.SetState(false);
	OtherGroup.PlaceLabledControl("Hours", this, &Clock); */

	/*	AmbientRed.SetFileId("otr_ambientred");
	AmbientRed.SetBoundaries(0.f, 1.f);
	AmbientRed.SetValue(0.f);
	OtherGroup.PlaceLabledControl("Ambient Red", this, &AmbientRed);

	AmbientGreen.SetFileId("otr_ambientgreen");
	AmbientGreen.SetBoundaries(0.f, 1.f);
	AmbientGreen.SetValue(0.f);
	OtherGroup.PlaceLabledControl("Ambient Green", this, &AmbientGreen);

	AmbientBlue.SetFileId("otr_ambientblue");
	AmbientBlue.SetBoundaries(0.f, 1.f);
	AmbientBlue.SetValue(0.f);
	OtherGroup.PlaceLabledControl("Ambient Blue", this, &AmbientBlue); */ //just for now lol 

	/*	SightLines.SetFileId("otp_bulletrace");
	OtherGroup.PlaceLabledControl("Sight Lines - BROKE RN", this, &SightLines);

	SightLinesLength.SetFileId("otp_bulletracerange");
	OtherGroup.PlaceLabledControl("", this, &SightLinesLength);
	SightLinesLength.SetBoundaries(0.f, 200.f);
	SightLinesLength.SetValue(100.f); */

#pragma endregion Setting up the Other controls

#pragma region Effects
	FiltersGroup.SetText("Effects");
	FiltersGroup.SetPosition(285 + 85, 230);
	FiltersGroup.SetSize(240, 300 - 10);
	RegisterControl(&FiltersGroup);

	OtherNoFlash.SetFileId("otr_noflash");
	FiltersGroup.PlaceLabledControl("Remove Flashbang  Effects", this, &OtherNoFlash);

	OtherNoSmoke.SetFileId("otr_nosmoke");
	FiltersGroup.PlaceLabledControl("Remove Smoke grenades", this, &OtherNoSmoke);

	OtherNoVisualRecoil.SetFileId("eff_visrecoil");
	FiltersGroup.PlaceLabledControl("Visual Recoil Adjustment", this, &OtherNoVisualRecoil);

	OtherNoScope.SetFileId("otr_noscope");
	FiltersGroup.PlaceLabledControl("Remove Scope", this, &OtherNoScope);

	DisablePostProcess.SetFileId("otr_disablepostprocess");
	FiltersGroup.PlaceLabledControl("Disable Post Process", this, &DisablePostProcess);

	OtherThirdperson.SetFileId("aa_thirdpsr");
	FiltersGroup.PlaceLabledControl("Force Third Person", this, &OtherThirdperson);

	ThirdpersonAngle1.SetFileId("aa_thirdpersonMode1");
	FiltersGroup.PlaceLabledControl("Third Person Angle", this, &ThirdpersonAngle1);

	ThirdpersonAngle.SetFileId("aa_thirdpersonMode");
	ThirdpersonAngle.AddItem("Real Angle");
	ThirdpersonAngle.AddItem("Fake Angle");
	ThirdpersonAngle.AddItem("LBY Angle");
	FiltersGroup.PlaceLabledControl("", this, &ThirdpersonAngle);

	ThirdPersonKeyBind1.SetFileId("aa_thirdpersonKey1");
	FiltersGroup.PlaceLabledControl("Third Person Key", this, &ThirdPersonKeyBind1);

	ThirdPersonKeyBind.SetFileId("aa_thirdpersonKey");
	FiltersGroup.PlaceLabledControl("", this, &ThirdPersonKeyBind);

	AmbientSkybox1.SetFileId("otr_skybox1");
	FiltersGroup.PlaceLabledControl("Skybox", this, &AmbientSkybox1);

	AmbientSkybox.SetFileId("otr_skybox");
	AmbientSkybox.AddItem("Off");
	AmbientSkybox.AddItem("Night");
	AmbientSkybox.AddItem("Vertigo");
	AmbientSkybox.AddItem("Jungle");
	FiltersGroup.PlaceLabledControl("", this, &AmbientSkybox);

	OtherCrosshair1.SetFileId("otr_crosshair1");
	FiltersGroup.PlaceLabledControl("Crosshair", this, &OtherCrosshair1);

	OtherCrosshair.SetFileId("otr_crosshair");
	OtherCrosshair.AddItem("Off");
	OtherCrosshair.AddItem("Recoil + Wall");
	OtherCrosshair.AddItem("Spread");
	OtherCrosshair.AddItem("Recoil + Damage");
	FiltersGroup.PlaceLabledControl("", this, &OtherCrosshair);

	//FiltersChickens.SetFileId("ftr_chickens");
	//FiltersGroup.PlaceLabledControl("Chickens", this, &FiltersChickens);

	/*CRASHES YOU*/
	//FiltersWeapons.SetFileId("ftr_granades");
	//FiltersGroup.PlaceLabledControl("Grenade Trajectory", this, &Granades);
#pragma endregion Setting up the Effects shit
}

void CMiscTab::Setup()
{
	SetTitle("D");

#pragma region Other
	OtherGroup.SetPosition(16 + 85, 22);
	OtherGroup.SetSize(240, 250);//500 is bottom js
	OtherGroup.SetText("Miscellaneous");
	RegisterControl(&OtherGroup);

/*#pragma region GloveModel

	GlovesApply.SetText("Apply Gloves");
	GlovesApply.SetCallback(GlovesApplyCallbk);
	GlovesApply.SetPosition(16 + 85, 410);//400, 480
	GlovesApply.SetSize(240, 106);
	RegisterControl(&GlovesApply);

#pragma endregion */
	//ThirdpersonGhost.SetFileId("ngr_1");
	//OtherGroup.PlaceLabledControl("Third Person Ghost", this, &ThirdpersonGhost);//just temp here
	OtherFOV1.SetFileId("otr_fov1");
	OtherGroup.PlaceLabledControl("Override FOV", this, &OtherFOV1);

	OtherFOV.SetFileId("otr_fov");
	OtherFOV.SetBoundaries(0.f, 180.f);
	OtherFOV.SetValue(90.f);
	OtherGroup.PlaceLabledControl("", this, &OtherFOV);

	OtherAutoJump.SetFileId("otr_autojump");;
	OtherGroup.PlaceLabledControl("Bunny Hop", this, &OtherAutoJump);

	OtherAutoStrafe1.SetFileId("otr_strafe1");
	OtherGroup.PlaceLabledControl("Air Strafe", this, &OtherAutoStrafe1);

	OtherAutoStrafe.SetFileId("otr_strafe");
	OtherAutoStrafe.AddItem("Off");
	OtherAutoStrafe.AddItem("Legit");
	OtherAutoStrafe.AddItem("Rage");
	OtherGroup.PlaceLabledControl("", this, &OtherAutoStrafe);

	OtherEdgeJump.SetFileId("otr_edgejump");
	OtherGroup.PlaceLabledControl("Jump At Edge", this, &OtherEdgeJump);

	OtherAirStuck1.SetFileId("otr_astuck1");
	OtherGroup.PlaceLabledControl("Air Stuck", this, &OtherAirStuck1);

	OtherAirStuck.SetFileId("otr_astuck");
	OtherGroup.PlaceLabledControl("", this, &OtherAirStuck);

	OtherFakeWalk1.SetFileId("otr_fakewalk1");
	OtherGroup.PlaceLabledControl("Fakewalk", this, &OtherFakeWalk1);

	OtherFakeWalk.SetFileId("otr_fakewalk");
	OtherGroup.PlaceLabledControl("", this, &OtherFakeWalk);

	OtherCircleStrafe.SetFileId("otr_circlestrafe");
	OtherGroup.PlaceLabledControl("Circle Strafe", this, &OtherCircleStrafe);

	OtherCircleStrafeKey.SetFileId("otr_circlestrafeKey");
	OtherGroup.PlaceLabledControl("", this, &OtherCircleStrafeKey);

//	TrashTalk.SetFileId("otr_trashtalk");
//	OtherGroup.PlaceLabledControl("ChatSpam Shittalk", this, &TrashTalk); (its an avoz meme)

	Watermark.SetFileId("otr_watermarker");
	Watermark.SetState(true);
	OtherGroup.PlaceLabledControl("Watermark", this, &Watermark); // make them rep that cheat name :^)

//	OtherTeamChat.SetFileId("otr_teamchat");
//	OtherGroup.PlaceLabledControl("Team Chat Only", this, &OtherTeamChat);

//	OtherChatDelay.SetFileId("otr_chatdelay");
//	OtherChatDelay.SetBoundaries(0.1, 3.0);
//	OtherChatDelay.SetValue(1.0);
//	OtherGroup.PlaceLabledControl("Spam Delay", this, &OtherChatDelay);

	OtherAutoAccept.SetFileId("otr_autoaccept");
	OtherGroup.PlaceLabledControl("Auto Accept", this, &OtherAutoAccept);

//	OtherSlowMotion.SetFileId("otr_slowmotionbutton");
//	OtherGroup.PlaceLabledControl("Slow Motion", this, &OtherSlowMotion);

	KnifeGroup.SetPosition(16 + 85, 22+250+20);
	KnifeGroup.SetText("Knife Changer");
	KnifeGroup.SetSize(240, 60);
	RegisterControl(&KnifeGroup);

	SkinEnable.SetFileId("Skin_enable");
	KnifeGroup.PlaceLabledControl("Enable Skins & Knife & Gloves", this, &SkinEnable);

	KnifeModel.SetFileId("knife_model");
	KnifeModel.AddItem("Bayonet");
	KnifeModel.AddItem("Bowie Knife");
	KnifeModel.AddItem("Butterfly Knife");
	KnifeModel.AddItem("Falchion Knife");
	KnifeModel.AddItem("Flip Knife");
	KnifeModel.AddItem("Gut Knife");
	KnifeModel.AddItem("Huntsman Knife");
	KnifeModel.AddItem("Karambit");
	KnifeModel.AddItem("M9 Bayonet");
	KnifeModel.AddItem("Shadow Daggers");
	KnifeGroup.PlaceLabledControl("", this, &KnifeModel);

	Gloves.SetPosition(16 + 85, 22 + 250 + 20+60 +20);
	Gloves.SetText("Override Gloves");
	Gloves.SetSize(240, 100);
	RegisterControl(&Gloves);

	EnableGloves.SetFileId("gloves_enable");
	Gloves.PlaceLabledControl("Enabled", this, &EnableGloves);

	GloveModel.SetFileId("glove_model");
	GloveModel.AddItem("Off");
	GloveModel.AddItem("Bloodhound");
	GloveModel.AddItem("Handwrap");
	GloveModel.AddItem("Driver");
	GloveModel.AddItem("Sport");
	GloveModel.AddItem("Motorcycle");
	GloveModel.AddItem("Specialist");
	Gloves.PlaceLabledControl("Model", this, &GloveModel);

	GloveSkin.SetFileId("glove_skin");
	GloveSkin.AddItem("Off");
	GloveSkin.AddItem("Bloodhound - Snakebite");
	GloveSkin.AddItem("Bloodhound - Charred");
	GloveSkin.AddItem("Bloodhound - Guerrilla");
	GloveSkin.AddItem("Bloodhound - Bronzed");
	GloveSkin.AddItem("Handwrap - Slaughter");
	GloveSkin.AddItem("Handwrap - Badlands");
	GloveSkin.AddItem("Handwrap - Leather");
	GloveSkin.AddItem("Handwrap - Spruce DDPAT");
	GloveSkin.AddItem("Driver - Crimson Weave");
	GloveSkin.AddItem("Driver - Lunar Weave");
	GloveSkin.AddItem("Driver - Diamondback");
	GloveSkin.AddItem("Driver - Convoy");
	GloveSkin.AddItem("Sport - Pandoras Box");
	GloveSkin.AddItem("Sport - Hedge Naze");
	GloveSkin.AddItem("Sport - Superconductor");
	GloveSkin.AddItem("Sport - Arid");
	GloveSkin.AddItem("Motorcycle - Spearmint");
	GloveSkin.AddItem("Motorcycle - Cool Mint");
	GloveSkin.AddItem("Motorcycle - Boom");
	GloveSkin.AddItem("Motorcycle - Eclipse");
	GloveSkin.AddItem("Specialist - Crimson Kimono");
	GloveSkin.AddItem("Specialist - Emerald Web");
	GloveSkin.AddItem("Specialist - Foundation");
	GloveSkin.AddItem("Specialist - Forest DDPAT");
	Gloves.PlaceLabledControl("Skin", this, &GloveSkin);

#pragma endregion other random options

#pragma region Settings
	FakeLagGroup.SetPosition(285 + 85, 22);
	FakeLagGroup.SetSize(240, 230 - 45);
	FakeLagGroup.SetText("Settings");
	RegisterControl(&FakeLagGroup);

	OtherSafeMode.SetFileId("otr_safemode");
	OtherSafeMode.SetState(true);
	FakeLagGroup.PlaceLabledControl("Anti Untrusted", this, &OtherSafeMode);

	FakeLagGroup.PlaceLabledControl("Name Changer", this, &word1);


	OtherChatSpam.SetFileId("otr_spam");
	OtherChatSpam.AddItem("Off");
	OtherChatSpam.AddItem("Name Stealer");
	OtherChatSpam.AddItem("Chatspammer");
	OtherChatSpam.AddItem("Ayyware Crasher");
	OtherChatSpam.AddItem("Blank Name");
	FakeLagGroup.PlaceLabledControl("", this, &OtherChatSpam);
	FakeLagGroup.PlaceLabledControl("Clantag Changer", this, &word2);

	OtherClantag.SetFileId("otr_spam");
	OtherClantag.AddItem("Off");
	OtherClantag.AddItem("Corruption Clantag");
	OtherClantag.AddItem("Fake Gamesense");
	OtherClantag.AddItem("Blank");
	OtherClantag.AddItem("Valve");
	OtherClantag.AddItem("Current Time");
	FakeLagGroup.PlaceLabledControl("", this, &OtherClantag);

#pragma endregion Settings

#pragma region Presets
	BuyBotGroup.SetPosition(285 + 85, 232);
	BuyBotGroup.SetSize(240, 300 - 10);
	BuyBotGroup.SetText("Presets");
	RegisterControl(&BuyBotGroup);

	ConfigBox.SetFileId("cfg_box");
	ConfigBox.AddItem("Legit");
	ConfigBox.AddItem("Legit 2");
	ConfigBox.AddItem("HvH");
	ConfigBox.AddItem("HvH 2");
	ConfigBox.AddItem("HvH 3");
	ConfigBox.AddItem("Nospread");
	BuyBotGroup.PlaceLabledControl("", this, &ConfigBox);
	
	SaveButton.SetFileId("save");
	SaveButton.SetText("Save");
	SaveButton.SetCallback(SaveCallbk);
	BuyBotGroup.PlaceLabledControl("", this, &SaveButton);

	ConfigSpacing1.SetFileId("otr_astuck1");
	BuyBotGroup.PlaceLabledControl("", this, &ConfigSpacing1);

	LoadButton.SetFileId("load");
	LoadButton.SetText("Load");
	LoadButton.SetCallback(LoadCallbk);
	BuyBotGroup.PlaceLabledControl("", this, &LoadButton);

	ConfigSpacing2.SetFileId("otr_astuck1");
	BuyBotGroup.PlaceLabledControl("", this, &ConfigSpacing2);

	SkinApply.SetFileId("apply");
	SkinApply.SetText("Apply");
	SkinApply.SetCallback(KnifeApplyCallbk);
	BuyBotGroup.PlaceLabledControl("", this, &SkinApply);

	ConfigSpacing3.SetFileId("otr_astuck1");
	BuyBotGroup.PlaceLabledControl("", this, &ConfigSpacing3);
	/*
	UnloadButton.SetFileId("unload");
	UnloadButton.SetText("Unload");
	UnloadButton.SetCallback(UnLoadCallbk);
	BuyBotGroup.PlaceLabledControl("", this, &UnloadButton);*/

	EnableBuyBot.SetFileId("bb_enable");
	BuyBotGroup.PlaceLabledControl("Automatic Buy Bot", this, &EnableBuyBot);

	BuyBot.SetFileId("buybot");
	BuyBot.AddItem("Off");
	BuyBot.AddItem("Ak-47/M4A1");
	BuyBot.AddItem("AWP");
	BuyBot.AddItem("Auto Sniper and Dual Elites");
	BuyBot.AddItem("SSG-08 (Scout)");
	BuyBotGroup.PlaceLabledControl("", this, &BuyBot);

	BuyBotGrenades1.SetFileId("buybot_grenades1");
	BuyBotGroup.PlaceLabledControl("Equipment Buy Bot", this, &BuyBotGrenades1);

	BuyBotGrenades.SetFileId("buybot_grenades");
	BuyBotGrenades.AddItem("Off");
	BuyBotGrenades.AddItem("FB, Smoke, and HE");
	BuyBotGrenades.AddItem("FB, Smoke, HE, and Fire");
	BuyBotGroup.PlaceLabledControl("", this, &BuyBotGrenades);

	BuyBotKevlar.SetFileId("buybot_kevlar");
	BuyBotGroup.PlaceLabledControl("Buy Armor", this, &BuyBotKevlar);

	BuyBotDefuser.SetFileId("buybot_defuser");
	BuyBotGroup.PlaceLabledControl("Buy a Defuse Kit", this, &BuyBotDefuser);


#pragma endregion Presets
}

void CSkinchangerTab::Setup()
{
	SetTitle("E");

#pragma region Knife

	//KnifeSkin.SetFileId("knife_skin");
	//KnifeGroup.PlaceLabledControl("Skin ID", this, &KnifeSkin);
#pragma endregion

#pragma region Machineguns
	MachinegunsGroup.SetPosition(279 + 85, 11);
	MachinegunsGroup.SetText("Heavy");
	MachinegunsGroup.SetSize(250, 80);
	RegisterControl(&MachinegunsGroup);

	NEGEVSkin.SetFileId("negev_skin");
	NEGEVSkin.AddItem("Anodized Navy");
	NEGEVSkin.AddItem("Man-o'-war");
	NEGEVSkin.AddItem("Bratatat");
	NEGEVSkin.AddItem("Desert-Strike");
	NEGEVSkin.AddItem("Nuclear Waste");
	NEGEVSkin.AddItem("Loudmouth");
	NEGEVSkin.AddItem("Power Loader");
	MachinegunsGroup.PlaceLabledControl("Negev", this, &NEGEVSkin);

	M249Skin.SetFileId("m249_skin");
	M249Skin.AddItem("System Lock");
	M249Skin.AddItem("Shipping Forecast");
	M249Skin.AddItem("Impact Drill");
	M249Skin.AddItem("Nebula Crusader");
	M249Skin.AddItem("Spectre");
	MachinegunsGroup.PlaceLabledControl("M249", this, &M249Skin);

#pragma endregion

#pragma region Snipers
	Snipergroup.SetPosition(16 + 85, 11);//78
	Snipergroup.SetText("Snipers");
	Snipergroup.SetSize(246, 125);
	RegisterControl(&Snipergroup);

	AWPSkin.SetFileId("awp_skin");
	AWPSkin.AddItem("BOOM");
	AWPSkin.AddItem("Dragon Lore");
	AWPSkin.AddItem("Pink DDPAT");
	AWPSkin.AddItem("Fever Dream");
	AWPSkin.AddItem("Lightning Strike");
	AWPSkin.AddItem("Corticera");
	AWPSkin.AddItem("Redline");
	AWPSkin.AddItem("Man-o'-war");
	AWPSkin.AddItem("Graphite");
	AWPSkin.AddItem("Electric Hive");
	AWPSkin.AddItem("Pit Viper");
	AWPSkin.AddItem("Asiimov");
	AWPSkin.AddItem("Oni Taiji");
	AWPSkin.AddItem("Medusa");
	AWPSkin.AddItem("Sun in Leo");
	AWPSkin.AddItem("Hyper Beast");
	AWPSkin.AddItem("Elite Build");
	Snipergroup.PlaceLabledControl("AWP", this, &AWPSkin);

	SSG08Skin.SetFileId("sgg08_skin");
	SSG08Skin.AddItem("Dragonfire");
	SSG08Skin.AddItem("Blood in the Water");
	SSG08Skin.AddItem("Ghost Crusader");
	SSG08Skin.AddItem("Detour");
	SSG08Skin.AddItem("Abyss");
	SSG08Skin.AddItem("Big Iron");
	Snipergroup.PlaceLabledControl("SGG O8", this, &SSG08Skin);

	SCAR20Skin.SetFileId("scar20_skin");
	SCAR20Skin.AddItem("Splash Jam");
	SCAR20Skin.AddItem("Emerald");
	SCAR20Skin.AddItem("Crimson Web");
	SCAR20Skin.AddItem("Cardiac");
	SCAR20Skin.AddItem("Bloodsport");
	SCAR20Skin.AddItem("Cyrex");
	SCAR20Skin.AddItem("Grotto");
	Snipergroup.PlaceLabledControl("SCAR-2O", this, &SCAR20Skin);

	G3SG1Skin.SetFileId("g3sg1_skin");
	G3SG1Skin.AddItem("Demeter");
	G3SG1Skin.AddItem("Azure Zebra");
	G3SG1Skin.AddItem("Green Apple");
	G3SG1Skin.AddItem("Orange Kimono");
	G3SG1Skin.AddItem("Neon Kimono");
	G3SG1Skin.AddItem("Murky");
	G3SG1Skin.AddItem("Chronos");
	G3SG1Skin.AddItem("Flux");
	G3SG1Skin.AddItem("The Executioner");
	Snipergroup.PlaceLabledControl("G3SG1", this, &G3SG1Skin);
#pragma endregion

#pragma region Shotguns
	Shotgungroup.SetPosition(279 + 85, 98);
	Shotgungroup.SetText("Shotguns");
	Shotgungroup.SetSize(250, 125);
	RegisterControl(&Shotgungroup);

	MAG7Skin.SetFileId("mag7_skin");
	MAG7Skin.AddItem("Counter Terrace");
	MAG7Skin.AddItem("Bulldozer");
	MAG7Skin.AddItem("Heat");
	Shotgungroup.PlaceLabledControl("Mag-7", this, &MAG7Skin);

	XM1014Skin.SetFileId("xm1014_skin");
	XM1014Skin.AddItem("Tranquality");
	XM1014Skin.AddItem("Teclu Burner");
	XM1014Skin.AddItem("Scumbria");
	XM1014Skin.AddItem("Heaven Guard");
	XM1014Skin.AddItem("Quicksilver");
	Shotgungroup.PlaceLabledControl("XM1O14", this, &XM1014Skin);

	SAWEDOFFSkin.SetFileId("sawedoff_skin");
	SAWEDOFFSkin.AddItem("Serenity");
	SAWEDOFFSkin.AddItem("Orange DDPAT");
	SAWEDOFFSkin.AddItem("Fade");
	SAWEDOFFSkin.AddItem("The Kraken");
	SAWEDOFFSkin.AddItem("Wasteland Princess");
	SAWEDOFFSkin.AddItem("Highwayman");
	Shotgungroup.PlaceLabledControl("Sawed-Off", this, &SAWEDOFFSkin);

	NOVASkin.SetFileId("nova_skin");
	NOVASkin.AddItem("Candy Apple");
	NOVASkin.AddItem("Blaze Orange");
	NOVASkin.AddItem("Modern Hunter");
	NOVASkin.AddItem("Forest Leaves");
	NOVASkin.AddItem("Bloomstick");
	NOVASkin.AddItem("Sand Dune");
	NOVASkin.AddItem("Polar Mesh");
	NOVASkin.AddItem("Walnut");
	NOVASkin.AddItem("Predator");
	NOVASkin.AddItem("Tempest");
	NOVASkin.AddItem("Graphite");
	NOVASkin.AddItem("Ghost Camo");
	NOVASkin.AddItem("Rising Skull");
	NOVASkin.AddItem("Antique");
	NOVASkin.AddItem("Green Apple");
	NOVASkin.AddItem("Caged Steel");
	NOVASkin.AddItem("Koi");
	NOVASkin.AddItem("Moon in Libra");
	NOVASkin.AddItem("Ranger");
	NOVASkin.AddItem("HyperBeast");
	Shotgungroup.PlaceLabledControl("Nova", this, &NOVASkin);
#pragma endregion

#pragma region Rifles
	Riflegroup.SetPosition(16 + 85, 213-68);
	Riflegroup.SetText("Rifles");
	Riflegroup.SetSize(246, 200);
	RegisterControl(&Riflegroup);

	AK47Skin.SetFileId("ak47_skin");
	AK47Skin.AddItem("First Class");
	AK47Skin.AddItem("Red Laminate");
	AK47Skin.AddItem("Case Hardened");
	AK47Skin.AddItem("Black Laminate");
	AK47Skin.AddItem("Fire Serpent");
	AK47Skin.AddItem("Cartel");
	AK47Skin.AddItem("Emerald Pinstripe");
	AK47Skin.AddItem("Blue Laminate");
	AK47Skin.AddItem("Redline");
	AK47Skin.AddItem("Vulcan");
	AK47Skin.AddItem("Jaguar");
	AK47Skin.AddItem("Jet Set");
	AK47Skin.AddItem("Wasteland Rebel");
	AK47Skin.AddItem("Orbit Mk01");
	AK47Skin.AddItem("Hydroponic");
	AK47Skin.AddItem("Aquamarine Revenge");
	AK47Skin.AddItem("Frontside Misty");
	AK47Skin.AddItem("Point Disarray");
	AK47Skin.AddItem("Fuel Injector");
	AK47Skin.AddItem("Neon Revolution");
	AK47Skin.AddItem("Bloodsport");
	Riflegroup.PlaceLabledControl("AK-47", this, &AK47Skin);

	M41SSkin.SetFileId("m4a1s_skin");
	M41SSkin.AddItem("Dark Water");
	M41SSkin.AddItem("Hyper Beast");
	M41SSkin.AddItem("Decimator");
	M41SSkin.AddItem("VariCamo");
	M41SSkin.AddItem("Nitro");
	M41SSkin.AddItem("Bright Water");
	M41SSkin.AddItem("Atomic Alloy");
	M41SSkin.AddItem("Blood Tiger");
	M41SSkin.AddItem("Guardian");
	M41SSkin.AddItem("Master Piece");
	M41SSkin.AddItem("Knight");
	M41SSkin.AddItem("Cyrex");
	M41SSkin.AddItem("Basilisk");
	M41SSkin.AddItem("Icarus Fell");
	M41SSkin.AddItem("Hot Rod");
	M41SSkin.AddItem("Golden Coi");
	M41SSkin.AddItem("Chantico's Fire");
	M41SSkin.AddItem("Mecha Industries");
	M41SSkin.AddItem("Flashback");
	Riflegroup.PlaceLabledControl("M4A1-S", this, &M41SSkin);

	M4A4Skin.SetFileId("m4a4_skin");
	M4A4Skin.AddItem("Bullet Rain");
	M4A4Skin.AddItem("Zirka");
	M4A4Skin.AddItem("Asiimov");
	M4A4Skin.AddItem("Howl");
	M4A4Skin.AddItem("X-Ray");
	M4A4Skin.AddItem("Desert-Strike");
	M4A4Skin.AddItem("Griffin");
	M4A4Skin.AddItem("Dragon King");
	M4A4Skin.AddItem("Poseidon");
	M4A4Skin.AddItem("Daybreak");
	M4A4Skin.AddItem("Evil Daimyo");
	M4A4Skin.AddItem("Royal Paladin");
	M4A4Skin.AddItem("The BattleStar");
	M4A4Skin.AddItem("Desolate Space");
	M4A4Skin.AddItem("Buzz Kill");
	M4A4Skin.AddItem("Hellfire");
	Riflegroup.PlaceLabledControl("M4A4", this, &M4A4Skin);

	AUGSkin.SetFileId("aug_skin");
	AUGSkin.AddItem("Bengal Tiger");
	AUGSkin.AddItem("Hot Rod");
	AUGSkin.AddItem("Chameleon");
	AUGSkin.AddItem("Torque");
	AUGSkin.AddItem("Radiation Hazard");
	AUGSkin.AddItem("Asterion");
	AUGSkin.AddItem("Daedalus");
	AUGSkin.AddItem("Akihabara Accept");
	AUGSkin.AddItem("Ricochet");
	AUGSkin.AddItem("Fleet Flock");
	AUGSkin.AddItem("Syd Mead");
	Riflegroup.PlaceLabledControl("AUG", this, &AUGSkin);

	FAMASSkin.SetFileId("famas_skin");
	FAMASSkin.AddItem("Djinn");
	FAMASSkin.AddItem("Afterimage");
	FAMASSkin.AddItem("Doomkitty");
	FAMASSkin.AddItem("Spitfire");
	FAMASSkin.AddItem("Teardown");
	FAMASSkin.AddItem("Hexane");
	FAMASSkin.AddItem("Pulse");
	FAMASSkin.AddItem("Sergeant");
	FAMASSkin.AddItem("Styx");
	FAMASSkin.AddItem("Neural Net");
	FAMASSkin.AddItem("Survivor");
	FAMASSkin.AddItem("Valence");
	FAMASSkin.AddItem("Roll Cage");
	FAMASSkin.AddItem("Mecha Industries");
	Riflegroup.PlaceLabledControl("FAMAS", this, &FAMASSkin);

	GALILSkin.SetFileId("galil_skin");
	GALILSkin.AddItem("Orange DDPAT");
	GALILSkin.AddItem("Eco");
	GALILSkin.AddItem("Stone Cold");
	GALILSkin.AddItem("Cerberus");
	GALILSkin.AddItem("Aqua Terrace");
	GALILSkin.AddItem("Chatterbox");
	GALILSkin.AddItem("Firefight");
	GALILSkin.AddItem("Rocket Pop");
	GALILSkin.AddItem("Sugar Rush");
	Riflegroup.PlaceLabledControl("GALIL", this, &GALILSkin);

	SG553Skin.SetFileId("sg552_skin");
	SG553Skin.AddItem("Bulldozer");
	SG553Skin.AddItem("Ultraviolet");
	SG553Skin.AddItem("Damascus Steel");
	SG553Skin.AddItem("Fallout Warning");
	SG553Skin.AddItem("Damascus Steel");
	SG553Skin.AddItem("Pulse");
	SG553Skin.AddItem("Army Sheen");
	SG553Skin.AddItem("Traveler");
	SG553Skin.AddItem("Fallout Warning");
	SG553Skin.AddItem("Cyrex");
	SG553Skin.AddItem("Tiger Moth");
	SG553Skin.AddItem("Atlas");
	Riflegroup.PlaceLabledControl("SG553", this, &SG553Skin);
#pragma endregion

#pragma region MPs
	MPGroup.SetPosition(16 + 85, 423-68);
	MPGroup.SetText("SMGs");
	MPGroup.SetSize(246, 173);
	RegisterControl(&MPGroup);

	MAC10Skin.SetFileId("mac10_skin");
	MAC10Skin.AddItem("Fade");
	MAC10Skin.AddItem("Neon Rider");
	MAC10Skin.AddItem("Ultraviolet");
	MAC10Skin.AddItem("Palm");
	MAC10Skin.AddItem("Graven");
	MAC10Skin.AddItem("Tatter");
	MAC10Skin.AddItem("Amber Fade");
	MAC10Skin.AddItem("Heat");
	MAC10Skin.AddItem("Curse");
	MAC10Skin.AddItem("Indigo");
	MAC10Skin.AddItem("Commuter");
	MAC10Skin.AddItem("Nuclear Garden");
	MAC10Skin.AddItem("Malachite");
	MAC10Skin.AddItem("Rangeen");
	MAC10Skin.AddItem("Lapis Gator");
	MPGroup.PlaceLabledControl("MAC-1O", this, &MAC10Skin);

	P90Skin.SetFileId("p90_skin");
	P90Skin.AddItem("Cold Blooded");
	P90Skin.AddItem("Death by Kitty");
	P90Skin.AddItem("Shapewood");
	P90Skin.AddItem("Emerald Dragon");
	P90Skin.AddItem("Asiimov");
	P90Skin.AddItem("Virus");
	MPGroup.PlaceLabledControl("P9O", this, &P90Skin);

	UMP45Skin.SetFileId("ump45_skin");
	UMP45Skin.AddItem("Blaze");
	UMP45Skin.AddItem("Primal Saber");
	UMP45Skin.AddItem("Minotaurs Labyrinth");
	UMP45Skin.AddItem("Grand Prix");
	MPGroup.PlaceLabledControl("UMP-45", this, &UMP45Skin);

	BIZONSkin.SetFileId("bizon_skin");
	BIZONSkin.AddItem("Blue Streak");
	BIZONSkin.AddItem("Antique");
	BIZONSkin.AddItem("Judgement of Anubis");
	BIZONSkin.AddItem("Osiris");
	BIZONSkin.AddItem("Fuel Rod");
	MPGroup.PlaceLabledControl("PP-Bizon", this, &BIZONSkin);

	MP7Skin.SetFileId("mp7_skin");
	MP7Skin.AddItem("Whiteout");
	MP7Skin.AddItem("Nemesis");
	MP7Skin.AddItem("Impire");
	MPGroup.PlaceLabledControl("MP7", this, &MP7Skin);

	MP9Skin.SetFileId("mp9_skin");
	MP9Skin.AddItem("Ruby Poison Dart");
	MP9Skin.AddItem("Hot Rod");
	MPGroup.PlaceLabledControl("MP9", this, &MP9Skin);

#pragma endregion

#pragma region Pistols
	PistolGroup.SetPosition(279 + 85, 233);
	PistolGroup.SetText("Pistols");
	PistolGroup.SetSize(250, 250);
	RegisterControl(&PistolGroup);

	GLOCKSkin.SetFileId("glock_skin");
	GLOCKSkin.AddItem("Wasteland Rebel");
	GLOCKSkin.AddItem("Twillight Galaxy");
	GLOCKSkin.AddItem("Fade");
	GLOCKSkin.AddItem("Dragon Tattoo");
	GLOCKSkin.AddItem("Water Elemental");
	GLOCKSkin.AddItem("Royal Legion");
	GLOCKSkin.AddItem("Grinder");
	GLOCKSkin.AddItem("Reactor");
	GLOCKSkin.AddItem("Weasel");
	PistolGroup.PlaceLabledControl("Glock", this, &GLOCKSkin);

	USPSSkin.SetFileId("usps_skin");
	USPSSkin.AddItem("Dark Water");;
	USPSSkin.AddItem("Overgrowth");
	USPSSkin.AddItem("Caiman");
	USPSSkin.AddItem("Blood Tiger");
	USPSSkin.AddItem("Serum");
	USPSSkin.AddItem("Neo-Noir");
	USPSSkin.AddItem("Stainless");
	USPSSkin.AddItem("Guardian");
	USPSSkin.AddItem("Orion");
	USPSSkin.AddItem("Road Rash");
	USPSSkin.AddItem("Royal Blue");
	USPSSkin.AddItem("Business Class");
	USPSSkin.AddItem("Para Green");
	USPSSkin.AddItem("Torque");
	USPSSkin.AddItem("Kill Confirmed");
	USPSSkin.AddItem("Lead Conduit");
	USPSSkin.AddItem("Cyrex");
	PistolGroup.PlaceLabledControl("USP-S", this, &USPSSkin);

	DEAGLESkin.SetFileId("deagle_skin");
	DEAGLESkin.AddItem("Blaze");
	DEAGLESkin.AddItem("Oxide Blaze");
	DEAGLESkin.AddItem("Kumicho Dragon");
	DEAGLESkin.AddItem("Sunset Storm");
	PistolGroup.PlaceLabledControl("Deagle", this, &DEAGLESkin);

	DUALSSkin.SetFileId("duals_skin");
	DUALSSkin.AddItem("Dualing Dragons");
	DUALSSkin.AddItem("Anodized Navy");
	DUALSSkin.AddItem("Duelist");
	DUALSSkin.AddItem("Hemoglobin");
	DUALSSkin.AddItem("Marina");
	DUALSSkin.AddItem("Urban Shock");
	PistolGroup.PlaceLabledControl("Duals", this, &DUALSSkin);

	RevolverSkin.SetFileId("revolver_skin");
	RevolverSkin.AddItem("Fade");
	RevolverSkin.AddItem("Crimson Web");
	RevolverSkin.AddItem("Amber Fade");
	RevolverSkin.AddItem("Reboot");
	PistolGroup.PlaceLabledControl("Revolver", this, &RevolverSkin);

	FIVESEVENSkin.SetFileId("fiveseven_skin");
	FIVESEVENSkin.AddItem("Case Hardened");
	FIVESEVENSkin.AddItem("Fowl Play");
	FIVESEVENSkin.AddItem("Retrobution");
	FIVESEVENSkin.AddItem("Triumvirate");
	FIVESEVENSkin.AddItem("Neon Kimono");
	FIVESEVENSkin.AddItem("Monkey Business");
	FIVESEVENSkin.AddItem("Copper Galaxy");
	FIVESEVENSkin.AddItem("Hyper Beast");
	PistolGroup.PlaceLabledControl("Five-Seven", this, &FIVESEVENSkin);

	TECNINESkin.SetFileId("tec9_skin");
	TECNINESkin.AddItem("Terrace");
	TECNINESkin.AddItem("Isaac");
	TECNINESkin.AddItem("Red Quartz");
	TECNINESkin.AddItem("Avalanche");
	TECNINESkin.AddItem("Toxic");
	TECNINESkin.AddItem("Fuel Injector");
	TECNINESkin.AddItem("Re-Entry");
	TECNINESkin.AddItem("Bamboo Forest");
	TECNINESkin.AddItem("Nuclear Threat");
	PistolGroup.PlaceLabledControl("Tec-9", this, &TECNINESkin);

	P2000Skin.SetFileId("p2000_skin");
	P2000Skin.AddItem("Handgun");
	P2000Skin.AddItem("Corticera");
	P2000Skin.AddItem("Ocean Foam");
	P2000Skin.AddItem("Fire Elemental");
	P2000Skin.AddItem("Imperial Dragon");
	P2000Skin.AddItem("Ocean Foam");
	P2000Skin.AddItem("Amber Fade");
	PistolGroup.PlaceLabledControl("P2OOO", this, &P2000Skin);

	P250Skin.SetFileId("p250_skin");
	P250Skin.AddItem("Whiteout");
	P250Skin.AddItem("Nuclear Threat");
	P250Skin.AddItem("Splash");
	P250Skin.AddItem("Mehndi");
	P250Skin.AddItem("Asiimov");
	P250Skin.AddItem("Undertow");
	P250Skin.AddItem("Franklin");
	P250Skin.AddItem("Supernova");
	P250Skin.AddItem("Cartel");
	PistolGroup.PlaceLabledControl("P25O", this, &P250Skin);

#pragma endregion
	/*
	#pragma region Skinsettings
	SkinsettingsGroup.SetPosition(279 + 85, 496);
	SkinsettingsGroup.SetText("Customization");
	SkinsettingsGroup.SetSize(250, 108);
	RegisterControl(&SkinsettingsGroup);

	StatTrakEnable.SetFileId("skin_stattrack");
	SkinsettingsGroup.PlaceLabledControl("Stat Trak", this, &StatTrakEnable);

	StatTrackAmount.SetFileId("skin_stamount");
	SkinsettingsGroup.PlaceLabledControl("Value", this, &StatTrackAmount);

	KnifeName.SetFileId("knife_name");
	SkinsettingsGroup.PlaceLabledControl("Knife Name", this, &KnifeName);

	SkinName.SetFileId("skin_name");
	SkinsettingsGroup.PlaceLabledControl("Skin Name", this, &SkinName);

	SkinApply.SetText("Apply Changes");
	SkinApply.SetCallback(KnifeApplyCallbk);
	SkinApply.SetPosition(279 + 85, 600);
	SkinApply.SetSize(250, 106);
	RegisterControl(&SkinApply);

	#pragma endregion
	*/
#pragma endregion other random options

}

void Menu::DoUIFrame()
{

	GUI.Update();
	GUI.Draw();


}

void Menu::SetupMenu()
{
	Window.Setup();

	GUI.RegisterWindow(&Window);
	GUI.BindWindow(VK_INSERT, &Window);
}