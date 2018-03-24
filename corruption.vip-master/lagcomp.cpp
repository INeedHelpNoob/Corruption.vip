#include "Hacks.h"
#include "LagComp.h"
//xxHarcs' backtrack
DWORD invalKostCashe = Utilities::Memory::FindPattern("client.dll", (PBYTE)"\x80\x3D\x00\x00\x00\x00\x00\x74\x16\xA1\x00\x00\x00\x00\x48\xC7\x81", "xx?????xxx????xxx");
#define TICK_INTERVAL			( Interfaces::Globals->interval_per_tick )
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
#define TICKS_TO_TIME(t) (Interfaces::Globals->interval_per_tick * (t) )
LagCompensation::LagCompensation()
{

}

void LagCompensation::logEntity(IClientEntity *player)
{
	int idx = player->getIdx();
	LagRecord *m_LagRecords = this->m_LagRecord[idx];

	if (!player || !(player->GetHealth() > 0))
	{
		for (int i = 0; i < 11; i++)
		{
			m_LagRecords[i].m_fSimulationTime = 0.0f;
		}
	}

	float simTime = player->getSimulTime();

	if (!isValidTick(simTime))
		return;

	int highestRecordIdx = -1;
	float highestSimTime = 0.0f;

	for (int i = 0; i < 11; i++)
	{
		if (m_LagRecords[i].m_fSimulationTime > simTime)
			m_LagRecords[i].m_fSimulationTime = 0.0f;

		if (m_LagRecords[i].m_fSimulationTime == 0.0f)
			continue;

		if (m_LagRecords[i].m_fSimulationTime == simTime)
			return;

		if (m_LagRecords[i].m_fSimulationTime > highestSimTime)
		{
			highestRecordIdx = i;
			highestSimTime = m_LagRecords[i].m_fSimulationTime;
		}
	}
	highestRecordIdx++;
	highestRecordIdx = highestRecordIdx % 11;


	// InvalidateBoneCache
	unsigned long g_iModelBoneCounter = **(unsigned long**)(invalKostCashe + 10);
	*(unsigned int*)((DWORD)player + 0x2914) = 0xFF7FFFFF; // m_flLastBoneSetupTime = -FLT_MAX;
	*(unsigned int*)((DWORD)player + 0x2680) = (g_iModelBoneCounter - 1); // m_iMostRecentModelBoneCounter = g_iModelBoneCounter - 1;

	m_LagRecords[highestRecordIdx].m_vAbsOrigin = player->GetOrigin();
	m_LagRecords[highestRecordIdx].m_fSimulationTime = player->getSimulTime();
	m_LagRecords[highestRecordIdx].m_angEyeAngles = *(Vector*)player->GetEyeAnglesXY();
	m_LagRecords[highestRecordIdx].flags = player->getFlags();
	m_LagRecords[highestRecordIdx].m_flLowerBodyYawTarget = player->getLowBodYtarg();
	for (int i = 0; i < 24; i++)
		m_LagRecords[highestRecordIdx].m_flPoseParameter[i] = player->getPoseParams(i);
	m_LagRecords[highestRecordIdx].headSpot = player->GetBonePos(8);
}

void LagCompensation::logCurrentEnt(IClientEntity *player)
{
	int idx = player->getIdx();

	// InvalidateBoneCache
	unsigned long g_iModelBoneCounter = **(unsigned long**)(invalKostCashe + 10);
	*(unsigned int*)((DWORD)player + 0x2914) = 0xFF7FFFFF; // m_flLastBoneSetupTime = -FLT_MAX;
	*(unsigned int*)((DWORD)player + 0x2680) = (g_iModelBoneCounter - 1); // m_iMostRecentModelBoneCounter = g_iModelBoneCounter - 1;

	this->m_PrevRecords[idx].m_vAbsOrigin = player->GetOrigin();
	this->m_PrevRecords[idx].m_fSimulationTime = player->getSimulTime();
	this->m_PrevRecords[idx].m_angEyeAngles = *(Vector*)player->GetEyeAnglesXY();
	this->m_PrevRecords[idx].flags = player->getFlags();
	this->m_PrevRecords[idx].m_flLowerBodyYawTarget = player->getLowBodYtarg();

	for (int i = 0; i < 24; i++)
		this->m_PrevRecords[idx].m_flPoseParameter[i] = player->getPoseParams(i);
}

