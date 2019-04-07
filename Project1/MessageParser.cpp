#include "MessageParser.h"
#include <algorithm>

MessageParser::Reply MessageParser::parseMessage(SleepyDiscord::Message& src)
{
	std::string lowerCaseString = src.content;
	std::transform(lowerCaseString.begin(), lowerCaseString.end(), lowerCaseString.begin(), ::tolower);
	std::stringstream msgStream(lowerCaseString);
	if (msgStream.eof())
		throw std::runtime_error("Empty message passed to message parser.");

	std::string currentArgument;
	if (msgStream >> currentArgument)
	{
		if (currentArgument == "+")
		{
			// toDo: Check Member list, if not there add member?
			return { true, "Entered + parse argument" };
			//execute + functionality for user
		}
		else if (currentArgument == "-")
		{
			// toDo: Check Member list, if not there add member?
			return { true, "Entered - parse argument" };
			//execute + functionality for user
		}
		else if (currentArgument == "!week")
		{
			// toDo: Check Admin list for valid person
			return { true, "Entered week parser branch!" };
			// execute further week command parsing
		}
		else if (currentArgument == "!list")
		{
			return { true, "Praticipation list print" };
			// output formated list
		}
		else if (currentArgument == "!admin")
		{
			// toDo: Check Admin list for valid person
			return { true, "Admin parser branch work in progress!" };
			// execute further admin command parsing
		}
		else if (currentArgument == "!help" || currentArgument == "help")
		{
			return { true, "No help for the weak!" };
		}
	}
	return { false, "" };
}

MessageParser::Reply MessageParser::getMessageInfo(SleepyDiscord::Message & message)
{
	if (informationMode)
	{
		std::string serverID = static_cast<std::string>(message.serverID);
		std::string channelID = static_cast<std::string>(message.channelID);
		std::string author = message.author.username;
		std::string authorID = static_cast<std::string>(message.author.ID);
		std::string msgStream = message.content;

		std::string messageDataFormatted =
			+"Server ID: " + serverID
			+ "\\nChannel ID: " + channelID
			+ "\\n"
			+ "\\nAuthor: " + author
			+ "\\nAuthor ID: " + authorID
			+ "\\n"
			+ "\\nContent: " + msgStream;

		return { true, to_discordCodeBlock(messageDataFormatted) };
	}
	return { false, "" };
}

std::string MessageParser::to_discordCodeBlock(const std::string& s)
{
	return "```c\\n" + s + "```";
}

std::string MessageParser::to_personShoutout(const std::string& authorID)
{
	return " <@" + authorID + ">";
}

std::string MessageParser::to_personShoutout(SleepyDiscord::Snowflake<SleepyDiscord::User> authorID)
{
	return to_personShoutout(static_cast<std::string>(authorID));
}
