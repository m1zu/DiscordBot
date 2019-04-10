#include "Botclient.h"
#include <sstream>
#include <iostream>

void BotClient::onMessage(SleepyDiscord::Message message)
{
	// General Message Filters
	if (message.author.bot)
		return;
	if (message.type != SleepyDiscord::Message::MessageType::DEFAULT)
		return;

	// Check for reset nec.?
	if (resetTimer.resetIsDue())
	{
		userDatabase.reset();
		schedule([this, message]() {
			SleepyDiscord::DiscordClient::sendMessage(message.channelID, userDatabase.getFormatedAttendanceList());
			SleepyDiscord::DiscordClient::sendMessage(message.channelID, "Adventurers! A new week has come and with it a new way of me annoying you!");
		}, 5000);
	}

	// Message Parser
	try
	{
		MessageParser::Reply reply = parser.parseMessage(message, userDatabase);
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