
#pragma once

#include "Hacks.h"

Vector GetAutostrafeView();

class CMiscHacks : public CHack
{
public:
	void Init();
	void Draw();
	void Move(CUserCmd *pCmd, bool &bSendPacket);
	void RotateMovement(CUserCmd * pCmd, float rotation);
private:
	void AutoJump(CUserCmd *pCmd);
	//void AutoJumpSMAC(CUserCmd *pCmd);
	void PostProcces();

	//void EdgeJump(CUserCmd *pCmd);
	void SlowMo(CUserCmd * pCmd);
	void FakeWalk(CUserCmd* pCmd, bool &bSendPacket);
	//void Walkbot(CUserCmd *pCmd);
	//void PerfectStrafe(CUserCmd *pCmd);
	void doCircularStrafe(CUserCmd * pCmd, Vector & Originalview);
	void LegitStrafe(CUserCmd *pCmd);
	void RageStrafe(CUserCmd *pCmd);
	void ChatSpamInterwebz();
	void ChatSpamName();
	void ChatSpamDisperseName();
	void ChatSpamRegular();
	//void ChatSpamReports();
	//void ChatSpamCallout();
	//void ChatSpamAchievement();
	//void NameDispersion();
	//void ChatSpamAchievement();

	//std::string GetNameChange();

	/*void ChatSpam();
	void Namechanger();

	std::string GetRegularSpam();
	std::string GetCustomSpam();
	std::string GetPlayerSpam();
	std::string GetSinglePlayerSpam();

	std::string GetNameChange();
	std::string GetNameCalloutChange();*/
};



