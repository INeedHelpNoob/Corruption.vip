
#include "ESP.h"
#include "Interfaces.h"
#include "RenderManager.h"
#include "Glowmanager.h"
#include "RageBot.h"
#include "SDK.h"
#include "lagcomp.h"
#include "Autowall.h"
DWORD GlowManager = *(DWORD*)(Utilities::Memory::FindPatternV2("client.dll", "0F 11 05 ?? ?? ?? ?? 83 C8 01 C7 05 ?? ?? ?? ?? 00 00 00 00") + 3);
IClientEntity *BombCarrier;
void CEsp::Init()
{
	BombCarrier = nullptr;
}
void CEsp::DrawGranades()
{
	auto granade = Interfaces::CVar->FindVar("sv_grenade_trajectory");
	auto granadespoof = new SpoofedConvar(granade);
	granadespoof->SetInt(1);
}
// Yeah dude we're defo gunna do some sick moves for the esp yeah
void CEsp::Move(CUserCmd *pCmd, bool &bSendPacket)
{

}

// Main ESP Drawing loop
void CEsp::Draw()
{
	IClientEntity *pLocal = hackManager.pLocal();
	if (Menu::Window.VisualsTab.OtherSpectators.GetState())//no reason to put it in 2 diff loops
	{
		SpecList();
	}
	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		player_info_t pinfo;

		if (pEntity &&  pEntity != pLocal && !pEntity->IsDormant())
		{
			if (Menu::Window.VisualsTab.OtherRadar.GetState())
			{
				ptrdiff_t m_bSpotted = 0x939;
				*(char*)((DWORD)(pEntity)+m_bSpotted) = 1;
			}

			if (Menu::Window.VisualsTab.Active.GetState() && Interfaces::Engine->GetPlayerInfo(i, &pinfo) && pEntity->IsAlive())
			{
				DrawPlayer(pEntity, pinfo);
			}

			ClientClass* cClass = (ClientClass*)pEntity->GetClientClass();

			if (Menu::Window.VisualsTab.OtherNades.GetState() && strstr(cClass->m_pNetworkName, "Projectile"))
			{
				DrawThrowable(pEntity);
			}

			if (Menu::Window.VisualsTab.Granades.GetState())
				DrawGranades();

			if (Menu::Window.VisualsTab.FiltersWeapons.GetState() && cClass->m_ClassID != (int)CSGOClassID::CBaseWeaponWorldModel && ((strstr(cClass->m_pNetworkName, "Weapon") || cClass->m_ClassID == (int)CSGOClassID::CDeagle || cClass->m_ClassID == (int)CSGOClassID::CAK47)))
			{
				DrawDrop(pEntity, cClass);
			}

			if (Menu::Window.VisualsTab.FiltersC4.GetState())
			{
				if (cClass->m_ClassID == (int)CSGOClassID::CPlantedC4)
					DrawBombPlanted2(pEntity, cClass);

				if (cClass->m_ClassID == (int)CSGOClassID::CPlantedC4)
					DrawBombPlanted(pEntity, cClass);

				if (cClass->m_ClassID == (int)CSGOClassID::CC4)
					DrawBomb(pEntity, cClass);

				if (Menu::Window.VisualsTab.OptionsPlant.GetState())
					DefuseWarning(pEntity);
			}

			if (Menu::Window.VisualsTab.FiltersChickens.GetState())
			{
				if (cClass->m_ClassID == (int)CSGOClassID::CChicken)
					DrawChicken(pEntity, cClass);
			}

		}
	}
}



void CEsp::DLight(IClientEntity *local, IClientEntity* entity)
{
	if (local->IsAlive())
	{
		player_info_t pinfo;
		if (local && entity && entity != local)
		{
			if (Interfaces::Engine->GetPlayerInfo(entity->GetIndex(), &pinfo) && entity->IsAlive() && !entity->IsDormant())
			{
				if (local->GetTeamNum() != entity->GetTeamNum())
				{
					dlight_t* pElight = Interfaces::g_Dlight->CL_AllocElight(entity->GetIndex());
					pElight->origin = entity->GetOrigin() + Vector(0.0f, 0.0f, 35.0f);
					pElight->radius = 300.0f;
					pElight->color.b = 255;
					pElight->color.g = 25;
					pElight->color.r = 25;
					pElight->die = Interfaces::Globals->curtime + 0.05f;
					pElight->decay = pElight->radius / 5.0f;
					pElight->key = entity->GetIndex();

					dlight_t* pDlight = Interfaces::g_Dlight->CL_AllocDlight(entity->GetIndex());
					pDlight->origin = entity->GetOrigin();
					pDlight->radius = 300.0f;
					pDlight->color.b = 255;
					pDlight->color.g = 25;
					pDlight->color.r = 25;
					pDlight->die = Interfaces::Globals->curtime + 0.05f;
					pDlight->decay = pDlight->radius; // / 5.0f;
					pDlight->key = entity->GetIndex();
				}
			}
		}
	}
}

/*
void DrawAutowallCrosshair()
{
	float damage = 0.f;
	if (CanWallbang(damage))
	{
		Draw->Rect(MidX - 2, MidY - 2, 5, 5, Color(0, 255, 0));
		RECT text = Draw->GetTextSize(NormalFont, "-%.1f HP", damage);
		Draw->Text(MidX - (text.right / 2), MidY - 18, Color(255, 255, 255, 210), NormalFont, "-%.1f HP", damage);
	}
	else
	{
		Draw->Rect(MidX - 2, MidY - 2, 5, 5, Color(255, 0, 0));
	}
}*/





