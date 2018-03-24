#include "lagcomp.h"

#include "Hooks.h"
#include "Hacks.h"
#include "Chams.h"
#include "Menu.h"
#include "hitmarker.h"
#include "Interfaces.h"
#include "RenderManager.h"
#include "MiscHacks.h"
#include "CRC32.h"
#include "Resolver.h"
#include <intrin.h>
#include "EnginePrediction.h"
#include "XorStr.h"
//LagCompensation *lagComp;
#define TICK_INTERVAL			( Interfaces::Globals->interval_per_tick )
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
#define TICKS_TO_TIME(t) (Interfaces::Globals->interval_per_tick * (t) )

//Resolver.h Include --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Vector LastAngleAA;
Vector LastAngleAAReal;
extern float lineFakeAngle;
extern float lineRealAngle;
extern float lineLBY;
Vector LBYThirdpersonAngle; bool flipAA;
bool Resolver::didhitHS;
namespace Global
{
	CUserCmd* UserCmd;
	IClientEntity* Target;
	int Shots;
	bool change;
	int choked_ticks;
	int TargetID;
	bool Up2date;
	std::map<int, QAngle>storedshit;
	int missedshots;
	float oldSimulTime[65];
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//LagCompensation *lagComp = nullptr;

LagCompensation *lagComp = nullptr;

// Funtion Typedefs
typedef void(__thiscall* DrawModelEx_)(void*, void*, void*, const ModelRenderInfo_t&, matrix3x4*);
typedef void(__thiscall* PaintTraverse_)(PVOID, unsigned int, bool, bool);
typedef bool(__thiscall* InPrediction_)(PVOID);
typedef void(__stdcall *FrameStageNotifyFn)(ClientFrameStage_t);
typedef bool(__thiscall *FireEventClientSideFn)(PVOID, IGameEvent*);
typedef void(__thiscall* RenderViewFn)(void*, CViewSetup&, CViewSetup&, int, int);

using OverrideViewFn = void(__fastcall*)(void*, void*, CViewSetup*);
typedef float(__stdcall *oGetViewModelFOV)();

#define MakePtr(cast, ptr, addValue) (cast)( (DWORD)(ptr) + (DWORD)(addValue))
// Function Pointers to the originals
PaintTraverse_ oPaintTraverse;
DrawModelEx_ oDrawModelExecute;
FrameStageNotifyFn oFrameStageNotify;
OverrideViewFn oOverrideView;
FireEventClientSideFn oFireEventClientSide;
RenderViewFn oRenderView;

// Hook function prototypes
void __fastcall PaintTraverse_Hooked(PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce);
bool __stdcall Hooked_InPrediction();
bool __fastcall Hooked_FireEventClientSide(PVOID ECX, PVOID EDX, IGameEvent *Event);
void __fastcall Hooked_DrawModelExecute(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo_t &pInfo, matrix3x4 *pCustomBoneToWorld);
bool __stdcall CreateMoveClient_Hooked(/*void* self, int edx,*/ float frametime, CUserCmd* pCmd);
void  __stdcall Hooked_FrameStageNotify(ClientFrameStage_t curStage);
void __fastcall Hooked_OverrideView(void* ecx, void* edx, CViewSetup* pSetup);
float __stdcall GGetViewModelFOV();
void __fastcall Hooked_RenderView(void* ecx, void* edx, CViewSetup &setup, CViewSetup &hudViewSetup, int nClearFlags, int whatToDraw);
typedef MDLHandle_t(__thiscall* iFindMdl)(void*, char*);
iFindMdl oFindMDL;
MDLHandle_t __fastcall hkFindMDL(void*, void*, char*);
uint8_t* m_present;
uint8_t* m_reset;
// VMT Managers
namespace Hooks
{
	// VMT Managers
	Utilities::Memory::VMTManager VMTPanel; // Hooking drawing functions
	Utilities::Memory::VMTManager VMTClient; // Maybe CreateMove
	Utilities::Memory::VMTManager VMTClientMode; // CreateMove for functionality
	Utilities::Memory::VMTManager VMTModelRender; // DrawModelEx for chams
	Utilities::Memory::VMTManager VMTPrediction; // InPrediction for no vis recoil
	Utilities::Memory::VMTManager VMTPlaySound; // Autoaccept 
	Utilities::Memory::VMTManager VMTRenderView;
	Utilities::Memory::VMTManager VMTEventManager;
	Utilities::Memory::VMTManager VMTModelCache;
};

// Undo our hooks
void Hooks::UndoHooks()
{
	VMTPanel.RestoreOriginal();
	VMTPrediction.RestoreOriginal();
	VMTModelRender.RestoreOriginal();
	VMTClientMode.RestoreOriginal();
}
//typedef HRESULT(_stdcall *Present_T)(void*, const RECT*, RECT*, HWND, RGNDATA*);
//typedef HRESULT(__stdcall *Reset_t) (IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);

//Present_T oPresent;
//Reset_t		oReset = nullptr;

// Initialise all our hooks
void Hooks::Initialise()
{

	// Panel hooks for drawing to the screen via surface functions
	VMTPanel.Initialise((DWORD*)Interfaces::Panels);
	oPaintTraverse = (PaintTraverse_)VMTPanel.HookMethod((DWORD)&PaintTraverse_Hooked, Offsets::VMT::Panel_PaintTraverse);

	VMTPrediction.Initialise((DWORD*)Interfaces::Prediction);
	VMTPrediction.HookMethod((DWORD)&Hooked_InPrediction, 14);

	VMTModelRender.Initialise((DWORD*)Interfaces::ModelRender);
	oDrawModelExecute = (DrawModelEx_)VMTModelRender.HookMethod((DWORD)&Hooked_DrawModelExecute, Offsets::VMT::ModelRender_DrawModelExecute);

	VMTClientMode.Initialise((DWORD*)Interfaces::ClientMode);
	VMTClientMode.HookMethod((DWORD)CreateMoveClient_Hooked, 24);

	oOverrideView = (OverrideViewFn)VMTClientMode.HookMethod((DWORD)&Hooked_OverrideView, 18);
	VMTClientMode.HookMethod((DWORD)&GGetViewModelFOV, 35);

	VMTEventManager.Initialise((DWORD*)Interfaces::EventManager);
	oFireEventClientSide = (FireEventClientSideFn)VMTEventManager.HookMethod((DWORD)&Hooked_FireEventClientSide, 9);

	VMTClient.Initialise((DWORD*)Interfaces::Client);
	oFrameStageNotify = (FrameStageNotifyFn)VMTClient.HookMethod((DWORD)&Hooked_FrameStageNotify, 36);

	VMTModelCache.Initialise((DWORD*)Interfaces::ModelCache);
	oFindMDL = (iFindMdl)VMTModelCache.HookMethod((DWORD)&hkFindMDL, 10);

	m_present = Utilities::Memory::pattern_scan(GetModuleHandleW(L"gameoverlayrenderer.dll"), "FF 15 ? ? ? ? 8B F8 85 DB 74 1F") + 0x2;//big ( large ) obs bypass
	m_reset = Utilities::Memory::pattern_scan(GetModuleHandleW(L"gameoverlayrenderer.dll"), "FF 15 ? ? ? ? 8B F8 85 FF 78 18") + 0x2;  //big ( large ) obs bypass



	//oPresent = **reinterpret_cast<Present_T**>(m_present);
	//oReset = **reinterpret_cast<Reset_t**>(m_reset);

	//**reinterpret_cast<void***>(m_present) = reinterpret_cast<void*>(&hkPresent);
	//**reinterpret_cast<void***>(m_reset) = reinterpret_cast<void*>(&hkReset);
	// Console message
	Interfaces::Engine->ClientCmd_Unrestricted("clear");
	Interfaces::Engine->ClientCmd_Unrestricted("toggleconsole");
	Sleep(1000);//PLEASE DONT BE STUPID CHANGE SHIT WE HAVE TO CHANGE TO VARIABLES SO WE DONT HAVE TO EDIT SOO MUCH
	Interfaces::Engine->ClientCmd_Unrestricted("clear");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Corruption.vip CSGO Private Cheat");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Version: 1.1.5");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Build Date: 11/20/2017");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Loading: 0%");
	Sleep(250);
	Interfaces::Engine->ClientCmd_Unrestricted("clear");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Corruption.vip CSGO Private Cheat");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Version: 1.1.5");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Build Date: 11/20/2017");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Loading: 19%");
	Sleep(250);
	Interfaces::Engine->ClientCmd_Unrestricted("clear");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Corruption.vip CSGO Private Cheat");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Version: 1.1.5");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Build Date: 11/20/2017");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Loading: 32%");
	Sleep(250);
	Interfaces::Engine->ClientCmd_Unrestricted("clear");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Corruption.vip CSGO Private Cheat");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Version: 1.1.5");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Build Date: 11/20/2017");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Loading: 45%");
	Sleep(250);
	Interfaces::Engine->ClientCmd_Unrestricted("clear");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Corruption.vip CSGO Private Cheat");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Version: 1.1.5");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Build Date: 11/20/2017");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Loading: 58%");
	Sleep(250);
	Interfaces::Engine->ClientCmd_Unrestricted("clear");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Corruption.vip CSGO Private Cheat");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Version: 1.1.5");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Build Date: 11/20/2017");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Loading: 74%");
	Sleep(250);
	Interfaces::Engine->ClientCmd_Unrestricted("clear");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Corruption.vip CSGO Private Cheat");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Version: 1.1.5");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Build Date: 11/20/2017");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Loading: 89%");
	Sleep(250);
	Interfaces::Engine->ClientCmd_Unrestricted("clear");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Corruption.vip CSGO Private Cheat");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Version: 1.1.5");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Build Date: 11/20/2017");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Loading: 100%");
	Sleep(250);
	Interfaces::Engine->ClientCmd_Unrestricted("clear");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Corruption.vip CSGO Private Cheat");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Version: 1.1.5");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Build Date: 11/20/2017");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Loading: Completed");
	Interfaces::Engine->ClientCmd_Unrestricted("echo");
	Interfaces::Engine->ClientCmd_Unrestricted("echo [Corruption.vip]: Welcome To Corruption.vip!");
	GUI.LoadWindowState(&Menu::Window, "CorruptionLegit.cfg");
	Interfaces::Engine->ClientCmd_Unrestricted("echo [Corruption.vip]: Default config has been loaded");
	Interfaces::Engine->ClientCmd_Unrestricted("echo [Corruption.vip]: if you experience any issues, dm us on discord or use @administrators to get our attention");
	Interfaces::Engine->ClientCmd_Unrestricted("echo [Corruption.vip] Use the insert key to open the menu!");
	Interfaces::Engine->ClientCmd_Unrestricted("echo");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Corruption.vip Update Log");
	Interfaces::Engine->ClientCmd_Unrestricted("echo Version: 1.1.5");
	Interfaces::Engine->ClientCmd_Unrestricted("echo");
	Interfaces::Engine->ClientCmd_Unrestricted("echo + added memes");
	Interfaces::Engine->ClientCmd_Unrestricted("echo - removed memes");
	Interfaces::Engine->ClientCmd_Unrestricted("echo");
}


void MovementCorrection(CUserCmd* pCmd)
{

}
//---------------------------------------------------------------------------------------------------------
//                                         Hooked Functions
//---------------------------------------------------------------------------------------------------------

void SetClanTag(const char* tag, const char* name)//190% paste
{
	static auto pSetClanTag = reinterpret_cast<void(__fastcall*)(const char*, const char*)>(((DWORD)Utilities::Memory::FindPattern("engine.dll", (PBYTE)"\x53\x56\x57\x8B\xDA\x8B\xF9\xFF\x15\x00\x00\x00\x00\x6A\x24\x8B\xC8\x8B\x30", "xxxxxxxxx????xxxxxx")));
	pSetClanTag(tag, name);
}
void NoClantag()
{
	SetClanTag("", "");
}