void LagCompensation::setEntity(IClientEntity *player, LagRecord record)
{
	if (!isValidTick(record.m_fSimulationTime))
		return;

	this->logCurrentEnt(player);

	// InvalidateBoneCache
	unsigned long g_iModelBoneCounter = **(unsigned long**)(invalKostCashe + 10);
	*(unsigned int*)((DWORD)player + 0x2914) = 0xFF7FFFFF; // m_flLastBoneSetupTime = -FLT_MAX;
	*(unsigned int*)((DWORD)player + 0x2680) = (g_iModelBoneCounter - 1); // m_iMostRecentModelBoneCounter = g_iModelBoneCounter - 1;

	player->setAbsOriginal(record.m_vAbsOrigin);//i seeee
	player->setAbsAechse(record.m_angEyeAngles);
	player->getFlags() = record.flags;
	player->getLowBodYtarg() = record.m_flLowerBodyYawTarget;

	for (int i = 0; i < 24; i++)
		player->getPoseParams(i) = record.m_flPoseParameter[i];

	player->updateClientSideAnimation();
}

void LagCompensation::setCurrentEnt(IClientEntity *player)
{
	int idx = player->getIdx();

	if (!isValidTick(m_PrevRecords[idx].m_fSimulationTime))
		return;

	// InvalidateBoneCache
	unsigned long g_iModelBoneCounter = **(unsigned long**)(invalKostCashe + 10);
	*(unsigned int*)((DWORD)player + 0x2914) = 0xFF7FFFFF; // m_flLastBoneSetupTime = -FLT_MAX;
	*(unsigned int*)((DWORD)player + 0x2680) = (g_iModelBoneCounter - 1); // m_iMostRecentModelBoneCounter = g_iModelBoneCounter - 1;

	player->setAbsOriginal(m_PrevRecords[idx].m_vAbsOrigin);
	player->setAbsAechse(m_PrevRecords[idx].m_angEyeAngles);
	player->getFlags() = m_PrevRecords[idx].flags;
	player->getLowBodYtarg() = m_PrevRecords[idx].m_flLowerBodyYawTarget;

	for (int i = 0; i < 24; i++)
		player->getPoseParams(i) = m_PrevRecords[idx].m_flPoseParameter[i];

	player->updateClientSideAnimation();
}

template<class T, class U>
T LagCompensation::clamp(T in, U low, U high)
{
	if (in <= low)
		return low;

	if (in >= high)
		return high;

	return in;
}

float LagCompensation::lerpTime()
{
	int ud_rate = Interfaces::CVar->FindVar("cl_updaterate")->GetInt();
	ConVar *min_ud_rate = Interfaces::CVar->FindVar("sv_minupdaterate");
	ConVar *max_ud_rate = Interfaces::CVar->FindVar("sv_maxupdaterate");

	if (min_ud_rate && max_ud_rate)
		ud_rate = max_ud_rate->GetInt();

	float ratio = Interfaces::CVar->FindVar("cl_interp_ratio")->GetFloat();

	if (ratio == 0)
		ratio = 1.0f;

	float lerp = Interfaces::CVar->FindVar("cl_interp")->GetFloat();
	ConVar *c_min_ratio = Interfaces::CVar->FindVar("sv_client_min_interp_ratio");
	ConVar *c_max_ratio = Interfaces::CVar->FindVar("sv_client_max_interp_ratio");

	if (c_min_ratio && c_max_ratio && c_min_ratio->GetFloat() != 1)
		ratio = clamp(ratio, c_min_ratio->GetFloat(), c_max_ratio->GetFloat());

	return max(lerp, (ratio / ud_rate));
}