void CEsp::SpecList()
{

	IClientEntity *pLocal = hackManager.pLocal();

	RECT scrn = Render::GetViewport();
	static int AC = 0;//this needs to be defined wayyyy earlier...

		player_info_t pinfo;
		Render::Text(scrn.left, (scrn.bottom / 2 - 16), Color(255, 255, 255, 255), Render::Fonts::ESP, "SPECTATORS:");
		if (pLocal->IsAlive())
		{
			for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
			{
				IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
				if (pEntity == nullptr)
					return;
				if (Interfaces::Engine->GetPlayerInfo(i, &pinfo) && !pEntity->IsAlive() && !pEntity->IsDormant())
				{

					HANDLE obs = pEntity->GetObserverTargetHandle();

					if (obs)
					{
						IClientEntity *pTarget = Interfaces::EntList->GetClientEntityFromHandle(obs);
						player_info_t pinfo2;
						if (pTarget)
						{
							if (Interfaces::Engine->GetPlayerInfo(pTarget->GetIndex(), &pinfo2))
							{
								if (strlen(pinfo.name) > 16 && strlen(pinfo2.name) > 16)
								{
									pinfo.name[16] = 0;
									strcat(pinfo.name, "...");
									puts(pinfo.name);
									pinfo2.name[16] = 0;
									strcat(pinfo2.name, "...");
									puts(pinfo2.name);
									char buf[256]; sprintf_s(buf, "%s", pinfo.name, pinfo2.name);
									RECT TextSize = Render::GetTextSize(Render::Fonts::ESP, buf);
									//Render::Clear(scrn.left, (scrn.bottom / 3) + (16 * AC), 260, 16, Color(0, 0, 0, 140));
									//Render::Text(scrn.left, (scrn.bottom / 2 - 16), pTarget->GetIndex() == pLocal->GetIndex() ? Color(240, 70, 80, 255) : Color(255, 255, 255, 0), Render::Fonts::ESP, "Spectating me list:");
									Render::Text(scrn.left, (scrn.bottom / 2) + (16 * AC), pTarget->GetIndex() == pLocal->GetIndex() ? Color(240, 25, 25, 255) : Color(255, 255, 255, 0), Render::Fonts::ESP, buf);
									AC++;
								}
								else {
									char buf[256]; sprintf_s(buf, "%s", pinfo.name, pinfo2.name);
									RECT TextSize = Render::GetTextSize(Render::Fonts::ESP, buf);
									//Render::Clear(scrn.left, (scrn.bottom / 3) + (16 * AC), 260, 16, Color(0, 0, 0, 140));
									//Render::Text(scrn.left, (scrn.bottom / 2 - 16), pTarget->GetIndex() == pLocal->GetIndex() ? Color(240, 70, 80, 255) : Color(255, 255, 255, 0), Render::Fonts::ESP, "Spectating me list:");
									Render::Text(scrn.left, (scrn.bottom / 2) + (16 * AC), pTarget->GetIndex() == pLocal->GetIndex() ? Color(240, 25, 25, 255) : Color(255, 255, 255, 0), Render::Fonts::ESP, buf);
									AC++;
								}
							}
						}
					}
				}
			}
		}
	//Render::Outline(scrn.right - 261, (scrn.bottom / 2) - 1, 262, (16 * AC) + 2, Color(23, 23, 23, 255));
	//Render::Outline(scrn.right - 260, (scrn.bottom / 2), 260, (16 * AC), Color(90, 90, 90, 255));
}

//  Yeah m8
void CEsp::DrawPlayer(IClientEntity* pEntity, player_info_t pinfo)
{
	ESPBox Box;
	Color Color;
	IClientEntity *pLocal = hackManager.pLocal();

	// Show own team false? well gtfo teammate lol // should fix a stupid fucking esp box issue
	if (!Menu::Window.VisualsTab.OptionsTeammates.GetState() && (pEntity->GetTeamNum() == hackManager.pLocal()->GetTeamNum()))
		return;
	if (pEntity != pLocal && pEntity)
	{
		if (GetBox(pEntity, Box))
		{
			Color = GetPlayerColor(pEntity);

			if (Menu::Window.VisualsTab.OptionsBox.GetState())
				DrawBox(Box, Color);

			if (Menu::Window.VisualsTab.DLight.GetState())
				DLight(pLocal, pEntity);

			if (Menu::Window.VisualsTab.OptionsName.GetState())
				DrawName(pinfo, Box);

			if (Menu::Window.VisualsTab.OptionsHealth.GetState())
				DrawHealth(pEntity, Box);

			if (Menu::Window.VisualsTab.OptionsGray.GetState())
				DrawGray();

			if (Menu::Window.VisualsTab.OptionsInfo.GetState() || Menu::Window.VisualsTab.OptionsWeapon.GetState())
				DrawInfo(pEntity, Box);

			if (Menu::Window.VisualsTab.OptionsMoney.GetState())
				DrawMoney(pEntity, Box);

			if (Menu::Window.VisualsTab.OptionsAimSpot.GetState())
				DrawCross(pEntity);

			if (Menu::Window.VisualsTab.OptionsInfo.GetState())
				DrawCircle1(pEntity);//WHEN ESP IS ON SHOULD APPLY CIRCLE WHERE AIMBOT IS AIMING

			if (Menu::Window.VisualsTab.backtrackskele.GetState())
				drawBacktrackedSkelet(pEntity,pinfo);//WHEN ESP IS ON SHOULD APPLY CIRCLE WHERE AIMBOT IS AIMING

			if (Menu::Window.VisualsTab.OptionsArmor.GetState())
				Armor(pEntity, Box);

			if (Menu::Window.VisualsTab.SightLines.GetState())
				SightLines(pEntity, Color);

			if (Menu::Window.VisualsTab.OptionsSkeleton.GetState())
				DrawSkeleton(pEntity);

			if (Menu::Window.VisualsTab.OptionsGlow.GetState())
				DrawGlow();

			if (Menu::Window.VisualsTab.OptionsBarrels.GetState())
				Barrel(Box, Color, pEntity);

			if (Menu::Window.VisualsTab.DrawLinesAA.GetState())
				DrawLinesAA(Color);
			if(pLocal->IsAlive())
			if (Menu::Window.VisualsTab.Grenades2.GetState())
				NewNadePred();

			if (Menu::Window.VisualsTab.DrawDistance.GetState())
				DrawDistance(pEntity, Box);
		}
	}
}