void ClanTag()
{
	static int counter = 0;
	switch (Menu::Window.MiscTab.OtherClantag.GetIndex())
	{
	case 0:
		// No 
		break;
	case 1:
	{
		static int motion = 0;
		int ServerTime = (float)Interfaces::Globals->interval_per_tick * hackManager.pLocal()->GetTickBase() * 3;

		if (counter % 48 == 0)
			motion++;
		int value = ServerTime % 25;
		switch (value) {
		case 0:SetClanTag(XorStr("               C"), "Corruption.vip"); break;// WHY IS THIS IN CAPS MUY UGLY
		case 1:SetClanTag(XorStr("              CO"), "Corruption.vip"); break;//JS i added XorStr because it encrypts.. if you HxD edit
		case 2:SetClanTag(XorStr("             COR"), "Corruption.vip"); break;//this now you cannot see this tag and just edit it..
		case 3:SetClanTag(XorStr("            CORR"), "Corruption.vip"); break;
		case 4:SetClanTag(XorStr("           CORRU"), "Corruption.vip"); break;
		case 5:SetClanTag(XorStr("          CORRUP"), "Corruption.vip"); break;
		case 6:SetClanTag(XorStr("         CORRUPT"), "Corruption.vip"); break;
		case 7:SetClanTag(XorStr("        CORRUPTI"), "Corruption.vip"); break;
		case 8:SetClanTag(XorStr("       CORRUPTIO"), "Corruption.vip"); break;
		case 9:SetClanTag(XorStr("      CORRUPTION"), "Corruption.vip"); break;
		case 10:SetClanTag(XorStr("    CORRUPTION "), "Corruption.vip"); break;
		case 11:SetClanTag(XorStr("   CORRUPTION  "), "Corruption.vip"); break;
		case 12:SetClanTag(XorStr("  CORRUPTION   "), "Corruption.vip"); break;
		case 13:SetClanTag(XorStr(" CORRUPTION    "), "Corruption.vip"); break;
		case 14:SetClanTag(XorStr("CORRUPTION     "), "Corruption.vip"); break;
		case 15:SetClanTag(XorStr("ORRUPTION      "), "Corruption.vip"); break;
		case 16:SetClanTag(XorStr("RRUPTION       "), "Corruption.vip"); break;
		case 17:SetClanTag(XorStr("RUPTION        "), "Corruption.vip"); break;
		case 18:SetClanTag(XorStr("UPTION         "), "Corruption.vip"); break;
		case 19:SetClanTag(XorStr("PTION          "), "Corruption.vip"); break;
		case 20:SetClanTag(XorStr("TION           "), "Corruption.vip"); break;
		case 21:SetClanTag(XorStr("ION            "), "Corruption.vip"); break;
		case 22:SetClanTag(XorStr("ON             "), "Corruption.vip"); break;
		case 23:SetClanTag(XorStr("N              "), "Corruption.vip"); break;
		case 24:SetClanTag(XorStr("               "), "Corruption.vip"); break;
		}
		counter++;
	}
	break;
	case 2:
	{
		static int motion = 0;
		int ServerTime = (float)Interfaces::Globals->interval_per_tick * hackManager.pLocal()->GetTickBase() * 2.5;

		if (counter % 48 == 0)
			motion++;
		int value = ServerTime % 29;
		switch (value) {
		case 0:SetClanTag("              g", "gamesense"); break;
		case 1:SetClanTag("             ga", "gamesense"); break;
		case 2:SetClanTag("            gam", "gamesense"); break;
		case 3:SetClanTag("           game", "gamesense"); break;
		case 4:SetClanTag("          games", "gamesense"); break;
		case 5:SetClanTag("         gamese", "gamesense"); break;
		case 6:SetClanTag("        gamesen", "gamesense"); break;
		case 7:SetClanTag("       gamesens", "gamesense"); break;
		case 8:SetClanTag("      gamesense", "gamesense"); break;
		case 9:SetClanTag("     gamesense ", "gamesense"); break;
		case 10:SetClanTag("     gamesense ", "gamesense"); break;
		case 11:SetClanTag("    gamesense  ", "gamesense"); break;
		case 12:SetClanTag("    gamesense  ", "gamesense"); break;
		case 13:SetClanTag("   gamesense   ", "gamesense"); break;
		case 14:SetClanTag("   gamesense   ", "gamesense"); break;
		case 15:SetClanTag("  gamesense    ", "gamesense"); break;
		case 16:SetClanTag("  gamesense    ", "gamesense"); break;
		case 17:SetClanTag(" gamesense     ", "gamesense"); break;
		case 18:SetClanTag(" gamesense     ", "gamesense"); break;
		case 19:SetClanTag("gamesense      ", "gamesense"); break;
		case 20:SetClanTag("gamesense      ", "gamesense"); break;
		case 21:SetClanTag("amesense       ", "gamesense"); break;
		case 22:SetClanTag("mesense        ", "gamesense"); break;
		case 23:SetClanTag("sense          ", "gamesense"); break;
		case 24:SetClanTag("ense           ", "gamesense"); break;
		case 25:SetClanTag("nse            ", "gamesense"); break;
		case 26:SetClanTag("se             ", "gamesense"); break;
		case 27:SetClanTag("e              ", "gamesense"); break;
		case 28:SetClanTag("               ", "gamesense"); break;
		}
		counter++;
	}
	break;
	case 3:
		// INVIS CLANTAG
		SetClanTag("\r", "\r");
		break;
	case 4:
		SetClanTag("[VALV\xE1\xB4\xB1]", "Valve");//this is some ayyware type shit... i'll add some colors :|
		break;
	case 5:
		time_t now = time(0);
		char timestamp[10] = "";
		strftime(timestamp, 10, "CORRUPTION: %H : %M : %S", localtime(&now));
		SetClanTag(timestamp, "Time");
		break;

	}
}

static bool roundStartTest;
static float myLBYTimer;
static int roundStartTimer;

static float saveLastHeadshotFloat[65];
static float saveLastBaimFloat[65];
static float saveLastBaim30Float[65];

static float saveLastBaim10Float[65];

int hitmarkertime = 0;

static float testtimeToTick;
static float testServerTick;
static float testTickCount64 = 1;
template <typename T, std::size_t N> T* end_(T(&arr)[N]) { return arr + N; }
template <typename T, std::size_t N> T* begin_(T(&arr)[N]) { return arr; }


int kek = 0;

int LagCompBreak() {
	IClientEntity *pLocalPlayer = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	Vector velocity = pLocalPlayer->GetVelocity();
	velocity.z = 0;
	float speed = velocity.Length();
	if (speed > 0.f) {
		auto distance_per_tick = speed *
			Interfaces::Globals->interval_per_tick;
		int choked_ticks = std::ceilf(65.f / distance_per_tick);
		return std::min<int>(choked_ticks, 14);
	}
	return 1;
}
static float next1 = 0;
bool __stdcall CreateMoveClient_Hooked(/*void* self, int edx,*/ float frametime, CUserCmd* pCmd)
{
	if (!pCmd->command_number)
		return true;
	IClientEntity* local = hackManager.pLocal();
	if (Menu::Window.RageBotTab.FakeLagFix.GetState()|| Menu::Window.LegitBotTab.FakeLagFix.GetState())
		backtracking->legitBacktrack(pCmd, local);

	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
	{
		PVOID pebp;
		__asm mov pebp, ebp;
		bool* pbSendPacket = (bool*)(*(DWORD*)pebp - 0x1C);
		bool& bSendPacket = *pbSendPacket;

		if (Menu::Window.MiscTab.OtherClantag.GetIndex() > 0)
			ClanTag();

		//	CUserCmd* cmdlist = *(CUserCmd**)((DWORD)Interfaces::pInput + 0xEC);
		//	CUserCmd* pCmd = &cmdlist[sequence_number % 150];


			// Backup for safety
		Vector origView = pCmd->viewangles;
		Vector viewforward, viewright, viewup, aimforward, aimright, aimup;
		Vector qAimAngles;
		qAimAngles.Init(0.0f, pCmd->viewangles.y, 0.0f);
		AngleVectors(qAimAngles, &viewforward, &viewright, &viewup);

		// Do da hacks
		IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());


		if(pLocal->IsAlive() && Menu::Window.RageBotTab.LBY.GetState())
		{
			IClientEntity* LocalPlayer = hackManager.pLocal();
			float tickbase = TICKS_TO_TIME(LocalPlayer->GetTickBase());
			float flServerTime = (LocalPlayer->GetTickBase()  * Interfaces::Globals->interval_per_tick);
			lbyupdate1 = false;
			if (next1 - tickbase > 1.1)
			{
				next1 = 0;
				lbyupdate1 = false;
			}
			 if (pLocal->GetVelocity().Length2D() > 0.1)
			 {
				 next1 = flServerTime;
			 lbyupdate1 = false;
			 }
			 /*
			if (LocalPlayer->GetVelocity().Length2D() == 0)
			{
				if (tickbase > next1 + 1.1f)
				{
					next1 = tickbase + TICKS_TO_TIME(1);
					lbyupdate1 = true;
				}
			}*/
			
			if ((next1 + 1.1 <= flServerTime) && (LocalPlayer->GetFlags() & FL_ONGROUND) && pLocal->GetVelocity().Length2D() == 0.0)
			{
				next1 = flServerTime + Interfaces::Globals->interval_per_tick;
				lbyupdate1 = true;
			}
		}


		/*fake lag memes*/
		if (Menu::Window.RageBotTab.FakeLagEnable.GetState())
		{
			IClientEntity* pLocal = hackManager.pLocal();
			int ticksMax = 16;
			static int ticks = 0;
			static int ticks1 = 0;
			static int iTick = 0;
			static int iTick1 = 0;
			static int iTick2 = 0;
			int value = Menu::Window.RageBotTab.FakeLagChoke.GetValue();

			static int iFakeLag = -1;
			iFakeLag++;
			//if (pLocal->GetVelocity().Length2D() > 0 || !Menu::Window.RageBotTab.LBY.GetState())
			//{
			if (iFakeLag <= value && iFakeLag > -1 && Menu::Window.RageBotTab.FakeLagType.GetIndex() < 1)
			{
				bSendPacket = false;
			}
			else
			{
				bSendPacket = true;
				iFakeLag = -1;
			}

			if (value > 0 && Menu::Window.RageBotTab.FakeLagType.GetIndex() == 2)
			{
				if (ticks >= ticksMax)
				{
					bSendPacket = true;
					ticks = 0;
				}
				else
				{
					int packetsToChoke;
					if (pLocal->GetVelocity().Length() > 0.f)
					{
						packetsToChoke = (int)((128.f / Interfaces::Globals->interval_per_tick) / pLocal->GetVelocity().Length()) + 1;
						if (packetsToChoke >= 15)
							packetsToChoke = 14;
						if (packetsToChoke < value)
							packetsToChoke = value;
					}
					else
						packetsToChoke = 0;

					bSendPacket = ticks < 18 - packetsToChoke;;
				}
				ticks++;
			}

			else if (Menu::Window.RageBotTab.FakeLagEnable.GetState() && value > 0 && Menu::Window.RageBotTab.FakeLagType.GetIndex() == 1)
			{

				if (iTick < value) {
					bSendPacket = false;
					iTick++;
				}
				else {
					bSendPacket = true;
					iTick = 0;
				}
			}
			else if (Menu::Window.RageBotTab.FakeLagEnable.GetState() && value > 0 && Menu::Window.RageBotTab.FakeLagType.GetIndex() == 3)
			{
				value = LagCompBreak();
				if (iTick2 < value) {
					bSendPacket = false;
					iTick2++;
				}
				else {
					bSendPacket = true;
					iTick2 = 0;
				}

			}
		}

#pragma region Timer4LBY
		static float myOldLby;
		static float myoldTime;
		testtimeToTick = TIME_TO_TICKS(0.1);
		testServerTick = TIME_TO_TICKS(1);
		static int timerino;
		static float oneTickMinues;

		if (testServerTick == 128) {
			oneTickMinues = testServerTick / 128;
		}
		else {
			oneTickMinues = testServerTick / 64;
		}
		//IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

		
#pragma endregion

			Hacks::MoveHacks(pCmd, bSendPacket);
		
		int bestTargetIndex = -1;
		float bestFov = FLT_MAX;
		player_info_t info;



		switch (Menu::Window.VisualsTab.AmbientSkybox.GetIndex())
		{
		case 0:
			/*Disabled*/
			break;
		case 1:
			if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame()) {
				ConVar* NightSkybox1 = Interfaces::CVar->FindVar("sv_skyname"); /*Night-Skybox*/
				*(float*)((DWORD)&NightSkybox1->fnChangeCallback + 0xC) = NULL;
				NightSkybox1->SetValue("sky_csgo_night02b");
			}
			break;
		case 2:
			if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame()) {
				ConVar* NightSkybox1 = Interfaces::CVar->FindVar("sv_skyname"); /*Vertigo-Skybox*/
				*(float*)((DWORD)&NightSkybox1->fnChangeCallback + 0xC) = NULL;
				NightSkybox1->SetValue("vertigoblue_hdr");
			}
			break;
		case 3:
			if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame()) {
				ConVar* NightSkybox1 = Interfaces::CVar->FindVar("sv_skyname"); /*Jungle-Skybox*/
				*(float*)((DWORD)&NightSkybox1->fnChangeCallback + 0xC) = NULL;
				NightSkybox1->SetValue("jungle");
			}
			break;
		}

