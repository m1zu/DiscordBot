#include "MessageParser.h"
#include <algorithm>

MessageParser::Reply MessageParser::parseMessage(SleepyDiscord::Message& src, UserDatabase& userDatabase)
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
		else if (currentArgument == "week")
		{
			// toDo: Check Admin list for valid person
			return { true, "Entered week parser branch!" };
			// execute further week command parsing
		}
		else if (currentArgument == "list")
		{
			return { true, to_discordCodeBlock(userDatabase.getFormatedList())};
			// output formated list
		}
		else if (currentArgument == "!admin")
		{
			// toDo: Check Admin list for valid person
			msgStream >> currentArgument;
			if (currentArgument == "!reset")
			{
				return { true, "Performed reset on availability data of all users!" };
			}
			return { true, "Admin parser branch work in progress!" };
			// execute further admin command parsing
		}
		else if (currentArgument == "!help" || currentArgument == "help")
		{
			return { true, ":poop: No help for the weak!" };
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
	return "```md\\n" + s + "```";
}

std::string MessageParser::to_personShoutout(const std::string& authorID)
{
	return " <@" + authorID + ">";
}

std::string MessageParser::to_personShoutout(SleepyDiscord::Snowflake<SleepyDiscord::User> authorID)
{
	return to_personShoutout(static_cast<std::string>(authorID));
}

std::string MessageParser::extractDiscordID_fromPing(std::string userPinged)
{
	//	content example	"<@!168729205960343562>"
	size_t start = userPinged.find_first_of('!') + 1;
	size_t end = userPinged.find_first_of('>');
	return userPinged.substr(3, end-start);
}