void CEsp::SightLines(IClientEntity* pEntity, Color color)
{
	Vector src3D, dst3D, forward, src, dst;
	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;
	IClientRenderable* pUnknoown = pEntity->GetClientRenderable();
	if (pUnknoown == nullptr)
		return;
	AngleVectors(pEntity->GetEyeAngles(), &forward);
	filter.pSkip = pEntity;
	src3D = pEntity->GetBonePos(6) - Vector(0, 0, 0);
	dst3D = src3D + (forward * Menu::Window.VisualsTab.SightLinesLength.GetValue());

	ray.Init(src3D, dst3D);
	tr.endpos = pEntity->GetEyeAngles();
	Interfaces::Trace->TraceRay(ray, MASK_SHOT, &filter, &tr);

	if (!Render::WorldToScreen(src3D, src) || !Render::WorldToScreen(tr.endpos, dst))
		return;

	Render::Line(src.x, src.y, dst.x, dst.y, Color(255, 255, 255, 225));
	Render::DrawRect(dst.x - 3, dst.y - 3, 6, 6, Color(255, 255, 255, 255));
};

// Gets the 2D bounding box for the entity
// Returns false on failure nigga don't fail me
bool CEsp::GetBox(IClientEntity* pEntity, CEsp::ESPBox &result)
{
	// Variables
	Vector  vOrigin, min, max, sMin, sMax, sOrigin,
		flb, brt, blb, frt, frb, brb, blt, flt;
	float left, top, right, bottom;

	// Get the locations
	vOrigin = pEntity->GetOrigin();
	min = pEntity->collisionProperty()->GetMins() + vOrigin;
	max = pEntity->collisionProperty()->GetMaxs() + vOrigin;

	// Points of a 3d bounding box
	Vector points[] = { Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z) };

	// Get screen positions
	if (!Render::WorldToScreen(points[3], flb) || !Render::WorldToScreen(points[5], brt)
		|| !Render::WorldToScreen(points[0], blb) || !Render::WorldToScreen(points[4], frt)
		|| !Render::WorldToScreen(points[2], frb) || !Render::WorldToScreen(points[1], brb)
		|| !Render::WorldToScreen(points[6], blt) || !Render::WorldToScreen(points[7], flt))
		return false;

	// Put them in an array (maybe start them off in one later for speed?)
	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	// Init this shit
	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;

	// Find the bounding corners for our box
	for (int i = 1; i < 8; i++)
	{
		if (left > arr[i].x)
			left = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
	}

	// Width / height
	result.x = left;
	result.y = top;
	result.w = right - left;
	result.h = bottom - top;

	return true;
}

void CEsp::DrawGlow()
{
	CGlowObjectManager* GlowObjectManager = (CGlowObjectManager*)GlowManager;

	for (int i = 0; i < GlowObjectManager->size; ++i)
	{
		CGlowObjectManager::GlowObjectDefinition_t* glowEntity = &GlowObjectManager->m_GlowObjectDefinitions[i];
		IClientEntity* Entity = glowEntity->getEntity();

		if (glowEntity->IsEmpty() || !Entity)
			continue;

		switch (Entity->GetClientClass()->m_ClassID)
		{
		case 35:
				if (GameUtils::IsVisible(hackManager.pLocal(), Entity, 0))
				{
					glowEntity->set((Entity->GetTeamNum() == hackManager.pLocal()->GetTeamNum()) ? Color(180, 60, 120, 160) : Color(180, 60, 120, 160));
				}

				else if (!Menu::Window.VisualsTab.OptionsTeammates.GetState() && hackManager.pLocal()->GetTeamNum())
				{
					glowEntity->set((Entity->GetTeamNum() == hackManager.pLocal()->GetTeamNum()) ? Color(180, 60, 120, 160) : Color(180, 60, 120, 160));
				}
			}
		}
}


void CEsp::Barrel(CEsp::ESPBox size, Color color, IClientEntity* pEntity)
{
	Vector src3D, src;
	src3D = pEntity->GetOrigin() - Vector(0, 0, 0);

	if (!Render::WorldToScreen(src3D, src))
		return;

	int ScreenWidth, ScreenHeight;
	Interfaces::Engine->GetScreenSize(ScreenWidth, ScreenHeight);

	int x = (int)(ScreenWidth * 0.5f);
	int y = 0;


	y = ScreenHeight;

	Render::Line((int)(src.x), (int)(src.y), x, y, Color(0, 0, 0, 255));
}

