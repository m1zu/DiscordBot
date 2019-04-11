#include "MessageHistory.h"

MessageHistory::MessageHistory(SleepyDiscord::DiscordClient & client)
	:client(client)
{
}

void MessageHistory::add(const SleepyDiscord::Message& message)
{
	if (map_keyChannelID_vMessageID.size() > nMaxMessages)
	{
		map_keyChannelID_vMessageID.clear();
	}
	int64_t channelID = stoll(message.channelID.string());
	int64_t messageID = stoll(message.ID.string());

	map_keyChannelID_vMessageID.insert({ channelID, messageID });
}

bool MessageHistory::prune(SleepyDiscord::Snowflake<SleepyDiscord::Channel>& snow_channelID)
{
	SleepyDiscord::Snowflake<SleepyDiscord::Channel> out_channelID = snow_channelID;
	std::vector<SleepyDiscord::Snowflake<SleepyDiscord::Message>> out_messageIDs;

	int64_t channelID = stoll(snow_channelID.string());
	auto range = map_keyChannelID_vMessageID.equal_range(channelID);
	std::for_each(range.first, range.second, [&](const std::pair<const int64_t, int64_t> pair)
	{
		SleepyDiscord::Snowflake<SleepyDiscord::Message> msg = static_cast<SleepyDiscord::Snowflake<SleepyDiscord::Message>>(pair.second);
		out_messageIDs.push_back(msg);
	});
	map_keyChannelID_vMessageID.erase(range.first, range.second);

	return client.bulkDeleteMessages(out_channelID, out_messageIDs);
}