#define TICK_INTERVAL			( Interfaces::Globals->interval_per_tick )
#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
#define TICKS_TO_TIME(t) (Interfaces::Globals->interval_per_tick * (t) )

			IClientEntity* LocalPlayer = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
			float flServerTime = (float)(LocalPlayer->GetTickBase()  * Interfaces::Globals->interval_per_tick);
			static float next_time = 0;
			if (!bSendPacket && LocalPlayer->IsAlive() && LocalPlayer->GetVelocity().Length2D() == 0) {

				float TickStuff = TICKS_TO_TIME(LocalPlayer->GetTickBase());
				Global::Up2date = false;
				//flServerTime = next_time;

				if (next_time - TICKS_TO_TIME(LocalPlayer->GetTickBase()) > 1.1)
				{
					next_time = 0;
				}

				if (TickStuff > next_time + 1.1f)
				{
					next_time = TickStuff + TICKS_TO_TIME(1);
					Global::Up2date = true;
				}
			}
		//Movement Fix
		//GameUtils::CL_FixMove(pCmd, origView);
		qAimAngles.Init(0.0f, GetAutostrafeView().y, 0.0f); // if pCmd->viewangles.x > 89, set pCmd->viewangles.x instead of 0.0f on first
		AngleVectors(qAimAngles, &viewforward, &viewright, &viewup);
		qAimAngles.Init(0.0f, pCmd->viewangles.y, 0.0f);
		AngleVectors(qAimAngles, &aimforward, &aimright, &aimup);
		Vector vForwardNorm;		Normalize(viewforward, vForwardNorm);
		Vector vRightNorm;			Normalize(viewright, vRightNorm);
		Vector vUpNorm;				Normalize(viewup, vUpNorm);

		// Original shit for movement correction
		float forward = pCmd->forwardmove;
		float right = pCmd->sidemove;
		float up = pCmd->upmove;
		if (forward > 450) forward = 450;
		if (right > 450) right = 450;
		if (up > 450) up = 450;
		if (forward < -450) forward = -450;
		if (right < -450) right = -450;
		if (up < -450) up = -450;
		pCmd->forwardmove = DotProduct(forward * vForwardNorm, aimforward) + DotProduct(right * vRightNorm, aimforward) + DotProduct(up * vUpNorm, aimforward);
		pCmd->sidemove = DotProduct(forward * vForwardNorm, aimright) + DotProduct(right * vRightNorm, aimright) + DotProduct(up * vUpNorm, aimright);
		pCmd->upmove = DotProduct(forward * vForwardNorm, aimup) + DotProduct(right * vRightNorm, aimup) + DotProduct(up * vUpNorm, aimup);

		// Angle normalisation
		if (Menu::Window.MiscTab.OtherSafeMode.GetState())
		{
			GameUtils::NormaliseViewAngle(pCmd->viewangles);

			if (pCmd->viewangles.z != 0.0f)
			{
				pCmd->viewangles.z = 0.00;
			}

			if (pCmd->viewangles.x < -89 || pCmd->viewangles.x > 89 || pCmd->viewangles.y < -180 || pCmd->viewangles.y > 180)
			{
				Utilities::Log("Having to re-normalise!");
				GameUtils::NormaliseViewAngle(pCmd->viewangles);
				Beep(750, 800); // Why does it do this
				if (pCmd->viewangles.x < -89 || pCmd->viewangles.x > 89 || pCmd->viewangles.y < -180 || pCmd->viewangles.y > 180)
				{
					pCmd->viewangles = origView;
					pCmd->sidemove = right;
					pCmd->forwardmove = forward;
				}
			}
		}

		if (pCmd->viewangles.x > 90)
		{
			pCmd->forwardmove = -pCmd->forwardmove;
		}

		if (pCmd->viewangles.x < -90)
		{
			pCmd->forwardmove = -pCmd->forwardmove;
		}

		// LBY
		LBYThirdpersonAngle = Vector(pLocal->GetEyeAnglesXY()->x, pLocal->GetLowerBodyYaw(), pLocal->GetEyeAnglesXY()->z);

		if (bSendPacket == true) {
			LastAngleAA = pCmd->viewangles;
		}
		else if (bSendPacket == false) {
			LastAngleAAReal = pCmd->viewangles;
		}

		/*If you guys want real and fake indicators?*/
		lineLBY = pLocal->GetLowerBodyYaw();
		if (bSendPacket == true) {
			lineFakeAngle = pCmd->viewangles.y;
		}
		else if (bSendPacket == false) {
			lineRealAngle = pCmd->viewangles.y;
		}
	}
	return false;
}

std::string GetTimeString()
{
	time_t current_time;
	struct tm *time_info;
	static char timeString[10];
	time(&current_time);
	time_info = localtime(&current_time);
	strftime(timeString, sizeof(timeString), "%X", time_info);
	return timeString;
}

// Paint Traverse Hooked function
void __fastcall PaintTraverse_Hooked(PVOID pPanels, int edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
	if (Menu::Window.VisualsTab.Active.GetState() && Menu::Window.VisualsTab.OtherNoScope.GetState() && strcmp("HudZoom", Interfaces::Panels->GetName(vguiPanel)) == 0)
		return;

	oPaintTraverse(pPanels, vguiPanel, forceRepaint, allowForce);

	static unsigned int FocusOverlayPanel = 0;
	static bool FoundPanel = false;

	if (!FoundPanel)
	{
		PCHAR szPanelName = (PCHAR)Interfaces::Panels->GetName(vguiPanel);
		if (strstr(szPanelName, "MatSystemTopPanel"))
		{
			FocusOverlayPanel = vguiPanel;
			FoundPanel = true;
		}
	}
	else if (FocusOverlayPanel == vguiPanel)
	{

		if (Menu::Window.VisualsTab.Clock.GetState())
		{
			static float rainbow;
			rainbow += 0.0004f;
			if (rainbow > 1.f) rainbow = 0.f;
			Render::Textf(10, 24, Color::FromHSB(rainbow, 1.f, 1.f), Render::Fonts::Clock, ("\n Version: 1.1.6 \n"));
		}
		if (Menu::Window.MiscTab.Watermark.GetState())
		{
			static float rainbow;
			rainbow += 0.0004f;
			if (rainbow > 1.f) rainbow = 0.f;

			Render::Text(7, 7, Color::FromHSB(rainbow, 1.f, 1.f), Render::Fonts::WaterMark, (XorStr("CORRUPTION")));

			RECT scrn = Render::GetViewport();

			if(Menu::Window.LegitBotTab.AimbotEnable.GetState() || Menu::Window.LegitBotTab.TriggerEnable.GetState() || Menu::Window.LegitBotTab.LegitChoice.GetIndex() > 0 || Menu::Window.VisualsTab.Active.GetState() && !(Menu::Window.RageBotTab.AimbotEnable.GetState() || Menu::Window.RageBotTab.AntiAimPitch.GetIndex() > 0))
				Render::Text(scrn.right-300, scrn.top+5, Color(100, 200, 200), Render::Fonts::Clock, (XorStr("Legit-Mode")));
			else if (Menu::Window.RageBotTab.AimbotEnable.GetState() || Menu::Window.RageBotTab.AntiAimPitch.GetIndex() > 0 && !(Menu::Window.LegitBotTab.AimbotEnable.GetState() || Menu::Window.LegitBotTab.TriggerEnable.GetState() || Menu::Window.LegitBotTab.LegitChoice.GetIndex() > 0))
				Render::Text(scrn.right - 300, scrn.top +5, Color(200, 150, 100), Render::Fonts::Clock, (XorStr("Rage-Mode")));
		}

		if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
			Hacks::DrawHacks();

		if (Menu::Window.VisualsTab.lbyidicador.GetState())
		{
			CUserCmd* cmdlist = *(CUserCmd**)((DWORD)Interfaces::pInput + 0xEC);
			CUserCmd* pCmd = cmdlist;

			IClientEntity* localplayer = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
			//RECT TextSize = Render::GetTextSize(Render::Fonts::LBY, "LBY");
			RECT scrn = Render::GetViewport();
			if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
				if ((pCmd->viewangles.y - *localplayer->GetLowerBodyYawTarget() >= -35 && pCmd->viewangles.y - *localplayer->GetLowerBodyYawTarget() <= 35) && localplayer->GetVelocity().Length2D() >= 0)
					Render::Text(10, scrn.bottom - 22, Color(255, 0, 0, 255), Render::Fonts::LBY, "LBY");
				else
					Render::Text(10, scrn.bottom - 22, Color(0, 255, 0, 255), Render::Fonts::LBY, "LBY");
		}

		if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame() && Menu::Window.VisualsTab.OtherHitmarker.GetState())
			hitmarker::singleton()->on_paint();

		if (Menu::Window.VisualsTab.Active.GetState())
		{
			float AmbientRedAmount = Menu::Window.VisualsTab.AmbientRed.GetValue();
			float AmbientGreenAmount = Menu::Window.VisualsTab.AmbientGreen.GetValue();
			float AmbientBlueAmount = Menu::Window.VisualsTab.AmbientBlue.GetValue();

			ConVar* AmbientRedCvar = Interfaces::CVar->FindVar("mat_ambient_light_r");
			*(float*)((DWORD)&AmbientRedCvar->fnChangeCallback + 0xC) = NULL;
			AmbientRedCvar->SetValue(AmbientRedAmount);

			ConVar* AmbientGreenCvar = Interfaces::CVar->FindVar("mat_ambient_light_g");
			*(float*)((DWORD)&AmbientGreenCvar->fnChangeCallback + 0xC) = NULL;
			AmbientGreenCvar->SetValue(AmbientGreenAmount);

			ConVar* AmbientBlueCvar = Interfaces::CVar->FindVar("mat_ambient_light_b");
			*(float*)((DWORD)&AmbientBlueCvar->fnChangeCallback + 0xC) = NULL;
			AmbientBlueCvar->SetValue(AmbientBlueAmount);
		}

		player_info_t pinfo;
		/*TESTING SOME SHIT*/
		Menu::DoUIFrame();
	}
}

// InPrediction Hooked Function
bool __stdcall Hooked_InPrediction()
{
	bool result;
	static InPrediction_ origFunc = (InPrediction_)Hooks::VMTPrediction.GetOriginalFunction(14);
	static DWORD *ecxVal = Interfaces::Prediction;
	result = origFunc(ecxVal);

	// If we are in the right place where the player view is calculated
	// Calculate the change in the view and get rid of it
	if (Menu::Window.VisualsTab.OtherNoVisualRecoil.GetState() && (DWORD)(_ReturnAddress()) == Offsets::Functions::dwCalcPlayerView)
	{
		IClientEntity* pLocalEntity = NULL;

		float* m_LocalViewAngles = NULL;

		__asm
		{
			MOV pLocalEntity, ESI
			MOV m_LocalViewAngles, EBX
		}

		Vector viewPunch = pLocalEntity->localPlayerExclusive()->GetViewPunchAngle();
		Vector aimPunch = pLocalEntity->localPlayerExclusive()->GetAimPunchAngle();

		m_LocalViewAngles[0] -= (viewPunch[0] + (aimPunch[0] * 2 * 0.4499999f));
		m_LocalViewAngles[1] -= (viewPunch[1] + (aimPunch[1] * 2 * 0.4499999f));
		m_LocalViewAngles[2] -= (viewPunch[2] + (aimPunch[2] * 2 * 0.4499999f));
		return true;
	}

	return result;
}

