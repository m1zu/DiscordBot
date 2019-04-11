#pragma once
#include <unordered_map>
#include <sleepy_discord/websocketpp_websocket.h>

class MessageHistory
{
public:
	MessageHistory(SleepyDiscord::DiscordClient& client);

public:
	// if a channel reaches nMaxMessagesPerChannel channelID automatic prune is induced
	void add(const SleepyDiscord::Message& message);

	bool prune(SleepyDiscord::Snowflake<SleepyDiscord::Channel>& channelID);

private:
	size_t nMaxMessages = 100;
	std::unordered_multimap<int64_t, int64_t> map_keyChannelID_vMessageID;
	SleepyDiscord::DiscordClient& client;
};

