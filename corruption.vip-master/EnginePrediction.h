#pragma once
#include "Hacks.h"
class MoveData;

class MoveData
{
public:

	bool m_bFirstRunOfFunctions : 1;
	bool m_bGameCodeMovedPlayer : 1;
	bool m_bNoAirControl : 1;

	void *m_nPlayerHandle;    // edict index on server, client entity handle on client

	int m_nImpulseCommand;    // Impulse command issued.
	Vector m_vecViewAngles;    // Command view angles (local space)
	Vector m_vecAbsViewAngles;    // Command view angles (world space)
	int m_nButtons;            // Attack buttons.
	int m_nOldButtons;        // From host_client->oldbuttons;
	float m_flForwardMove;
	float m_flSideMove;
	float m_flUpMove;

	float m_flMaxSpeed;
	float m_flClientMaxSpeed;

	// Variables from the player edict (sv_player) or entvars on the client.
	// These are copied in here before calling and copied out after calling.
	Vector m_vecVelocity;        // edict::velocity        // Current movement direction.
	Vector m_vecOldVelocity;
	float somefloat;
	Vector m_vecAngles;        // edict::angles
	Vector m_vecOldAngles;

	// Output only
	float m_outStepHeight;    // how much you climbed this move
	Vector m_outWishVel;        // This is where you tried 
	Vector m_outJumpVel;        // This is your jump velocity
								// Movement constraints    (radius 0 means no constraint)
	Vector m_vecConstraintCenter;
	float m_flConstraintRadius;
	float m_flConstraintWidth;
	float m_flConstraintSpeedFactor;
	bool m_bConstraintPastRadius;        ///< If no, do no constraining past Radius.  If yes, cap them to SpeedFactor past radius

	void SetAbsOrigin(const Vector &vec);
	const Vector &GetAbsOrigin() const;

private:
	Vector m_vecAbsOrigin;        // edict::origin
};

class PredictionSystem
{
public:

	void start(CUserCmd *userCMD);
	void end();

private:

	float oldCurtime;
	float oldFrametime;

	MoveData moveData;

	int *predictionRandomSeed;
};

extern std::unique_ptr<PredictionSystem> predictionSystem;