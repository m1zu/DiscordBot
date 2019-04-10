#include "MessageHistory.h"

void MessageHistory::manageMessageHistory(const SleepyDiscord::Message& message, SleepyDiscord::BaseDiscordClient & client)
{
	int64_t channelID = stoll(message.channelID.string());
	int64_t messageID = stoll(message.ID.string());

	if (map_keyChannelID_vMessageID.count(channelID) > nMaxMessagesPerChannel)
	{
		SleepyDiscord::Snowflake<SleepyDiscord::Channel> out_channelID = channelID;
		std::vector<SleepyDiscord::Snowflake<SleepyDiscord::Message>> out_messageIDs;

		auto range = map_keyChannelID_vMessageID.equal_range(channelID);
		std::for_each(range.first, range.second, [&](const std::pair<const int64_t, int64_t> pair)
		{
			SleepyDiscord::Snowflake<SleepyDiscord::Message> msg = static_cast<SleepyDiscord::Snowflake<SleepyDiscord::Message>>(pair.second);
			out_messageIDs.push_back(msg);
		});
		map_keyChannelID_vMessageID.erase(range.first, range.second);
		client.bulkDeleteMessages(out_channelID, out_messageIDs);
	}
	map_keyChannelID_vMessageID.insert({ channelID, messageID });
}