void LagCompensation::fakeLagFix(IClientEntity *player, int historyIdx)
{
	int idx = player->getIdx();

	LagRecord *m_LagRecords = this->m_LagRecord[idx];

	LagRecord &recentLR = m_LagRecords[historyIdx];
	LagRecord prevLR;
	if (historyIdx == 0)
		prevLR = m_LagRecords[8];
	else
		prevLR = m_LagRecords[historyIdx - 1];

	if (recentLR.m_fSimulationTime == 0.0f)
		return;

	if (!isValidTick(recentLR.m_fSimulationTime))
		return;
	/*
	if ((recentLR.m_vAbsOrigin - prevLR.m_vAbsOrigin).LengthSqr() > 4096.f)
	{
		for (auto i = 0; i <= player->getChockedTicks(); i++)
		{
			// ~ deleted parts cause pub release

			//rebGameMovement->FullWalkMove(player); // resimulate
		}
		recentLR.m_bIsBreakingLagComp = true;
		//p_Console->ConsoleColorPrintf(Color::Green(), "m_bIsBreakingLagComp: true\n");

		return;
	}
	*/
	setEntity(player, recentLR);
}

void LagCompensation::initLagRecord()
{
	for (int i = 0; i <= 32; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			m_LagRecord[i][j].m_fSimulationTime = 0.0f;
		}
	}
}

int LagCompensation::fixTickcount(IClientEntity *player)
{
	int idx = player->getIdx();

	LagRecord *m_LagRecords = this->m_LagRecord[idx];

	LagRecord &recentLR = m_LagRecords[10];//slider for ticks goes where 11 is

	if (!isValidTick(recentLR.m_fSimulationTime))
		return TIME_TO_TICKS(player->getSimulTime() + lerpTime());

	int iLerpTicks = TIME_TO_TICKS(lerpTime());
	int iTargetTickCount = TIME_TO_TICKS(recentLR.m_fSimulationTime) + iLerpTicks;

	return iTargetTickCount;
}
/*
bool LagCompensation::isValidTick(int tick) {
	INetChannelInfo *nci = Interfaces::Engine->GetNetChannelInfo();
	if (!nci)
		return false; 
	float correct = 0.f; 
	correct += nci->GetLatency(FLOW_OUTGOING); 
	correct += lerpTime();
	correct = clamp(correct, 0.f, 1.f);  
	float deltaTime = correct - (Interfaces::Globals->curtime - TICKS_TO_TIME(tick));  
	return fabsf(deltaTime) < 0.2f; }*/

bool LagCompensation::isValidTick(int simTime)
{
	INetChannelInfo *nci = Interfaces::Engine->GetNetChannelInfo();

	if (!nci)
		return false;

	auto LerpTicks = TIME_TO_TICKS(lerpTime());

	int predCmdArrivTick = Interfaces::Globals->tickcount + 1 + TIME_TO_TICKS(nci->GetAvgLatency(FLOW_INCOMING) + nci->GetAvgLatency(FLOW_OUTGOING));

	float flCorrect = clamp(lerpTime() + nci->GetLatency(FLOW_OUTGOING), 0.f, 1.f) - TICKS_TO_TIME(predCmdArrivTick + LerpTicks - (TIME_TO_TICKS(simTime) + TIME_TO_TICKS(lerpTime())));

	return abs(flCorrect) < 0.2f;
}

void LagCompensation::log(ClientFrameStage_t stage)
{/*
	if (!Interfaces::Engine->IsInGame())
		return;
	IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (!pLocal || pLocal == NULL)
		return;

	if (Menu::Window.RageBotTab.FakeLagFix.GetState())//checkmark
	{
		return;
		if (stage == ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START)
		{
			for (int i = 1; i < Interfaces::Engine->GetMaxClients(); i++)
			{
				IClientEntity *player = Interfaces::EntList->GetClientEntity(i);

				if (!player || player == nullptr)
					continue;

				if (player == pLocal)
					continue;

				if (player->GetVelocity().Length2D() < 0.5 )
					continue;

				if (!player->IsAlive())
					continue;

				if (player->GetTeamNum() == pLocal->GetTeamNum())
					continue;

				this->logEntity(player);
			}
		}
	}*/
}