int Kills2 = 0;
int Kills = 0;
bool RoundInfo = false;
size_t Delay = 0;
bool __fastcall Hooked_FireEventClientSide(PVOID ECX, PVOID EDX, IGameEvent *Event)
{
	int k = 0;
	//if (strcmp(Event->GetName(), "round_start") == 0)
		//lagComp->initLagRecord();

	if (Menu::Window.MiscTab.EnableBuyBot.GetState())
	{
		if (Menu::Window.MiscTab.BuyBot.GetIndex() == 1)
		{
			if (strcmp(Event->GetName(), "round_start") == 0)
				Interfaces::Engine->ClientCmd_Unrestricted("buy ak47; buy m4a1;");
		}
		else if (Menu::Window.MiscTab.BuyBot.GetIndex() == 2)
		{
			if (strcmp(Event->GetName(), "round_start") == 0)
				Interfaces::Engine->ClientCmd_Unrestricted("buy awp;");
		}
		else if (Menu::Window.MiscTab.BuyBot.GetIndex() == 3)
		{
			if (strcmp(Event->GetName(), "round_start") == 0)
				Interfaces::Engine->ClientCmd_Unrestricted("buy scar20; buy g3sg1;buy elite;");
		}
		else if (Menu::Window.MiscTab.BuyBot.GetIndex() == 4)
		{
			if (strcmp(Event->GetName(), "round_start") == 0)
				Interfaces::Engine->ClientCmd_Unrestricted("buy ssg08;");
		}
	}

	if (Menu::Window.MiscTab.EnableBuyBot.GetState())
	{
		if (Menu::Window.MiscTab.BuyBotGrenades.GetIndex() == 1)
		{
			if (strcmp(Event->GetName(), "round_start") == 0)
				Interfaces::Engine->ClientCmd_Unrestricted("buy flashbang; buy flashbang; buy smokegrenade; buy hegrenade;");
		}
		else if (Menu::Window.MiscTab.BuyBotGrenades.GetIndex() == 2)
		{
			if (strcmp(Event->GetName(), "round_start") == 0)
				Interfaces::Engine->ClientCmd_Unrestricted("buy flashbang; buy smokegrenade; buy hegrenade; buy molotov; buy incgrenade;");
		}
	}

	if (Menu::Window.MiscTab.EnableBuyBot.GetState())
	{
		if (Menu::Window.MiscTab.BuyBotKevlar.GetState())
		{
			if (strcmp(Event->GetName(), "round_start") == 0)
				Interfaces::Engine->ClientCmd_Unrestricted("buy vest; buy vesthelm;");
		}
	}

	if (Menu::Window.MiscTab.EnableBuyBot.GetState())
	{
		if (Menu::Window.MiscTab.BuyBotDefuser.GetState())
		{
			if (strcmp(Event->GetName(), "round_start") == 0)
				Interfaces::Engine->ClientCmd_Unrestricted("buy defuser;");
		}
	}
	if (strcmp(Event->GetName(), "round_start") == 0)
		k = 0;
	/*
	if (strcmp(Event->GetName(), "player_hurt")==0)
	{
		auto buyer = Event->GetInt("userid");
		auto attacker = Event->GetInt("attacker");

		int screenSizeX, screenCenterX;
		int screenSizeY, screenCenterY;
		Interfaces::Engine->GetScreenSize(screenSizeX, screenSizeY);

		screenCenterX = screenSizeX / 2;
		screenCenterY = screenSizeY / 2;
		player_info_t killed_info;
		player_info_t killer_info;
		auto player_index = Interfaces::Engine->GetPlayerForUserID(buyer);
		auto attacker2 = Interfaces::Engine->GetPlayerForUserID(attacker);
		Interfaces::Engine->GetPlayerInfo(player_index, &killed_info);
		Interfaces::Engine->GetPlayerInfo(attacker2, &killer_info);
		Render::Textf(screenCenterX,screenCenterY + (7 * k), Color(255, 255, 255, 255), Render::Fonts::ESP,("[Corruption.vip] ", killer_info.name, " hit ", killed_info.name, " for ", Event->GetString("dmg_health"), " with weapon ", Event->GetString("weapon")));
		k++;
	}*/
	if (!strcmp(Event->GetName(), "player_hurt"))
		{
			int screenSizeX, screenCenterX;
			int screenSizeY, screenCenterY;
			Interfaces::Engine->GetScreenSize(screenSizeX, screenSizeY);

			screenCenterX = screenSizeX / 2;
			screenCenterY = screenSizeY / 2;
			int dmg = Event->GetInt("dmg_health");
			int deadfag = Event->GetInt("userid");
			int attackingfag = Event->GetInt("attacker");
			int hit = Event->GetInt("hitgroup");

			auto testindx = Interfaces::Engine->GetPlayerForUserID(deadfag);
			auto testidx2 = Interfaces::Engine->GetPlayerForUserID(attackingfag);
			IClientEntity* userid1 = Interfaces::EntList->GetClientEntity(testindx);
			IClientEntity* hitnig = Interfaces::EntList->GetClientEntity(testidx2);

			IClientEntity* localplayer = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
			IClientEntity* hittedplayer = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetPlayerForUserID(deadfag));

			player_info_t bitch_info;
			player_info_t coolguy49_info;
			if (Interfaces::Engine->IsInGame())
			{

				if (deadfag && localplayer && Interfaces::Engine->GetPlayerInfo(testindx, &bitch_info), Interfaces::Engine->GetPlayerInfo(testidx2, &coolguy49_info) && userid1 && hitnig)
				{
					Render::Textf(screenCenterX,screenCenterY + (7*k),Color(255,255,255,255), Render::Fonts::ESP,"[DMG] ""%s did %i to %s\n",coolguy49_info.name,dmg,bitch_info.name);
					k++;
				}

				if (Interfaces::Engine->GetPlayerForUserID(deadfag) != Interfaces::Engine->GetLocalPlayer() && Interfaces::Engine->GetPlayerForUserID(attackingfag) == Interfaces::Engine->GetLocalPlayer())
				{
					if ((hit == 1) && hittedplayer && deadfag && attackingfag)//this is headshot only
					{
						Resolver::didhitHS = true;
						Global::missedshots = 0;
						Render::Text(150, 150, Color(255, 255, 255, 255), Render::Fonts::ESP, "Test KILL RENDER?");
						//hittedLogHits[hittedplayer->GetIndex()]++;
					}
					else
					{
						Resolver::didhitHS = false;
						Global::missedshots = localplayer->GetShotsFired();
					}

					if ((hit <= 7) && hittedplayer && deadfag && attackingfag && eff > 0)
					{
						//Render::Text(screenCenterX + 6, screenCenterY + 6, Color(255, 255, 0), Render::Fonts::ESP, std::to_string(eff).c_str());
						hittedLogHits[hittedplayer->GetIndex()]++;
					}
					else
					{

					}

				}
		}
	}

	if (Menu::Window.RageBotTab.FlipAA.GetState())
	{
		if (!strcmp(Event->GetName(), "player_hurt"))
		{
			int deadfag = Event->GetInt("userid");
			int attackingfag = Event->GetInt("attacker");
			IClientEntity* pLocal = hackManager.pLocal();
			if (Interfaces::Engine->GetPlayerForUserID(deadfag) == Interfaces::Engine->GetLocalPlayer() && Interfaces::Engine->GetPlayerForUserID(attackingfag) != Interfaces::Engine->GetLocalPlayer())
			{
				flipAA = true;

			}
			else
			{
				flipAA = false;
			}
		}
	}
	
	if (Menu::Window.MiscTab.TrashTalk.GetState()) {
		if (strcmp(Event->GetName(), "round_end") == 0) {
			RoundInfo = true;
			Delay = GetTickCount();
		}
		else if ((strcmp(Event->GetName(), "game_newmap") == 0) || (strcmp(Event->GetName(), "game_start") == 0) || (strcmp(Event->GetName(), "game_end") == 0)) {
			Kills = 0;
			Kills2 = 0;
		}
		else if (strcmp(Event->GetName(), "player_connect") == 0) {
			int PlayerID = Interfaces::Engine->GetPlayerForUserID(Event->GetInt("userid"));
			/*
			player_info_t PlayerInfo;
			if (Interfaces::Engine->GetPlayerInfo(PlayerID, &PlayerInfo)) {
				char Buffer[256];
				sprintf(Buffer, "say Ola, %s.", PlayerInfo.name);
				Interfaces::Engine->ClientCmd(Buffer);
			}*/
		}
		/*else if ((strcmp(Event->GetName(), "player_death") == 0) || (strcmp(Event->GetName(), "entity_killed") == 0)) {
			int PlayerID = Interfaces::Engine->GetPlayerForUserID(Event->GetInt("userid"));
			int AttackerID = Interfaces::Engine->GetPlayerForUserID(Event->GetInt("attacker"));

			player_info_t PlayerInfo;
			player_info_t AttackerInfo;

			if (Interfaces::Engine->GetPlayerInfo(PlayerID, &PlayerInfo) &&
				Interfaces::Engine->GetPlayerInfo(AttackerID, &AttackerInfo)) {
				
				if (PlayerID == Interfaces::Engine->GetLocalPlayer())
					Kills = 0;

				if (PlayerID != Interfaces::Engine->GetLocalPlayer() && AttackerID == Interfaces::Engine->GetLocalPlayer()) {
					Kills++;
					Kills2++;

					char Buffer[4];

					switch (Kills) {
					case 1: sprintf(Buffer, "say   1 ez", PlayerInfo.name); break;
					case 2:	sprintf(Buffer, "say   2", PlayerInfo.name); break;
					case 3:	sprintf(Buffer, "say   3", PlayerInfo.name); break;
					case 4:	sprintf(Buffer, "say   4", PlayerInfo.name); break;
					default: sprintf(Buffer, "say   5", PlayerInfo.name); break;
					}

					Interfaces::Engine->ClientCmd(Buffer);
				}
			}
		}*/
	}
	return oFireEventClientSide(ECX, Event);
	}

#define TEXTURE_GROUP_OTHER							"Other textures"

