#include "Visuals.h"
#include "Interfaces.h"
#include "RenderManager.h"
#include "Autowall.h"
void CVisuals::Init()
{
}

void CVisuals::Move(CUserCmd *pCmd, bool &bSendPacket) {}

void CVisuals::Draw()
{
	IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (Menu::Window.VisualsTab.OtherNoScope.GetState() && pLocal->IsAlive() && pLocal->IsScoped())
		NoScopeCrosshair();

	switch (Menu::Window.VisualsTab.OtherCrosshair.GetIndex())
	{
	case 0:
		Interfaces::Engine->ClientCmd_Unrestricted("crosshair 1");
		break;
	case 1:
		Interfaces::Engine->ClientCmd_Unrestricted("crosshair 1");
		DrawRecoilCrosshair();
		break;
	case 2:
		Interfaces::Engine->ClientCmd_Unrestricted("crosshair 0");
		SpreadCrosshair();
		break;
	case 3://this will be damage recoil crosshair
		Interfaces::Engine->ClientCmd_Unrestricted("crosshair 1");
		DrawRecoilCrosshair2();
		break;
	}

	if (Menu::Window.VisualsTab.SniperCrosshair.GetState())
		DefaultCrosshair();
}

void CVisuals::NoScopeCrosshair()
{
	RECT View = Render::GetViewport();
	int MidX = View.right / 2;
	int MidY = View.bottom / 2;

	IClientEntity* pLocal = hackManager.pLocal();
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	if (GameUtils::IsSniper(pWeapon))
	{
		Render::Line(MidX - 1000, MidY, MidX + 1000, MidY, Color(0, 0, 0, 255));
		Render::Line(MidX, MidY - 1000, MidX, MidY + 1000, Color(0, 0, 0, 255));
	}
}

bool CanWallbang(float &dmg)
{
	IClientEntity *pLocal = hackManager.pLocal();
	if (!pLocal)
		return false;
	FireBulletData data = FireBulletData(pLocal->GetEyePosition());
	data.filter = CTraceFilter();
	data.filter.pSkip = pLocal;

	Vector EyeAng;
	Interfaces::Engine->GetViewAngles(EyeAng);

	Vector dst, forward;

	AngleVectors(EyeAng, &forward);
	dst = data.src + (forward * 8196.f);

	Vector angles;
	CalcAngle(data.src, dst, angles);
	AngleVectors(angles, &data.direction);
	VectorNormalize(data.direction);

	CBaseCombatWeapon* weapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	if (!weapon)
		return false;

	data.penetrate_count = 1;
	data.trace_length = 0.0f;

	CSWeaponInfo *weaponData = weapon->GetCSWpnData();

	if (!weaponData)
		return false;

	data.current_damage = (float)weaponData->iDamage;
	data.trace_length_remaining = weaponData->flRange - data.trace_length;

	Vector end = data.src + data.direction * data.trace_length_remaining;

	UTIL_TraceLine(data.src, end, MASK_SHOT | CONTENTS_GRATE, pLocal, 0, &data.enter_trace);

	if (data.enter_trace.fraction == 1.0f)
		return false;

	if (HandleBulletPenetration(weaponData, data))
	{
		dmg = data.current_damage;
		return true;
	}

	return false;
}
void CVisuals::DrawCrosshair()
{
	RECT View = Render::GetViewport();
	int MidX = View.right / 2;
	int MidY = View.bottom / 2;

	Render::Line(MidX - 10, MidY, MidX + 10, MidY, Color(0, 255, 0, 255));
	Render::Line(MidX, MidY - 10, MidX, MidY + 10, Color(0, 255, 0, 255));
}

void CVisuals::SpreadCrosshair()
{
	IClientEntity *pLocal = hackManager.pLocal();
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	Vector start = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector ViewAngles;
	Interfaces::Engine->GetViewAngles(ViewAngles);
	ViewAngles += pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f;
	Vector fowardVec;
	AngleVectors(ViewAngles, &fowardVec);
	fowardVec *= 10000;
	Vector end = start + fowardVec, endScreen;
	int xs;
	int ys;
	Interfaces::Engine->GetScreenSize(xs, ys);

	auto cone = pWeapon->GetSpread() + pWeapon->GetInaccuracy();
	if (cone > 0.0f)
	{
		if (cone < 0.01f) cone = 0.01f;
		float size = (cone * ys) * 0.7f;
		float size2 = (cone * ys) * 0.6f;
		if (Render::WorldToScreen(end, endScreen) && pLocal->IsAlive())
		{
			Render::DrawCircle(endScreen.x, endScreen.y, (int)size, 48, Color(255, 25, 25, 255));
		}
	
	}
}