#include "LagComp.h"
#include "RageBot.h"
void Backtrack::Update(int tick_count)
{
	if (!Menu::Window.LegitBotTab.FakeLagFix.GetState() || !Menu::Window.RageBotTab.FakeLagFix.GetState())
		return;

	latest_tick = tick_count;
	for (int i = 0; i < 64; i++)
	{
		UpdateRecord(i);
	}
}

bool Backtrack::IsTickValid(int tick)
{
	int delta = latest_tick - tick;
	float deltaTime = delta * Interfaces::Globals->interval_per_tick;
	return (fabs(deltaTime) <= 0.2f);
}

void Backtrack::UpdateRecord(int i)
{
	IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
	if (pEntity && pEntity->IsAlive() && !pEntity->IsDormant())
	{
		float simtime = pEntity->GetSimulationTime();
		if (!IsTickValid(simtime))
			return;
		//if (pEntity->GetVelocity().Length2D() < 0.1)
			//return;
		float lby = pEntity->GetLowerBodyYaw();
		if (lby != records[i].lby)
		{
			records[i].tick_count = latest_tick;
			records[i].lby = lby;
			records[i].EyeAng = pEntity->GetEyeAnglesXY()->y;
			records[i].headPosition = pEntity->GetBonePos(0);
			records[i].lby = pEntity->getLowBodYtarg();

		}
	}
	else
	{
		records[i].tick_count = 0;
	}
}

bool Backtrack::RunLBYBacktrack(int i, CUserCmd* cmd, Vector& aimPoint)
{
	IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
	if (IsTickValid(records[i].tick_count))
	{
		aimPoint = records[i].headPosition;
		pEntity->GetEyeAnglesXY()->y = records[i].EyeAng;
		//pEntity->GetLowerBodyYawTarget() = records[i].lby;//no reason to set lby :thinking:
		cmd->tick_count = records[i].tick_count;
		return true;
	}
	return false;
}
/*
void Backtrack::rageBacktrack(CInput::CUserCmd* cmd, IClientEntity* pLocal)
{
	if (g_Options.Ragebot.FakeLagFix)
	{
		int bestTargetIndex = -1;
		float bestFov = FLT_MAX;
		player_info_t info;
		if (!pLocal->IsAlive())
			return;

		for (int i = 0; i < Interfaces::Engine->GetMaxClients(); i++)
		{
			auto entity = (IClientEntity*)Interfaces::EntList->GetClientEntity(i);

			if (!entity || !pLocal)
				continue;

			if (entity == pLocal)
				continue;

			if (!Interfaces::Engine->GetPlayerInfo(i, &info))
				continue;

			if (entity->IsDormant())
				continue;

			if (entity->GetTeamNum() == pLocal->GetTeamNum())
				continue;

			if (entity->IsAlive())
			{

				float simtime = entity->GetSimulationTime();
				Vector hitboxPos = AimPoint;
				if (!IsTickValid(simtime))//i added?
					return;//same
				headPositions[i][cmd->command_number % 13] = backtrackData{ simtime, hitboxPos };
				Vector ViewDir = angle_vector(cmd->viewangles + (pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f));
				float FOVDistance = distance_point_to_line(hitboxPos, pLocal->GetEyePosition(), ViewDir);

				if (bestFov > FOVDistance)
				{
					bestFov = FOVDistance;
					bestTargetIndex = i;
				}
			}
		}

		float bestTargetSimTime;
		if (bestTargetIndex != -1)
		{
			float tempFloat = FLT_MAX;
			Vector ViewDir = angle_vector(cmd->viewangles + (pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f));
			for (int t = 0; t < 12; ++t)
			{
				float tempFOVDistance = distance_point_to_line(headPositions[bestTargetIndex][t].hitboxPos, pLocal->GetEyePosition(), ViewDir);
				if (tempFloat > tempFOVDistance && headPositions[bestTargetIndex][t].simtime > pLocal->GetSimulationTime() - 1)
				{
					tempFloat = tempFOVDistance;
					bestTargetSimTime = headPositions[bestTargetIndex][t].simtime;
				}
			}
			if (cmd->buttons & IN_ATTACK)
			{
				cmd->tick_count = TIME_TO_TICKS(bestTargetSimTime);
			}
		}
	}
}
*/


