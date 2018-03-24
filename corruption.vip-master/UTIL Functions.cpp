#include "MathFunctions.h"
#include "UTIL Functions.h"
#include "Utilities.h"
#include "Menu.h"
#include "Hacks.h"
#include "Autowall.h"
#include "RenderManager.h"

ServerRankRevealAllFn GameUtils::ServerRankRevealAllEx;
IsReadyFn GameUtils::IsReady;

DWORD GameUtils::FindPattern1(std::string moduleName, std::string pattern)
{
	const char* pat = pattern.c_str();
	DWORD firstMatch = 0;
	DWORD rangeStart = (DWORD)GetModuleHandleA(moduleName.c_str());
	MODULEINFO miModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
	DWORD rangeEnd = rangeStart + miModInfo.SizeOfImage;
	for (DWORD pCur = rangeStart; pCur < rangeEnd; pCur++)
	{
		if (!*pat)
			return firstMatch;

		if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat))
		{
			if (!firstMatch)
				firstMatch = pCur;

			if (!pat[2])
				return firstMatch;

			if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
				pat += 3;

			else
				pat += 2;    //one ?
		}
		else
		{
			pat = pattern.c_str();
			firstMatch = 0;
		}
	}
	return NULL;
}

void UTIL_TraceLine(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask,const IClientEntity *ignore, int collisionGroup, trace_t *ptr)
{
	typedef int(__fastcall* UTIL_TraceLine_t)(const Vector&, const Vector&, unsigned int, const IClientEntity*, int, trace_t*);
	static UTIL_TraceLine_t TraceLine = (UTIL_TraceLine_t)Utilities::Memory::FindPattern("client.dll", (PBYTE)"\x55\x8B\xEC\x83\xE4\xF0\x83\xEC\x7C\x56\x52", "xxxxxxxxxxx");
	TraceLine(vecAbsStart, vecAbsEnd, mask, ignore, collisionGroup, ptr);
}

