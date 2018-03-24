#pragma once
#pragma once

#include "SDK.h"
#include "IMoveHelper.h"

class CMoveData
{
public:
	bool    m_bFirstRunOfFunctions : 1;
	bool    m_bGameCodeMovedPlayer : 1;
	int     m_nPlayerHandle;        // edict index on server, client entity handle on client=
	int     m_nImpulseCommand;      // Impulse command issued.
	Vector  m_vecViewAngles;        // Command view angles (local space)
	Vector  m_vecAbsViewAngles;     // Command view angles (world space)
	int     m_nButtons;             // Attack buttons.
	int     m_nOldButtons;          // From host_client->oldbuttons;
	float   m_flForwardMove;
	float   m_flSideMove;
	float   m_flUpMove;
	float   m_flMaxSpeed;
	float   m_flClientMaxSpeed;
	Vector  m_vecVelocity;          // edict::velocity        // Current movement direction.
	Vector  m_vecAngles;            // edict::angles
	Vector  m_vecOldAngles;
	float   m_outStepHeight;        // how much you climbed this move
	Vector  m_outWishVel;           // This is where you tried 
	Vector  m_outJumpVel;           // This is your jump velocity
	Vector  m_vecConstraintCenter;
	float   m_flConstraintRadius;
	float   m_flConstraintWidth;
	float   m_flConstraintSpeedFactor;
	float   m_flUnknown[5];
	Vector  m_vecAbsOrigin;
};

class IClientEntity;

class IGameMovement
{
public:
	virtual			~IGameMovement(void) {}

	virtual void	          ProcessMovement(IClientEntity *pPlayer, CMoveData *pMove) = 0;
	virtual void	          Reset(void) = 0;
	virtual void	          StartTrackPredictionErrors(IClientEntity *pPlayer) = 0;
	virtual void	          FinishTrackPredictionErrors(IClientEntity *pPlayer) = 0;
	virtual void	          DiffPrint(char const *fmt, ...) = 0;
	virtual Vector const&	  GetPlayerMins(bool ducked) const = 0;
	virtual Vector const&	  GetPlayerMaxs(bool ducked) const = 0;
	virtual Vector const&   GetPlayerViewOffset(bool ducked) const = 0;
	virtual bool		        IsMovingPlayerStuck(void) const = 0;
	virtual IClientEntity*   GetMovingPlayer(void) const = 0;
	virtual void		        UnblockPusher(IClientEntity *pPlayer, IClientEntity *pPusher) = 0;
	virtual void            SetupMovementBounds(CMoveData *pMove) = 0;
};

class CGameMovement
	: public Interface
{
public:
	void ProcessMovement(IClientEntity *pPlayer, void *pMove)
	{
		typedef void(__thiscall* ProcessMovementFn)(void*, IClientEntity*, void*);
		return VFunc<ProcessMovementFn>(1)(this, pPlayer, pMove);
	}

	void StartTrackPredictionErrors(IClientEntity *pPlayer)
	{
		typedef void(__thiscall *o_StartTrackPredictionErrors)(void*, void*);
		return VFunc<o_StartTrackPredictionErrors>(3)(this, pPlayer);
	}

	void FinishTrackPredictionErrors(IClientEntity *pPlayer)
	{
		typedef void(__thiscall *o_FinishTrackPredictionErrors)(void*, void*);
		return VFunc<o_FinishTrackPredictionErrors>(4)(this, pPlayer);
	}
};

class IPrediction
{
public:
	bool InPrediction()
	{
		typedef bool(__thiscall* oInPrediction)(void*);
		return call_vfunc<oInPrediction>(this, 14)(this);
	}

	void RunCommand(IClientEntity *player, CUserCmd *ucmd, IMoveHelper *moveHelper)
	{
		typedef void(__thiscall* oRunCommand)(void*, IClientEntity*, CUserCmd*, IMoveHelper*);
		return call_vfunc<oRunCommand>(this, 19)(this, player, ucmd, moveHelper);
	}

	void SetupMove(IClientEntity *player, CUserCmd *ucmd, IMoveHelper *moveHelper, void* pMoveData)
	{
		typedef void(__thiscall* oSetupMove)(void*, IClientEntity*, CUserCmd*, IMoveHelper*, void*);
		return call_vfunc<oSetupMove>(this, 20)(this, player, ucmd, moveHelper, pMoveData);
	}

	void FinishMove(IClientEntity *player, CUserCmd *ucmd, void*pMoveData)
	{
		typedef void(__thiscall* oFinishMove)(void*, IClientEntity*, CUserCmd*, void*);
		return call_vfunc<oFinishMove>(this, 21)(this, player, ucmd, pMoveData);
	}
};