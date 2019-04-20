#pragma once

#include <map>
#include <sleepy_discord/websocketpp_websocket.h>
#include <unordered_map>

#include "UserDatabase.h"
#include "MessageHistory.h"

class MessageParser
{
public:
	struct Reply
	{
		bool hasToSaySomething;
		std::string answer;
	};
public:
	MessageParser(UserDatabase& userdatabase, SleepyDiscord::DiscordClient& client);

public:
	Reply parseMessage(SleepyDiscord::Message& message);
	Reply getMessageInfo(SleepyDiscord::Message& message);

public:	
	static std::string extractDiscordID_fromPing(std::string userPinged);
	static std::string& toLowerCase(std::string& s);

public:
	std::string getCommandList() const;

private:
	bool parseWeekArgument(std::stringstream& ss, std::string discordIDuser);

private:
	UserDatabase& userDatabase;
	SleepyDiscord::DiscordClient& client;

private:
	std::unordered_map < std::string, std::function<MessageParser::Reply(MessageParser, std::stringstream& ss, SleepyDiscord::Message&)> > m_LvL_0_user;
	std::unordered_map < std::string, std::function<MessageParser::Reply(MessageParser, std::stringstream& ss, SleepyDiscord::Message&)> > m_LvL_0_admin;

private:
	Reply f_LvL_0_plus(std::stringstream&, SleepyDiscord::Message&);
	Reply f_LvL_0_minus(std::stringstream&, SleepyDiscord::Message&);
	Reply f_LvL_0_questionmark(std::stringstream&, SleepyDiscord::Message&);
	Reply f_LvL_0_week(std::stringstream&, SleepyDiscord::Message&);
	Reply f_LvL_0_list(std::stringstream&, SleepyDiscord::Message&);
	Reply f_LvL_0_help(std::stringstream&, SleepyDiscord::Message&);
	Reply f_LvL_0_adminList(std::stringstream&, SleepyDiscord::Message&);
	Reply f_LvL_0_remind(std::stringstream&, SleepyDiscord::Message&);
	Reply f_LvL_0_pruneMessagesInChannel(std::stringstream&, SleepyDiscord::Message&);
	Reply f_LvL_0_addMember(std::stringstream&, SleepyDiscord::Message&);
	Reply f_LvL_0_removeMember(std::stringstream&, SleepyDiscord::Message&);
	Reply f_LvL_0_changeName(std::stringstream&, SleepyDiscord::Message&);
	Reply f_LvL_0_addAdmin(std::stringstream&, SleepyDiscord::Message&);
	Reply f_LvL_0_removeAdmin(std::stringstream&, SleepyDiscord::Message&);
	Reply f_LvL_0_resetAttendanceList(std::stringstream&, SleepyDiscord::Message&);
};

