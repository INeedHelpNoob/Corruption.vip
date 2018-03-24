
#include "Interfaces.h"
#include "Menu.h"
#include "AntiAntiAim.h"
#include "Hooks.h"
RecvVarProxyFn oSmokeEffectTickBegin = NULL;
RecvVarProxyFn oFlashMaxAlpha = NULL;
RecvVarProxyFn oDidSmokeEffect = NULL;
float YawDelta[64];
float reset[64];
float Delta[64];
float OldLowerBodyYaw[64];
float Resolved_angles[64];
int i;
// Shad0ws Yaw fix
// (FIX ME UP LATER)
RecvVarProxyFn oRecvnModelIndex;

void Hooked_RecvProxy_Viewmodel(CRecvProxyData *pData, void *pStruct, void *pOut)
{
	int default_t = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_default_t.mdl");
	int default_ct = Interfaces::ModelInfo->GetModelIndex("models/weapons/v_knife_default_ct.mdl");
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

	IClientEntity* pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (Menu::Window.MiscTab.SkinEnable.GetState() && pLocal)
	{

		if (pLocal->IsAlive() && (
			pData->m_Value.m_Int == default_t ||
			pData->m_Value.m_Int == default_ct ||
			pData->m_Value.m_Int == iBayonet ||
			pData->m_Value.m_Int == iButterfly ||
			pData->m_Value.m_Int == iFlip ||
			pData->m_Value.m_Int == iGunGame ||
			pData->m_Value.m_Int == iGut ||
			pData->m_Value.m_Int == iKarambit ||
			pData->m_Value.m_Int == iM9Bayonet ||
			pData->m_Value.m_Int == iHuntsman ||
			pData->m_Value.m_Int == iFalchion ||
			pData->m_Value.m_Int == iDagger ||
			pData->m_Value.m_Int == iBowie))
		{

			if (Menu::Window.MiscTab.KnifeModel.GetIndex() == 0)
				pData->m_Value.m_Int = iBayonet;
			else if (Menu::Window.MiscTab.KnifeModel.GetIndex() == 1)
				pData->m_Value.m_Int = iBowie;
			else if (Menu::Window.MiscTab.KnifeModel.GetIndex() == 2)
				pData->m_Value.m_Int = iButterfly;
			else if (Menu::Window.MiscTab.KnifeModel.GetIndex() == 3)
				pData->m_Value.m_Int = iFalchion;
			else if (Menu::Window.MiscTab.KnifeModel.GetIndex() == 4)
				pData->m_Value.m_Int = iFlip;
			else if (Menu::Window.MiscTab.KnifeModel.GetIndex() == 5)
				pData->m_Value.m_Int = iGut;
			else if (Menu::Window.MiscTab.KnifeModel.GetIndex() == 6)
				pData->m_Value.m_Int = iHuntsman;
			else if (Menu::Window.MiscTab.KnifeModel.GetIndex() == 7)
				pData->m_Value.m_Int = iKarambit;
			else if (Menu::Window.MiscTab.KnifeModel.GetIndex() == 8)
				pData->m_Value.m_Int = iM9Bayonet;
			else if (Menu::Window.MiscTab.KnifeModel.GetIndex() == 9)
				pData->m_Value.m_Int = iDagger;
		}
	}


	oRecvnModelIndex(pData, pStruct, pOut);
}

void RecvProxy_DidSmokeEffect(CRecvProxyData *pData, void *pStruct, void *pOut)
{
	int *value = (int*)&pData->m_Value.m_Int;

	if (Menu::Window.VisualsTab.OtherNoSmoke.GetState())
	{
		//IClientEntity *entity = (IClientEntity*)pStruct;
		//*value = 0;

		ConVar* particles = Interfaces::CVar->FindVar("r_drawparticles");
		SpoofedConvar* particles_spoofed = new SpoofedConvar(particles);
		if (Menu::Window.VisualsTab.OtherNoSmoke.GetState())
			particles_spoofed->SetInt(0);
		else
			particles_spoofed->SetInt(1);

		ConVar* fog = Interfaces::CVar->FindVar("fog_enable");
		SpoofedConvar* fog_spoofed = new SpoofedConvar(fog);
		if (Menu::Window.VisualsTab.OtherNoSmoke.GetState())
			fog_spoofed->SetInt(0);
		else
			fog_spoofed->SetInt(1);
	}

	oDidSmokeEffect(pData, pStruct, pOut);
}

