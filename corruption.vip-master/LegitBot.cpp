#include "LegitBot.h"
#include "RenderManager.h"
#include "MathFunctions.h"

#define    HITGROUP_GENERIC    0
#define    HITGROUP_HEAD        1
#define    HITGROUP_CHEST        2
#define    HITGROUP_STOMACH    3
#define HITGROUP_LEFTARM    4    
#define HITGROUP_RIGHTARM    5
#define HITGROUP_LEFTLEG    6
#define HITGROUP_RIGHTLEG    7

void CLegitBot::Init()
{
	IsLocked = false;
	TargetID = -1;
	HitBox = -1;
}

void CLegitBot::Draw()
{

}

static int CustomDelay = 0;
static int CustomBreak = 0;

void CLegitBot::LegitAA(CUserCmd *pCmd, bool& bSendPacket)
{
	IClientEntity* pLocal = hackManager.pLocal();

	if ((pCmd->buttons & IN_USE) || pLocal->GetMoveType() == MOVETYPE_LADDER || pCmd->buttons & IN_ATTACK || pCmd->buttons & IN_ATTACK2)
		return;
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
	if (pWeapon)
	{
		CSWeaponInfo* pWeaponInfo = pWeapon->GetCSWpnData();
		// Knives or grenades
		CCSGrenade* csGrenade = (CCSGrenade*)pWeapon;

		if (GameUtils::IsBallisticWeapon(pWeapon))
		{
				if (csGrenade->GetThrowTime() > 0.f)
					return;
		}
	}
	//for the memes
	Vector oldAngle = pCmd->viewangles;
	float oldForward = pCmd->forwardmove;
	float oldSideMove = pCmd->sidemove;
	if (Menu::Window.LegitBotTab.LegitChoice.GetIndex() > 0)
	{
		switch (Menu::Window.LegitBotTab.LegitChoice.GetIndex())
		{
		case 1:
		{//you dont need brackets but for some shit you do like statics //wrapzii C++ class 101
			static bool kFlip = true;
			static int ChokedPackets = -1;
			static bool yFlip = true;
			if (1 > ChokedPackets)
			{
				bSendPacket = true;
				ChokedPackets++;
			}
			else
			{
				bSendPacket = false;
				//pCmd->viewangles.y += yFlip ? 90.f : -90.f;
				pCmd->viewangles.y += 180.f;
				ChokedPackets = -1;
			}
		}
			break;
		case 2:
		{
			static bool kFlip = true;
			static int ChokedPackets = -1;
			static bool yFlip = true;
			if (1 > ChokedPackets)
			{
				bSendPacket = true;
				ChokedPackets++;
			}
			else
			{
				bSendPacket = false;
				//pCmd->viewangles.y += yFlip ? 90.f : -90.f;
				pCmd->viewangles.y += 90.f;//to the right im pretty sure
				ChokedPackets = -1;
			}
		}
			break;
		case 3://put my p ass vel check one
			   //Sideways-switch
		{
			static int ChokedPackets = -1;//we choking 2 cuz 1 is too main stream
			if (1> ChokedPackets) {
				bSendPacket = false;
				static bool dir = false;
				static bool dir2 = false;
				int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); ++i;
				IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
				//if (pCmd->forwardmove > 1 || (IsVisible(pLocal, pEntity, 0) && pEntity->GetTeamNum() != pLocal->GetTeamNum()))// was trying to make a vis check to make it -180 if their visible //didnt seem to work
					//dir2 = true;
				//else {
					dir2 = false;
					if (pCmd->sidemove > 1) dir = true;
					else if (pCmd->sidemove < -1) dir = false;
					pCmd->viewangles.y = (dir) ? (pCmd->viewangles.y - 180) - 270.f : (pCmd->viewangles.y - 180) - 90.f;
				//}
				//if (dir2 = true)
					//pCmd->viewangles.y = pCmd->viewangles.y - 180;
				ChokedPackets++;
			}
			else
			{
				bSendPacket = true;
				ChokedPackets = -1;

			}
		}
			break;
		}

	}
	else
		pCmd->viewangles.y += 0;
}

