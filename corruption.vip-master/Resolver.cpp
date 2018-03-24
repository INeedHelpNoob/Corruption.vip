#include "Resolver.h"
#include "Ragebot.h"
#include "Hooks.h"
#include "RenderManager.h"
#include "Autowall.h"
#include <iostream>
#include "UTIL Functions.h"
#include "lagcomp.h"
#include "EnginePrediction.h"
bool bLowerBodyUpdated;
bool IsUsingFakeAngles;
float Differences[65];
inline float RandomFloat(float min, float max)
{
	static auto fn = (decltype(&RandomFloat))(GetProcAddress(GetModuleHandle("vstdlib.dll"), "RandomFloat"));
	return fn(min, max);
}
bool HasFakeHead(IClientEntity* pEntity) {
	//lby should update if distance from lby to eye angles exceeds 35 degrees
	return abs(pEntity->GetEyeAnglesXY()->y - pEntity->GetLowerBodyYaw()) > 35;
}
bool Lbywithin35(IClientEntity* pEntity) {
	//lby should update if distance from lby to eye angles less than 35 degrees
	return abs(pEntity->GetEyeAnglesXY()->y - pEntity->GetLowerBodyYaw()) < 35;
}
bool IsMovingOnGround(IClientEntity* pEntity) {
	//Check if player has a velocity greater than 0 (moving) and if they are onground.
	return pEntity->GetVelocity().Length2D() > 0.1f && pEntity->GetFlags() & FL_ONGROUND;
}
bool IsMovingOnInAir(IClientEntity* pEntity) {
	//Check if player has a velocity greater than 0 (moving) and if they are onground.
	return !(pEntity->GetFlags() & FL_ONGROUND);
}
bool OnGround(IClientEntity* pEntity) {
	//Check if player has a velocity greater than 0 (moving) and if they are onground.
	return pEntity->GetFlags() & FL_ONGROUND;
}
bool IsFakeWalking(IClientEntity* pEntity) {
	//Check if a player is moving, but at below a velocity of 36
	return IsMovingOnGround(pEntity) && pEntity->GetVelocity().Length2D() < 36.0f;
}

int GetEstimatedServerTickCount(float latency)
{
	return (int)floorf((float)((float)(latency) / (float)((uintptr_t)&Interfaces::Globals->interval_per_tick)) + 0.5) + 1 + (int)((uintptr_t)&Interfaces::Globals->tickcount);
}
void LowerBodyYawFix(IClientEntity* pEntity)
{
	static Vector vLast[65];
	static bool bShotLastTime[65];
	static bool bJitterFix[65];


	bool bHasAA;
	bool bSpinbot;

	if (Menu::Window.RageBotTab.PitchCorrection.GetState())
	{
		if (!pEntity) return;
		if (pEntity->GetClientClass()->m_ClassID != (int)CSGOClassID::CCSPlayer) return;
		if (!pEntity->IsAlive() || !pEntity->GetActiveWeaponHandle()) return;
		if (Interfaces::Engine->GetLocalPlayer()) return;
		bLowerBodyUpdated = false;
		float oldlowerbodyyaw;
		IsUsingFakeAngles = false;
		static bool isMoving;
		float PlayerIsMoving = abs(pEntity->GetVelocity().Length());
		if (PlayerIsMoving > 0.1) isMoving = true;
		else if (PlayerIsMoving <= 0.1) isMoving = false;
		float TLBY = pEntity->GetLowerBodyYaw();
		float backtrack;
		for (int i = 0; i <= Interfaces::Engine->GetMaxClients(); ++i)
		{
			IClientEntity *player = (IClientEntity*)Interfaces::EntList->GetClientEntity(i);
			Vector* eyeAngles = player->GetEyeAnglesXY();
			float flYaw = eyeAngles->y;

			if (oldlowerbodyyaw != TLBY)
			{
				bLowerBodyUpdated = true;
			}
			float bodyeyedelta = pEntity->GetEyeAngles().y - flYaw;
			if (PlayerIsMoving || bLowerBodyUpdated)// || LastUpdatedNetVars->eyeangles.x != CurrentNetVars->eyeangles.x || LastUpdatedNetVars->eyeyaw != CurrentNetVars->eyeangles.y)
			{
				if (bLowerBodyUpdated || (PlayerIsMoving && bodyeyedelta >= 35.0f))
				{
					flYaw = TLBY;
					oldlowerbodyyaw = flYaw;
				}

				IsUsingFakeAngles = false;
			}
			else
			{
				if (bodyeyedelta > 35.0f)
				{
					flYaw = oldlowerbodyyaw;
					IsUsingFakeAngles = true;
				}
				else
				{
					IsUsingFakeAngles = false;
				}
			}
			/*Removed this for testing?*/
			/*
			if (IsUsingFakeAngles)
			{
				int com = GetEstimatedServerTickCount(90);

				if (com % 2)
				{
					flYaw += 90;
				}
				else if (com % 3)
					flYaw -= 90;
				else
					flYaw -= 0;
			}*/
			

			//auto EyeAngles = pEntity->GetEyeAnglesXY();
			//if (pEntity->GetVelocity().Length() > 35 && (pEntity->GetFlags() & (int)pEntity->GetFlags() & FL_ONGROUND))
				//EyeAngles->y = pEntity->GetLowerBodyYaw();
		}
	}
}