bool GameUtils::IsRevolver(void* weapon)
{
	if (weapon == nullptr) return false;
	CBaseCombatWeapon *pWeapon = (CBaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_REVOLVER };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool GameUtils::IsPistol(void* weapon)
{
	if (weapon == nullptr) return false;
	CBaseCombatWeapon *pWeapon = (CBaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_DEAGLE,WEAPON_CZ75A,WEAPON_ELITE,WEAPON_USP_SILENCER,WEAPON_P250,WEAPON_HKP2000, WEAPON_TEC9,WEAPON_REVOLVER,WEAPON_FIVESEVEN,WEAPON_GLOCK };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool GameUtils::IsSniper(void* weapon)
{
	if (weapon == nullptr) return false;
	CBaseCombatWeapon *pWeapon = (CBaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_AWP,WEAPON_SSG08,WEAPON_G3SG1,WEAPON_SCAR20 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool GameUtils::IsShotgun(void* weapon)
{
	if (weapon == nullptr) return false;
	IClientEntity* weaponEnt = (IClientEntity*)weapon;
	ClientClass* pWeaponClass = weaponEnt->GetClientClass();

	if (pWeaponClass->m_ClassID == (int)CSGOClassID::CWeaponMag7 || pWeaponClass->m_ClassID == (int)CSGOClassID::CWeaponXM1014 || pWeaponClass->m_ClassID == (int)CSGOClassID::CWeaponSawedoff || pWeaponClass->m_ClassID == (int)CSGOClassID::CWeaponNOVA)
		return true;
	else
		return false;
}

bool GameUtils::IsMachinegun(void* weapon)
{
	if (weapon == nullptr) return false;
	IClientEntity* weaponEnt = (IClientEntity*)weapon;
	ClientClass* pWeaponClass = weaponEnt->GetClientClass();

	if (pWeaponClass->m_ClassID == (int)CSGOClassID::CWeaponNegev || pWeaponClass->m_ClassID == (int)CSGOClassID::CWeaponM249)
		return true;
	else
		return false;
}

bool GameUtils::IsMP(void* weapon)
{
	if (weapon == nullptr) return false;
	IClientEntity* weaponEnt = (IClientEntity*)weapon;
	ClientClass* pWeaponClass = weaponEnt->GetClientClass();

	if (pWeaponClass->m_ClassID == (int)CSGOClassID::CWeaponMAC10 || pWeaponClass->m_ClassID == (int)CSGOClassID::CWeaponP90 || pWeaponClass->m_ClassID == (int)CSGOClassID::CWeaponUMP45 || pWeaponClass->m_ClassID == (int)CSGOClassID::CWeaponBizon || pWeaponClass->m_ClassID == (int)CSGOClassID::CWeaponMP7 || pWeaponClass->m_ClassID == (int)CSGOClassID::CWeaponMP9)
		return true;
	else
		return false;
}

bool GameUtils::AWP(void* weapon)
{
	if (weapon == nullptr) return false;
	IClientEntity* weaponEnt = (IClientEntity*)weapon;
	ClientClass* pWeaponClass = weaponEnt->GetClientClass();

	if (pWeaponClass->m_ClassID == (int)CSGOClassID::CWeaponAWP)
		return true;
	else
		return false;
}

bool GameUtils::IsScopedWeapon(void* weapon)
{
	if (weapon == nullptr) return false;
	CBaseCombatWeapon *pWeapon = (CBaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_AWP,WEAPON_SSG08,WEAPON_G3SG1,WEAPON_SCAR20, WEAPON_AUG, WEAPON_SG556 };
	return (std::find(v.begin(), v.end(), id) != v.end());
}/*
void UTIL_ClipTraceToPlayers(IClientEntity* pEntity, Vector start, Vector end, unsigned int mask, ITraceFilter* filter, trace_t* tr)
{
	trace_t playerTrace;
	Ray_t ray;
	float smallestFraction = tr->fraction;

	ray.Init(start, end);

	if (!pEntity || !pEntity->IsAlive() || pEntity->IsDormant())
		return;

	if (filter && filter->ShouldHitEntity(pEntity, mask) == false)
		return;

	Interfaces::Trace->ClipRayToEntity(ray, mask | CONTENTS_HITBOX, pEntity, &playerTrace);//this shit is fucked the clipraytoentity
	if (playerTrace.fraction < smallestFraction)
	{
		// we shortened the ray - save off the trace
		*tr = playerTrace;
		smallestFraction = playerTrace.fraction;
	}
}*/

// OLD
void UTIL_ClipTraceToPlayers(const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr)
{
	static DWORD dwAddress = Utilities::Memory::FindPattern("client.dll", (BYTE*)"\x53\x8B\xDC\x83\xEC\x08\x83\xE4\xF0\x83\xC4\x04\x55\x8B\x6B\x04\x89\x6C\x24\x04\x8B\xEC\x81\xEC\x00\x00\x00\x00\x8B\x43\x10", "xxxxxxxxxxxxxxxxxxxxxxxx????xxx");

	if (!dwAddress)
		return;

	_asm
	{
		MOV		EAX, filter
		LEA		ECX, tr
			PUSH	ECX
			PUSH	EAX
			PUSH	mask
			LEA		EDX, vecAbsEnd
			LEA		ECX, vecAbsStart
			CALL	dwAddress
			ADD		ESP, 0xC
	}
}

bool GameUtils::IsG(void* weapon)
{
	if (weapon == nullptr) return false;
	CBaseCombatWeapon *pWeapon = (CBaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	static const std::vector<int> v = { WEAPON_FLASHBANG,WEAPON_HEGRENADE,WEAPON_SMOKEGRENADE,WEAPON_MOLOTOV,WEAPON_DECOY,WEAPON_INCGRENADE };
	return (std::find(v.begin(), v.end(), id) != v.end());
}

bool IsBreakableEntity(IClientEntity* ent)
{
	typedef bool(__thiscall* IsBreakbaleEntity_t)(IClientEntity*);
	IsBreakbaleEntity_t IsBreakbaleEntityFn = (IsBreakbaleEntity_t)Utilities::Memory::FindPattern("client.dll", (PBYTE)"\x55\x8B\xEC\x51\x56\x8B\xF1\x85\xF6\x74\x68", "xxxxxxxxxxx");
	if (IsBreakbaleEntityFn)
		return IsBreakbaleEntityFn(ent);
	else
		return false;
}

bool  TraceToExit(Vector& end, trace_t& tr, Vector start, Vector vEnd, trace_t* trace)
{
	typedef bool(__fastcall* TraceToExitFn)(Vector&, trace_t&, float, float, float, float, float, float, trace_t*);
	static TraceToExitFn TraceToExit = (TraceToExitFn)Utilities::Memory::FindPattern("client.dll", (BYTE*)"\x55\x8B\xEC\x83\xEC\x30\xF3\x0F\x10\x75", "xxxxxxxxxx");

	if (!TraceToExit)
		return false;

	return TraceToExit(end, tr, start.x, start.y, start.z, vEnd.x, vEnd.y, vEnd.z, trace);
}
DWORD dwEntBreakable = NULL;/*
bool TraceToExit(Vector& vecEnd, trace_t& pEnterTrace, Vector vecStart, Vector vecDir, trace_t* pExitTrace)
{
	auto distance = 0.0f;
	auto DidHitNonWorldCBaseEntity = [](IClientEntity* m_pEnt) -> bool
	{
		return m_pEnt != nullptr && m_pEnt == Interfaces::EntList->GetClientEntity(0);
	};

	while (distance <= 90.0f) {
		distance += 4.0f;
		vecEnd = vecStart + vecDir * distance;

		auto point_contents = Interfaces::Trace->GetPointContents(vecEnd, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr);
		if (point_contents & MASK_SHOT_HULL && (!(point_contents & CONTENTS_HITBOX)))
			continue;

		auto new_end = vecEnd - (vecDir * 4.0f);

		Ray_t ray;
		ray.Init(vecEnd, new_end);
		Interfaces::Trace->TraceRay(ray, MASK_SHOT | CONTENTS_GRATE, nullptr, pExitTrace);

		if (pExitTrace->m_pEnt == nullptr)
			return false;

		if (pExitTrace->startsolid && pExitTrace->surface.flags & SURF_HITBOX) {
			CTraceFilter filter;
			filter.pSkip = pExitTrace->m_pEnt;

			ray.Init(vecEnd, vecStart);
			Interfaces::Trace->TraceRay(ray, 0x600400B, &filter, pExitTrace);

			if ((pExitTrace->fraction < 1.0f || pExitTrace->allsolid) && !pExitTrace->startsolid) {
				vecEnd = pExitTrace->endpos;
				return true;
			}

			continue;
		}

		if (!(pExitTrace->fraction < 1.0 || pExitTrace->allsolid || pExitTrace->startsolid) || pExitTrace->startsolid) {
			if (pExitTrace->m_pEnt) {

				IClientEntity* pBreakEnt = pExitTrace->m_pEnt;

				if (dwEntBreakable == NULL) {
					dwEntBreakable = Utilities::Memory::FindPatternV2("client.dll", "55 8B EC 51 56 8B F1 85 F6 74 68");
				}
				bool bBreakable;

				__asm
				{
					MOV ECX, pBreakEnt
					CALL dwEntBreakable
					MOV bBreakable, AL
				}


				if (!DidHitNonWorldCBaseEntity(pExitTrace->m_pEnt) && bBreakable)
					return true;
			}

			continue;
		}

		if (((pExitTrace->surface.flags >> 7) & 1) && !((pExitTrace->surface.flags >> 7) & 1))
			continue;

		if (pExitTrace->plane.normal.Dot(vecDir) < 1.1f) {
			float fraction = pExitTrace->fraction * 4.0f;
			vecEnd = vecEnd - (vecDir * fraction);

			return true;
		}
	}

	return false;
}
*/
void GameUtils::NormaliseViewAngle(Vector &angle)
{
	if (!Menu::Window.MiscTab.OtherSafeMode.GetState())
	{
		return;
	}
	else
	{
		while (angle.y <= -180) angle.y += 360;
		while (angle.y > 180) angle.y -= 360;
		while (angle.x <= -180) angle.x += 360;
		while (angle.x > 180) angle.x -= 360;


		if (angle.x > 89) angle.x = 89;
		if (angle.x < -89) angle.x = -89;
		if (angle.y < -180) angle.y = -179.999;
		if (angle.y > 180) angle.y = 179.999;

		angle.z = 0;
	}
}

void GameUtils::CL_FixMove(CUserCmd* pCmd, Vector viewangles)
{
	/*pCmd->forwardmove = DotProduct(forward * vForwardNorm, aimforward) + DotProduct(right * vRightNorm, aimforward) + DotProduct(up * vUpNorm, aimforward);
	pCmd->sidemove = DotProduct(forward * vForwardNorm, aimright) + DotProduct(right * vRightNorm, aimright) + DotProduct(up * vUpNorm, aimright);
	pCmd->upmove = DotProduct(forward * vForwardNorm, aimup) + DotProduct(right * vRightNorm, aimup) + DotProduct(up * vUpNorm, aimup);*/
}

char shit[16];
trace_t Trace;
char shit2[16];
IClientEntity* entCopy;

bool GameUtils::IsVisible(IClientEntity* pLocal, IClientEntity* pEntity, int BoneID)
{
	if (BoneID < 0) return false;

	entCopy = pEntity;
	Vector start = pLocal->GetOrigin() + pLocal->GetViewOffset();
	Vector end = GetHitboxPosition(pEntity, BoneID);//pEntity->GetBonePos(BoneID);
	char shit3[32];
	
	//Interfaces::Trace->TraceRay(Ray,MASK_SOLID, NULL/*&filter*/, &Trace);
	UTIL_TraceLine(start, end, MASK_SOLID, pLocal, 0, &Trace);

	if (Trace.m_pEnt == entCopy)
	{
		return true;
	}

	if (Trace.fraction == 1.0f)
	{
		return true;
	}

	return false;

}

bool GameUtils::IsBallisticWeapon(void* weapon)
{
	if (weapon == nullptr) return false;
	CBaseCombatWeapon *pWeapon = (CBaseCombatWeapon*)weapon;
	int id = *pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex();
	return (id >= WEAPON_KNIFE_CT && id <= WEAPON_KNIFE_T || id == 0 || id >= WEAPON_KNIFE_BAYONET);
}


void SayInChat(const char *text)
{
	char buffer[250];
	sprintf_s(buffer, "say \"%s\"", text);
	Interfaces::Engine->ClientCmd_Unrestricted(buffer);
}

void SayInTeamChat(const char *text)
{
	char buffer[250];
	sprintf_s(buffer, "say_team \"%s\"", text);
	Interfaces::Engine->ClientCmd_Unrestricted(buffer);
}
/*
Vector GetHitboxPosition(IClientEntity* pEntity, int Hitbox)
{
	matrix3x4_t matrix[128];

	if (pEntity->SetupBones(matrix, 128, 0x100, 0.f))
	{
		studiohdr_t* hdr = Interfaces::ModelInfo->GetStudiomodel(pEntity->GetModel());
		mstudiohitboxset_t* set = hdr->GetHitboxSet(0);

		mstudiobbox_t* hitbox = set->GetHitbox(Hitbox);

		if (hitbox)
		{
			Vector vMin, vMax, vCenter, sCenter;
			VectorTransform(hitbox->bbmin, matrix[hitbox->bone], vMin);
			VectorTransform(hitbox->bbmax, matrix[hitbox->bone], vMax);

			vCenter = ((vMin + vMax) *0.5f);

			return vCenter;
		}
	}

	return Vector(0, 0, 0);
}*/

/*
bool GetBestPoint(IClientEntity* pEntity, Hitbox* hitbox, BestPoint *point)
{
	Vector center = hitbox->points[0];
	std::vector<int> HitBoxesToScan;

	if (hitbox->hitbox == ((int)CSGOHitboxID::Head))
	{
		Vector high = ((hitbox->points[3] + hitbox->points[5]) * .5f);

		float pitch = pEntity->GetEyeAngles().x;
		if ((pitch > 0.f) && (pitch <= 89.f))
		{
			Vector height = (((high - hitbox->points[0]) / 3) * 4);
			Vector new_height = (hitbox->points[0] + (height * (pitch / 89.f)));

			hitbox->points[0] = new_height;
			point->flags |= FL_HIGH;
		}
		else if ((pitch < 292.5f) && (pitch >= 271.f))
		{
			hitbox->points[0] -= Vector(0.f, 0.f, 1.f);
		point->flags |= FL_LOW;
		}
	}

	for (int index = 0; index <= 8; ++index)
	{
		int temp_damage = GetDamage(hitbox->points[index]);

		if ((point->dmg < temp_damage))
		{
			point->dmg = temp_damage;
			point->point = hitbox->points[index];
			point->index = index;
		}
	}

	return (point->dmg > Menu::Window.RageBotTab.AimbotMinimumDamage.GetValue());
}*/
/* WORKING BUT GLITCHED TO HELL
Vector GetHitboxPosition(IClientEntity* pEntity, int Hitbox)
{
	matrix3x4_t matrix[128];

	if (pEntity->SetupBones(matrix, 128, 0x00000100, GetTickCount64()))
	{
		studiohdr_t* hdr = Interfaces::ModelInfo->GetStudiomodel(pEntity->GetModel());
		mstudiohitboxset_t* set = hdr->GetHitboxSet(0);

		mstudiobbox_t* hitbox = set->GetHitbox(Hitbox);

		if (hitbox)
		{	
			Vector points[9] =
			{
				((hitbox->bbmin + hitbox->bbmax) * .5f),
				Vector(hitbox->bbmin.x, hitbox->bbmin.y, hitbox->bbmin.z),
				Vector(hitbox->bbmin.x, hitbox->bbmax.y, hitbox->bbmin.z),
				Vector(hitbox->bbmax.x, hitbox->bbmax.y, hitbox->bbmin.z),
				Vector(hitbox->bbmax.x, hitbox->bbmin.y, hitbox->bbmin.z),
				Vector(hitbox->bbmax.x, hitbox->bbmax.y, hitbox->bbmax.z),
				Vector(hitbox->bbmin.x, hitbox->bbmax.y, hitbox->bbmax.z),
				Vector(hitbox->bbmin.x, hitbox->bbmin.y, hitbox->bbmax.z),
				Vector(hitbox->bbmax.x, hitbox->bbmin.y, hitbox->bbmax.z)
			};

			Vector vPoint = ((hitbox->bbmin + hitbox->bbmax) * .5f);

			for (int index = 0; index <= 8; ++index)
			{
				if (index != 0)
				{
					// scale down the hitbox size
					points[index] = ((((points[index] + points[0]) * .5f) + points[index]) * .5f);
				}

				// transform the vector
				VectorTransform(points[index], matrix[hitbox->bone], hitbox->points[index]);
			}

			return vPoint;
		}
	}

	return Vector(0, 0, 0);
}^/

/*Vector GetHitboxPosition(IClientEntity* pEntity, int Hitbox)
{
	matrix3x4 matrix[128];

	if (pEntity->SetupBones(matrix, 128, 0x00000100, GetTickCount64()))
	{
		studiohdr_t* hdr = Interfaces::ModelInfo->GetStudiomodel(pEntity->GetModel());
		mstudiohitboxset_t* set = hdr->GetHitboxSet(0);

		mstudiobbox_t* hitbox = set->GetHitbox(Hitbox);

		if (hitbox)
		{
			Vector vMin, vMax, vCenter, sCenter;
			VectorTransform(hitbox->bbmin, matrix[hitbox->bone], vMin);
			VectorTransform(hitbox->bbmax, matrix[hitbox->bone], vMax);
			vCenter = (vMin + vMax) *0.5f;
			return vCenter;
		}
	}

	return Vector(0, 0, 0);
}*/


QAngle CalculateAngle(Vector vecOrigin, Vector vecOther)
{
	auto ret = Vector();
	Vector delta = vecOrigin - vecOther;
	double hyp = delta.Length2D();
	ret.y = (atan(delta.y / delta.x) * 57.295779513082f);
	ret.x = (atan(delta.z / hyp) * 57.295779513082f);
	ret[2] = 0.00;

	if (delta.x >= 0.0)
		ret.y += 180.0f;
	return ret;
}
/*
std::vector<Vector> GetMultiplePointsForHitbox(IClientEntity* pBaseEntity, int iHitbox, VMatrix BoneMatrix[128])
{
	std::vector<Vector> vPoints;

	auto VectorTransform_Wrapper = [](const Vector& in1, const VMatrix &in2, Vector &out)
	{
		auto VectorTransform = [](const float *in1, const VMatrix& in2, float *out)
		{
			auto DotProducts = [](const float *v1, const float *v2)
			{
				return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
			};
			out[0] = DotProducts(in1, in2[0]) + in2[0][3];
			out[1] = DotProducts(in1, in2[1]) + in2[1][3];
			out[2] = DotProducts(in1, in2[2]) + in2[2][3];
		};
		VectorTransform(&in1.x, in2, &out.x);
	};
	if (!pBaseEntity)
		return vPoints;

	studiohdr_t* pStudioModel = Interfaces::ModelInfo->GetStudiomodel(pBaseEntity->GetModel());
	mstudiohitboxset_t* set = pStudioModel->GetHitboxSet(0);

	if (!set)
		return vPoints;

	mstudiobbox_t* untransformedBox = set->GetHitbox(iHitbox);
	if (!untransformedBox)
		return vPoints;



	Vector vecMin;
	VectorTransform_Wrapper(untransformedBox->bbmin, BoneMatrix[untransformedBox->bone], vecMin);

	Vector vecMax;
	VectorTransform_Wrapper(untransformedBox->bbmax, BoneMatrix[untransformedBox->bone], vecMax);


	Vector center = (vecMin + vecMax) * 0.5f;


	int type = Menu::Window.RageBotTab.AimbotPointscale.GetValue();
	if (iHitbox == 0 && pBaseEntity->GetEyeAngles().x >= 66 && type > 0) {
		trace_t tr;
		Ray_t ray;
		CTraceCBaseEntity filter;
		//CTraceFilter filter;
		//trace_t Trace;
		filter.pHit = pBaseEntity;
		ray.Init(center + Vector(0, 0, 15), center);
		Interfaces::Trace->TraceRay(ray, MASK_SHOT, &filter, &tr);

		if (type == 1) {
			center.z += ((5.3f / 89) * pBaseEntity->GetEyeAngles().x);
		}
		else {
			if ((tr.endpos.z - center.z) > 1 && (tr.endpos.z - center.z) < 6 && (tr.fraction == 1.f || tr.m_pEnt == pBaseEntity)) {
				center = tr.endpos;
				center.z -= 0.5;//0.7
			}
			else {
				center.z += (5.3 / 89) * pBaseEntity->GetEyeAngles().x;
			}
		}
	}
	vPoints.push_back(center);
	if (!Menu::Window.RageBotTab.AimbotMultipoint.GetState())
		return vPoints;

	//try without multipoint ty
	//return vPoints;

	//float rad = (untransformedBox->radius / 2) * Menu::Window.RageBotTab.AimbotMultipointslide.GetValue();

	/*Vector top = Vector(center.x, center.y, center.z + rad);
	vPoints.push_back(Vector(top.x + rad, top.y + rad, top.z));
	vPoints.push_back(Vector(top.x + rad, top.y - rad, top.z));
	vPoints.push_back(Vector(top.x - rad, top.y + rad, top.z));
	vPoints.push_back(Vector(top.x - rad, top.y - rad, top.z));
	Vector bot = Vector(center.x, center.y, center.z - rad);
	vPoints.push_back(Vector(bot.x + rad, bot.y + rad, bot.z));
	vPoints.push_back(Vector(bot.x + rad, bot.y - rad, bot.z));
	vPoints.push_back(Vector(bot.x - rad, bot.y + rad, bot.z));
	vPoints.push_back(Vector(bot.x - rad, bot.y - rad, bot.z));
	if (Menu::Window.RageBotTab.AimbotMultipoint.GetState())
	{
		vPoints.push_back(Vector(center.x, center.y, center.z + rad));
		vPoints.push_back(Vector(center.x + rad, center.y, center.z));
		vPoints.push_back(Vector(center.x - rad, center.y, center.z));
		vPoints.push_back(Vector(center.x, center.y + rad, center.z));
		vPoints.push_back(Vector(center.x, center.y - rad, center.z));
	}*/
	//return vPoints;
//}
/*
Vector GetHitboxPosition(IClientEntity* pEntity, int Hitbox)
{
	matrix3x4a_t matrix[128];

	//if (pEntity->SetupBones(matrix, 128, 0x100, 0.f))
	//{
		studiohdr_t* hdr = Interfaces::ModelInfo->GetStudiomodel(pEntity->GetModel());
		mstudiohitboxset_t* set = hdr->GetHitboxSet(0);

		mstudiobbox_t* hitbox = set->GetHitbox(Hitbox);
		if (hitbox == nullptr)
			return Vector(0, 0, 0);

			Vector vMin, vMax, vCenter, sCenter;
			VectorTransform(hitbox->bbmin, matrix[hitbox->bone], vMin);
			VectorTransform(hitbox->bbmax, matrix[hitbox->bone], vMax);
			
			vCenter = ((vMin + vMax) *0.5f);

			return vCenter;
	//}

}*/
Vector GetHitboxPosition(IClientEntity* pEntity,int hitbox_id)
{
	studiohdr_t* hdr = Interfaces::ModelInfo->GetStudiomodel(pEntity->GetModel());
	mstudiohitboxset_t* set = hdr->GetHitboxSet(0);
	IClientRenderable* pUnknoown = pEntity->GetClientRenderable();
	if (pUnknoown == nullptr)
		return NULL;
	if (hitbox_id > -1) {
		auto hitbox = set->GetHitbox(hitbox_id);
		if (!hitbox)
			return Vector(0, 0, 0);

		auto vMin = hitbox->bbmin + pEntity->GetBonePos(hitbox->bone);
		auto vMax = hitbox->bbmax + pEntity->GetBonePos(hitbox->bone);

		return (vMin + vMax) * 0.5f;
	}
}

/*
Vector GetHitboxPosition1(IClientEntity* pEntity, int Hitbox)
{
	matrix3x4 matrix[128];
	if (pEntity->SetupBones(matrix, 128, 0x100, 0.f))
	{
		studiohdr_t* hdr = Interfaces::ModelInfo->GetStudiomodel(pEntity->GetModel());
		mstudiohitboxset_t* set = hdr->GetHitboxSet(0);

		mstudiobbox_t* hitbox = set->GetHitbox(Hitbox);
		
		//Points to MultiPoint
		Vector Point[] =
		{
			Vector(hitbox->bbmin.x, hitbox->bbmin.y, hitbox->bbmin.z),
			Vector(hitbox->bbmin.x, hitbox->bbmax.y, hitbox->bbmin.z),
			Vector(hitbox->bbmax.x, hitbox->bbmax.y, hitbox->bbmin.z),
			Vector(hitbox->bbmax.x, hitbox->bbmin.y, hitbox->bbmin.z),
			Vector(hitbox->bbmax.x, hitbox->bbmax.y, hitbox->bbmax.z),
			Vector(hitbox->bbmin.x, hitbox->bbmax.y, hitbox->bbmax.z),
			Vector(hitbox->bbmin.x, hitbox->bbmin.y, hitbox->bbmax.z),
			Vector(hitbox->bbmax.x, hitbox->bbmin.y, hitbox->bbmax.z)
		};
			//Just Hitscan
		Vector vMin, vMax, vCenter, sCenter;

			VectorTransform(hitbox->bbmin, matrix[hitbox->bone], vMin);
			VectorTransform(hitbox->bbmax, matrix[hitbox->bone], vMax);
			/*for (int j = 0; j < 9; j++)
			{
				VectorTransform(Point[j], matrix[hitbox->bone], vCenter);
			}*//*
			vCenter = ((vMin + vMax) *0.5f);
			return vCenter;
			//Multipoint
			if (Menu::Window.RageBotTab.AimbotMultipoint.GetState())
			{
				Vector vTargetPos;
				vTargetPos = vCenter;
				int iCount = 7;
				for (int i = 0; i <= iCount; i++)
				{
					switch (i)
					{
					case 0:
					default:
						vTargetPos = vCenter; break;
					case 1:
						vTargetPos = (Point[7] + Point[1]) * Menu::Window.RageBotTab.AimbotPointscale.GetValue(); break;
					case 2:
						vTargetPos = (Point[3] + Point[4]) * Menu::Window.RageBotTab.AimbotPointscale.GetValue(); break;
					case 3:
						vTargetPos = (Point[4] + Point[0]) * Menu::Window.RageBotTab.AimbotPointscale.GetValue(); break;
					case 4:
						vTargetPos = (Point[2] + Point[7]) * Menu::Window.RageBotTab.AimbotPointscale.GetValue(); break;
					case 5:
						vTargetPos = (Point[6] + Point[7]) * Menu::Window.RageBotTab.AimbotPointscale.GetValue(); break;
					case 6:
						vTargetPos = (Point[5] + Point[3]) * Menu::Window.RageBotTab.AimbotPointscale.GetValue(); break;
					case 7:
						vTargetPos = (Point[1] + Point[2]) * Menu::Window.RageBotTab.AimbotPointscale.GetValue(); break;
					}
					return vTargetPos;
				}
		}
			else
				return vCenter;
	}

	return Vector(0, 0, 0);
}*/

/*Vector GetHitscan(IClientEntity* pEntity, int Hitbox)
{
	matrix3x4 matrix[128];

	if (!pEntity->SetupBones(matrix, 128, 0x00000100, GetTickCount64()))
		return Vector(0, 0, 0);

	studiohdr_t* hdr = Interfaces::ModelInfo->GetStudiomodel(pEntity->GetModel());
	mstudiohitboxset_t* set = hdr->GetHitboxSet(0);

	for (int i = 0; i < 18; i++)
	{
		mstudiobbox_t* hitbox = set->GetHitbox(i);
		if (!hitbox)
			continue;

		Vector vAimPoint;

		Vector corners[17] =
		{
			Vector((hitbox->bbmin + hitbox->bbmax) * 0.5),
			Vector(hitbox->bbmax.x - 1.65, hitbox->bbmax.y - 1.65, hitbox->bbmin.z + 1.65),
			Vector(hitbox->bbmax.x - 1.65, hitbox->bbmax.y - 1.65, hitbox->bbmax.z - 1.65),
			Vector(hitbox->bbmin.x + 1.65, hitbox->bbmax.y - 1.65, hitbox->bbmin.z + 1.65),
			Vector(hitbox->bbmin.x + 1.65, hitbox->bbmax.y - 1.65, hitbox->bbmax.z - 1.65),
			Vector(hitbox->bbmax.x - 1.65, hitbox->bbmin.y + 1.65, hitbox->bbmin.z + 1.65),
			Vector(hitbox->bbmax.x - 1.65, hitbox->bbmin.y + 1.65, hitbox->bbmax.z - 1.65),
			Vector(hitbox->bbmin.x + 1.65, hitbox->bbmin.y + 1.65, hitbox->bbmin.z + 1.65),
			Vector(hitbox->bbmin.x + 1.65, hitbox->bbmin.y + 1.65, hitbox->bbmax.z - 1.65),
			Vector(hitbox->bbmin.x, hitbox->bbmin.y, hitbox->bbmin.z),
			Vector(hitbox->bbmin.x, hitbox->bbmin.y, hitbox->bbmax.z),
			Vector(hitbox->bbmin.x, hitbox->bbmax.y, hitbox->bbmin.z),
			Vector(hitbox->bbmin.x, hitbox->bbmax.y, hitbox->bbmax.z),
			Vector(hitbox->bbmax.x, hitbox->bbmin.y, hitbox->bbmin.z),
			Vector(hitbox->bbmax.x, hitbox->bbmin.y, hitbox->bbmax.z),
			Vector(hitbox->bbmax.x, hitbox->bbmax.y, hitbox->bbmin.z),
			Vector(hitbox->bbmax.x, hitbox->bbmax.y, hitbox->bbmax.z)
		};

		for (int j = 0; j < 17; j++)
		{
			if (!corners[j])
				continue;

			VectorTransform(corners[j], matrix[hitbox->bone], vAimPoint);
		}

		return vAimPoint;
	}
}*/

Vector GetEyePosition(IClientEntity* pEntity)
{
	Vector vecViewOffset = *reinterpret_cast<Vector*>(reinterpret_cast<int>(pEntity) + 0x104);

	return pEntity->GetOrigin() + vecViewOffset;
}

int GameUtils::GetPlayerCompRank(IClientEntity* pEntity) // 0x75671f7f is crc32 for comp rank netvar, 0x2ED6198 is CCSPlayerResource, 0x1A44 is netvar offset
{
	DWORD m_iCompetitiveRanking = NetVar.GetNetVar(0x75671F7F); //NetVar.GetNetVar(0x75671F7F);
	DWORD GameResources = *(DWORD*)(Utilities::Memory::FindPattern("client.dll", (PBYTE)"\x8B\x3D\x00\x00\x00\x00\x85\xFF\x0F\x84\x00\x00\x00\x00\x81\xC7", "xx????xxxx????xx") + 0x2);
	
	return *(int*)((DWORD)GameResources + 0x1A44 + (int)pEntity->GetIndex() * 4);
}

extern void GameUtils::ServerRankRevealAll()
{
	static float fArray[3] = { 0.f, 0.f, 0.f };

	GameUtils::ServerRankRevealAllEx = (ServerRankRevealAllFn)(Offsets::Functions::dwGetPlayerCompRank);
	//GameUtils::ServerRankRevealAllEx = (ServerRankRevealAllFn)(offsets.ServerRankRevealAllEx);
	GameUtils::ServerRankRevealAllEx(fArray);
}

void ForceUpdate()
{
	// Shh
	static DWORD clientstateaddr = Utilities::Memory::FindPattern("engine.dll", (PBYTE)"\x8B\x3D\x00\x00\x00\x00\x8A\xF9\xF3\x0F\x11\x45\xF8\x83\xBF\xE8\x00\x00\x00\x02", "xx????xxxxxxxxxxxxxx");
	static uintptr_t pEngineBase = (uintptr_t)GetModuleHandleA("engine.dll");

	static uintptr_t pClientState = **(uintptr_t**)(Utilities::Memory::FindPattern("engine.dll", (PBYTE)"\x8B\x3D\x00\x00\x00\x00\x8A\xF9", "xx????xx") + 2);

	static uintptr_t dwAddr1 = Utilities::Memory::FindPattern("engine.dll", (PBYTE)"\xE8\x00\x00\x00\x00\x68\x00\x00\x00\x00\x68\x00\x00\x00\x00\xC7\x87\x00\x00\x00\x00\x00\x00\x00\x00", "x????x????x????xx????????");

	//E8 call is being used here
	static uintptr_t dwRelAddr = *(uintptr_t*)(dwAddr1 + 1);
	static uintptr_t sub_B5E60 = ((dwAddr1 + 5) + dwRelAddr);

	__asm
	{
		pushad
		mov edi, pClientState
		lea ecx, dword ptr[edi + 0x8]
		call sub_B5E60
		mov dword ptr[edi + 0x154], 0xFFFFFFFF
		popad
	}
}

/*NET_SetConVar::NET_SetConVar(const char* name, const char* value)
{
	typedef void(__thiscall* SetConVarConstructor_t)(void*);
	static SetConVarConstructor_t pNET_SetConVar = (SetConVarConstructor_t)(Utilities::Memory::FindPattern("engine.dll", (PBYTE)"\x83\xE9\x04\xE9\x00\x00\x00\x00\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x56\x8B\xF1\xC7\x06\x00\x00\x00\x00\x8D\x4E\x08", "xxxx????xxxxxxxxxxxxxxxx????xxx") + 19);
	pNET_SetConVar(this);

	typedef void(__thiscall* SetConVarInit_t)(void*, const char*, const char*);
	static SetConVarInit_t pNET_SetConVarInit = (SetConVarInit_t)Utilities::Memory::FindPattern("engine.dll", (PBYTE)"\x55\x8B\xEC\x56\x8B\xF1\x57\x83\x4E\x14\x01\x83\x7E\x0C\x00", "xxxxxxxxxxxxxxx");
	pNET_SetConVarInit(this, name, value);
}

NET_SetConVar::~NET_SetConVar()
{
	typedef void(__thiscall* SetConVarConstructor_t)(void*);
	static SetConVarConstructor_t pNET_SetConVar = (SetConVarConstructor_t)(Utilities::Memory::FindPattern("engine.dll", (PBYTE)"\x00\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x56\x8B\xF1\x57\x8D\x7E\x04\xC7\x07\x00\x00\x00\x00", "xxxxxxxxxxxxxxxxxxxxx????") + 12);
	pNET_SetConVar(this);
}*/