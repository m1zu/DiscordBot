#pragma once

#include <sleepy_discord/websocketpp_websocket.h>

#include "MessageParser.h"
#include "UserDatabase.h"

class BotClient : public SleepyDiscord::DiscordClient 
{
	using SleepyDiscord::DiscordClient::DiscordClient;

public:
	void onMessage(SleepyDiscord::Message message) override;
	void sendMessage(SleepyDiscord::Snowflake<SleepyDiscord::Channel> channelID, std::string message);

private:
	MessageParser parser;
	UserDatabase userDatabase;
};