void PitchCorrection()
{
	CUserCmd* pCmd;
	for (int i = 0; i <= Interfaces::Engine->GetMaxClients(); ++i)
	{
		IClientEntity* pLocal = hackManager.pLocal();
		IClientEntity *player = (IClientEntity*)Interfaces::EntList->GetClientEntity(i);

		if (!player || player->IsDormant() || player->GetHealth() < 1 || (DWORD)player == (DWORD)pLocal)
			continue;

		if (!player)
			continue;

		if (pLocal)
			continue;

		if (pLocal && player && pLocal->IsAlive())
		{
			if (Menu::Window.RageBotTab.PitchCorrection.GetState())
			{
				Vector* eyeAngles = player->GetEyeAnglesXY();
				if (eyeAngles->x < -179.f) eyeAngles->x += 360.f;
				else if (eyeAngles->x > 90.0 || eyeAngles->x < -90.0) eyeAngles->x = 89.f;
				else if (eyeAngles->x > 89.0 && eyeAngles->x < 91.0) eyeAngles->x -= 90.f;
				else if (eyeAngles->x > 179.0 && eyeAngles->x < 181.0) eyeAngles->x -= 180;
				else if (eyeAngles->x > -179.0 && eyeAngles->x < -181.0) eyeAngles->x += 180;
				else if (fabs(eyeAngles->x) == 0) eyeAngles->x = std::copysign(89.0f, eyeAngles->x);
			}
		}
	}
}
void ResolverSetup::Resolve(IClientEntity* pEntity)
{
	CInput::CUserCmd* pCmd;
		float Delta[64];
		float OldLowerBodyYaw;
		float Resolved_angles[64];
		ResolverSetup::NewANgles[pEntity->GetIndex()] = *pEntity->GetEyeAnglesXY();
		ResolverSetup::newlby[pEntity->GetIndex()] = pEntity->GetLowerBodyYaw();
		ResolverSetup::newsimtime = pEntity->GetSimulationTime();
		ResolverSetup::newdelta[pEntity->GetIndex()] = pEntity->GetEyeAnglesXY()->y;
		ResolverSetup::newlbydelta[pEntity->GetIndex()] = pEntity->GetLowerBodyYaw();
		ResolverSetup::finaldelta[pEntity->GetIndex()] = (ResolverSetup::newdelta[pEntity->GetIndex()] + ResolverSetup::storeddelta[pEntity->GetIndex()])/2;//average
		ResolverSetup::finallbydelta[pEntity->GetIndex()] = (ResolverSetup::newlbydelta[pEntity->GetIndex()] + ResolverSetup::storedlbydelta[pEntity->GetIndex()])/2;
		ResolverSetup::final1[pEntity->GetIndex()] = finaldelta[pEntity->GetIndex()] - finallbydelta[pEntity->GetIndex()];
		 static bool lbyupdated = false;
		 if (storedlby[pEntity->GetIndex()] != pEntity->GetLowerBodyYaw())
			 lbyupdated = true;
		 else
			 lbyupdated = false;

		 float flLowerBodyYaw = pEntity->GetLowerBodyYaw();
		 int difference = pEntity->GetEyeAnglesXY()->y - flLowerBodyYaw;

		 int temp = difference;
		 while (temp < 0)
			 temp += 360;
		 while (temp > 360)
			 temp -= 360;
		 auto getdst = [](int a, int b) {
			 int x = abs(b - a) % 360;
			 return x > 180 ? 360 - x : x;
		 };
		 auto record = Backtracking->arr_infos.at(pEntity->GetIndex());
		 float prevlby = 0.f;
		 int avg = 1;
		 int count = 1;
		switch (Menu::Window.RageBotTab.AimbotResolver.GetIndex())//we need to do some shit with index so we resolver properly/apply angles properly 
		{
		case 0:

			break;
		case 1:
				/*if (Resolver::didhitHS)
				{
					if (MeetsLBYReq && islbyupdate)
					{
						pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw();
						//continue;
					}

					/*else {
						switch (Globals::Shots % 5)
						{
						case 1:
							*pEntity->GetEyeAnglesXY() = StoredAngles[pEntity->GetIndex()];
							break;
						case 2:
							*pEntity->GetEyeAnglesXY() = StoredAngles[pEntity->GetIndex()];
							break;
						case 3:
							pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 15;
							break;
						case 4:
							pEntity->GetEyeAnglesXY()->y = pEntity->GetEyeAnglesXY()->y + 40;
							break;
						case 5:
							pEntity->GetEyeAnglesXY()->y = pEntity->GetEyeAnglesXY()->y + 15;
							break;
						case 6:
							pEntity->GetEyeAnglesXY()->y = pEntity->GetEyeAnglesXY()->y - 40;
							break;
							//continue;
						}
					}*/
				//}

			for (auto& r : record.TickRecords) {
				if (r.m_flLowerBodyYawTarget == prevlby)
					continue;
				prevlby = r.m_flLowerBodyYawTarget;

				avg += getdst(static_cast<int>(r.m_angEyeAngles.y), static_cast<int>(r.m_flLowerBodyYawTarget));

				count++;
			}
			for (int i = 1; i <= Interfaces::Engine->GetMaxClients(); i++)
			{
				float test = backtracking->records[i].lby;
				float test2 = backtracking->records[i].EyeAng;//enemies yaw as a float
				if (test2 - test > 35)
					pEntity->GetEyeAnglesXY()->y -= 180;


			}
				if (GetAsyncKeyState(Menu::Window.RageBotTab.SomeShit.GetKey()) && Menu::Window.RageBotTab.SomeShit.GetKey() > 0)
				{
					auto local_player = Interfaces::EntList->GetClientEntity(
						Interfaces::Engine->GetLocalPlayer());
					if (!local_player)
						return;

					Vector viewangles;
					Interfaces::Engine->GetViewAngles(viewangles);
					auto delta = NormalizeYaw(viewangles.y - pEntity->GetEyeAnglesXY()->y);

					if (delta > 0)
						pEntity->GetEyeAnglesXY()->y + 180;
					else
						pEntity->GetEyeAnglesXY()->y - 180;
				}
			else {
				if (Global::Shots > 3)
				{/*
					static bool k = false;
					avg /= count;

					float add = 0;

					if (avg > 145)
						add = 180;
					else if (avg > 110)
						add = 135;
					else if (avg > 75)
						add = 90;
					else if (avg > 40)
						add = 45;
					k != k;
					pEntity->GetEyeAnglesXY()->y += (k) ? add : -add;*/
				}

				else if (IsMovingOnGround(pEntity))
				{
					if (IsFakeWalking(pEntity) && Menu::Window.RageBotTab.AccuracyResolver2.GetState())
					{
						HitBoxesToScan.clear();
						HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
						HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
						HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
						HitBoxesToScan.push_back((int)CSGOHitboxID::LowerChest);
						HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
					}
					pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw();
				}
				else if (IsMovingOnInAir(pEntity))
				{
					switch (Global::Shots % 4)//logging hits for everyhitgroup//not anymore
					{
					case 1:
						pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 45;
						break;
					case 2:
						pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 45;
						break;
					case 3:
						pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 90;
						break;
					case 4:
						pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 90;
						break;
						//continue;
					}
				}
				else//NOT MOVING
				{
					//NormalizeYaw(pEntity->GetEyeAnglesXY()->y);
					/*if (missedLogHits[pEntity->GetIndex()]>= 1)
					{
						switch (missedLogHits[pEntity->GetIndex()]-1)
						{
						case 0:
							pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw();
							break;
						case 1:
							pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 180;
							break;
						case 2:
							pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() + 90;
							break;
						case 3:
							pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw() - 90;
							break;
						}
					}*/
					if (Lbywithin35(pEntity))//if lby and eye angles subtracted < 35 then we assume that theyre using an lby breaker
					{
						if (Backtracking->arr_infos.at(pEntity->GetIndex()).TickRecords.size() < 3)
							return;
						for (auto& r : record.TickRecords) {
							if (r.m_flLowerBodyYawTarget == prevlby)
								continue;

							prevlby = r.m_flLowerBodyYawTarget;

							float diff = r.m_flLowerBodyYawTarget - prevlby;

							avg += static_cast<int>(diff);
							count++;
						}
						avg /= count;
						pEntity->GetEyeAnglesXY()->y += avg;
					}
					else if (HasFakeHead(pEntity))//lby and eye angles arent synced eZ resolve
					{
						pEntity->GetEyeAnglesXY()->y = pEntity->GetEyeAnglesXY()->y - pEntity->GetLowerBodyYaw();
					}
					else {
						pEntity->GetEyeAnglesXY()->y = pEntity->GetEyeAnglesXY()->y - pEntity->GetLowerBodyYaw() + RandomFloat(30, -30);

					}
				}
				//}
				float yClamped = pEntity->GetEyeAnglesXY()->y;
				while (yClamped < -180.0f)
					yClamped += 360.0f;

				while (yClamped > 180.0f)
					yClamped -= 360.0f;
				pEntity->GetEyeAnglesXY()->y = yClamped;
			}

				break;
		case 2:
//NO MORE GAY AYYWARE RESOLVER 
			//P ASS AIMWARE TEST RESOLVER this is the first one....;
			/*float v18 = pEntity->GetEyeAnglesXY()->y + 180.0;
			float v19 = pEntity->GetEyeAnglesXY()->y + 180.0;
			double v20;
			if (v19 <= 180.0)
			{
				if (v18 < -180.0)
					v19 = v18 + 360.0;
			}
			else
			{
				v19 = v18 - 360.0;
			}
			v20 = RandomFloat(-90.0, 90.0) + v19;*/

			/*Second one...*/
			float v22;//lets assume v20 = viewangles or some shit
			v22 = pEntity->GetEyeAnglesXY()->y + 180.0;
			float v23; float v24;
			double v20;
			if(IsMovingOnGround(pEntity))
				pEntity->GetEyeAnglesXY()->y = pEntity->GetLowerBodyYaw();
			else {
				if ((pEntity->GetEyeAnglesXY()->y + 180.0) <= 180.0)
				{
					if (v22 < -180.0)
						v22 = v22 + 360.0;
				}
				else
				{
					v22 = v22 - 360.0;
				}
				v23 = v22 - pEntity->GetLowerBodyYaw();
				if (v23 <= 180.0)
				{
					if (v23 < -180.0)
						v23 = v23 + 360.0;
				}
				else
				{
					v23 = v23 - 360.0;
				}
				if (v23 >= 0.0)
					v24 = RandomFloat(0.0, v23/2);
				else
					v24 = RandomFloat(v23/2, 0.0);
				v20 = v24 + pEntity->GetEyeAnglesXY()->y;
				pEntity->GetEyeAnglesXY()->y = v20;
			}
			break;
			}
	LowerBodyYawFix(pEntity);
	PitchCorrection();
}