void CLegitBot::Move(CUserCmd *pCmd, bool& bSendPacket)
{
	if (Menu::Window.RageBotTab.AimbotEnable.GetState())
		return;

	IClientEntity* pLocal = hackManager.pLocal();
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	static int CustomAimTime = 0;
	static int CustomAimStart = 0;
	if (Menu::Window.LegitBotTab.AimbotEnable.GetState())
	{
		if (StartAim > 0)
		{
			if (CustomAimStart < (StartAim * 333))
			{
				CustomAimStart++;
			}
			else
			{
				if (Aimtime > 0)
				{
					if (CustomAimTime < (Aimtime * 333))
					{
						DoAimbot(pCmd);
						CustomAimTime++;
					}
					if (!GetAsyncKeyState(Menu::Window.LegitBotTab.AimbotKeyBind.GetKey()) || Menu::Window.LegitBotTab.AimbotAutoFire.GetState())
					{
						CustomAimTime = 0;
						CustomAimStart = 0;
					}
				}
				else
				{
					DoAimbot(pCmd);
					CustomAimTime = 0;
					CustomAimStart = 0;
				}
			}

			if (!GetAsyncKeyState(Menu::Window.LegitBotTab.AimbotKeyBind.GetKey()) || Menu::Window.LegitBotTab.AimbotAutoFire.GetState())
			{
				CustomAimStart = 0;
				CustomAimTime = 0;
			}
		}
		else
		{
			if (Aimtime > 0)
			{
				if (CustomAimTime < (Aimtime * 333))
				{
					DoAimbot(pCmd);
					CustomAimTime++;
				}
				if (!GetAsyncKeyState(Menu::Window.LegitBotTab.AimbotKeyBind.GetKey()) || Menu::Window.LegitBotTab.AimbotAutoFire.GetState())
				{
					CustomAimTime = 0;
					CustomAimStart = 0;
				}
			}
			else
			{
				DoAimbot(pCmd);
				CustomAimTime = 0;
				CustomAimStart = 0;
			}
		}
	}
	if (pLocal->IsAlive())
	{
		LegitAA(pCmd, bSendPacket);
	}
	// Triggerbot
	if (Menu::Window.LegitBotTab.TriggerEnable.GetState() && (Menu::Window.LegitBotTab.TriggerKeyBind.GetKey() == 0 || GetAsyncKeyState(Menu::Window.LegitBotTab.TriggerKeyBind.GetKey())))
		DoTrigger(pCmd);

	SyncWeaponSettings();
}