void NewNadePred()
{
	IClientEntity *pLocal = hackManager.pLocal();
	IClientEntity *pEntity = hackManager.pLocal();
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	const float TIMEALIVE = 5.f;
	const float GRENADE_COEFFICIENT_OF_RESTITUTION = 0.4f;

	float fStep = 0.1f;
	float fGravity = 800.0f / 8.f;

	Vector vPos;
	Vector vStart;
	Vector vThrow;
	Vector vThrow2;
	int iCollisions = 0;

	Vector vViewAngles;
	Interfaces::Engine->GetViewAngles(vViewAngles);

	vThrow[0] = vViewAngles[0];
	vThrow[1] = vViewAngles[1];
	vThrow[2] = vViewAngles[2];

	if (vThrow[0] < 0)
		vThrow[0] = -10 + vThrow[0] * ((90 - 10) / 90.0);
	else
		vThrow[0] = -10 + vThrow[0] * ((90 + 10) / 90.0);

	float fVel = (90 - vThrow[0]) * 4;
	if (fVel > 500)
		fVel = 500;

	AngleVectors(vThrow, &vThrow2);

	Vector vEye = pEntity->GetEyePosition();
	vStart[0] = vEye[0] + vThrow2[0] * 16;
	vStart[1] = vEye[1] + vThrow2[1] * 16;
	vStart[2] = vEye[2] + vThrow2[2] * 16;

	vThrow2[0] = (vThrow2[0] * fVel) + pEntity->GetVelocity()[0];
	vThrow2[1] = (vThrow2[1] * fVel) + pEntity->GetVelocity()[1];
	vThrow2[2] = (vThrow2[2] * fVel) + pEntity->GetVelocity()[2]; // casualhacker for da magic calc help

	for (float fTime = 0.0f; fTime < TIMEALIVE; fTime += fStep)
	{
		vPos = vStart + vThrow2 * fStep;

		Ray_t ray;
		trace_t tr;

		CTraceFilter loc;
		loc.pSkip = pEntity;

		ray.Init(vStart, vPos);
		Interfaces::Trace->TraceRay(ray, MASK_SOLID, &loc, &tr);

		if (tr.DidHit()) // if the trace hits a surface
		{
			//float proj = DotProduct(throwvec, tr.plane.normal);
			vThrow2 = tr.plane.normal * -2.0f *DotProduct(vThrow2, tr.plane.normal) + vThrow2;
			vThrow2 *= GRENADE_COEFFICIENT_OF_RESTITUTION;

			iCollisions++;
			if (iCollisions > 2)
				break;

			vPos = vStart + vThrow2*tr.fraction*fStep;

			fTime += (fStep * (1 - tr.fraction));
		} // end of bounce

		Vector vOutStart, vOutEnd;
		if (GameUtils::IsG(pWeapon))
		{
			if (Render::WorldToScreen(vStart, vOutStart), Render::WorldToScreen(vPos, vOutEnd))
			{
				Render::Line(vOutStart.x, vOutStart.y, vOutEnd.x, vOutEnd.y, Color(0, 150, 255, 255));
			}

			vStart = vPos;
			vThrow2.z -= fGravity*tr.fraction*fStep;
		}
	}
}

void CEsp::DrawMoney(IClientEntity* pEntity, CEsp::ESPBox size)
{
	ESPBox ArmorBar = size;

	int MoneyEnemy = 100;
	MoneyEnemy = pEntity->GetMoney();
	char nameBuffer[512];
	sprintf_s(nameBuffer, "$ %d", MoneyEnemy);
	RECT nameSize = Render::GetTextSize(Render::Fonts::ESP, nameBuffer);
	int i = 0;
	Render::Text(size.x + size.w + 3, size.y + (i*(nameSize.bottom + 2)), Color(144, 194, 54, 255), Render::Fonts::ESP, nameBuffer);
}

// Get an entities color depending on team and vis ect
Color CEsp::GetPlayerColor(IClientEntity* pEntity)
{
	int TeamNum = pEntity->GetTeamNum();
	bool IsVis = GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::Head);

	Color color;

	if (TeamNum == TEAM_CS_T)
	{
		if (IsVis)
			color = Color(240, 240, 240, 240);
		else
			color = Color(240, 240, 240, 240);
	}
	else
	{
		if (IsVis)
			color = Color(240, 240, 240, 240);
		else
			color = Color(240, 240, 240, 240);
	}


	return color;
}

void CEsp::DrawLinesAA(Color color) {
	Vector src3D, dst3D, forward, src, dst;
	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;

	filter.pSkip = hackManager.pLocal();

	// LBY
	AngleVectors(QAngle(0, lineLBY, 0), &forward);
	src3D = hackManager.pLocal()->GetOrigin();
	dst3D = src3D + (forward * 50.f); //replace 50 with the length you want the line to have

	ray.Init(src3D, dst3D);

	Interfaces::Trace->TraceRay(ray, 0, &filter, &tr);

	if (!Render::WorldToScreen(src3D, src) || !Render::WorldToScreen(tr.endpos, dst))
		return;

	Render::Line(src.x, src.y, dst.x, dst.y, Color(0, 0, 255, 255));
	Render::DrawCircle(dst.x,dst.y, 2, 2 * 4,Color(0,0,255,255));
	//Render::DrawRect(dst.x - 4, dst.y - 4, 8, 8, Color(0, 0, 255, 255));
	// REAL AGNEL
	AngleVectors(QAngle(0, lineRealAngle, 0), &forward);
	dst3D = src3D + (forward * 40.f); //replace 50 with the length you want the line to have

	ray.Init(src3D, dst3D);

	Interfaces::Trace->TraceRay(ray, 0, &filter, &tr);

	if (!Render::WorldToScreen(src3D, src) || !Render::WorldToScreen(tr.endpos, dst))
		return;

	Render::Line(src.x, src.y, dst.x, dst.y, Color(0, 255, 0, 255));
	Render::DrawCircle(dst.x, dst.y, 2, 2 * 4, Color(0, 255, 0, 255));

	// Fake AGNEL
	AngleVectors(QAngle(0, lineFakeAngle, 0), &forward);
	dst3D = src3D + (forward * 40.f); //replace 50 with the length you want the line to have

	ray.Init(src3D, dst3D);

	Interfaces::Trace->TraceRay(ray, 0, &filter, &tr);

	if (!Render::WorldToScreen(src3D, src) || !Render::WorldToScreen(tr.endpos, dst))
		return;

	Render::Line(src.x, src.y, dst.x, dst.y, Color(255, 25, 25, 255));
	Render::DrawCircle(dst.x, dst.y, 2, 2 * 4, Color(255, 25, 25, 255));

}

