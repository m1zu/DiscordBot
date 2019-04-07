#pragma once

#include<map>
#include <sleepy_discord/websocketpp_websocket.h>

class MessageParser
{
public:
	struct Reply
	{
		bool hasToSaySomething;
		std::string answer;
	};
public:
	Reply parseMessage(SleepyDiscord::Message& message);
	Reply getMessageInfo(SleepyDiscord::Message& message);

public:
	static std::string to_discordCodeBlock(const std::string& message);
	static std::string to_personShoutout(const std::string& authorID);
	static std::string to_personShoutout(SleepyDiscord::Snowflake<SleepyDiscord::User> authorID);

private:
	bool informationMode = false;
};

