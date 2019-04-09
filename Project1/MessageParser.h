#pragma once

#include <map>
#include <sleepy_discord/websocketpp_websocket.h>

#include "UserDatabase.h"

class MessageParser
{
public:
	struct Reply
	{
		bool hasToSaySomething;
		std::string answer;
	};
public:
	Reply parseMessage(SleepyDiscord::Message& message, UserDatabase& dataBase);
	Reply getMessageInfo(SleepyDiscord::Message& message);

public:
	static std::string to_discordCodeBlock(const std::string& message);
	static std::string to_personShoutout(const std::string& authorID);
	static std::string to_personShoutout(SleepyDiscord::Snowflake<SleepyDiscord::User> authorID);
	
	static std::string extractDiscordID_fromPing(std::string userPinged);
	static std::string& toLowerCase(std::string& s);

public:
	std::string getCommandList() const;

private:
	bool parseWeekArgument(std::stringstream& ss, std::string discordIDuser, UserDatabase& dataBase);

private:
	bool informationMode = false;
};