void RecvProxy_SmokeEffectTickBegin(CRecvProxyData *pData, void *pStruct, void *pOut)
{
	float *value = &pData->m_Value.m_Float;

	if (Menu::Window.VisualsTab.OtherNoSmoke.GetState())
	{
		IClientEntity *entity = (IClientEntity*)pStruct;

		if (entity)
		{
			//*value = Interfaces::Globals->tickcount - 60000;
			*entity->GetOriginPtr() = Vector(10000, 10000, 10000);
		}
	}

	oSmokeEffectTickBegin(pData, pStruct, pOut);
}

void RecvProxy_FlashMaxAlpha(CRecvProxyData *pData, void *pStruct, void *pOut)
{
	float *value = &pData->m_Value.m_Float;

	if (Menu::Window.VisualsTab.OtherNoFlash.GetState())
	{
		*value = 0.0f;
	}
	else
	{
		*value = 255.0f;
	}

	oFlashMaxAlpha(pData, pStruct, pOut);
}

void ApplyNetVarsHooks()
{
	ClientClass *pClass = Interfaces::Client->GetAllClasses();

	while (pClass)
	{
		const char *pszName = pClass->m_pRecvTable->m_pNetTableName;

		for (int i = 0; i < pClass->m_pRecvTable->m_nProps; i++)
		{
			RecvProp *pProp = &(pClass->m_pRecvTable->m_pProps[i]);
			const char *name = pProp->m_pVarName;

			if (!strcmp(pszName, "DT_BaseViewModel"))
			{
				// Knives
				if (!strcmp(name, "m_nModelIndex"))
				{
					oRecvnModelIndex = (RecvVarProxyFn)pProp->m_ProxyFn;
					pProp->m_ProxyFn = Hooked_RecvProxy_Viewmodel;
				}
			}
			else if (!strcmp(pszName, "DT_SmokeGrenadeProjectile"))
			{
				if (!strcmp(name, "m_nSmokeEffectTickBegin"))
				{
					oSmokeEffectTickBegin = (RecvVarProxyFn)pProp->m_ProxyFn;
					pProp->m_ProxyFn = RecvProxy_SmokeEffectTickBegin;
				}
				else if (!strcmp(name, "m_bDidSmokeEffect"))
				{
					oDidSmokeEffect = (RecvVarProxyFn)pProp->m_ProxyFn;
					pProp->m_ProxyFn = RecvProxy_DidSmokeEffect;
				}
			}
			else if (!strcmp(pszName, "DT_CSPlayer"))
			{
				if (!strcmp(name, "m_flFlashMaxAlpha"))
				{
					oFlashMaxAlpha = (RecvVarProxyFn)pProp->m_ProxyFn;
					pProp->m_ProxyFn = RecvProxy_FlashMaxAlpha;
				}
			}
		}

		pClass = pClass->m_pNext;
	}
}

void RemoveNetVarsHooks()
{
	ClientClass *pClass = Interfaces::Client->GetAllClasses();

	while (pClass)
	{
		const char *pszName = pClass->m_pRecvTable->m_pNetTableName;

		for (int i = 0; i < pClass->m_pRecvTable->m_nProps; i++)
		{
			RecvProp *pProp = &(pClass->m_pRecvTable->m_pProps[i]);
			const char *name = pProp->m_pVarName;

			if (!strcmp(pszName, "DT_BaseViewModel"))
			{
				// Knives
				if (!strcmp(name, "m_nModelIndex"))
				{
					pProp->m_ProxyFn = oRecvnModelIndex;
				}
			}
			else if (!strcmp(pszName, "DT_SmokeGrenadeProjectile"))
			{
				if (!strcmp(name, "m_nSmokeEffectTickBegin"))
				{
					pProp->m_ProxyFn = oSmokeEffectTickBegin;
				}
				else if (!strcmp(name, "m_bDidSmokeEffect"))
				{
					pProp->m_ProxyFn = oDidSmokeEffect;
				}
			}
			else if (!strcmp(pszName, "DT_CSPlayer"))
			{
				if (!strcmp(name, "m_flFlashMaxAlpha"))
				{
					pProp->m_ProxyFn = oFlashMaxAlpha;
				}
			}
		}

		pClass = pClass->m_pNext;
	}
}