void Backtrack::legitBacktrack(CUserCmd* cmd, IClientEntity* pLocal)
{
	if (Menu::Window.LegitBotTab.FakeLagFix.GetState() || Menu::Window.RageBotTab.FakeLagFix.GetState())
	{
		int bestTargetIndex = -1;
		float bestFov = FLT_MAX;
		player_info_t info;
		if (!pLocal->IsAlive())
			return;

		for (int i = 0; i < Interfaces::Engine->GetMaxClients(); i++)
		{
			auto entity = (IClientEntity*)Interfaces::EntList->GetClientEntity(i);

			if (!entity || !pLocal)
				continue;

			if (entity == pLocal)
				continue;

			if (!Interfaces::Engine->GetPlayerInfo(i, &info))
				continue;
			
			//if (entity->GetVelocity().Length2D() < 0.5)
				//continue;

			if (entity->IsDormant())
				continue;

			if (entity->GetTeamNum() == pLocal->GetTeamNum())
				continue;
			if (!IsTickValid(records[i].tick_count))
				continue;

			if (entity->IsAlive())
			{

				float simtime = entity->GetSimulationTime();
				Vector hitboxPos = GetHitboxPosition(entity, 0);
				Vector hitboxPoschest = GetHitboxPosition(entity, 4);
				Vector hitboxPospelvis = GetHitboxPosition(entity, 7);

				headPositions[i][cmd->command_number % 13] = backtrackData{ simtime, hitboxPos };
				chestPositions[i][cmd->command_number % 13] = backtrackData{ simtime, hitboxPoschest };
				pelvisPositions[i][cmd->command_number % 13] = backtrackData{ simtime, hitboxPospelvis };

				float FOVDistance;
				if (Menu::Window.LegitBotTab.FakeLagFix.GetState())
				{
					Vector ViewDir = angle_vector(cmd->viewangles + (pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f));
					FOVDistance = distance_point_to_line(hitboxPos, pLocal->GetEyePosition(), ViewDir);
				}
				else if(Menu::Window.RageBotTab.FakeLagFix.GetState())
				{
					FOVDistance = distance_point_to_line(hitboxPos, pLocal->GetEyePosition(), pLocal->GetOrigin());
					Vector ViewDir = angle_vector(cmd->viewangles + (pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f));
					//FOVDistance = distance_point_to_line(hitboxPos, pLocal->GetEyePosition(), ViewDir);
				}
				if (bestFov > FOVDistance)
				{
					bestFov = FOVDistance;
					bestTargetIndex = i;
				}
			}
		}

		float bestTargetSimTime;
		if (bestTargetIndex != -1)
		{
			float tempFloat = FLT_MAX;
			Vector ViewDir = angle_vector(cmd->viewangles + (pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f));
			for (int t = 0; t < 12; ++t)
			{
				float tempFOVDistance;
				if (Menu::Window.LegitBotTab.FakeLagFix.GetState())
				{
					tempFOVDistance = distance_point_to_line(headPositions[bestTargetIndex][t].hitboxPos, pLocal->GetEyePosition(), ViewDir);
				}
				else if (Menu::Window.RageBotTab.FakeLagFix.GetState())
				{
					tempFOVDistance = distance_point_to_line(headPositions[bestTargetIndex][t].hitboxPos, pLocal->GetEyePosition(), pLocal->GetOrigin());
					//tempFOVDistance = distance_point_to_line(headPositions[bestTargetIndex][t].hitboxPos, pLocal->GetEyePosition(), ViewDir);
				}
				if (tempFloat > tempFOVDistance  && headPositions[bestTargetIndex][t].simtime > pLocal->GetSimulationTime() - 1)
				{
					tempFloat = tempFOVDistance;
					bestTargetSimTime = headPositions[bestTargetIndex][t].simtime;
				}
			}
			if (cmd->buttons & IN_ATTACK)
			{
				cmd->tick_count = TIME_TO_TICKS(bestTargetSimTime);
			}
		}
	}
}

