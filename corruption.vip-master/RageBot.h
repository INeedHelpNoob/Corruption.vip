#pragma once

#include "Hacks.h"
extern Vector AimPoint;
extern float autowalldmgtest[65];
extern bool lbyupdate1;

class CRageBot : public CHack
{
public:
	void Init();
	void Draw();
	void Move(CUserCmd *pCmd, bool &bSendPacket);
	void DrawCircle1(IClientEntity* pEntity);

private:

	int GetTargetCrosshair();
	int GetTargetDistance();
	int GetTargetNextShot();
	int GetTargetThreat(CUserCmd * pCmd);
	int GetTargetHealth();
	bool TargetMeetsRequirements(IClientEntity* pEntity);
	void aimAtPlayer(CUserCmd * pCmd);
	float FovToPlayer(Vector ViewOffSet, Vector View, IClientEntity* pEntity, int HitBox);
	int HitScan(IClientEntity* pEntity);
	bool AimAtPoint(IClientEntity* pLocal, Vector point, CUserCmd *pCmd, bool &bSendPacket);
	void DoAimbot(CUserCmd *pCmd, bool &bSendPacket);
	void DoNoRecoil(CUserCmd *pCmd);
	void PositionAdjustment(CUserCmd* pCmd);
	void DoAntiAim(CUserCmd *pCmd, bool&bSendPacket);
	int iChokedTicks = -1;


	bool IsAimStepping;
	Vector LastAimstepAngle;
	Vector LastAngle;

	bool IsLocked;
	int TargetID;
	int HitBox;
};