void ResolverSetup::StoreFGE(IClientEntity* pEntity)
{
	ResolverSetup::storedanglesFGE = pEntity->GetEyeAnglesXY()->y;
	ResolverSetup::storedlbyFGE = pEntity->GetLowerBodyYaw();
	ResolverSetup::storedsimtimeFGE = pEntity->GetSimulationTime();
}

void ResolverSetup::StoreThings(IClientEntity* pEntity)
{
	ResolverSetup::StoredAngles[pEntity->GetIndex()] = *pEntity->GetEyeAnglesXY();
	ResolverSetup::storedlby[pEntity->GetIndex()] = pEntity->GetLowerBodyYaw();
	ResolverSetup::storedsimtime = pEntity->GetSimulationTime();
	ResolverSetup::storeddelta[pEntity->GetIndex()] = pEntity->GetEyeAnglesXY()->y;
	ResolverSetup::storedlbydelta [pEntity->GetIndex()] = pEntity->GetLowerBodyYaw();
}

void ResolverSetup::CM(IClientEntity* pEntity)
{

	for (int x = 1; x <= Interfaces::Engine->GetMaxClients(); x++)
	{

		pEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(x);

		if (!pEntity
			|| pEntity == hackManager.pLocal()
			|| pEntity->IsDormant()
			|| !pEntity->IsAlive())
			continue;
		ResolverSetup::StoreThings(pEntity);
	}
}

void ResolverSetup::FSN(IClientEntity* pEntity, ClientFrameStage_t stage)
{
	if (stage == ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		for (int i = 1; i <= Interfaces::Engine->GetMaxClients(); i++)
		{

			pEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(i);

			if (!pEntity
				|| pEntity == hackManager.pLocal()
				|| pEntity->IsDormant()
				|| !pEntity->IsAlive())
				continue;

			ResolverSetup::Resolve(pEntity);
			auto record = &Backtracking->arr_infos.at(pEntity->GetIndex());

			record->Entity = pEntity;

			if (record->TickRecords.size() > 1 && record->TickRecords.front().tickcount == Interfaces::Globals->tickcount)
				return;

			record->TickRecords.push_front(CTickRecord(pEntity));

			while ((int)record->TickRecords.size() > /*Config->Ragebot.ResolveTicks*/ 11)
				record->TickRecords.pop_back();
		}

	}
}