void CLegitBot::SyncWeaponSettings()
{
	IClientEntity* pLocal = hackManager.pLocal();
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	if (!pWeapon)
		return;

	if (GameUtils::IsPistol(pWeapon))
	{
		Speed = Menu::Window.LegitBotTab.WeaponPistSpeed.GetValue() / 100;
		FoV = Menu::Window.LegitBotTab.WeaponPistFoV.GetValue() * 2;
		RecoilControl = Menu::Window.LegitBotTab.WeaponPistRecoil.GetValue() * 10;
		Inacc = Menu::Window.LegitBotTab.WeaponPistInacc.GetValue();

		switch (Menu::Window.LegitBotTab.WeaponPistHitbox.GetIndex())
		{
		case 0:
			HitBox = ((int)CSGOHitboxID::Head);
			Multihitbox = false;
			break;
		case 1:
			HitBox = ((int)CSGOHitboxID::Neck);
			Multihitbox = false;
			break;
		case 2:
			HitBox = ((int)CSGOHitboxID::Chest);
			Multihitbox = false;
			break;
		case 3:
			HitBox = ((int)CSGOHitboxID::Stomach);
			Multihitbox = false;
			break;
		case 4:
			Multihitbox = true;
			break;
		}

		Aimtime = Menu::Window.LegitBotTab.WeaponPistAimtime.GetValue() / 10;
		StartAim = Menu::Window.LegitBotTab.WeaoponPistStartAimtime.GetValue() / 10;
	}
	else if (GameUtils::IsSniper(pWeapon))
	{
		Speed = Menu::Window.LegitBotTab.WeaponSnipSpeed.GetValue() / 100;
		FoV = Menu::Window.LegitBotTab.WeaponSnipFoV.GetValue() * 2;
		RecoilControl = Menu::Window.LegitBotTab.WeaponSnipRecoil.GetValue() * 10;
		Inacc = Menu::Window.LegitBotTab.WeaponSnipInacc.GetValue();

		switch (Menu::Window.LegitBotTab.WeaponSnipHitbox.GetIndex())
		{
		case 0:
			HitBox = ((int)CSGOHitboxID::Head);
			Multihitbox = false;
			break;
		case 1:
			HitBox = ((int)CSGOHitboxID::Neck);
			Multihitbox = false;
			break;
		case 2:
			HitBox = ((int)CSGOHitboxID::Chest);
			Multihitbox = false;
			break;
		case 3:
			HitBox = ((int)CSGOHitboxID::Stomach);
			Multihitbox = false;
			break;
		case 4:
			Multihitbox = true;
			break;
		}

		Aimtime = Menu::Window.LegitBotTab.WeaponSnipAimtime.GetValue() / 10;
		StartAim = Menu::Window.LegitBotTab.WeaoponSnipStartAimtime.GetValue() / 10;
	}
	else if (GameUtils::IsMachinegun(pWeapon))
	{
		Speed = Menu::Window.LegitBotTab.WeaponOtherSpeed.GetValue() / 100;
		FoV = Menu::Window.LegitBotTab.WeaponOtherFoV.GetValue() * 2;
		RecoilControl = Menu::Window.LegitBotTab.WeaponOtherRecoil.GetValue();

		switch (Menu::Window.LegitBotTab.WeaponOtherHitbox.GetIndex())
		Speed = Menu::Window.LegitBotTab.WeaponMGSpeed.GetValue() / 100;
		FoV = Menu::Window.LegitBotTab.WeaponMGFoV.GetValue() * 2;
		RecoilControl = Menu::Window.LegitBotTab.WeaponMGRecoil.GetValue() * 10;
		switch (Menu::Window.LegitBotTab.WeaponMGHitbox.GetIndex())
		{
		case 0:
			HitBox = ((int)CSGOHitboxID::Head);
			Multihitbox = false;
			break;
		case 1:
			HitBox = ((int)CSGOHitboxID::Neck);
			Multihitbox = false;
			break;
		case 2:
			HitBox = ((int)CSGOHitboxID::Chest);
			Multihitbox = false;
			break;
		case 3:
			HitBox = ((int)CSGOHitboxID::Stomach);
			Multihitbox = false;
			break;
		case 4:
			Multihitbox = true;
			break;
		}

		Aimtime = Menu::Window.LegitBotTab.WeaponOtherAimtime.GetValue() / 10;
		StartAim = Menu::Window.LegitBotTab.WeaoponOtherStartAimtime.GetValue() / 10;
	}
	else if (GameUtils::IsShotgun(pWeapon))
	{
		Speed = Menu::Window.LegitBotTab.WeaponOtherSpeed.GetValue() / 100;
		FoV = Menu::Window.LegitBotTab.WeaponOtherFoV.GetValue() * 2;
		RecoilControl = Menu::Window.LegitBotTab.WeaponOtherRecoil.GetValue() * 10;

		switch (Menu::Window.LegitBotTab.WeaponOtherHitbox.GetIndex())
		{
		case 0:
			HitBox = ((int)CSGOHitboxID::Head);
			Multihitbox = false;
			break;
		case 1:
			HitBox = ((int)CSGOHitboxID::Neck);
			Multihitbox = false;
			break;
		case 2:
			HitBox = ((int)CSGOHitboxID::Chest);
			Multihitbox = false;
			break;
		case 3:
			HitBox = ((int)CSGOHitboxID::Stomach);
			Multihitbox = false;
			break;
		case 4:
			Multihitbox = true;
			break;
		}

		Aimtime = Menu::Window.LegitBotTab.WeaponOtherAimtime.GetValue() / 10;
		StartAim = Menu::Window.LegitBotTab.WeaoponOtherStartAimtime.GetValue() / 10;
	}
	else if (GameUtils::IsMP(pWeapon))
	{
		Speed = Menu::Window.LegitBotTab.WeaponOtherSpeed.GetValue() / 100;
		FoV = Menu::Window.LegitBotTab.WeaponOtherFoV.GetValue() * 2;
		RecoilControl = Menu::Window.LegitBotTab.WeaponOtherRecoil.GetValue();
		Inacc = Menu::Window.LegitBotTab.WeaponOtherInacc.GetValue();

		switch (Menu::Window.LegitBotTab.WeaponOtherHitbox.GetIndex())
		{
		case 0:
			HitBox = ((int)CSGOHitboxID::Head);
			Multihitbox = false;
			break;
		case 1:
			HitBox = ((int)CSGOHitboxID::Neck);
			Multihitbox = false;
			break;
		case 2:
			HitBox = ((int)CSGOHitboxID::Chest);
			Multihitbox = false;
			break;
		case 3:
			HitBox = ((int)CSGOHitboxID::Stomach);
			Multihitbox = false;
			break;
		case 4:
			Multihitbox = true;
			break;
		}

		Aimtime = Menu::Window.LegitBotTab.WeaponOtherAimtime.GetValue() / 10;
		StartAim = Menu::Window.LegitBotTab.WeaoponOtherStartAimtime.GetValue() / 10;
	}
	else
	{
		Speed = Menu::Window.LegitBotTab.WeaponMainSpeed.GetValue() / 100;
		FoV = Menu::Window.LegitBotTab.WeaponMainFoV.GetValue() * 2;
		RecoilControl = Menu::Window.LegitBotTab.WeaponMainRecoil.GetValue();
		Inacc = Menu::Window.LegitBotTab.WeaponMainInacc.GetValue();

		switch (Menu::Window.LegitBotTab.WeaponMainHitbox.GetIndex())
		{
		case 0:
			HitBox = ((int)CSGOHitboxID::Head);
			Multihitbox = false;
			break;
		case 1:
			HitBox = ((int)CSGOHitboxID::Neck);
			Multihitbox = false;
			break;
		case 2:
			HitBox = ((int)CSGOHitboxID::Chest);
			Multihitbox = false;
			break;
		case 3:
			HitBox = ((int)CSGOHitboxID::Stomach);
			Multihitbox = false;
			break;
		case 4:
			Multihitbox = true;
			break;
		}
		Aimtime = Menu::Window.LegitBotTab.WeaponMainAimtime.GetValue() / 10;
		StartAim = Menu::Window.LegitBotTab.WeaoponMainStartAimtime.GetValue() / 10;
	}
}

