#include "BotClient.h"
#include <sstream>
#include <iostream>

void BotClient::onMessage(SleepyDiscord::Message message)
{
	// General Message Filters
	if (message.author.bot)
		return;
	if (message.type != SleepyDiscord::Message::MessageType::DEFAULT)
		return;

	// Message Parser
	try
	{
		MessageParser::Reply reply = parser.parseMessage(message);
		if(reply.hasToSaySomething)
			SleepyDiscord::DiscordClient::sendMessage(message.channelID, reply.answer);

		reply = parser.getMessageInfo(message);
		if(reply.hasToSaySomething)
			SleepyDiscord::DiscordClient::sendMessage(message.channelID, reply.answer);
	}
	catch (std::runtime_error& err)
	{
		std::cout << "Runtime exception caught in BotClient::onMessage():\n";
		std::cout << err.what();
		throw;
	}
	//if (message.startsWith("!hello"))
	//	sendMessage(message.channelID, "Hello " + message.author.username);
	//if (message.startsWith("!pingme"))
	//	sendMessage(message.channelID, to_personShoutout(message.author.ID));
}

void BotClient::sendMessage(SleepyDiscord::Snowflake<SleepyDiscord::Channel> channelID, std::string message)
{
	SleepyDiscord::DiscordClient::sendMessage(channelID, message);
}
