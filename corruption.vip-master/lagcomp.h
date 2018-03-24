#pragma once

#include <vector>
#include "Hacks.h"
#include <Windows.h>
#include <Psapi.h>
#include <iostream>
#include <algorithm>
#include <functional>
#include <numeric>
#include <string>
#include <vector>
#include <time.h>
#include <random>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <set>
#include <map>
#include <unordered_map>
#include <memory>
#include <thread>
#include <array>
#include <deque>
#include <intrin.h>
#include <atomic>
#include <mutex>

extern int historyIdx;
namespace Global
{
	extern CUserCmd* UserCmd;
	extern IClientEntity* Target;
	extern int Shots;
	extern bool change;
	extern int choked_ticks;
	extern int TargetID;
	extern bool Up2date;
	extern bool didhitHS;
	extern int missedshots;
	extern float oldSimulTime[65];
	extern std::map<int, QAngle> storedshit;
}

struct LagRecord
{
	Vector m_vAbsOrigin;
	Vector m_angEyeAngles;
	float m_fSimulationTime;
	float m_flPoseParameter[24];
	float m_flLowerBodyYawTarget;
	unsigned char flags;
	bool m_bIsBreakingLagComp;
	Vector headSpot;

	LagRecord()
	{
		m_vAbsOrigin.clearVekt();
		m_angEyeAngles.clearVekt();
		m_fSimulationTime = -1.f;
		m_flLowerBodyYawTarget = 0.f;
		flags = -1;
		m_bIsBreakingLagComp = false;
		headSpot.clearVekt();
	}
};

class LagCompensation
{
public:

	LagRecord m_LagRecord[64][11] = {};
	LagRecord m_PrevRecords[64] = {};

public:

	LagCompensation();

	void logEntity(IClientEntity *player);
	void logCurrentEnt(IClientEntity *player);
	void setEntity(IClientEntity *player, LagRecord record);
	void setCurrentEnt(IClientEntity *player);
	void fakeLagFix(IClientEntity *player, int historyIdx);
	void initLagRecord();
	int fixTickcount(IClientEntity *player);
	bool isValidTick(int simTime);
	void log(ClientFrameStage_t stage);
	bool lbyShallUpdate(IClientEntity *player);

private:

	template<class T, class U>
	T clamp(T in, U low, U high);
	float lerpTime();
};

extern LagCompensation *lagComp;

struct lbyRecords
{
	int tick_count;
	float lby;
	Vector headPosition;
	Vector pelvisPosition;
	Vector chestPosition;
	float EyeAng;
};

struct backtrackData
{
	float simtime;
	Vector hitboxPos;
};

class Backtrack
{
	int latest_tick;
	bool IsTickValid(int tick);
	void UpdateRecord(int i);
public:
	lbyRecords records[64];
	bool RunLBYBacktrack(int i, CUserCmd* cmd, Vector& aimPoint);
	void Update(int tick_count);
	void legitBacktrack(CUserCmd* cmd, IClientEntity* pLocal);
	void rageBacktrack(CUserCmd* cmd, IClientEntity* pLocal);
};
extern 	Vector AimPoint;
extern backtrackData headPositions[64][12];
extern backtrackData chestPositions[64][12];
extern backtrackData pelvisPositions[64][12];
extern Backtrack* backtracking;


struct CTickRecord {
	CTickRecord() {}
	CTickRecord(IClientEntity* ent) {
		m_flLowerBodyYawTarget = ent->GetLowerBodyYaw();
		m_angEyeAngles = ent->GetEyeAngles();
		m_flCycle = ent->GetCycle();
		m_flSimulationTime = ent->GetSimulationTime();
		m_nSequence = ent->GetSequence();
		m_vecOrigin = ent->GetOrigin();
		m_vecVelocity = ent->GetVelocity();
		//m_flPoseParameter = ent->getPoseParams();
		m_angAbsAngles = ent->GetAbsAngles();
		m_vecAbsOrigin = ent->GetAbsOrigin();
		m_flDuckSpeed = ent->GetDuckSpeed();
		m_flDuckAmount = ent->GetDuckAmount();
	}

	explicit operator bool() const noexcept {
		return m_flSimulationTime > 0.f;
	}

	bool operator>(const CTickRecord& others) {
		return (m_flSimulationTime > others.m_flSimulationTime);
	}
	bool operator>=(const CTickRecord& others) {
		return (m_flSimulationTime >= others.m_flSimulationTime);
	}
	bool operator<(const CTickRecord& others) {
		return (m_flSimulationTime < others.m_flSimulationTime);
	}
	bool operator<=(const CTickRecord& others) {
		return (m_flSimulationTime <= others.m_flSimulationTime);
	}
	bool operator==(const CTickRecord& others) {
		return (m_flSimulationTime == others.m_flSimulationTime);
	}

	float m_flDuckAmount = 0.f;
	float m_flDuckSpeed = 0.f;
	float m_flLowerBodyYawTarget = 0.f;
	QAngle m_angEyeAngles = QAngle(0, 0, 0);
	float m_flCycle = 0.f;
	float m_flSimulationTime = 0.f;
	int m_nSequence = 0;
	Vector m_vecOrigin = Vector(0, 0, 0);
	Vector m_vecAbsOrigin = Vector(0, 0, 0);
	Vector m_vecVelocity = Vector(0, 0, 0);
	//std::array<float, 24> m_flPoseParameter = {};
	QAngle m_angAbsAngles = QAngle(0, 0, 0);
	int tickcount = 0;
};

struct C_PlayerStored {
	CBaseCombatWeapon* CurrentWeapon = nullptr;
	int BulletsLeft = 0;
	bool EyeAnglesUpdated = false;
	bool IsFiring = false;
	int TickCount = 0;
	float SimulationTime = 0;
	QAngle EyeAngles = QAngle(0, 0, 0);
	CTickRecord ValidTick = CTickRecord();
};

struct CBacktrackInfo {
	CBacktrackInfo() : shot(false), player(C_PlayerStored()), tick_count(0), temporaryRecord(CTickRecord()) {}

	bool shot = false;
	C_PlayerStored player = C_PlayerStored();
	int tick_count = 0;
	CTickRecord temporaryRecord = CTickRecord();


	IClientEntity* Entity;
	std::deque<CTickRecord> TickRecords;
};

#define Backtracking CBacktracking::GetInstance()

class CBacktracking {
	friend class CLagcompensation;
	friend class CResolver;

	IMPLEMENT_SINGLETON(CBacktracking);

public:
	int ShotBackTrackedTick(IClientEntity* ent);
	void ShotBackTrackStoreFSN(IClientEntity* ent);
	void OverridePosesFsnRenderStart(IClientEntity* ent);
	void ShotBackTrackBeforeAimbot(IClientEntity* ent);
	void ShotBackTrackAimbotStart(IClientEntity* ent);
	void ShotBackTrackAimbotEnd(IClientEntity* ent);
	void ClearRecord(IClientEntity* Entity);

public:
	bool bTickIsValid(int tick);
	void SaveTemporaryRecord(IClientEntity* ent, CTickRecord record = CTickRecord());
	void RestoreTemporaryRecord(IClientEntity* ent);
	std::array<CBacktrackInfo, 32> arr_infos;


private:
	void ApplyTickrecord(IClientEntity* ent, CTickRecord record);
	void ApplyTickRecordFSN(IClientEntity* pEntity, CTickRecord record);
	void ApplyTickRecordCM(IClientEntity* pEntity, CTickRecord record);

private:
};