// 2D  Esp box
void CEsp::DrawBox(CEsp::ESPBox size, Color color)
{
	{
		{
			//bool IsVis = GameUtils::IsVisible(hackManager.pLocal(), pEntity, (int)CSGOHitboxID::Chest);  da dream
				{
					Render::Outline(size.x, size.y, size.w, size.h, color);
					Render::Outline(size.x - 1, size.y - 1, size.w + 1, size.h + 1, Color(0, 0, 0, 0));
					Render::Outline(size.x + 1, size.y + 1, size.w - 1, size.h - 1, Color(0, 0, 0, 0));
				}

			if (Menu::Window.VisualsTab.OptionsFilled.GetState())//this has to stay
			{
				Render::DrawRect(size.x, size.y, size.w, size.h, Color(20, 20, 20, 120));
			}
		}
	}
}


// Unicode Conversions
static wchar_t* CharToWideChar(const char* text)
{
	size_t size = strlen(text) + 1;
	wchar_t* wa = new wchar_t[size];
	mbstowcs_s(NULL, wa, size / 4, text, size);
	return wa;
}

// Player name
void CEsp::DrawName(player_info_t pinfo, CEsp::ESPBox size)
{
	if (strlen(pinfo.name) > 16)
	{
		pinfo.name[16] = 0;
		strcat(pinfo.name, "...");
		puts(pinfo.name);
		RECT nameSize = Render::GetTextSize(Render::Fonts::ESP, pinfo.name);
		Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y - 16, Color(255, 255, 255, 255), Render::Fonts::ESP, pinfo.name);
	}
	else
	{
		RECT nameSize = Render::GetTextSize(Render::Fonts::ESP, pinfo.name);
		Render::Text(size.x + (size.w / 2) - (nameSize.right / 2), size.y - 16, Color(255, 255, 255, 255), Render::Fonts::ESP, pinfo.name);
	}
}

void CEsp::Armor(IClientEntity* pEntity, CEsp::ESPBox size)
{
	ESPBox ArBar = size;
	ArBar.y += (ArBar.h + 1);
	ArBar.h = 12;

	float ArValue = pEntity->ArmorValue();
	float ArPerc = ArValue / 100.f;
	float Width = (size.w * ArPerc);
	ArBar.w = Width;

	Vertex_t Verts[4];
	Verts[0].Init(Vector2D(ArBar.x, ArBar.y + 1));
	Verts[1].Init(Vector2D(ArBar.x + size.w + 0, ArBar.y + 1));
	Verts[2].Init(Vector2D(ArBar.x + size.w, ArBar.y + 3));
	Verts[3].Init(Vector2D(ArBar.x - 0, ArBar.y + 3));

	Render::PolygonOutline(4, Verts, Color(50, 50, 50, 255), Color(50, 50, 50, 255));

	Vertex_t Verts2[4];
	Verts2[0].Init(Vector2D(ArBar.x, ArBar.y + 2));
	Verts2[1].Init(Vector2D(ArBar.x + ArBar.w + 0, ArBar.y + 2));
	Verts2[2].Init(Vector2D(ArBar.x + ArBar.w, ArBar.y + 3));
	Verts2[3].Init(Vector2D(ArBar.x, ArBar.y + 3));

	Color c = GetPlayerColor(pEntity);
	Render::Polygon(4, Verts2, Color(0, 180, 255, 200));
}


// Draw a health bar. For Tf2 when a bar is bigger than max health a second bar is displayed
void CEsp::DrawHealth(IClientEntity* pEntity, CEsp::ESPBox size)
{
	int HPEnemy = 100;
	HPEnemy = pEntity->GetHealth();
	char nameBuffer[512];
	sprintf_s(nameBuffer, "%d", HPEnemy);


	float h = (size.h);
	float offset = (h / 4.f) + 5;
	float w = h / 48.f;
	float health = pEntity->GetHealth();
	UINT hp = h - (UINT)((h * health) / 100);

	int Red = 255 - (health*2.55);
	int Green = health*2.55;

	Render::DrawRect((size.x - 3) - 1, size.y - 1, 3, h + 2, Color(0, 0, 0, 180));

	Render::Line((size.x - 3), size.y + hp, (size.x - 3), size.y + h, Color(Red, Green, 0, 180));

	if (health < 100) {

		Render::Text(size.x - 9, size.y + hp, Color(255, 255, 255, 255), Render::Fonts::ESP, nameBuffer);
	}
}

void CEsp::DrawGray()
{
	auto impact = Interfaces::CVar->FindVar("mat_drawgray");
	auto impactspoof = new SpoofedConvar(impact);
	if (Menu::Window.VisualsTab.OptionsGray.GetState())
		impactspoof->SetInt(1);
	else
		impactspoof->SetInt(0);
}

void CEsp::DrawDistance(IClientEntity* pEntity, CEsp::ESPBox size)
{
	IClientEntity *pLocal = hackManager.pLocal();

	Vector vecOrigin = pEntity->GetOrigin();
	Vector vecOriginLocal = pLocal->GetOrigin();
	static RECT defSize = Render::GetTextSize(Render::Fonts::Default, "");

	char dist_to[32];
	sprintf_s(dist_to, "%.0fm", DistanceTo(vecOrigin, vecOriginLocal));
	RECT nameSize = Render::GetTextSize(Render::Fonts::ESP, "");
	int i = 0;

	Render::Text(size.x + size.w + 3, size.y + (i*(nameSize.bottom + 6) + 18), Color(255, 255, 255, 255), Render::Fonts::ESP, dist_to);
}