void CLegitBot::DoAimbot(CUserCmd *pCmd)
{
	IClientEntity* pTarget = nullptr;
	IClientEntity* pLocal = hackManager.pLocal();
	bool FindNewTarget = true;

	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	if (pWeapon)
	{
		if (pWeapon->GetAmmoInClip() == 0 || GameUtils::IsBallisticWeapon(pWeapon))
		{
			return;
		}
		SyncWeaponSettings();
	}
	else
		return;
	if (IsLocked && TargetID >= 0 && HitBox >= 0)
	{
		pTarget = Interfaces::EntList->GetClientEntity(TargetID);
		if (pTarget  && TargetMeetsRequirements(pTarget))
		{
			SyncWeaponSettings();
			if (HitBox >= 0)
			{
				Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
				Vector View; Interfaces::Engine->GetViewAngles(View);
				if (pLocal->GetVelocity().Length() > 45.f);
				View += pLocal->localPlayerExclusive()->GetAimPunchAngle() * RecoilControl;
				float nFoV = FovToPlayer(ViewOffset, View, pTarget, HitBox);
				if (nFoV < FoV)
					FindNewTarget = false;
			}
		}
	}

	if (FindNewTarget)
	{
		TargetID = 0;
		pTarget = nullptr;
		HitBox = -1;

		TargetID = GetTargetCrosshair();

		if (TargetID >= 0)
		{
			pTarget = Interfaces::EntList->GetClientEntity(TargetID);
		}
		else
		{
			pTarget = nullptr;
			HitBox = -1;
		}
	}

	SyncWeaponSettings();

	if (TargetID >= 0 && pTarget)
	{
		SyncWeaponSettings();

		if (Menu::Window.LegitBotTab.AimbotKeyBind.GetKey() > 0)
		{
			int Key = Menu::Window.LegitBotTab.AimbotKeyBind.GetKey();
			if (Key >= 0 && !GetAsyncKeyState(Key))
			{
				TargetID = -1;
				pTarget = nullptr;
				HitBox = -1;
				return;
			}
		}

		//AimPoint;

		if (Multihitbox)
		{
			AimPoint = GetHitboxPosition(pTarget, besthitbox);
		}
		else
		{
			AimPoint = GetHitboxPosition(pTarget, HitBox);
		}
		pTarget->GetPredicted(AimPoint); /*Velocity Prediction*/

		if (AimAtPoint(pLocal, AimPoint, pCmd))
		{
			if (Menu::Window.LegitBotTab.AimbotAutoFire.GetState() && !(pCmd->buttons & IN_ATTACK))
			{
				pCmd->buttons |= IN_ATTACK;
			}
		}
	}
}


