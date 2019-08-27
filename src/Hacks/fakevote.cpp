#include "fakevote.h"
#include "namechanger.h"

#include "../Utils/xorstring.h"
#include "../settings.h"
#include "../interfaces.h"

char Settings::FakeVote::message[128] = "Call timeout?";

bool fakevote_enabled = false;
int fakevote_id = -1;
int step = 0;
std::string oname = "invalid";

static std::string GetLocalName()
{
	C_BasePlayer* localplayer = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());
	if (!localplayer) return std::string("unconnected");

	IEngineClient::player_info_t playerInfo;
	engine->GetPlayerInfo(engine->GetLocalPlayer(), &playerInfo);
	return std::string(playerInfo.name);
}

/*void CallVote(std::string vote) // TODO: Other votes.
{
	std::ostringstream voteCommand;
	voteCommand << XORSTR("callvote ");
	voteCommand << vote;
	engine->ClientCmd_Unrestricted(voteCommand.str().c_str());
}*/

void CallVotekick(int userid)
{
	std::ostringstream votekickCommand;
	votekickCommand << XORSTR("callvote Kick ");
	votekickCommand << userid;
	engine->ClientCmd_Unrestricted(votekickCommand.str().c_str());
}

void CallSpoofname()
{
	std::string name = "";
	name.insert(name.begin(), 30, '\n');
	name += Settings::FakeVote::message;
	name.insert(name.end(), 180, '\n');
	NameChanger::SetName(name.c_str());
}

void FakeVote::Votekick(int userid)
{
	oname = GetLocalName();
	NameChanger::SetName(XORSTR("\n\xAD\xAD\xAD"));
	fakevote_id = userid;
	fakevote_enabled = true;
	return;
}

void FakeVote::CreateMove(CUserCmd* cmd)
{
	if (!fakevote_enabled)
		return;

	C_BasePlayer* localplayer = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());
	if (!localplayer/* || !localplayer->GetAlive()*/)
		return;

	float currentTime = globalVars->curtime;
	static float timeStamp = currentTime - 1; // - 1 seg (wait for glitch name)

	if (currentTime - timeStamp > 1 && step == 0)
	{
		timeStamp = currentTime;
		step = 1;
		NameChanger::SetName(XORSTR("\n\xAD\xAD\xAD"));
	}
	if (currentTime - timeStamp > 1 && step == 1)
	{
		timeStamp = currentTime;
		step = 2;
		CallSpoofname();
	}
	else if (currentTime - timeStamp > 1 && step == 2)
	{
		timeStamp = currentTime;
		step = 3;
		CallVotekick(fakevote_id);
	}
	else if (currentTime - timeStamp > 1 && step == 3)
	{
		timeStamp = currentTime;
		step = 4;
		NameChanger::SetName(oname.c_str());
	}

	if (step == 4)
	{
		step = 0;
		fakevote_id = -1;
		fakevote_enabled = false;
	}
}

/*void FakeVote::FireGameEvent(IGameEvent* event)
{
	if (!event)
		return;
}*/