// Cleans the internal class name up to something human readable and nice
std::string CleanItemName(std::string name)
{
	std::string Name = name;
	// Tidy up the weapon Name
	if (Name[0] == 'C')
		Name.erase(Name.begin());

	// Remove the word Weapon
	auto startOfWeap = Name.find("Weapon");
	if (startOfWeap != std::string::npos)
		Name.erase(Name.begin() + startOfWeap, Name.begin() + startOfWeap + 6);

	return Name;
}

// Anything else: weapons, class state? idk
void CEsp::DrawInfo(IClientEntity* pEntity, CEsp::ESPBox size)
{
	std::vector<std::string> Info;
	int i = 0;

	// Player Weapon ESP
	IClientEntity* pWeapon = Interfaces::EntList->GetClientEntityFromHandle((HANDLE)pEntity->GetActiveWeaponHandle());
	if (Menu::Window.VisualsTab.OptionsWeapon.GetState() && pWeapon)
	{
		ClientClass* cClass = (ClientClass*)pWeapon->GetClientClass();
		if (cClass)
		{
			// Draw it
			Info.push_back(CleanItemName(cClass->m_pNetworkName));
		}
	}

	if (Menu::Window.VisualsTab.OptionsInfo.GetState() && pEntity == BombCarrier)
	{
		RECT nameSize = Render::GetTextSize(Render::Fonts::ESP, "");

		Render::Text(size.x + size.w + 3, size.y + (i*(nameSize.bottom + 6) + 16), Color(255, 25, 25, 255), Render::Fonts::Menu, ("Bomb Carrier"));
		i++;
	}

	if (Menu::Window.VisualsTab.OptionsInfo.GetState() && pEntity->IsScoped())
	{
		RECT nameSize = Render::GetTextSize(Render::Fonts::ESP, "");
		int i = 0;
		Render::Text(size.x + size.w + 3, size.y + (i*(nameSize.bottom + 4)), Color(0, 51, 102, 255), Render::Fonts::ESP, "ZOOM");
	}
	RECT nameSize = Render::GetTextSize(Render::Fonts::ESP, "");
	char dmgmemes[64];
	sprintf_s(dmgmemes, "DMG: %.1f", autowalldmgtest[pEntity->GetIndex()]);
	Render::Text(size.x + size.w + 3, size.y + (i*(nameSize.bottom + 6)+10), Color(208, 202, 0, 255), Render::Fonts::ESP, dmgmemes);
	static RECT Size = Render::GetTextSize(Render::Fonts::Menu, "Text");
	for (auto Text : Info)
	{
		Render::Text(size.x + (size.w / 2) - (Size.right / 2), size.y + size.h + 2, Color(255, 255, 255, 255), Render::Fonts::Menu, Text.c_str());
		i++;
	}

}

void CEsp::drawBacktrackedSkelet(IClientEntity *base, player_info_t pinfo)
{
	//lagComp = new LagCompensation;

	int idx = base->getIdx();
	IClientEntity* pLocal = hackManager.pLocal();

	LagRecord *m_LagRecords = lagComp->m_LagRecord[idx];
	LagRecord recentLR = m_LagRecords[10];
	static int Scale = 2;
	Vector screenSpot;
	Vector screenSpot2;
	/*
	for (int i = 0; i < 9; i++)
	{

		if (Render::WorldToScreen(m_LagRecords[i].headSpot, screenSpot))
		{
			Render::DrawCircle(screenSpot.x, screenSpot.y, (2), (2 * 4), Color(0, 255, 25, 255));

		}
	}

	if (Render::WorldToScreen(recentLR.headSpot, screenSpot2))
	{
		Render::DrawCircle(screenSpot2.x, screenSpot2.y, (2), (2 * 4), Color(255, 25, 25, 255));

	}*/
	bool IsVis = GameUtils::IsVisible(hackManager.pLocal(), base, (int)CSGOHitboxID::Head);

	for (int i = 0; i < Interfaces::Engine->GetMaxClients(); i++)
	{
		if (Interfaces::Engine->GetPlayerInfo(i, &pinfo) && base->IsAlive())
		{
			if (Menu::Window.LegitBotTab.FakeLagFix.GetState() || Menu::Window.RageBotTab.FakeLagFix.GetState())
			{
				if ( Menu::Window.VisualsTab.LegitMode1.GetState()) {
					if (!IsVis)
						return;

					if (pLocal->IsAlive())
					{
						for (int t = 0; t < 12; ++t)
						{
							Vector screenbacktrack[64][12];

							if (headPositions[i][t].simtime && headPositions[i][t].simtime + 1 > pLocal->GetSimulationTime())
							{
								if (Render::WorldToScreen(headPositions[i][t].hitboxPos, screenbacktrack[i][t]))
								{

									Interfaces::Surface->DrawSetColor(Color::Green());
									Interfaces::Surface->DrawOutlinedRect(screenbacktrack[i][t].x, screenbacktrack[i][t].y, screenbacktrack[i][t].x + 2, screenbacktrack[i][t].y + 2);

								}
							}
						}
					}
					else
					{
						memset(&headPositions[0][0], 0, sizeof(headPositions));
					}
				}
				else if(!Menu::Window.VisualsTab.LegitMode1.GetState()) {//these are where we are going to put a legitmode check shit 

					if (pLocal->IsAlive())
					{
						for (int t = 0; t < 12; ++t)
						{
							Vector screenbacktrack[64][12];

							if (headPositions[i][t].simtime && headPositions[i][t].simtime + 1 > pLocal->GetSimulationTime())
							{
								if (Render::WorldToScreen(headPositions[i][t].hitboxPos, screenbacktrack[i][t]))
								{

									Interfaces::Surface->DrawSetColor(Color::Green());
									Interfaces::Surface->DrawOutlinedRect(screenbacktrack[i][t].x, screenbacktrack[i][t].y, screenbacktrack[i][t].x + 2, screenbacktrack[i][t].y + 2);

								}
							}
						}
					}
					else
					{
						memset(&headPositions[0][0], 0, sizeof(headPositions));
					}

				}
			}
		}
	}

}


