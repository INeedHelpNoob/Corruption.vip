
#pragma once

// Includes
#include "CommonIncludes.h"
#include "SDK.h"


class IMoveHelper : public Interface
{
public:

	bool m_bFirstRunOfFunctions : 1;
	bool m_bGameCodeMovedPlayer : 1;
	int m_nPlayerHandle; // edict index on server, client entity handle on client=
	int m_nImpulseCommand; // Impulse command issued.
	Vector m_vecViewAngles; // Command view angles (local space)
	Vector m_vecAbsViewAngles; // Command view angles (world space)
	int m_nButtons; // Attack buttons.
	int m_nOldButtons; // From host_client->oldbuttons;
	float m_flForwardMove;
	float m_flSideMove;
	float m_flUpMove;
	float m_flMaxSpeed;
	float m_flClientMaxSpeed;
	Vector m_vecVelocity; // edict::velocity // Current movement direction.
	Vector m_vecAngles; // edict::angles
	Vector m_vecOldAngles;
	float m_outStepHeight; // how much you climbed this move
	Vector m_outWishVel; // This is where you tried 
	Vector m_outJumpVel; // This is your jump velocity
	Vector m_vecConstraintCenter;
	float m_flConstraintRadius;
	float m_flConstraintWidth;
	float m_flConstraintSpeedFactor;
	float m_flUnknown[5];
	Vector m_vecAbsOrigin;

	void SetHost(IClientEntity* pEnt)
	{
		typedef void(__thiscall* SetHostFn)(void*, IClientEntity*);
		return VFunc<SetHostFn>(1)(this, pEnt);
	}

};


class IPrediction : public Interface
{
public:
	void    SetupMove(IClientEntity *player, CUserCmd *pCmd, IMoveHelper *pHelper, void *move)
	{
		typedef void(__thiscall* SetupMoveFn)(void*, IClientEntity*, CUserCmd*, IMoveHelper*, void*);
		return VFunc<SetupMoveFn>(20)(this, player, pCmd, pHelper, move);
	}

	void    FinishMove(IClientEntity *player, CUserCmd *pCmd, void *move)
	{
		typedef void(__thiscall* FinishMoveFn)(void*, IClientEntity*, CUserCmd*, void*);
		return VFunc<FinishMoveFn>(21)(this, player, pCmd, move);
	}
};


class IGameMovement : public Interface
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


// Namespace to contain all the valve interfaces
namespace Interfaces
{
	// Gets handles to all the interfaces needed
	void Initialise();
	extern IBaseClientDLL* Client;
	extern IVEngineClient* Engine;
	extern IPanel* Panels;
	extern IClientEntityList* EntList;
	extern ISurface* Surface;
	extern IVDebugOverlay* DebugOverlay;
	extern IClientModeShared* ClientMode;
	extern IVEffects* g_Dlight;

	extern CGlobalVarsBase *Globals;
	extern DWORD *Prediction;
	extern CMaterialSystem* MaterialSystem;
	extern CVRenderView* RenderView;
	extern IVModelRender* ModelRender;
	extern CModelInfo* ModelInfo;
	extern IEngineTrace* Trace;
	extern IGameEventManager2 *EventManager;
	extern IPhysicsSurfaceProps* PhysProps;
	extern ICVar *CVar;
	extern CInput* pInput;
	extern IInputSystem* InputSystem;
	extern IMDLCache* ModelCache;
	extern IMoveHelper *MoveHelper;
	extern IPrediction *Prediction1;
	extern IGameMovement *GameMovement;
};