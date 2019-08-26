#include "doorspam.h"

#include "../settings.h"
#include "../interfaces.h"

bool Settings::DoorSpam::enabled = false;
static bool ShouldSpamDoors = false;
ButtonCode_t Settings::DoorSpam::key = ButtonCode_t::KEY_H;

void DoorSpam::CreateMove(CUserCmd* cmd)
{
	if (!Settings::DoorSpam::enabled)
		return;

	C_BasePlayer* localplayer = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());
	if (!localplayer)
		return;

	if (inputSystem->IsButtonDown(Settings::DoorSpam::key)){		
		if (ShouldSpamDoors){
			cmd->buttons &= ~IN_USE;
		} else {
			cmd->buttons |= IN_USE;			
		}
		ShouldSpamDoors = !ShouldSpamDoors;	
	}
}