// Little cross on their heads
void CEsp::DrawCross(IClientEntity* pEntity)
{
	Vector cross = pEntity->GetHeadPos(), screen;
	static int Scale = 2;
	if (Render::WorldToScreen(cross, screen))
	{

		Render::Clear(screen.x - Scale, screen.y - (Scale * 2), (Scale * 2), (Scale * 4), Color(20, 20, 20, 160));
		Render::Clear(screen.x - (Scale * 2), screen.y - Scale, (Scale * 4), (Scale * 2), Color(20, 20, 20, 160));
		Render::Clear(screen.x - Scale - 1, screen.y - (Scale * 2) - 1, (Scale * 2) - 2, (Scale * 4) - 2, Color(250, 250, 250, 160));
		Render::Clear(screen.x - (Scale * 2) - 1, screen.y - Scale - 1, (Scale * 4) - 2, (Scale * 2) - 2, Color(250, 250, 250, 160));
	}
}

void CEsp::DefuseWarning(IClientEntity* pEntity)
{
	if (pEntity->IsDefusing())
		Render::Text(10, 100, Color(255, 25, 25, 255), Render::Fonts::ESP, ("Enemy is defusing"));
	else
		Render::Text(10, 100, Color(0, 255, 0, 255), Render::Fonts::ESP, (""));
}

void CEsp::DrawCircle1(IClientEntity* pEntity)
{
	Vector cross = AimPoint, screen;
	static int Scale = 2;
	if (Render::WorldToScreen(cross, screen))
	{
		Render::DrawCircle(screen.x, screen.y, (2), (2 * 4), Color(255, 25, 25, 255));
	}

}

// Draws a dropped CS:GO Item
void CEsp::DrawDrop(IClientEntity* pEntity, ClientClass* cClass)
{
	Vector Box;
	CBaseCombatWeapon* Weapon = (CBaseCombatWeapon*)pEntity;
	IClientEntity* plr = Interfaces::EntList->GetClientEntityFromHandle((HANDLE)Weapon->GetOwnerHandle());
	if (!plr && Render::WorldToScreen(Weapon->GetOrigin(), Box))
	{
		if (Menu::Window.VisualsTab.FiltersWeapons.GetState())
		{
			std::string ItemName = CleanItemName(cClass->m_pNetworkName);
			RECT TextSize = Render::GetTextSize(Render::Fonts::ESP, ItemName.c_str());
			Render::Text(Box.x - (TextSize.right / 2), Box.y - 16, Color(255, 255, 255, 255), Render::Fonts::ESP, ItemName.c_str());
		}
	}
}

// Draws a chicken
void CEsp::DrawChicken(IClientEntity* pEntity, ClientClass* cClass)
{
	ESPBox Box;
	/*
	if (GetBox(pEntity, Box))
	{
		player_info_t pinfo; strcpy_s(pinfo.name, "Chicken");
		if (Menu::Window.VisualsTab.OptionsBox.GetState())
			DrawBox(Box, Color(255, 255, 255, 255));

		if (Menu::Window.VisualsTab.OptionsName.GetState())
			DrawName(pinfo, Box);
	}*/
}


// Draw the planted bomb and timer
void CEsp::DrawBombPlanted(IClientEntity* pEntity, ClientClass* cClass)
{
	// Null it out incase bomb has been dropped or planted
	BombCarrier = nullptr;

	Vector vOrig; Vector vScreen;
	vOrig = pEntity->GetOrigin();
	CCSBomb* Bomb = (CCSBomb*)pEntity;

	if (Render::WorldToScreen(vOrig, vScreen))
	{
		float flBlow = Bomb->GetC4BlowTime();
		float TimeRemaining = flBlow - (Interfaces::Globals->interval_per_tick * hackManager.pLocal()->GetTickBase());
		char buffer[64];
		if (TimeRemaining > 0)//happy now no more negative
		{
			sprintf_s(buffer, "[ Planted C4 %.1fs ]", TimeRemaining);
			Render::Text(vScreen.x, vScreen.y, Color(255, 25, 25, 255), Render::Fonts::ESP, buffer);
		}
		else {//this is a meme but i keep so the esp doesnt just dissappear once it explodes... makes it look cleaner
			sprintf_s(buffer, "[ Exploded C4 ]", TimeRemaining);
			Render::Text(vScreen.x, vScreen.y, Color(255, 25, 25, 255), Render::Fonts::ESP, buffer);
		}
	}
}

void CEsp::DrawBombPlanted2(IClientEntity* pEntity, ClientClass* cClass)
{
	// Null it out incase bomb has been dropped or planted
	BombCarrier = nullptr;

	Vector vOrig; Vector vScreen;
	vOrig = pEntity->GetOrigin();
	CCSBomb* Bomb = (CCSBomb*)pEntity;

	float flBlow = Bomb->GetC4BlowTime();
	float TimeRemaining = flBlow - (Interfaces::Globals->interval_per_tick * hackManager.pLocal()->GetTickBase());
	char buffer[64];
	sprintf_s(buffer, "%.1fs Time To Defuse", TimeRemaining);
	if(hackManager.pLocal()->IsAlive())
		Render::Text(10, 400, Color(0, 255, 0, 255), Render::Fonts::ESP, buffer);
	else
		Render::Text(10, 350, Color(0, 255, 0, 255), Render::Fonts::ESP, buffer);

}