bool CLegitBot::TargetMeetsTriggerRequirements(IClientEntity* pEntity)
{

	if (pEntity && pEntity->IsDormant() == false && pEntity->IsAlive() && pEntity->GetIndex() != hackManager.pLocal()->GetIndex())
	{

		ClientClass *pClientClass = pEntity->GetClientClass();
		player_info_t pinfo;
		if (pClientClass->m_ClassID == (int)CSGOClassID::CCSPlayer && Interfaces::Engine->GetPlayerInfo(pEntity->GetIndex(), &pinfo))
		{

			if (pEntity->GetTeamNum() != hackManager.pLocal()->GetTeamNum() || Menu::Window.LegitBotTab.AimbotFriendlyFire.GetState())
			{

				if (!pEntity->HasGunGameImmunity())
				{
					return true;
				}
			}
		}
	}


	return false;
}

void CLegitBot::DoTrigger(CUserCmd *pCmd)
{
	IClientEntity* pLocal = hackManager.pLocal();

	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	if (pWeapon)
	{
		if (pWeapon->GetAmmoInClip() == 0 || GameUtils::IsBallisticWeapon(pWeapon))
		{
			return;
		}
	}
	else
		return;

	Vector ViewAngles;
	Interfaces::Engine->GetViewAngles(ViewAngles);
	ViewAngles += pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f;

	Vector fowardVec;
	AngleVectors(ViewAngles, &fowardVec);
	fowardVec.NormalizeInPlace();
	fowardVec *= 10000;

	Vector start = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector end = start + fowardVec, endScreen;

	trace_t Trace;

	Ray_t ray;

	CTraceFilter traceFilter;
	traceFilter.pSkip = pLocal;

	ray.Init(start, end);
	Interfaces::Trace->TraceRay(ray, MASK_SHOT, &traceFilter, &Trace);

	if (Trace.m_pEnt && 0 < Trace.hitgroup <= 7)
	{
		if (TargetMeetsTriggerRequirements(Trace.m_pEnt))
		{
			pCmd->buttons |= IN_ATTACK;
		}
	}

	static bool WasFiring = false;
	CSWeaponInfo* WeaponInfo = pWeapon->GetCSWpnData();
	if (!WeaponInfo->bFullAuto /*&& Menu::Window.LegitBotTab.AimbotAutoPistol.GetState()*/)
	{
		if (pCmd->buttons & IN_ATTACK)
		{
			if (WasFiring)
			{
				pCmd->buttons &= ~IN_ATTACK;
			}
		}

		WasFiring = pCmd->buttons & IN_ATTACK ? true : false;
	}
}

bool CLegitBot::TargetMeetsRequirements(IClientEntity* pEntity)
{
	if (pEntity && pEntity->IsDormant() == false && pEntity->IsAlive() && pEntity->GetIndex() != hackManager.pLocal()->GetIndex())
	{

		ClientClass *pClientClass = pEntity->GetClientClass();
		player_info_t pinfo;
		if (pClientClass->m_ClassID == (int)CSGOClassID::CCSPlayer && Interfaces::Engine->GetPlayerInfo(pEntity->GetIndex(), &pinfo))
		{

			if (pEntity->GetTeamNum() != hackManager.pLocal()->GetTeamNum() || Menu::Window.LegitBotTab.AimbotFriendlyFire.GetState())
			{
				if (Menu::Window.LegitBotTab.AimbotSmokeCheck.GetState()) {
					typedef bool(__cdecl* GoesThroughSmoke)(Vector, Vector);
					static uint32_t GoesThroughSmokeOffset = (uint32_t)Utilities::Memory::FindPatternV2("client.dll", "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0");
					static GoesThroughSmoke GoesThroughSmokeFunction = (GoesThroughSmoke)GoesThroughSmokeOffset;
					IClientRenderable* pUnknoown = pEntity->GetClientRenderable();
					if (pUnknoown == nullptr)
						return false;
						if (GoesThroughSmokeFunction(hackManager.pLocal()->GetEyePosition(), pEntity->GetBonePos(8)))
						 return false;
					
				}
				
					if (Multihitbox) {
						if (!pEntity->HasGunGameImmunity())
							 {
							if (GameUtils::IsVisible(hackManager.pLocal(), pEntity, 0) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 8) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 4 || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 1 || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 2 || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 3 || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 5 || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 7 || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 9 || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 10 || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 6))))))))))
								 return true;
							else
								 return false;
							}
						
					}
					else
						 {
						if (!pEntity->HasGunGameImmunity() && GameUtils::IsVisible(hackManager.pLocal(), pEntity, HitBox))
							 {
							return true;
							}
					return true;
				}
			}
		}
	}

	return false;
}

float Get3dDistance(Vector me, Vector ent)
{
	return sqrt(pow(double(ent.x - me.x), 2.0) + pow(double(ent.y - me.y), 2.0) + pow(double(ent.z - me.z), 2.0));
}

