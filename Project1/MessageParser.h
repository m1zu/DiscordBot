#pragma once

#include <map>
#include <sleepy_discord/websocketpp_websocket.h>

#include "UserDatabase.h"
#include "MessageHistory.h"

class MessageParser
{
public:
	struct Reply
	{
		bool hasToSaySomething;
		std::string answer;
	};
public:
	MessageParser(UserDatabase& userdatabase, MessageHistory& messagehistory);

public:
	Reply parseMessage(SleepyDiscord::Message& message);
	Reply getMessageInfo(SleepyDiscord::Message& message);

public:	
	static std::string extractDiscordID_fromPing(std::string userPinged);
	static std::string& toLowerCase(std::string& s);

public:
	std::string getCommandList() const;

private:
	bool parseWeekArgument(std::stringstream& ss, std::string discordIDuser);

private:
	UserDatabase& userDatabase;
	MessageHistory& msgHistory;
};