void CEsp::DrawBomb(IClientEntity* pEntity, ClientClass* cClass)
{
	// Null it out incase bomb has been dropped or planted
	BombCarrier = nullptr;
	CBaseCombatWeapon *BombWeapon = (CBaseCombatWeapon *)pEntity;
	Vector vOrig; Vector vScreen;
	vOrig = pEntity->GetOrigin();
	bool adopted = true;
	HANDLE parent = BombWeapon->GetOwnerHandle();
	if (parent || (vOrig.x == 0 && vOrig.y == 0 && vOrig.z == 0))
	{
		IClientEntity* pParentEnt = (Interfaces::EntList->GetClientEntityFromHandle(parent));
		if (pParentEnt && pParentEnt->IsAlive())
		{
			BombCarrier = pParentEnt;
			adopted = false;
		}
	}

	if (adopted)
	{
		if (Render::WorldToScreen(vOrig, vScreen))
		{
			Render::Text(vScreen.x, vScreen.y, Color(255, 25, 25, 255), Render::Fonts::ESP, "[ Dropped C4 ]");
		}
	}
}

void DrawBoneArray(int* boneNumbers, int amount, IClientEntity* pEntity, Color color)
{
	Vector LastBoneScreen;
	for (int i = 0; i < amount; i++)
	{

		Vector Bone = pEntity->GetBonePos(boneNumbers[i]);
		Vector BoneScreen;

		if (Render::WorldToScreen(Bone, BoneScreen))
		{
			if (i>0)
			{
				Render::Line(LastBoneScreen.x, LastBoneScreen.y, BoneScreen.x, BoneScreen.y, color);
			}
		}
		LastBoneScreen = BoneScreen;
	}
}

void DrawBoneTest(IClientEntity *pEntity)
{
	for (int i = 0; i < 127; i++)
	{
		IClientRenderable* pUnknoown = pEntity->GetClientRenderable();
		if (pUnknoown == nullptr)
			return;
		Vector BoneLoc = pEntity->GetBonePos(i);
		Vector BoneScreen;
		if (Render::WorldToScreen(BoneLoc, BoneScreen))
		{
			char buf[10];
			_itoa_s(i, buf, 10);
			Render::Text(BoneScreen.x, BoneScreen.y, Color(255, 255, 255, 180), Render::Fonts::ESP, buf);
		}
	}
}

void CEsp::DrawSkeleton(IClientEntity* pEntity)
{
	studiohdr_t* pStudioHdr = Interfaces::ModelInfo->GetStudiomodel(pEntity->GetModel());
	 IClientEntity* pLocal = hackManager.pLocal();
	if (!pStudioHdr)
		return;

	Vector vParent, vChild, sParent, sChild;
	for (int j = 0; j < pStudioHdr->numbones; j++)
	{
		mstudiobone_t* pBone = pStudioHdr->GetBone(j);

		if (pBone && (pBone->flags & BONE_USED_BY_HITBOX) && (pBone->parent != -1))
		{
			IClientRenderable* pUnknoown = pEntity->GetClientRenderable();
			if (pUnknoown == nullptr)
				return;
			vChild = pEntity->GetBonePos(j);
			vParent = pEntity->GetBonePos(pBone->parent);

			if (Render::WorldToScreen(vParent, sParent) && Render::WorldToScreen(vChild, sChild))
			{
				Render::Line(sParent[0], sParent[1], sChild[0], sChild[1], Color(0, 0, 0, 255));
			}
		}
	}
}

void CEsp::BoxAndText(IClientEntity* entity, std::string text)
{
	ESPBox Box;
	std::vector<std::string> Info;
	if (GetBox(entity, Box))
	{
		Info.push_back(text);
			int i = 0;
			for (auto kek : Info)
			{
				Render::Text(Box.x + 1, Box.y + 1, Color(255, 255, 255, 255), Render::Fonts::ESP, kek.c_str());
				i++;
			}
	}
}

void CEsp::DrawThrowable(IClientEntity* throwable)
{
	model_t* nadeModel = (model_t*)throwable->GetModel();

	if (!nadeModel)
		return;

	studiohdr_t* hdr = Interfaces::ModelInfo->GetStudiomodel(nadeModel);

	if (!hdr)
		return;

	if (!strstr(hdr->name, "thrown") && !strstr(hdr->name, "dropped"))
		return;

	std::string nadeName = "Unknown Grenade";

	IMaterial* mats[32];
	Interfaces::ModelInfo->GetModelMaterials(nadeModel, hdr->numtextures, mats);

	for (int i = 0; i < hdr->numtextures; i++)
	{
		IMaterial* mat = mats[i];
		if (!mat)
			continue;

		if (strstr(mat->GetName(), "flashbang"))
		{
			nadeName = "Flashbang";
			break;
		}
		else if (strstr(mat->GetName(), "m67_grenade") || strstr(mat->GetName(), "hegrenade"))
		{
			nadeName = "Grenade";
			break;
		}
		else if (strstr(mat->GetName(), "smoke"))
		{
			nadeName = "Smoke";
			break;
		}
		else if (strstr(mat->GetName(), "decoy"))
		{
			nadeName = "Decoy";
			break;
		}
		else if (strstr(mat->GetName(), "incendiary") || strstr(mat->GetName(), "molotov"))
		{
			nadeName = "Molotov";
			break;
		}
	}
	BoxAndText(throwable, nadeName);
}