float CLegitBot::FovToPlayer(Vector ViewOffSet, Vector View, IClientEntity* pEntity, int aHitBox)
{
	CONST FLOAT MaxDegrees = 180.0f;

	Vector Angles = View;

	Vector Origin = ViewOffSet;

	Vector Delta(0, 0, 0);

	Vector Forward(0, 0, 0);

	AngleVectors(Angles, &Forward);
	Vector AimPos = GetHitboxPosition(pEntity, aHitBox);

	VectorSubtract(AimPos, Origin, Delta);

	Normalize(Delta, Delta);

	float Distance = Get3dDistance(Origin, AimPos);

	float pitch = sin(Forward.x - Delta.x) * Distance;
	float yaw = sin(Forward.y - Delta.y) * Distance;
	float zaw = sin(Forward.z - Delta.z) * Distance;

	float mag = sqrt((pitch*pitch) + (yaw*yaw) + (zaw*zaw));
	return mag;
}

int CLegitBot::GetTargetCrosshair()
{
	SyncWeaponSettings();
	int target = -1;


	IClientEntity* pLocal = hackManager.pLocal();
	Vector ViewOffset = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector View; Interfaces::Engine->GetViewAngles(View);
	View += pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f;

	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);

		if (TargetMeetsRequirements(pEntity))
		{
			if (Multihitbox)
			{

				float fov1 = FovToPlayer(ViewOffset, View, pEntity, 0);
				float fov2 = FovToPlayer(ViewOffset, View, pEntity, 4);
				float fov3 = FovToPlayer(ViewOffset, View, pEntity, 6);

				if (fov1 < FoV || fov2 < FoV && fov1 < FoV || fov3 < FoV)
				{
					FoV = fov1;
					target = i;
					besthitbox = 0;
				}

				if (fov2 < FoV || fov1 < FoV && fov2 < FoV || fov3 < FoV)
				{
					FoV = fov2;
					target = i;
					besthitbox = 4;
				}

				if (fov3 < FoV || fov1 < FoV && fov3 < FoV || fov2 < FoV)
				{
					FoV = fov3;
					target = i;
					besthitbox = 6;
				}

			}
			else
			{
				int NewHitBox = HitBox;//use the float for fov to determine crosshair size
				if (NewHitBox >= 0)
				{
					float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
					if (fov < FoV)
					{
						FoV = fov;
						target = i;
					}
				}
			}
		}
	}

	return target;
}

bool CLegitBot::AimAtPoint(IClientEntity* pLocal, Vector point, CUserCmd *pCmd)
{
	if (point.Length() == 0) return false;

	Vector angles;
	Vector src = pLocal->GetOrigin() + pLocal->GetViewOffset();

	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	static Vector Inaccuracy;
	static Vector Inaccuracy1;

	if (timeSoFar > 0.2)
	{
		Inaccuracy.Init(-50 + rand() % 100, -50 + rand() % 100, -50 + rand() % 100);
		Inaccuracy.NormalizeInPlace();
		Inaccuracy *= Inacc;
		Inaccuracy1.Init(-50 + rand() % 100, -50 + rand() % 100, -50 + rand() % 100);
		Inaccuracy1.NormalizeInPlace();
		Inaccuracy1 *= Inacc;
		start_t = clock();
	}
	angles += Inaccuracy1*10;
	point += Inaccuracy;

	CalcAngle(src, point, angles);
	GameUtils::NormaliseViewAngle(angles);

	if (angles[0] != angles[0] || angles[1] != angles[1])
	{
		return false;
	}

	if (/*RecoilControl*/ 1.5 > 0)
	{
		Vector AimPunch = pLocal->localPlayerExclusive()->GetAimPunchAngle();
		if (AimPunch.Length2D() > 0 && AimPunch.Length2D() < 150)
		{
			angles -= AimPunch * 1.5/*RecoilControl*/;//temp 1.5 so half recoil control just because
			GameUtils::NormaliseViewAngle(angles);
		}
	}

	IsLocked = true;

	Vector shit = angles - pCmd->viewangles;
	bool v = false;
	GameUtils::NormaliseViewAngle(shit);
	if (shit.Length() > Speed)
	{
		Normalize(shit, shit);
		shit *= Speed;
	}
	else
	{
		v = true;
	}

	pCmd->viewangles += shit;//hopefully gives us a bit of inacc while aiming towards them
	Interfaces::Engine->SetViewAngles(pCmd->viewangles);

	return v;
}