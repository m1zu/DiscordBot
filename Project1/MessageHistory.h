#pragma once
#include <unordered_map>
#include <sleepy_discord/websocketpp_websocket.h>

class MessageHistory
{
public:
	// if a channel reaches nMaxMessagesPerChannel channelID automatic prune is induced
	void manageMessageHistory(const SleepyDiscord::Message& message, SleepyDiscord::BaseDiscordClient& client);

private:
	unsigned int nMaxMessagesPerChannel = 8;
	std::unordered_multimap<int64_t, int64_t> map_keyChannelID_vMessageID;
};