Backtrack* backtracking = new Backtrack();
backtrackData headPositions[64][12];
backtrackData chestPositions[64][12];
backtrackData pelvisPositions[64][12];





bool CBacktracking::bTickIsValid(int tick) {
	static auto cl_interp_ratio = Interfaces::CVar->FindVar("cl_interp_ratio");
	static auto cl_updaterate = Interfaces::CVar->FindVar("cl_updaterate");

	float m_flLerpTime = cl_interp_ratio->GetFloat() / cl_updaterate->GetFloat();
	if (Menu::Window.RageBotTab.FakeLagFix.GetState())
		tick += TIME_TO_TICKS(m_flLerpTime);

	return (fabsf(TICKS_TO_TIME(Interfaces::Globals->tickcount) - TICKS_TO_TIME(tick)) <= 0.2f);
	
}

void CBacktracking::SaveTemporaryRecord(IClientEntity* ent, CTickRecord record) {
	if (!record.m_flSimulationTime)
		record = CTickRecord(ent);
	arr_infos.at(ent->GetIndex()).temporaryRecord = record;
}

void CBacktracking::RestoreTemporaryRecord(IClientEntity* ent) {
	ApplyTickRecordCM(ent, arr_infos.at(ent->GetIndex()).temporaryRecord);
}

void CBacktracking::ApplyTickrecord(IClientEntity* ent, CTickRecord record) {
	ApplyTickRecordFSN(ent, record);
	ApplyTickRecordCM(ent, record);
}