// DrawModelExec for chams and shit
void __fastcall Hooked_DrawModelExecute(void* thisptr, int edx, void* ctx, void* state, const ModelRenderInfo_t &pInfo, matrix3x4 *pCustomBoneToWorld)
{
	Color color;
	float flColor[3] = { 0.f };
	static IMaterial* CoveredLit = CreateMaterial(true);
	static IMaterial* OpenLit = CreateMaterial(false);
	static IMaterial* CoveredFlat = CreateMaterial(true, false);
	static IMaterial* OpenFlat = CreateMaterial(false, false);
	static IMaterial* Chrome = CreateMaterial("$envmap env_cube");
	bool DontDraw = false;

	if (Menu::Window.VisualsTab.Active.GetState())
	{
		const char* ModelName = Interfaces::ModelInfo->GetModelName((model_t*)pInfo.pModel);
		IClientEntity* pModelEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(pInfo.entity_index);
		IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

		int ChamsStyle = Menu::Window.VisualsTab.OptionsChams.GetIndex();
		int HandsStyle = Menu::Window.VisualsTab.OtherNoHands.GetIndex();

		if (Menu::Window.VisualsTab.Active.GetState() && strstr(ModelName, "models/player"))
		{
			if (pLocal && pModelEntity && ChamsStyle != 0)
			{
				IMaterial *material1 = Interfaces::MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/crystal_clear", TEXTURE_GROUP_OTHER);
				if (pLocal->IsScoped())
				{
					//color.SetColor(255, 255, 255, 255);
					//ForceMaterial(color, material1);
					Interfaces::RenderView->SetBlend(0.3);

				}
				if ((Menu::Window.VisualsTab.OptionsTeammates.GetState() || pModelEntity->GetTeamNum() != pLocal->GetTeamNum()))
				{
					IMaterial *covered = ChamsStyle == 1 ? CoveredLit : CoveredFlat;
					IMaterial *open = ChamsStyle == 1 ? OpenLit : OpenFlat;

					if (pModelEntity->IsAlive() && pModelEntity->GetHealth() > 0 /*&& pModelEntity->GetTeamNum() != local->GetTeamNum()*/)
					{
						float alpha = 1.f;

						if (pModelEntity->HasGunGameImmunity())
							alpha = 0.5f;

						if (pModelEntity->GetTeamNum() != pLocal->GetTeamNum())
						{
							flColor[0] = 60.f / 255.f;
							flColor[1] = 120.f / 255.f;
							flColor[2] = 180.f / 255.f;
						}
						else 
						{
							flColor[0] = 60.f / 255.f;
							flColor[1] = 120.f / 255.f;
							flColor[2] = 180.f / 255.f;
						}
						if (Menu::Window.VisualsTab.OptionsChams.GetIndex() == 1 || Menu::Window.VisualsTab.OptionsChams.GetIndex() == 2 && !pLocal->IsScoped())
						{
							Interfaces::RenderView->SetColorModulation(flColor);
							Interfaces::RenderView->SetBlend(alpha);
							Interfaces::ModelRender->ForcedMaterialOverride(covered);
							oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
						}
						
						if (pModelEntity->GetTeamNum() == pLocal->GetTeamNum())
						{
							flColor[0] = 150.f / 255.f;
							flColor[1] = 200.f / 255.f;
							flColor[2] = 60.f / 255.f;
						}
						else
						{
							flColor[0] = 150.f / 255.f;
							flColor[1] = 200.f / 255.f;
							flColor[2] = 60.f / 255.f;
						}
						if (Menu::Window.VisualsTab.OptionsChams.GetIndex() == 3)
						{
							Interfaces::RenderView->SetColorModulation(flColor);
							Interfaces::RenderView->SetBlend(alpha);
							Interfaces::ModelRender->ForcedMaterialOverride(OpenLit);
						}
						else {
							Interfaces::RenderView->SetColorModulation(flColor);
							Interfaces::RenderView->SetBlend(alpha);
							Interfaces::ModelRender->ForcedMaterialOverride(open);
						}
					}
					else
					{
						color.SetColor(255, 255, 255, 255);
						ForceMaterial(color, open);
					}
				}
			}
		}
		else if (strstr(ModelName, "arms"))
		{
			/*
			models/player/ct_fbi/ct_fbi_glass - platinum
			models/inventory_items/cologne_prediction/cologne_prediction_glass - glass
			models/inventory_items/trophy_majors/crystal_clear - crystal
			models/inventory_items/trophy_majors/gold - gold
			models/gibs/glass/glass - dark chrome
			models/inventory_items/trophy_majors/gloss - plastic/glass
			vgui/achievements/glow - glow
			*/
			IMaterial *material = Interfaces::MaterialSystem->FindMaterial("models/inventory_items/cologne_prediction/cologne_prediction_glass", TEXTURE_GROUP_OTHER);
			IMaterial *material1 = Interfaces::MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/crystal_clear", TEXTURE_GROUP_OTHER);

			if (HandsStyle != 0 && pLocal && pLocal->IsAlive())
			{
				if (HandsStyle == 1)
				{
					ForceMaterial(color, material1);
				}
				else if (HandsStyle == 2)
				{
					Interfaces::RenderView->SetBlend(0.3);
				}
				else if (HandsStyle == 3)
				{
					IMaterial *covered = ChamsStyle == 1 ? CoveredLit : CoveredFlat;
					IMaterial *open = ChamsStyle == 1 ? OpenLit : OpenFlat;
					if (pLocal)
					{
						if (pLocal->IsAlive())
						{
							int alpha = pLocal->HasGunGameImmunity() ? 150 : 255;

							if (pLocal->GetTeamNum() == 2)
								color.SetColor(240, 240, 240, alpha);
							else
								color.SetColor(240, 240, 240, alpha);

							ForceMaterial(color, covered);
							oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);

							if (pLocal->GetTeamNum() == 2)
								color.SetColor(240, 240, 240, alpha);
							else
								color.SetColor(240, 240, 240, alpha);
						}
						else
						{
							color.SetColor(240, 240, 240, 255);
						}

						ForceMaterial(color, covered);
					}
				}
				else
				{
					static int counter = 0;
					static float colors[3] = { 1.f, 0.f, 0.f };

					if (colors[counter] >= 1.0f)
					{
						colors[counter] = 1.0f;
						counter += 1;
						if (counter > 2)
							counter = 0;
					}
					else
					{
						int prev = counter - 1;
						if (prev < 0) prev = 2;
						colors[prev] -= 0.05f;
						colors[counter] += 0.05f;
					}

					Interfaces::RenderView->SetColorModulation(colors);
					Interfaces::RenderView->SetBlend(0.3);
					Interfaces::ModelRender->ForcedMaterialOverride(CoveredLit);
				}
			}
		}
		else if (Menu::Window.VisualsTab.FiltersWeapons.GetState() && strstr(ModelName, "_dropped.mdl"))
		{
			if (ChamsStyle != 0)
			{
				IMaterial *material1 = Interfaces::MaterialSystem->FindMaterial("models/inventory_items/trophy_majors/crystal_clear", TEXTURE_GROUP_OTHER);
				IMaterial *open = ChamsStyle == 1 ? OpenLit : OpenFlat;
				IMaterial *covered = ChamsStyle == 1 ? CoveredLit : CoveredFlat;
				if (Menu::Window.VisualsTab.OptionsDroppedc.GetIndex() != 0)//this needs to be a 
				{
					switch (Menu::Window.VisualsTab.OptionsDroppedc.GetIndex())
					{
					case 0:
						break;
					case 1://chams?
						//color.SetColor(255, 25, 25, 255);//red might be cool? also why not have check box to switch what dropped items look like?
							flColor[0] = 255.f / 255.f;
							flColor[1] = 25.f / 255.f;
							flColor[2] = 25.f / 255.f;
						Interfaces::RenderView->SetColorModulation(flColor);
						Interfaces::RenderView->SetBlend(0.3);
						Interfaces::ModelRender->ForcedMaterialOverride(OpenLit);
						break;
					case 2:
						color.SetColor(255, 255, 255, 255);//red might be cool? also why not have check box to switch what dropped items look like?
						Interfaces::ModelRender->ForcedMaterialOverride(OpenLit);
						break;
					case 3://chams with color
						color.SetColor(255, 25, 25, 255);//red might be cool? also why not have check box to switch what dropped items look like?
						ForceMaterial(color, material1);
						break;
					}
				}
			}
		}
	}

	if (!DontDraw)
		oDrawModelExecute(thisptr, ctx, state, pInfo, pCustomBoneToWorld);
	Interfaces::ModelRender->ForcedMaterialOverride(NULL);
}


int RandomInt(int min, int max)
{
	return rand() % max + min;
}

