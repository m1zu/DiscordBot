#include "BotClient.h"

void BotClient::onMessage(SleepyDiscord::Message message)
{
	if (message.startsWith("!hello"))
		sendMessage(message.channelID, "Hello " + message.author.username);
	if (message.startsWith("!pingme"))
		sendMessage(message.channelID, " <@" + static_cast<std::string>(message.author.ID) + ">");
}