void CBacktracking::ApplyTickRecordFSN(IClientEntity* pEntity, CTickRecord record) {
	*(QAngle*)((uintptr_t)pEntity + 0x0000B23C) = record.m_angEyeAngles;
	*(float*)((uintptr_t)pEntity + 0x00000264) = record.m_flSimulationTime;
	*(Vector*)((uintptr_t)pEntity + 0x00000134) = record.m_vecOrigin;
	*(Vector*)((uintptr_t)pEntity + 0x00000110) = record.m_vecVelocity;
	*(float*)((uintptr_t)pEntity + 0x00002F9C) = record.m_flDuckAmount;
	*(float*)((uintptr_t)pEntity + 0x00002FA0) = record.m_flDuckSpeed;
	//pEntity->SetPoseParameters(record.m_flPoseParameter);

	if (pEntity->GetAbsOrigin() == Vector(0, 0, 0) || (record.m_vecOrigin - record.m_vecAbsOrigin).LengthSqr() > 100)
		pEntity->setAbsOriginal(record.m_vecOrigin);
	if (pEntity->GetAbsAngles() != pEntity->GetEyeAngles())
		pEntity->setAbsAechse(record.m_angEyeAngles);
}
float GetLerpTime()  {
	static ConVar* cl_interp = Interfaces::CVar->FindVar("cl_interp");
	static ConVar* cl_updaterate = Interfaces::CVar->FindVar("cl_updaterate");
	static ConVar* cl_interp_ratio = Interfaces::CVar->FindVar("cl_interp_ratio");
	static ConVar* sv_maxupdaterate = Interfaces::CVar->FindVar("sv_maxupdaterate");
	static ConVar* sv_minupdaterate = Interfaces::CVar->FindVar("sv_minupdaterate");
	static ConVar* sv_client_min_interp_ratio = Interfaces::CVar->FindVar("sv_client_min_interp_ratio");
	static ConVar* sv_client_max_interp_ratio = Interfaces::CVar->FindVar("sv_client_max_interp_ratio");

	auto Interp = cl_interp->GetFloat();
	auto UpdateRate = cl_updaterate->GetFloat();
	auto InterpRatio = static_cast<float>(cl_interp_ratio->GetInt());
	auto MaxUpdateRate = static_cast<float>(sv_maxupdaterate->GetInt());
	auto MinUpdateRate = static_cast<float>(sv_minupdaterate->GetInt());
	auto ClientMinInterpRatio = sv_client_min_interp_ratio->GetFloat();
	auto ClientMaxInterpRatio = sv_client_max_interp_ratio->GetFloat();

	if (ClientMinInterpRatio > InterpRatio)
		InterpRatio = ClientMinInterpRatio;

	if (InterpRatio > ClientMaxInterpRatio)
		InterpRatio = ClientMaxInterpRatio;

	if (MaxUpdateRate <= UpdateRate)
		UpdateRate = MaxUpdateRate;

	if (MinUpdateRate > UpdateRate)
		UpdateRate = MinUpdateRate;

	auto v20 = InterpRatio / UpdateRate;
	if (v20 <= Interp)
		return Interp;

	else
		return v20;
}
void CBacktracking::ApplyTickRecordCM(IClientEntity* pEntity, CTickRecord record) {
	pEntity->setAbsAechse(record.m_angEyeAngles);
	pEntity->setAbsOriginal(record.m_vecOrigin);
	if (pEntity->GetAbsOrigin() == Vector(0, 0, 0) || (record.m_vecOrigin - record.m_vecAbsOrigin).LengthSqr() > 100)
		pEntity->setAbsOriginal(record.m_vecOrigin);
	if (pEntity->GetAbsAngles() != pEntity->GetEyeAngles())
		pEntity->setAbsAechse(record.m_angEyeAngles);
	*(Vector*)((uintptr_t)pEntity + 0x00000134) = record.m_vecOrigin;
	//pEntity->SetPoseParameters(record.m_flPoseParameter);
}

int CBacktracking::ShotBackTrackedTick(IClientEntity* ent) {
	if (!ent)
		return 0;

	C_PlayerStored* pCPlayer = &arr_infos.at(ent->GetIndex()).player;
	if (bTickIsValid(pCPlayer->TickCount))
		return pCPlayer->TickCount;

	return 0;
}

