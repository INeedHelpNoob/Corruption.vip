// General shit
#include "Utilities.h"

// Injection stuff
#include "INJ/ReflectiveLoader.h"

// Stuff to initialise
#include "Offsets.h"
#include "Interfaces.h"
#include "Hooks.h"
#include "RenderManager.h"
#include "Hacks.h"
#include "Menu.h"
#include "MiscHacks.h"
#include "Dumping.h"
#include "AntiAntiAim.h"
#include "hitmarker.h"
#include "lagcomp.h"

// Used as part of the reflective DLL injection
extern HINSTANCE hAppInstance;
Vector AimPoint;
float intervalPerTick;
std::vector<int> HitBoxesToScan;
bool lbyupdate1;
// Our DLL Instance
float autowalldmgtest[65];
HINSTANCE HThisModule;
bool DoUnload;
bool pEntityLBYUpdate[65];
float pEntityLastUpdateTime[65];
float enemyLBYDelta[65];
int ResolverStage[65];
bool islbyupdate;
bool toggleSideSwitch;
float ProxyLBYtime;
float lineLBY;
float lineRealAngle;
float lineFakeAngle; int LBYBreakerTimer;
bool rWeInFakeWalk;
float fsnLBY;
bool switchInverse;
float testFloat1;
float enemyLBYTimer[65];
float testFloat2;
int shotsfired[65];
float testFloat4;
int historyIdx = 10;
int hittedLogHits[65];
int missedLogHits[65];
float consoleProxyLbyLASTUpdateTime; // This is in ProxyLBY AntiAim.cpp
float enemysLastProxyTimer[65];

// Our thread we use to setup everything we need
// Everything appart from code in hooks get's called from inside 
// here.

int InitialThread()
{

	//Utilities::OpenConsole("");

	// Intro banner with info
	PrintMetaHeader();
	//---------------------------------------------------------
	// Initialise all our shit
	Offsets::Initialise(); // Set our VMT offsets and do any pattern scans
	Interfaces::Initialise(); // Get pointers to the valve classes
	NetVar.RetrieveClasses(); // Setup our NetVar manager (thanks shad0w bby)
	NetvarManager::Instance()->CreateDatabase();
	Render::Initialise();
	hitmarker::singleton()->initialize();
	Hacks::SetupHacks();
	Menu::SetupMenu();
	Hooks::Initialise();
	ApplyNetVarsHooks();//something here when injecting throws errors
	lagComp = new LagCompensation;
	lagComp->initLagRecord();
	// Dumping
	//Dump::DumpClassIds();

	//---------------------------------------------------------
	//Utilities::Log("inject");
	// While our cheat is running
	while (DoUnload == false)
	{
		Sleep(500);
	}

	RemoveNetVarsHooks();
	Hooks::UndoHooks();
	Sleep(500); // Make sure none of our hooks are running
	FreeLibraryAndExitThread(HThisModule, 0);

	return 0;

}


// DllMain
// Entry point for our module
BOOL WINAPI DllMain(_In_ HINSTANCE hinstDLL, _In_ DWORD fdwReason, _In_ LPVOID lpvReserved)
{
	switch (fdwReason)
	{
		/*checks if its already there then breaks if it is*/
	case DLL_QUERY_HMODULE:
		if (lpvReserved != NULL)
			*(HMODULE *)lpvReserved = hAppInstance;
		break;
	case DLL_PROCESS_ATTACH:
		HThisModule = hinstDLL;
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)InitialThread, NULL, NULL, NULL);
		break;
	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}