bool bGlovesNeedUpdate;
void ApplyCustomGloves(IClientEntity* pLocal)
{
	if (Menu::Window.MiscTab.EnableGloves.GetState())
	{
		if (!Interfaces::Engine->IsConnected() || !Interfaces::Engine->IsInGame())
			return;

		if (bGlovesNeedUpdate && pLocal->IsAlive())
		{
			DWORD* hMyWearables = (DWORD*)((size_t)pLocal + 0x2EF4);

			if (!Interfaces::EntList->GetClientEntity(hMyWearables[0] & 0xFFF))
			{
				for (ClientClass* pClass = Interfaces::Client->GetAllClasses(); pClass; pClass = pClass->m_pNext)
				{
					if (pClass->m_ClassID != (int)CSGOClassID::CEconWearable)
						continue;

					int iEntry = (Interfaces::EntList->GetHighestEntityIndex() + 1);
					int	iSerial = RandomInt(0x0, 0xFFF);

					pClass->m_pCreateFn(iEntry, iSerial);
					hMyWearables[0] = iEntry | (iSerial << 16);

					break;
				}
			}

			player_info_t LocalPlayerInfo;
			Interfaces::Engine->GetPlayerInfo(Interfaces::Engine->GetLocalPlayer(), &LocalPlayerInfo);

			CBaseCombatWeapon* glovestochange = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntity(hMyWearables[0] & 0xFFF);

			if (!glovestochange)
				return;


			switch (Menu::Window.MiscTab.GloveModel.GetIndex())
			{
			case 1:
			{
				*glovestochange->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 5027;
				glovestochange->SetModelIndex(Interfaces::ModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl"));
				break;
			}
			case 2:
			{
				*glovestochange->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 5032;
				glovestochange->SetModelIndex(Interfaces::ModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl"));
				break;
			}
			case 3:
			{
				*glovestochange->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 5031;
				glovestochange->SetModelIndex(Interfaces::ModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl"));
				break;
			}
			case 4:
			{
				*glovestochange->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 5030;
				glovestochange->SetModelIndex(Interfaces::ModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl"));
				break;
			}
			case 5:
			{
				*glovestochange->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 5033;
				glovestochange->SetModelIndex(Interfaces::ModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl"));
				break;
			}
			case 6:
			{
				*glovestochange->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 5034;
				glovestochange->SetModelIndex(Interfaces::ModelInfo->GetModelIndex("models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl"));
				break;
			}
			default:
				break;
			}

			switch (Menu::Window.MiscTab.GloveSkin.GetIndex())
			{
			case 0:
				break;
			case 1:
				*glovestochange->FallbackPaintKit() = 10007;
				break;
			case 2:
				*glovestochange->FallbackPaintKit() = 10006;
				break;
			case 3:
				*glovestochange->FallbackPaintKit() = 10039;
				break;
			case 4:
				*glovestochange->FallbackPaintKit() = 10008;
				break;
			case 5:
				*glovestochange->FallbackPaintKit() = 10021;
				break;
			case 6:
				*glovestochange->FallbackPaintKit() = 10036;
				break;
			case 7:
				*glovestochange->FallbackPaintKit() = 10009;
				break;
			case 8:
				*glovestochange->FallbackPaintKit() = 10010;
				break;
			case 9:
				*glovestochange->FallbackPaintKit() = 10016;
				break;
			case 10:
				*glovestochange->FallbackPaintKit() = 10013;
				break;
			case 11:
				*glovestochange->FallbackPaintKit() = 10040;
				break;
			case 12:
				*glovestochange->FallbackPaintKit() = 10015;
				break;
			case 13:
				*glovestochange->FallbackPaintKit() = 10037;
				break;
			case 14:
				*glovestochange->FallbackPaintKit() = 10038;
				break;
			case 15:
				*glovestochange->FallbackPaintKit() = 10018;
				break;
			case 16:
				*glovestochange->FallbackPaintKit() = 10019;
				break;
			case 17:
				*glovestochange->FallbackPaintKit() = 10026;
				break;
			case 18:
				*glovestochange->FallbackPaintKit() = 10028;
				break;
			case 19:
				*glovestochange->FallbackPaintKit() = 10027;
				break;
			case 20:
				*glovestochange->FallbackPaintKit() = 10024;
				break;
			case 21:
				*glovestochange->FallbackPaintKit() = 10033;
				break;
			case 22:
				*glovestochange->FallbackPaintKit() = 10034;
				break;
			case 23:
				*glovestochange->FallbackPaintKit() = 10035;
				break;
			case 24:
				*glovestochange->FallbackPaintKit() = 10030;
				break;
			}

			*glovestochange->m_AttributeManager()->m_Item()->ItemIDHigh() = -1;
			*glovestochange->FallbackWear() = 0.001f;
			*glovestochange->m_AttributeManager()->m_Item()->AccountID() = LocalPlayerInfo.xuidlow;


			glovestochange->PreDataUpdate(0);
			bGlovesNeedUpdate = false;
		}
	}
}
/*Wrapzii SHIT*/

void AutoResolver(Vector* & Angle, IClientEntity* Player)
{

	static int iLastUpdatedTick = 0;

	Player->reset.y = Angle->y;

	static Vector orginalview = Vector(0, 0, 0);
	if (orginalview.x != Angle->x)
		orginalview.x = Angle->x;
	if (Angle->y != Player->resolved)
	{
		orginalview.y = Angle->y;

		float flResolve = 0.f;
		float flLowerBodyYaw = Player->GetLowerBodyYaw();

		int difference = Player->GetEyeAnglesXY()->y - flLowerBodyYaw;

		iLastUpdatedTick++;

		if (flLowerBodyYaw != Player->flLastPelvisAng)
		{
			if (Player->GetVelocity().Length2D() < 1)
			{
				int temp = static_cast<int>(floor(Player->GetEyeAnglesXY()->y - Player->flLastPelvisAng));
				while (temp < 0)
					temp += 360;
				while (temp > 360)
					temp -= 360;
				Player->Backtrack[temp] = flLowerBodyYaw - Player->GetEyeAnglesXY()->y;
			}

			iLastUpdatedTick = 0;
			Player->flLastPelvisAng = flLowerBodyYaw;
			Player->GetEyeAnglesXY()->y = orginalview.y;
		}

		if (Player->GetVelocity().Length2D() >= 1)
		{
			flResolve = flLowerBodyYaw;
		}
		else
		{
			flResolve = Player->flLastPelvisAng;
		}
		Angle->y = flResolve;
		Player->resolved = Angle->y;
	}
}

void Smart1Resolver(Vector* & Angle, IClientEntity* Player)
{
	static int iLastUpdatedTick = 0;

	Player->reset.y = Angle->y;

	static Vector orginalview = Vector(0, 0, 0);
	if (orginalview.x != Angle->x)
		orginalview.x = Angle->x;
	if (Angle->y != Player->resolved)
	{
		orginalview.y = Angle->y;

		float flResolve = 0.f;
		float flLowerBodyYaw = Player->GetLowerBodyYaw();

		static float TimedYaw;

		int difference = orginalview.y - flLowerBodyYaw;

		iLastUpdatedTick++;

		if (flLowerBodyYaw != Player->flLastPelvisAng)
		{
			if (Player->GetVelocity().Length2D() == 0)
			{
				int temp = static_cast<int>(floor(Player->flEyeAng - Player->flLastPelvisAng));
				while (temp < 0)
					temp += 360;
				while (temp > 360)
					temp -= 360;
				Player->Backtrack[temp] = flLowerBodyYaw - Player->flEyeAng;

				TimedYaw = Player->GetEyeAngles().y - Player->GetLowerBodyYaw();
				flResolve = abs(TimedYaw);
			}

			iLastUpdatedTick = 0;
			Player->flLastPelvisAng = flLowerBodyYaw;
			Player->flEyeAng = orginalview.y;
		}

		if (Player->GetVelocity().Length2D() >= 1)
		{
			flResolve = flLowerBodyYaw;
		}
		else
		{
			int temp = static_cast<int>(floor(orginalview.y - flLowerBodyYaw));
			while (temp < 0)
				temp += 360;
			while (temp > 360)
				temp -= 360;
			flResolve = Player->Backtrack[temp] + orginalview.y;

			TimedYaw = Player->GetEyeAngles().y - Player->GetLowerBodyYaw();
			flResolve = abs(TimedYaw);
		}
		Angle->y = flResolve;
		Player->resolved = Angle->y;
	}
}

// Hooked FrameStageNotify for removing visual recoil
void  __stdcall Hooked_FrameStageNotify(ClientFrameStage_t curStage)
{
	lagComp->log(curStage);
	DWORD eyeangles = NetVar.GetNetVar(0xBFEA4E7B);
	IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame() && curStage == FRAME_RENDER_START)
	{
		if ((Menu::Window.RageBotTab.OtherAimbotDebug.GetState()))
		{
			static bool debug = false;
			if (!debug)
			{
				ConVar* sv_cheats = Interfaces::CVar->FindVar("sv_cheats");
				SpoofedConvar* sv_cheats_spoofed = new SpoofedConvar(sv_cheats);
				sv_cheats_spoofed->SetInt(1);
				debug = true;
			}
		}

		static bool debug = false;
		if (Menu::Window.RageBotTab.OtherAimbotDebug.GetState() && pLocal->IsAlive())
		{
			if (!debug)
			{
				Interfaces::Engine->ClientCmd_Unrestricted("net_fakeloss 3");
				Interfaces::Engine->ClientCmd_Unrestricted("net_fakelag 75");
				Interfaces::Engine->ClientCmd_Unrestricted("net_fakejitter 53535353");
				debug = true;
			}
		}
		else if (!Menu::Window.RageBotTab.OtherAimbotDebug.GetState() && pLocal->IsAlive())
		{
			debug = false;
		}

		static bool debug1 = false;
		if (!Menu::Window.RageBotTab.OtherAimbotDebug.GetState() && pLocal->IsAlive())
		{
			if (!debug1)
			{
				Interfaces::Engine->ClientCmd_Unrestricted("net_fakeloss 0");
				Interfaces::Engine->ClientCmd_Unrestricted("net_fakelag 0");
				Interfaces::Engine->ClientCmd_Unrestricted("net_fakejitter 0");
				debug1 = true;
			}
		}
		else if (Menu::Window.RageBotTab.OtherAimbotDebug.GetState())
		{
			debug1 = false;
		}
		/*LEGIT BOT VERSION OF FAKE PING EXPLOIT*/

		if ((Menu::Window.RageBotTab.OtherAimbotDebug.GetState()))
		{
			static bool debug3 = false;
			if (!debug3)
			{
				ConVar* sv_cheats = Interfaces::CVar->FindVar("sv_cheats");
				SpoofedConvar* sv_cheats_spoofed = new SpoofedConvar(sv_cheats);
				sv_cheats_spoofed->SetInt(1);
				debug3 = true;
			}
		}

		static bool debug3 = false;
		if (Menu::Window.LegitBotTab.OtherAimbotDebug.GetState() && pLocal->IsAlive())
		{
			if (!debug3)
			{
				Interfaces::Engine->ClientCmd_Unrestricted("net_fakeloss 3");
				Interfaces::Engine->ClientCmd_Unrestricted("net_fakelag 50");
				Interfaces::Engine->ClientCmd_Unrestricted("net_fakejitter 53535353");
				debug3 = true;
			}
		}
		else if (!Menu::Window.LegitBotTab.OtherAimbotDebug.GetState() && pLocal->IsAlive())
		{
			debug3 = false;
		}

		static bool debug2 = false;
		if (!Menu::Window.LegitBotTab.OtherAimbotDebug.GetState() && pLocal->IsAlive())
		{
			if (!debug2)
			{
				Interfaces::Engine->ClientCmd_Unrestricted("net_fakeloss 0");
				Interfaces::Engine->ClientCmd_Unrestricted("net_fakelag 0");
				Interfaces::Engine->ClientCmd_Unrestricted("net_fakejitter 0");
				debug2 = true;
			}
		}
		else if (Menu::Window.LegitBotTab.OtherAimbotDebug.GetState())
		{
			debug2 = false;
		}



		if (pLocal->IsAlive() && Menu::Window.VisualsTab.OtherThirdperson.GetState())
		{

			if (Menu::Window.MiscTab.ThirdpersonGhost.GetState()) {
				IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
				if (pLocal->IsAlive())
				{
					static IMaterial* CoveredLit = CreateMaterial(true);
					static IMaterial* OpenLit = CreateMaterial(false);
					static IMaterial* CoveredFlat = CreateMaterial(true, false);
					static IMaterial* OpenFlat = CreateMaterial(false, false);
					static IMaterial* Chrome = CreateMaterial("$envmap env_cube");

					IMaterial *open = 1 == 1 ? OpenLit : OpenFlat;


					//IMaterial* mat = Globals.g_pChamsMat;
					if (open)
					{
						pLocal->setAbsAechse(Vector(0,LastAngleAA.y,0)); // paste here ur AA.y value or pLocal->GetLby() (for example)
						bool LbyColor = false; // u can make LBY INDICATOR. When LbyColor is true. Color will be Green , if false it will be White
						float NormalColor[3] = { 1, 1, 1 };
						float lbyUpdateColor[3] = { 0, 1, 0 };
						Interfaces::RenderView->SetColorModulation(LbyColor ? lbyUpdateColor : NormalColor);
						Interfaces::ModelRender->ForcedMaterialOverride(open);
						pLocal->DrawModel(STUDIO_RENDER, 255);
						Interfaces::ModelRender->ForcedMaterialOverride(nullptr);
						pLocal->setAbsAechse(LastAngleAAReal);//should be real
					}
				}
			}

			Vector thirdpersonMode;

			switch (Menu::Window.VisualsTab.ThirdpersonAngle.GetIndex())
			{
			case 0:
				thirdpersonMode = LastAngleAAReal;
				break;
			case 1:
				thirdpersonMode = LastAngleAA;
				break;
			case 2:
				thirdpersonMode = LBYThirdpersonAngle;
				break;
			}

			static bool rekt = false;
			if (!rekt)
			{
				ConVar* sv_cheats = Interfaces::CVar->FindVar("sv_cheats");
				SpoofedConvar* sv_cheats_spoofed = new SpoofedConvar(sv_cheats);
				sv_cheats_spoofed->SetInt(1);
				rekt = true;
			}


			static bool kek = false;

			if (!kek)
			{
				Interfaces::Engine->ClientCmd_Unrestricted("thirdperson");
				kek = true;
			}

			static bool toggleThirdperson;
			static float memeTime;
			int ThirdPersonKey = Menu::Window.VisualsTab.ThirdPersonKeyBind.GetKey();
			if (ThirdPersonKey >= 0 && GetAsyncKeyState(ThirdPersonKey) && abs(memeTime - Interfaces::Globals->curtime) > 0.5)
			{
				toggleThirdperson = !toggleThirdperson;
				memeTime = Interfaces::Globals->curtime;
			}


			if (toggleThirdperson)
			{
				Interfaces::pInput->m_fCameraInThirdPerson = true;
				if (*(bool*)((DWORD)Interfaces::pInput + 0xA5))
					*(Vector*)((DWORD)pLocal + 0x31C8) = thirdpersonMode;
			}
			else {
				// No Thirdperson
				static Vector vecAngles;
				Interfaces::Engine->GetViewAngles(vecAngles);
				Interfaces::pInput->m_fCameraInThirdPerson = false;
				Interfaces::pInput->m_vecCameraOffset = Vector(vecAngles.x, vecAngles.y, 0);
			}


		}
		else if (pLocal->IsAlive() == 0)
		{
			kek = false;
			Interfaces::Engine->ClientCmd_Unrestricted("firstperson");

		}

		if (!Menu::Window.VisualsTab.OtherThirdperson.GetState()) {

			// No Thirdperson
			static Vector vecAngles;
			Interfaces::Engine->GetViewAngles(vecAngles);
			Interfaces::pInput->m_fCameraInThirdPerson = false;
			Interfaces::pInput->m_vecCameraOffset = Vector(vecAngles.x, vecAngles.y, 0);
		}
		else if (pLocal->GetHealth() <= 0)
		{
			Interfaces::Engine->ClientCmd_Unrestricted("firstperson");
			kek = false;
		}
	}


	if (curStage == FRAME_NET_UPDATE_POSTDATAUPDATE_START) {

		// 1. Time on Server LBY Update Predicten
		IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
		for (int i = 0; i <= Interfaces::Engine->GetMaxClients(); ++i)
		{
			IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);

			if (!pEntity || pEntity->IsDormant() || !pEntity->IsAlive())
				continue;

			if (pEntity->GetTeamNum() == pLocal->GetTeamNum() || !pLocal->IsAlive())
				continue;
			//Utilities::Log("APPLY SKIN APPLY SKIN");
			ResolverSetup::GetInst().FSN(pEntity, curStage);
			IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
			INetChannelInfo *nci = Interfaces::Engine->GetNetChannelInfo();
			if (Menu::Window.RageBotTab.FakeLagFix.GetState() || Menu::Window.LegitBotTab.FakeLagFix.GetState())
				backtracking->Update(Interfaces::Globals->tickcount);

			VarMapping_t *map = pLocal->GetVarMap(pEntity);
			/*interpolation*/
			
			if (map)
			{
				if (Menu::Window.RageBotTab.FakeLagFix.GetState())//make this backtracking
					map->m_nInterpolatedEntries = 0;
				else
					if (map->m_nInterpolatedEntries == 0)
						map->m_nInterpolatedEntries = 6;
			}
		}
		static int startTickBase;
		static int timerxd;
		static float oldlbyyy[65];
		static float oldtimer[65];
		static bool isLBYPredictited[65];


		for (int i = 0; i <= Interfaces::Engine->GetMaxClients(); ++i)
		{
			IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
			if (!pEntity || pEntity->IsDormant() || !pEntity->IsAlive())
				continue;
			if (pEntity->GetTeamNum() == pLocal->GetTeamNum() || !pLocal->IsAlive())
				continue;
			Vector* eyeAngles = pEntity->GetEyeAnglesPointer();
			player_info_t pTemp;

			if (!Interfaces::Engine->GetPlayerInfo(i, &pTemp))
				continue;

			if (abs(Global::Shots - hittedLogHits[pEntity->GetIndex()]) > 0) {
				missedLogHits[pEntity->GetIndex()] = abs(shotsfired[pEntity->GetIndex()] - hittedLogHits[pEntity->GetIndex()]);
			}

			printf("MissedLogHits: %i Index: %i\n", missedLogHits[pEntity->GetIndex()] , pEntity->GetIndex());
			printf("Shotsfired: %i\n", shotsfired);
			printf("Hitted: %i Index: %i\n", hittedLogHits[pEntity->GetIndex()], pEntity->GetIndex());
			
			// We dont use this just for deco



			//.... Delta
			float deltadif = abs(pEntity->GetEyeAngles().y - pEntity->GetLowerBodyYaw());


			static float oldlowerbodyyaw;
			static float lbyproxytime;
			static int bullets;
		}


	}


	if (Menu::Window.RageBotTab.AimbotEnable.GetState())// pvs fix
	{
		for (int i = 1; i <= Interfaces::Engine->GetMaxClients(); i++)
		{
			if (i == Interfaces::Engine->GetLocalPlayer()) continue;
			IClientEntity* pCurEntity = Interfaces::EntList->GetClientEntity(i);
			if (!pCurEntity) continue;
			*(int*)((uintptr_t)pCurEntity + 0xA30) = Interfaces::Globals->framecount;
			*(int*)((uintptr_t)pCurEntity + 0xA28) = 0; //clear occlusion flags
		}
	}

	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame() && curStage == FRAME_NET_UPDATE_POSTDATAUPDATE_START)
	{
		IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
		int iBayonet = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_bayonet.mdl");
		int iButterfly = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_butterfly.mdl");
		int iFlip = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_flip.mdl");
		int iGut = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_gut.mdl");
		int iKarambit = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_karam.mdl");
		int iM9Bayonet = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
		int iHuntsman = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_tactical.mdl");
		int iFalchion = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_falchion_advanced.mdl");
		int iDagger = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_push.mdl");
		int iBowie = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_survival_bowie.mdl");
		int iGunGame = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_gg.mdl");

		for (int i = 0; i <= Interfaces::EntList->GetHighestEntityIndex(); i++)
		{
			IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);

			if (pEntity)
			{
				ApplyCustomGloves(pLocal);

				ULONG hOwnerEntity = *(PULONG)((DWORD)pEntity + 0x148);

				IClientEntity* pOwner = Interfaces::EntList->GetClientEntityFromHandle((HANDLE)hOwnerEntity);

				if (pOwner)
				{
					if (pOwner == pLocal)
					{
						std::string sWeapon = Interfaces::ModelInfo->GetModelName(pEntity->GetModel());

						if (sWeapon.find("models/weapons", 0) != std::string::npos)
							continue;

						if (sWeapon.find("c4_planted", 0) != std::string::npos)
							continue;

						if (sWeapon.find("thrown", 0) != std::string::npos)
							continue;

						if (sWeapon.find("smokegrenade", 0) != std::string::npos)
							continue;

						if (sWeapon.find("flashbang", 0) != std::string::npos)
							continue;

						if (sWeapon.find("fraggrenade", 0) != std::string::npos)
							continue;

						if (sWeapon.find("molotov", 0) != std::string::npos)
							continue;

						if (sWeapon.find("decoy", 0) != std::string::npos)
							continue;

						if (sWeapon.find("incendiarygrenade", 0) != std::string::npos)
							continue;

						if (sWeapon.find("ied", 0) != std::string::npos)
							continue;

						if (sWeapon.find("w_eq_", 0) != std::string::npos)
							continue;

						CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)pEntity;

						ClientClass *pClass = Interfaces::Client->GetAllClasses();

						if (Menu::Window.MiscTab.SkinEnable.GetState())
						{
							int Model = Menu::Window.MiscTab.KnifeModel.GetIndex();
							int M41S = Menu::Window.SkinchangerTab.M41SSkin.GetIndex();
							int M4A4 = Menu::Window.SkinchangerTab.M4A4Skin.GetIndex();
							int AK47 = Menu::Window.SkinchangerTab.AK47Skin.GetIndex();
							int AWP = Menu::Window.SkinchangerTab.AWPSkin.GetIndex();
							int GLOCK = Menu::Window.SkinchangerTab.GLOCKSkin.GetIndex();
							int USPS = Menu::Window.SkinchangerTab.USPSSkin.GetIndex();
							int DEAGLE = Menu::Window.SkinchangerTab.DEAGLESkin.GetIndex();
							int FIVE7 = Menu::Window.SkinchangerTab.FIVESEVENSkin.GetIndex();
							int AUG = Menu::Window.SkinchangerTab.AUGSkin.GetIndex();
							int FAMAS = Menu::Window.SkinchangerTab.FAMASSkin.GetIndex();
							int G3SG1 = Menu::Window.SkinchangerTab.G3SG1Skin.GetIndex();
							int Galil = Menu::Window.SkinchangerTab.GALILSkin.GetIndex();
							int M249 = Menu::Window.SkinchangerTab.M249Skin.GetIndex();
							int MAC10 = Menu::Window.SkinchangerTab.MAC10Skin.GetIndex();
							int P90 = Menu::Window.SkinchangerTab.P90Skin.GetIndex();
							int UMP45 = Menu::Window.SkinchangerTab.UMP45Skin.GetIndex();
							int XM1014 = Menu::Window.SkinchangerTab.XM1014Skin.GetIndex();
							int BIZON = Menu::Window.SkinchangerTab.BIZONSkin.GetIndex();
							int MAG7 = Menu::Window.SkinchangerTab.MAG7Skin.GetIndex();
							int NEGEV = Menu::Window.SkinchangerTab.NEGEVSkin.GetIndex();
							int SAWEDOFF = Menu::Window.SkinchangerTab.SAWEDOFFSkin.GetIndex();
							int TEC9 = Menu::Window.SkinchangerTab.TECNINESkin.GetIndex();
							int P2000 = Menu::Window.SkinchangerTab.P2000Skin.GetIndex();
							int MP7 = Menu::Window.SkinchangerTab.MP7Skin.GetIndex();
							int MP9 = Menu::Window.SkinchangerTab.MP9Skin.GetIndex();
							int NOVA = Menu::Window.SkinchangerTab.NOVASkin.GetIndex();
							int P250 = Menu::Window.SkinchangerTab.P250Skin.GetIndex();
							int SCAR20 = Menu::Window.SkinchangerTab.SCAR20Skin.GetIndex();
							int SG553 = Menu::Window.SkinchangerTab.SG553Skin.GetIndex();
							int SSG08 = Menu::Window.SkinchangerTab.SSG08Skin.GetIndex();
							int Magnum = Menu::Window.SkinchangerTab.RevolverSkin.GetIndex();
							int Duals = Menu::Window.SkinchangerTab.DUALSSkin.GetIndex();

							if (pEntity->GetClientClass()->m_ClassID != (int)CSGOClassID::CKnife)
							{
								if (Menu::Window.SkinchangerTab.SkinName.getText().length() > 1)
								{
									auto pCustomName = MakePtr(char*, pWeapon, 0x301C);
									strcpy_s(pCustomName, 32, Menu::Window.SkinchangerTab.SkinName.getText().c_str());
								}
							}

							if (Menu::Window.SkinchangerTab.StatTrackAmount.getText().c_str() != NULL && Menu::Window.SkinchangerTab.StatTrackAmount.getText().c_str() != "")
							{
								int st = atoi(Menu::Window.SkinchangerTab.StatTrackAmount.getText().c_str());

								if (Menu::Window.SkinchangerTab.StatTrakEnable.GetState())
									*pWeapon->FallbackStatTrak() = st;
							}
							CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)pEntity;
							if (GameUtils::IsG(pWeapon))
								return;

							int weapon = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();//crash with nade?

							switch (weapon)
							{
							case 7: // AK47 
							{
								switch (AK47)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 341;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 14;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 44;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 172;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 180;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 394;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 300;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 226;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 282;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 302;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 316;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 340;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 380;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 656;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 456;
									break;
								case 15:
									*pWeapon->FallbackPaintKit() = 474;
									break;
								case 16:
									*pWeapon->FallbackPaintKit() = 490;
									break;
								case 17:
									*pWeapon->FallbackPaintKit() = 506;
									break;
								case 18:
									*pWeapon->FallbackPaintKit() = 524;
									break;
								case 19:
									*pWeapon->FallbackPaintKit() = 600;
									break;
								case 20:
									*pWeapon->FallbackPaintKit() = 639;
									break;
								default:
									break;
								}
							}
							break;
							case 16: // M4A4
							{
								switch (M4A4)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 155;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 187;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 255;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 309;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 215;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 336;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 384;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 400;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 449;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 471;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 480;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 512;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 533;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 588;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 632;
									break;
								case 15:
									*pWeapon->FallbackPaintKit() = 664;
									break;
								default:
									break;
								}
							}
							break;
							case 60:
							{
								switch (M41S)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 60;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 430;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 644;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 235;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 254;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 189;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 301;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 217;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 257;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 321;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 326;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 360;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 383;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 440;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 445;
									break;
								case 15:
									*pWeapon->FallbackPaintKit() = 497;
									break;
								case 16:
									*pWeapon->FallbackPaintKit() = 548;
									break;
								case 17:
									*pWeapon->FallbackPaintKit() = 587;
									break;
								case 18:
									*pWeapon->FallbackPaintKit() = 631;
									break;
								default:
									break;
								}
							}
							break;
							case 9:
							{
								switch (AWP)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 174;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 344;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 84;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 640;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 51;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 181;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 259;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 395;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 212;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 227;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 251;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 279;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 662;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 446;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 451;
									break;
								case 15:
									*pWeapon->FallbackPaintKit() = 475;
									break;
								case 16:
									*pWeapon->FallbackPaintKit() = 525;
									break;
								default:
									break;
								}
							}
							break;
							case 61:
							{
								switch (USPS)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 60;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 183;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 339;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 217;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 221;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 653;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 277;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 290;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 313;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 318;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 332;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 364;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 454;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 489;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 504;
									break;
								case 15:
									*pWeapon->FallbackPaintKit() = 540;
									break;
								case 16:
									*pWeapon->FallbackPaintKit() = 637;
									break;
								default:
									break;
								}
							}
							break;
							case 4:
							{
								switch (GLOCK)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 586;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 437;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 38;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 48;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 353;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 532;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 381;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 367;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 607;
									break;
								default:
									break;
								}
							}
							break;
							case 1: // Deagle
							{
								switch (DEAGLE)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 37;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 645;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 527;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 469;
									break;
								default:
									break;
								}
							}
							break;
							case 2: //duals
							{
								switch (Duals)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 491;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 28;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 447;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 220;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 261;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 396;
									break;
								default:
									break;
								}
							}
							break;
							case 3: // Five Seven
							{
								switch (FIVE7)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 44;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 352;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 510;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 530;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 464;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 427;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 274;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 660;
									break;
								default:
									break;
								}
							}
							break;
							case 8: // AUG
							{
								switch (AUG)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 9;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 33;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 280;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 305;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 375;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 442;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 444;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 455;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 507;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 541;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 601;
									break;
								default:
									break;
								}
							}
							break;
							case 10: // Famas
							{
								switch (FAMAS)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 429;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 154;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 178;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 194;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 244;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 218;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 260;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 288;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 371;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 477;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 492;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 529;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 604;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 626;
									break;
								default:
									break;
								}
							}
							break;
							case 11: // G3SG1
							{
								switch (G3SG1)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 195;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 229;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 294;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 465;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 464;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 382;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 438;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 493;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 511;
									break;
								default:
									break;
								}
							}
							break;
							case 13: // Galil
							{
								switch (Galil)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 83;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 428;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 494;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 379;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 460;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 398;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 546;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 478;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 661;
									break;
								default:
									break;
								}
							}
							break;
							case 14: // M249
							{
								switch (M249)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 401;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 452;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 472;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 496;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 547;
									break;
								default:
									break;
								}
							}
							break;
							case 17: // Mac 10
							{
								switch (MAC10)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 38;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 433;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 98;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 157;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 188;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 337;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 246;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 284;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 310;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 333;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 343;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 372;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 402;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 498;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 534;
									break;
								default:
									break;
								}
							}
							break;
							case 19: // P90
							{
								switch (P90)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 67;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 156;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 516;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 182;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 359;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 20;
									break;
								default:
									break;
								}
							}
							break;
							case 24: // UMP-45
							{
								switch (UMP45)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 37;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 556;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 441;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 436;
									break;
								default:
									break;
								}
							}
							break;
							case 25: // XM1014
							{
								switch (XM1014)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 393;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 521;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 505;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 314;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 407;
									break;
								default:
									break;
								}
							}
							break;
							case 26: // Bizon
							{
								switch (BIZON)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 13;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 267;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 542;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 349;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 508;
									break;
								default:
									break;
								}
							}
							break;
							case 27: // Mag 7
							{
								switch (MAG7)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 462;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 39;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 431;
									break;
								default:
									break;
								}
							}
							break;
							case 28: // Negev
							{
								switch (NEGEV)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 28;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 432;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 317;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 355;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 369;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 483;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 514;
									break;
								default:
									break;
								}
							}
							break;
							case 29: // Sawed Off
							{
								switch (SAWEDOFF)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 405;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 83;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 38;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 256;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 638;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 390;
									break;
								default:
									break;
								}
							}
							break;
							case 30: // Tec 9
							{
								switch (TEC9)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 463;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 303;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 248;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 520;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 374;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 614;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 555;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 459;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 179;
									break;
								default:
									break;
								}
							}
							break;
							case 32: // P2000
							{
								switch (P2000)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 485;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 184;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 211;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 389;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 591;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 246;
									break;
								default:
									break;
								}
							}
							break;
							case 33: // MP7
							{
								switch (MP7)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 102;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 481;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 536;
									break;
								default:
									break;
								}
							}
							break;
							case 34: // MP9
							{
								switch (MP9)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 482;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 33;
									break;
								default:
									break;
								}
							}
							break;
							case 35: // Nova
							{
								switch (NOVA)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 3;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 166;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 164;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 25;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 62;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 99;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 107;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 158;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 170;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 191;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 214;
									break;
								case 11:
									*pWeapon->FallbackPaintKit() = 225;
									break;
								case 12:
									*pWeapon->FallbackPaintKit() = 263;
									break;
								case 13:
									*pWeapon->FallbackPaintKit() = 286;
									break;
								case 14:
									*pWeapon->FallbackPaintKit() = 294;
									break;
								case 15:
									*pWeapon->FallbackPaintKit() = 299;
									break;
								case 16:
									*pWeapon->FallbackPaintKit() = 356;
									break;
								case 17:
									*pWeapon->FallbackPaintKit() = 450;
									break;
								case 18:
									*pWeapon->FallbackPaintKit() = 484;
									break;
								case 19:
									*pWeapon->FallbackPaintKit() = 537;
									break;
								default:
									break;
								}
							}
							break;
							case 36: // P250
							{
								switch (P250)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 102;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 168;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 162;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 258;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 551;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 271;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 295;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 358;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 388;
									break;
								default:
									break;
								}
							}
							break;
							case 38: // Scar 20
							{
								switch (SCAR20)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 165;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 196;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 232;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 391;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 597;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 312;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 406;
									break;
									break;
								}
							}
							break;
							case 39: // SG553
							{
								switch (SG553)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 39;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 98;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 410;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 347;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 287;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 298;
									break;
								case 6:
									*pWeapon->FallbackPaintKit() = 363;
									break;
								case 7:
									*pWeapon->FallbackPaintKit() = 378;
									break;
								case 8:
									*pWeapon->FallbackPaintKit() = 487;
									break;
								case 9:
									*pWeapon->FallbackPaintKit() = 519;
									break;
								case 10:
									*pWeapon->FallbackPaintKit() = 553;
									break;
								default:
									break;
								}
							}
							break;
							case 40: // SSG08
							{
								switch (SSG08)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 624;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 222;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 554;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 319;
									break;
								case 4:
									*pWeapon->FallbackPaintKit() = 361;
									break;
								case 5:
									*pWeapon->FallbackPaintKit() = 503;
									break;
								default:
									break;
								}
							}
							break;
							case 64: // Revolver
							{
								switch (Magnum)
								{
								case 0:
									*pWeapon->FallbackPaintKit() = 522;
									break;
								case 1:
									*pWeapon->FallbackPaintKit() = 12;
									break;
								case 2:
									*pWeapon->FallbackPaintKit() = 523;
									break;
								case 3:
									*pWeapon->FallbackPaintKit() = 595;
									break;
								default:
									break;
								}
							}
							break;
							default:
								break;
							}

							if (pEntity->GetClientClass()->m_ClassID == (int)CSGOClassID::CKnife)
							{
								auto pCustomName1 = MakePtr(char*, pWeapon, 0x301C);
								if (Menu::Window.SkinchangerTab.KnifeName.getText().length() > 1)
								{
									strcpy_s(pCustomName1, 32, Menu::Window.SkinchangerTab.KnifeName.getText().c_str());
								}

								if (Model == 0) // Bayonet
								{
									*pWeapon->ModelIndex() = iBayonet;
									*pWeapon->ViewModelIndex() = iBayonet;
									*pWeapon->WorldModelIndex() = iBayonet + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 500;
									*pWeapon->GetEntityQuality() = 3;
									*pWeapon->FallbackPaintKit() = atoi(Menu::Window.MiscTab.KnifeSkin.getText().c_str());
								}
								else if (Model == 1) // Bowie Knife
								{
									*pWeapon->ModelIndex() = iBowie;
									*pWeapon->ViewModelIndex() = iBowie;
									*pWeapon->WorldModelIndex() = iBowie + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 514;
									*pWeapon->GetEntityQuality() = 3;
									*pWeapon->FallbackPaintKit() = atoi(Menu::Window.MiscTab.KnifeSkin.getText().c_str());
								}
								else if (Model == 2) // Butterfly Knife
								{
									*pWeapon->ModelIndex() = iButterfly;
									*pWeapon->ViewModelIndex() = iButterfly;
									*pWeapon->WorldModelIndex() = iButterfly + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 515;
									*pWeapon->GetEntityQuality() = 3;
									*pWeapon->FallbackPaintKit() = atoi(Menu::Window.MiscTab.KnifeSkin.getText().c_str());
								}
								else if (Model == 3) // Falchion Knife
								{
									*pWeapon->ModelIndex() = iFalchion;
									*pWeapon->ViewModelIndex() = iFalchion;
									*pWeapon->WorldModelIndex() = iFalchion + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 512;
									*pWeapon->GetEntityQuality() = 3;
									*pWeapon->FallbackPaintKit() = atoi(Menu::Window.MiscTab.KnifeSkin.getText().c_str());
								}
								else if (Model == 4) // Flip Knife
								{
									*pWeapon->ModelIndex() = iFlip;
									*pWeapon->ViewModelIndex() = iFlip;
									*pWeapon->WorldModelIndex() = iFlip + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 505;
									*pWeapon->FallbackPaintKit() = atoi(Menu::Window.MiscTab.KnifeSkin.getText().c_str());
								}
								else if (Model == 5) // Gut Knife
								{
									*pWeapon->ModelIndex() = iGut;
									*pWeapon->ViewModelIndex() = iGut;
									*pWeapon->WorldModelIndex() = iGut + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 506;
									*pWeapon->GetEntityQuality() = 3;
									*pWeapon->FallbackPaintKit() = atoi(Menu::Window.MiscTab.KnifeSkin.getText().c_str());
								}
								else if (Model == 6) // Huntsman Knife
								{
									*pWeapon->ModelIndex() = iHuntsman;
									*pWeapon->ViewModelIndex() = iHuntsman;
									*pWeapon->WorldModelIndex() = iHuntsman + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 509;
									*pWeapon->GetEntityQuality() = 3;
									*pWeapon->FallbackPaintKit() = atoi(Menu::Window.MiscTab.KnifeSkin.getText().c_str());
								}
								else if (Model == 7) // Karambit
								{
									*pWeapon->ModelIndex() = iKarambit;
									*pWeapon->ViewModelIndex() = iKarambit;
									*pWeapon->WorldModelIndex() = iKarambit + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 507;
									*pWeapon->GetEntityQuality() = 3;
									*pWeapon->FallbackPaintKit() = atoi(Menu::Window.MiscTab.KnifeSkin.getText().c_str());
								}
								else if (Model == 8) // M9 Bayonet
								{
									*pWeapon->ModelIndex() = iM9Bayonet;
									*pWeapon->ViewModelIndex() = iM9Bayonet;
									*pWeapon->WorldModelIndex() = iM9Bayonet + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 508;
									*pWeapon->GetEntityQuality() = 3;
									*pWeapon->FallbackPaintKit() = atoi(Menu::Window.MiscTab.KnifeSkin.getText().c_str());
								}
								else if (Model == 10) // Shadow Daggers
								{
									*pWeapon->ModelIndex() = iDagger;
									*pWeapon->ViewModelIndex() = iDagger;
									*pWeapon->WorldModelIndex() = iDagger + 1;
									*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() = 516;
									*pWeapon->GetEntityQuality() = 3;
									*pWeapon->FallbackPaintKit() = atoi(Menu::Window.MiscTab.KnifeSkin.getText().c_str());
								}
							}

							*pWeapon->OwnerXuidLow() = 0;//crash?
							*pWeapon->OwnerXuidHigh() = 0;
							*pWeapon->FallbackWear() = 0.001f;
							*pWeapon->m_AttributeManager()->m_Item()->ItemIDHigh() = 1;

						}
					}
				}

			}/*
			for (int i = 1; i <= Interfaces::Engine->GetMaxClients(); ++i)
			{
				IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);

				if (!pEntity || !pLocal) {
					continue;
				}

				if (pEntity->GetTeamNum() == pLocal->GetTeamNum()) {
					continue;
				}

				if (pEntity->IsDormant() || !pLocal->IsAlive() || !pEntity->IsAlive()) {
					continue;
				}
				//lagComp->log(curStage);
			}*/
		}
	}

	oFrameStageNotify(curStage);
}


