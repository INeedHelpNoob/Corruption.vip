#include <array>
#include <string>
#include <deque>
#include <algorithm>
#include "Entities.h"
#include "CommonIncludes.h"
#include "Entities.h"
#include "Vector.h"
#include <map>
#include "Interfaces.h"
#include "Hacks.h"
#include "Hooks.h"
extern bool islbyupdate;
extern 	std::vector<int> HitBoxesToScan;

class ResolverSetup
{
public:
	bool didhit;
	void FSN(IClientEntity* pEntity, ClientFrameStage_t stage); //used to get the fresh THINGS and the resolve
	void CM(IClientEntity* pEntity); //cause this is slower than FSN so we are going to get older info and not updated at all ez to compare between etc.
	void Resolve(IClientEntity* pEntity); //resolve
	void StoreThings(IClientEntity* pEntity);
	void StoreThings2(IClientEntity* pEntity);
	std::map<int, QAngle>StoredAngles; //y and x lol (stored)
	std::map<int, QAngle>NewANgles; //y and x lol (new)
	std::map<int, float>storedlby;
	std::map<int, float>newlby;
	std::map<int, float>storeddelta;
	std::map<int, float>newdelta;
	std::map<int, float>finaldelta;
	std::map<int, float>storedlbydelta;
	std::map<int, float>newlbydelta;
	std::map<int, float>finallbydelta;
	std::map<int, float>final1;
	float newsimtime;
	float storedsimtime;
	float storedlbyFGE;
	float storedanglesFGE;
	float storedsimtimeFGE;
	bool didhitHS;
	void StoreFGE(IClientEntity* pEntity);
	static ResolverSetup GetInst()
	{
		static ResolverSetup instance;
		return instance;
	}
};
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