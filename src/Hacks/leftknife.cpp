#include "leftknife.h"

#include "../Utils/xorstring.h"
#include "../settings.h"
#include "../interfaces.h"

bool Settings::LeftKnife::enabled = false;

void LeftKnife::CreateMove(CUserCmd *cmd)
{
	if (!Settings::LeftKnife::enabled)
		return;

	C_BasePlayer* localplayer = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());
	if (!localplayer || !localplayer->GetAlive())
		return;

	C_BaseCombatWeapon* activeWeapon = (C_BaseCombatWeapon*) entityList->GetClientEntityFromHandle(localplayer->GetActiveWeapon());
	if (!activeWeapon || activeWeapon->GetInReload())
		return;

	static bool enabled = false;
	static int righthanded = 1;
	if (!enabled) righthanded = cvar->FindVar(XORSTR("cl_righthand"))->GetInt();
	if (activeWeapon->GetCSWpnData()->GetWeaponType() == CSWeaponType::WEAPONTYPE_KNIFE && righthanded == 1 && !enabled)
	{
		engine->ClientCmd_Unrestricted("cl_righthand 0");
		enabled = true;
	}
	else if (activeWeapon->GetCSWpnData()->GetWeaponType() != CSWeaponType::WEAPONTYPE_KNIFE && enabled)
	{
		engine->ClientCmd_Unrestrictedf("cl_righthand %d", righthanded);
		enabled = false;
	}

}