void CVisuals::DrawRecoilCrosshair()
{
	IClientEntity *pLocal = hackManager.pLocal();

	Vector ViewAngles;
	Interfaces::Engine->GetViewAngles(ViewAngles);
	ViewAngles += pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f;

	Vector fowardVec;
	AngleVectors(ViewAngles, &fowardVec);
	fowardVec *= 10000;

	Vector start = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector end = start + fowardVec, endScreen;


	if (Render::WorldToScreen(end, endScreen) && pLocal->IsAlive())
	{
		float damage = 0.f;
		if (CanWallbang(damage))
		{
			Render::Line(endScreen.x - 4, endScreen.y - 4, endScreen.x + 4, endScreen.y + 4, Color(150, 255, 150, 255));
			Render::Line(endScreen.x + 4, endScreen.y - 4, endScreen.x - 4, endScreen.y + 4, Color(150, 255, 150, 255));
			Render::Line(endScreen.x - 2, endScreen.y - 2, endScreen.x + 2, endScreen.y + 2, Color(150, 255, 150, 255));
			Render::Line(endScreen.x + 2, endScreen.y - 2, endScreen.x - 2, endScreen.y + 2, Color(150, 255, 150, 255));
		}
		else
		{
			Render::Line(endScreen.x - 4, endScreen.y - 4, endScreen.x + 4, endScreen.y + 4, Color(255, 255, 255, 255));
			Render::Line(endScreen.x + 4, endScreen.y - 4, endScreen.x - 4, endScreen.y + 4, Color(255, 255, 255, 255));
			Render::Line(endScreen.x - 2, endScreen.y - 2, endScreen.x + 2, endScreen.y + 2, Color(255, 255, 255, 255));
			Render::Line(endScreen.x + 2, endScreen.y - 2, endScreen.x - 2, endScreen.y + 2, Color(255, 255, 255, 255));
		}
	}
}

void CVisuals::DrawRecoilCrosshair2()
{
	IClientEntity *pLocal = hackManager.pLocal();

	Vector ViewAngles;
	Interfaces::Engine->GetViewAngles(ViewAngles);
	ViewAngles += pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f;

	Vector fowardVec;
	AngleVectors(ViewAngles, &fowardVec);
	fowardVec *= 10000;

	Vector start = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector end = start + fowardVec, endScreen;


	if (Render::WorldToScreen(end, endScreen) && pLocal->IsAlive())
	{
		float damage = 0.f;
		if (CanWallbang(damage))
		{
			Render::Textf(endScreen.x-4, endScreen.y -25, Color(255,255,255,255), Render::Fonts::ESP, "%.1f", damage);
			Render::Line(endScreen.x - 4, endScreen.y - 4, endScreen.x + 4, endScreen.y + 4, Color(150, 255, 150, 255));
			Render::Line(endScreen.x + 4, endScreen.y - 4, endScreen.x - 4, endScreen.y + 4, Color(150, 255, 150, 255));
			Render::Line(endScreen.x - 2, endScreen.y - 2, endScreen.x + 2, endScreen.y + 2, Color(150, 255, 150, 255));
			Render::Line(endScreen.x + 2, endScreen.y - 2, endScreen.x - 2, endScreen.y + 2, Color(150, 255, 150, 255));
		}
		else
		{
			Render::Line(endScreen.x - 4, endScreen.y - 4, endScreen.x + 4, endScreen.y + 4, Color(255, 255, 255, 255));
			Render::Line(endScreen.x + 4, endScreen.y - 4, endScreen.x - 4, endScreen.y + 4, Color(255, 255, 255, 255));
			Render::Line(endScreen.x - 2, endScreen.y - 2, endScreen.x + 2, endScreen.y + 2, Color(255, 255, 255, 255));
			Render::Line(endScreen.x + 2, endScreen.y - 2, endScreen.x - 2, endScreen.y + 2, Color(255, 255, 255, 255));
		}
	}
}


void CVisuals::DefaultCrosshair()
{
	IClientEntity *pLocal = hackManager.pLocal();

	if (!pLocal->IsScoped() && pLocal->IsAlive())
	{
		ConVar* cross = Interfaces::CVar->FindVar("weapon_debug_spread_show");
		SpoofedConvar* cross_spoofed = new SpoofedConvar(cross);
		cross_spoofed->SetInt(3);
	}
}