void CBacktracking::ShotBackTrackStoreFSN(IClientEntity* ent) {
	if (!ent)
		return;

	static std::array<float, 33> lastsimulationtimes;

	if (lastsimulationtimes[ent->GetIndex()] == ent->GetSimulationTime())
		return;

	lastsimulationtimes[ent->GetIndex()] = ent->GetSimulationTime();

	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
	bool PlayerShot = false;
	C_PlayerStored* pCPlayer = &arr_infos[ent->GetIndex()].player;
	if (ent->GetEyeAngles() != pCPlayer->EyeAngles)
		pCPlayer->EyeAnglesUpdated = true;

	if (!pWeapon)
		return;

	if (pCPlayer->CurrentWeapon == pWeapon && pWeapon->IsGun()) {
		//don't count weapon changes
		if (pWeapon->GetAmmoInClip() == pCPlayer->BulletsLeft - 1) {
			if (pCPlayer->EyeAnglesUpdated) {
				//Don't count reloading
				pCPlayer->IsFiring = true;
				PlayerShot = true;
			}
		}
		else
			pCPlayer->IsFiring = false;
	}
	else {
		pCPlayer->IsFiring = false;
	}

	if (PlayerShot) {
		if (Menu::Window.RageBotTab.FakeLagFix.GetState())
			pCPlayer->TickCount = TIME_TO_TICKS(ent->GetSimulationTime() - Interfaces::Globals->interval_per_tick) + TIME_TO_TICKS(GetLerpTime()) + 1;
		else
			pCPlayer->TickCount = Interfaces::Globals->tickcount;
		pCPlayer->ValidTick = CTickRecord(ent);
		pCPlayer->ValidTick.tickcount = pCPlayer->TickCount;
		pCPlayer->ValidTick.m_angEyeAngles = ent->GetEyeAngles();			//CURRENT angles yaw and pitch
																			//pCPlayer->ValidTick.m_flPoseParameter = std::array<float, 24>();
																			//if (ent->GetSimulationTime() - pCPlayer->SimulationTime > Interfaces::Globals->interval_per_tick) //if psilent or fake lag																//pCPlayer->ValidTick.m_angEyeAngles.x = pCPlayer->EyeAngles.x; //LAST angles pitch!
		arr_infos[ent->GetIndex()].shot = true;
	}

	pCPlayer->BulletsLeft = pWeapon->GetAmmoInClip();
	pCPlayer->EyeAngles = ent->GetEyeAngles();
	pCPlayer->CurrentWeapon = pWeapon;

	if (!bTickIsValid(pCPlayer->TickCount)) {
		pCPlayer->TickCount = 0;
		arr_infos[ent->GetIndex()].shot = false;
	}
}
/*
void CBacktracking::OverridePosesFsnRenderStart(IClientEntity* ent) {
	if (!ent)
		return;
	C_PlayerStored* pCPlayer = &arr_infos.at(ent->GetIndex()).player;
	pCPlayer->ValidTick.m_flPoseParameter = ent->GetPoseParameters();
}*/

void CBacktracking::ShotBackTrackBeforeAimbot(IClientEntity* ent) {
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
	if (!ent || !hackManager.pLocal() || !hackManager.pLocal()->GetActiveWeaponHandle() || !Interfaces::Globals)
		return;
	bool can_shoot = (pWeapon->GetNextPrimaryAttack() <= (hackManager.pLocal()->GetTickBase() * Interfaces::Globals->interval_per_tick));
	C_PlayerStored* pCPlayer = &arr_infos.at(ent->GetIndex()).player;
	if (Interfaces::Globals)
		Interfaces::Globals->tickcount = Interfaces::Globals->tickcount;
	if (can_shoot && bTickIsValid(pCPlayer->TickCount)) {
		ApplyTickRecordFSN(ent, pCPlayer->ValidTick);
	}
}

void CBacktracking::ShotBackTrackAimbotStart(IClientEntity* ent) {
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());

	if (!ent || !hackManager.pLocal() || !hackManager.pLocal()->GetActiveWeaponHandle() || !Interfaces::Globals)
		return;
	bool can_shoot = (pWeapon->GetNextPrimaryAttack() <= (hackManager.pLocal()->GetTickBase() * Interfaces::Globals->interval_per_tick));
	C_PlayerStored* pCPlayer = &arr_infos.at(ent->GetIndex()).player;
	if (Interfaces::Globals)
		Interfaces::Globals->tickcount = Interfaces::Globals->tickcount;
	if (can_shoot && bTickIsValid(pCPlayer->TickCount)) {
		SaveTemporaryRecord(ent);
		ApplyTickRecordCM(ent, pCPlayer->ValidTick);
	}
}

void CBacktracking::ShotBackTrackAimbotEnd(IClientEntity* ent) {
	return;
	if (!ent)
		return;
	C_PlayerStored* pCPlayer = &arr_infos.at(ent->GetIndex()).player;
	if (!bTickIsValid(pCPlayer->TickCount))
		return;

	RestoreTemporaryRecord(ent);
}

void CBacktracking::ClearRecord(IClientEntity* Entity) {
	arr_infos[Entity->GetIndex()] = CBacktrackInfo();
}