void __fastcall Hooked_OverrideView(void* ecx, void* edx, CViewSetup* pSetup)
{
	IClientEntity* pLocal = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
	{
		if (Menu::Window.VisualsTab.Active.GetState() && pLocal->IsAlive() && !pLocal->IsScoped())
		{
			if (pSetup->fov = 90)
				pSetup->fov = Menu::Window.MiscTab.OtherFOV.GetValue();
		}
		oOverrideView(ecx, edx, pSetup);
	}

}

void GetViewModelFOV(float& fov)
{
	IClientEntity* localplayer = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
	{

		if (!localplayer)
			return;


		if (Menu::Window.VisualsTab.Active.GetState())
		fov += Menu::Window.VisualsTab.OtherViewmodelFOV.GetValue();
	}
}

float __stdcall GGetViewModelFOV()
{
	float fov = Hooks::VMTClientMode.GetMethod<oGetViewModelFOV>(35)();

	GetViewModelFOV(fov);

	return fov;
}

void __fastcall Hooked_RenderView(void* ecx, void* edx, CViewSetup &setup, CViewSetup &hudViewSetup, int nClearFlags, int whatToDraw)
{
	static DWORD oRenderView = Hooks::VMTRenderView.GetOriginalFunction(6);

	__asm
	{
		PUSH whatToDraw
		PUSH nClearFlags
		PUSH hudViewSetup
		PUSH setup
		MOV ECX, ecx
		CALL oRenderView
	}
} //hooked for no reason yay

MDLHandle_t __fastcall hkFindMDL(void* ecx, void* edx, char* FilePath) // if we want to use custom models
{
	//removed models so we dont have to deal with memes rn
	return oFindMDL(ecx, FilePath);
}