
#pragma once

// It's actually in DLLMain but w/e
extern bool DoUnload;
extern bool bGlovesNeedUpdate;
#include "Utilities.h"
extern 	int historyIdx;
extern bool lbyupdate1;

namespace Hooks
{
	void Initialise();
	void UndoHooks();

	// VMT Managers
	extern Utilities::Memory::VMTManager VMTPanel; // Hooking drawing functions
	extern Utilities::Memory::VMTManager VMTClient; // Maybe CreateMove
	extern Utilities::Memory::VMTManager VMTClientMode; // CreateMove for functionality
	extern Utilities::Memory::VMTManager VMTModelRender; // DrawModelEx for chams
	extern Utilities::Memory::VMTManager VMTPrediction; // InPrediction for no vis recoil
	extern Utilities::Memory::VMTManager VMTPlaySound; // Autoaccept shit
	extern Utilities::Memory::VMTManager VMTRenderView;
	extern Utilities::Memory::VMTManager VMTMaterialSystem;
};

namespace Resolver
{
	extern bool didhitHS;
}

extern bool flipAA;
extern bool islbyupdate;
extern float ProxyLBYtime;
extern int LBYBreakerTimer;
extern float enemysLastProxyTimer[65];
extern float fsnLBY;
extern float enemyLBYTimer[65];
extern bool rWeInFakeWalk;
extern float consoleProxyLbyLASTUpdateTime; // This is in ProxyLBY AntiAim.cpp


extern float enemyLBYDelta[65];
//extern int ResolverStage[65];
extern int missedLogHits[65];

extern bool switchAntiAimSide;

extern int shotsfired[65];
//Resolver
extern int ResolverStage[65];
extern int hittedLogHits[65];
extern int missedLogHits[65];
extern float testFloat1;
extern float testFloat2;
extern float testFloat3;
extern float testFloat4;
extern bool antiAimSide;