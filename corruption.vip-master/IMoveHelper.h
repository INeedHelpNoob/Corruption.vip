#pragma once
#include "CommonIncludes.h"
#include "SDK.h"
class IClientEntity;

class IMoveHelper: public Interface
{
public:
	virtual	void _vpad() = 0;
	void SetHost(IClientEntity* pEnt)
	{
		typedef void(__thiscall* SetHostFn)(void*, IClientEntity*);
		return VFunc<SetHostFn>(1)(this, pEnt);
	}
};
