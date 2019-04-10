#pragma once

#include <sleepy_discord/websocketpp_websocket.h>

#include "MessageParser.h"
#include "UserDatabase.h"
#include "ResetTimer.h"
#include "MessageHistory.h"

class BotClient : public SleepyDiscord::DiscordClient 
{
	using SleepyDiscord::DiscordClient::DiscordClient;

public:
	void onMessage(SleepyDiscord::Message message) override;

private:
	UserDatabase userDatabase;
	MessageParser parser;
	ResetTimer resetTimer;
	MessageHistory messageHistoryManager;
};