#pragma once

#include <sleepy_discord/websocketpp_websocket.h>

class BotClient : public SleepyDiscord::DiscordClient {
public:
	using SleepyDiscord::DiscordClient::DiscordClient;

	void onMessage(SleepyDiscord::Message message);
};