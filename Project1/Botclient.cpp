#include "Botclient.h"
#include <sstream>
#include <iostream>

BotClient::BotClient(const std::string& token)
	:
	SleepyDiscord::DiscordClient(token),
	parser(userDatabase, *this)
{
}

void BotClient::onMessage(SleepyDiscord::Message message)
{
	// General Message Filters
	if (message.type != SleepyDiscord::Message::MessageType::DEFAULT)
		return;

	if (message.author.bot)
		return;

	// Check for reset of database attendance nec.?
	if (resetTimer.resetIsDue())
	{
		userDatabase.reset();
		schedule([this, message]() {
			SleepyDiscord::DiscordClient::sendMessage(message.channelID, userDatabase.getFormatedAttendanceList());
			SleepyDiscord::DiscordClient::sendMessage(message.channelID, "Adventurers! New week new attendance! @everyone");
		}, 5000);
	}

	// Message Parser
	try
	{
		MessageParser::Reply reply = parser.parseMessage(message);
		if(reply.hasToSaySomething)
			SleepyDiscord::DiscordClient::sendMessage(message.channelID, reply.answer);
	}
	catch (std::runtime_error& err)
	{
		std::cout << "Runtime exception caught in BotClient::onMessage():\n";
		std::cout << err.what();
		throw;
	}
}