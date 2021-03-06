#include "MessageParser.h"
#include <algorithm>

MessageParser::MessageParser(UserDatabase& userdatabase, SleepyDiscord::DiscordClient& client)
	:
	userDatabase(userdatabase),
	client(client)
{
	m_LvL_0_user.insert({ std::string("+"), &MessageParser::f_LvL_0_plus });
	m_LvL_0_user.insert({ std::string("-"), &MessageParser::f_LvL_0_minus });
	m_LvL_0_user.insert({ std::string("?"), &MessageParser::f_LvL_0_questionmark });
	m_LvL_0_user.insert({ std::string("l"), &MessageParser::f_LvL_0_list });
	m_LvL_0_user.insert({ std::string("list"), &MessageParser::f_LvL_0_list });
	m_LvL_0_user.insert({ std::string("week"), &MessageParser::f_LvL_0_week });
	m_LvL_0_user.insert({ std::string("w"), &MessageParser::f_LvL_0_week });
	m_LvL_0_user.insert({ std::string("help"), &MessageParser::f_LvL_0_help });
	m_LvL_0_user.insert({ std::string("!help"), &MessageParser::f_LvL_0_help });
	m_LvL_0_user.insert({ std::string("!commands"), &MessageParser::f_LvL_0_help });
	m_LvL_0_user.insert({ std::string("commands"), &MessageParser::f_LvL_0_help });

	m_LvL_0_admin.insert({ std::string("adminlist"), &MessageParser::f_LvL_0_adminList });
	m_LvL_0_admin.insert({ std::string("al"), &MessageParser::f_LvL_0_adminList });
	m_LvL_0_admin.insert({ std::string("remind"), &MessageParser::f_LvL_0_remind });
	m_LvL_0_admin.insert({ std::string("r"), &MessageParser::f_LvL_0_remind });
	m_LvL_0_admin.insert({ std::string("prunemessages"), &MessageParser::f_LvL_0_pruneMessagesInChannel });
	m_LvL_0_admin.insert({ std::string("pm"), &MessageParser::f_LvL_0_pruneMessagesInChannel });
	m_LvL_0_admin.insert({ std::string("addmember"), &MessageParser::f_LvL_0_addMember });
	m_LvL_0_admin.insert({ std::string("add"), &MessageParser::f_LvL_0_addMember });
	m_LvL_0_admin.insert({ std::string("removemember"), &MessageParser::f_LvL_0_removeMember });
	m_LvL_0_admin.insert({ std::string("rm"), &MessageParser::f_LvL_0_removeMember });
	m_LvL_0_admin.insert({ std::string("changename"), &MessageParser::f_LvL_0_changeName });
	m_LvL_0_admin.insert({ std::string("ch"), &MessageParser::f_LvL_0_changeName });
	m_LvL_0_admin.insert({ std::string("addadmin"), &MessageParser::f_LvL_0_addAdmin });
	m_LvL_0_admin.insert({ std::string("removeadmin"), &MessageParser::f_LvL_0_removeAdmin });
	m_LvL_0_admin.insert({ std::string("rmadmin"), &MessageParser::f_LvL_0_removeAdmin });
	m_LvL_0_admin.insert({ std::string("reset"), &MessageParser::f_LvL_0_resetAttendanceList });
	m_LvL_0_admin.insert({ std::string("resetList"), &MessageParser::f_LvL_0_resetAttendanceList });
}

MessageParser::Reply MessageParser::parseMessage(SleepyDiscord::Message& src)
{
	std::stringstream msgStream(src.content);
	if (msgStream.eof())
		throw std::runtime_error("Empty message passed to message parser.");

	std::string currentArgument;
	if (!(msgStream >> currentArgument))
		return { false, "" };
	toLowerCase(currentArgument);
	const std::string srcDiscordID = src.author.ID.string();

	if (userDatabase.isUser(srcDiscordID))
	{
		auto it = m_LvL_0_user.find(currentArgument);
		if (it != m_LvL_0_user.end())
			return it->second(*this, msgStream, src);
	}
	
	if (userDatabase.isAdmin(srcDiscordID))
	{
		auto it = m_LvL_0_admin.find(currentArgument);
		if (it != m_LvL_0_admin.end())
			return it->second(*this, msgStream, src);
	}

	return { false, "" };
}

MessageParser::Reply MessageParser::getMessageInfo(SleepyDiscord::Message & message)
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

	return { true, UserDatabase::to_discordCodeBlock_md(messageDataFormatted) };
}

std::string MessageParser::extractDiscordID_fromPing(std::string userPinged)
{
	//	content example	"<@!168729205960343562>"
	//  content example	"<@168729205960343562>"
	size_t check0 = userPinged.find('<');
	size_t check1 = userPinged.find('@');
	size_t check2 = userPinged.find('>');
	if (check0 != std::string::npos
		&& check1 != std::string::npos
		&& check2 != std::string::npos
		&& check0 < check2
		&& int(check2 - check0) > 10)
	{
		size_t start = userPinged.find('!');
		size_t end = userPinged.find('>');
		if (start == std::string::npos)
		{
			start = userPinged.find('@');
		}
		start++;
		return userPinged.substr(start, end - start);
	}
	else
		return std::string();
}

std::string & MessageParser::toLowerCase(std::string & s)
{
	std::transform(s.begin(), s.end(), s.begin(), ::tolower);
	return s;
}

std::string MessageParser::getCommandList() const
{
	int commandBuffer = 26;
	return std::string( 
		"```css\\n[User Command list]\\n\\n"
		+ UserDatabase::extendString("- list/l", commandBuffer)	+ "print out the attendance list for this week\\n"
		+ UserDatabase::extendString("- {+/-/?}", commandBuffer)+ "\\\"yes/no/maybe\\\" for todays attendance\\n"
		+ UserDatabase::extendString("- week/w", commandBuffer) + "attendece fill out: sunday -> (next weekend) Saturday\\n"
		+ UserDatabase::extendString("", commandBuffer)			+ "(syntax: week (sunday)(monday)(tuesday)..)\\n"
		+ UserDatabase::extendString("", commandBuffer)			+ "(example: week +-?++-+ (OR) w + + + - + ? +)\\n\\n"

		+ UserDatabase::extendString("[Admin Command List]", commandBuffer)		+"\\n\\n"
		+ UserDatabase::extendString("- adminList/al", commandBuffer)			+ "list of admins\\n"
		+ UserDatabase::extendString("- remind/r", commandBuffer)				+ "remind users to fill the week attendance\\n"
		+ UserDatabase::extendString("- pruneMessages/pm", commandBuffer)		+ "prune message history of the channel\\n"
		+ UserDatabase::extendString("- addMember/add", commandBuffer)			+ "add a member\\n"
		+ UserDatabase::extendString("", commandBuffer)							+ "(syntax: add <PINGmember> <displayName>)\\n"
		+ UserDatabase::extendString("- removeMember/rm", commandBuffer)		+ "remove a member\\n"
		+ UserDatabase::extendString("", commandBuffer)							+ "(syntax: rm <memberName>)\\n"
		+ UserDatabase::extendString("- changeName/ch", commandBuffer)			+ "change a member name\\n"
		+ UserDatabase::extendString("", commandBuffer)							+ "(syntax: ch <currentName> <newName>)\\n"
		+ UserDatabase::extendString("- addAdmin", commandBuffer)				+ "give admin rights to member\\n"
		+ UserDatabase::extendString("", commandBuffer)							+ "(syntax: addAdmin <memberName>)\\n"
		+ UserDatabase::extendString("- removeAdmin", commandBuffer)			+ "remove admin rights from member\\n"
		+ UserDatabase::extendString("", commandBuffer)							+ "(syntax: removeAdmin <memberName>)\\n"
		+ UserDatabase::extendString("- reset/resetList", commandBuffer)		+ "reset the attendance list\\n\\n"
		+ UserDatabase::extendString("", commandBuffer)							+ "! automatic reset Saturday->Sunday(local time)\\n\\n"
		+ UserDatabase::extendString("", commandBuffer)							+ "#ParanoidBot V1.03"
		+ "```"
	);
}

bool MessageParser::parseWeekArgument(std::stringstream& ss, std::string discordIDuser)
{
	std::string argument;
	std::vector<UserDatabase::availableIndex> av((int)UserDatabase::dayIndex::nCount);

	ss >> argument;
	if (argument.size() >= 7)
	{
		int counter = 0;
		for (char c : argument)
		{
			switch (c)
			{
			case '+':
				av[counter] = UserDatabase::availableIndex::yes;
				counter++;
				break;
			case '-':
				av[counter] = UserDatabase::availableIndex::no;
				counter++;
				break;
			case '?':
				av[counter] = UserDatabase::availableIndex::unsure;
				counter++;
				break;
			default:
				return false;
			}
			if (counter >= 7)
				break;
		}
		userDatabase.changeAvailability_week(discordIDuser, av);
		return true;
	}
	else if (argument.size() == 1)
	{
		int counter = 0;
		char c = argument.c_str()[0];
		do
		{
			switch (c)
			{
			case '+':
				av[counter] = UserDatabase::availableIndex::yes;
				counter++;
				break;
			case '-':
				av[counter] = UserDatabase::availableIndex::no;
				counter++;
				break;
			case '?':
				av[counter] = UserDatabase::availableIndex::unsure;
				counter++;
				break;
			default:
				return false;
			}
		}
		while ((ss >> c) && (counter < 7));
		if (counter == 7)
		{
			userDatabase.changeAvailability_week(discordIDuser, av);
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

MessageParser::Reply MessageParser::f_LvL_0_plus(std::stringstream &, SleepyDiscord::Message& msg)
{
	const std::string srcDiscordID = msg.author.ID.string();
	if (userDatabase.changeAvailability_day(srcDiscordID, UserDatabase::getWeekdayToday(), UserDatabase::availableIndex::yes))
		return { true, "Changed your availability status for today to: < yes >\\n" + userDatabase.getFormatedAttendanceSingleUser(msg.author.ID.string()) };
	else
		return { true, "Eeeh something went wrong could not change ur status.." };
}

MessageParser::Reply MessageParser::f_LvL_0_minus(std::stringstream &, SleepyDiscord::Message& msg)
{
	const std::string srcDiscordID = msg.author.ID.string();
	if (userDatabase.changeAvailability_day(srcDiscordID, UserDatabase::getWeekdayToday(), UserDatabase::availableIndex::no))
		return { true, "Changed your availability status for today to: * noo *\\n" + userDatabase.getFormatedAttendanceSingleUser(msg.author.ID.string()) };
	else
		return { true, "Eeeh something went wrong could not change ur status.." };
}

MessageParser::Reply MessageParser::f_LvL_0_questionmark(std::stringstream &, SleepyDiscord::Message& msg)
{
	const std::string srcDiscordID = msg.author.ID.string();
	if (userDatabase.changeAvailability_day(srcDiscordID, UserDatabase::getWeekdayToday(), UserDatabase::availableIndex::unsure))
		return { true, "Changed your availability status for today to: <maybe>\\n" + userDatabase.getFormatedAttendanceSingleUser(msg.author.ID.string()) };
	else
		return { true, "Eeeh something went wrong could not change ur status.." };
}

MessageParser::Reply MessageParser::f_LvL_0_week(std::stringstream & msgStream, SleepyDiscord::Message & msg)
{
	if (parseWeekArgument(msgStream, msg.author.ID.string()))
		return { true, "Updated your attendance information for this week!\\n" + userDatabase.getFormatedAttendanceSingleUser(msg.author.ID.string()) };
	else
		return { true, "Not like dis pleb.. example: week + + + - ? + + (or) w +++-?++" };
}

MessageParser::Reply MessageParser::f_LvL_0_list(std::stringstream &, SleepyDiscord::Message &)
{
	return { true, userDatabase.getFormatedAttendanceList() };
}

MessageParser::Reply MessageParser::f_LvL_0_help(std::stringstream &, SleepyDiscord::Message &)
{
	return { true, "Well, I could help you out but would you even get smarter from that? I doubt it.. :smirk:\\n" + getCommandList() };
}

MessageParser::Reply MessageParser::f_LvL_0_adminList(std::stringstream &, SleepyDiscord::Message &)
{
	return { true, userDatabase.getFormatedAdminList() };;
}

MessageParser::Reply MessageParser::f_LvL_0_remind(std::stringstream &, SleepyDiscord::Message &)
{
	return { true, userDatabase.getReminderMessage() };
}

MessageParser::Reply MessageParser::f_LvL_0_pruneMessagesInChannel(std::stringstream &, SleepyDiscord::Message & msg)
{
	SleepyDiscord::ArrayResponse<SleepyDiscord::Message> response = client.getMessages(msg.channelID, client.na, "");
	std::vector<SleepyDiscord::Message> temp_vec = response;
	std::vector<SleepyDiscord::Snowflake<SleepyDiscord::Message>> snow_msgHistory;
	std::for_each(temp_vec.begin(), temp_vec.end(), [&](SleepyDiscord::Message& m)
	{
		snow_msgHistory.push_back(m);
	});
	client.bulkDeleteMessages(msg.channelID, snow_msgHistory);
	return { false, std::string() };
}

MessageParser::Reply MessageParser::f_LvL_0_addMember(std::stringstream &msgStream, SleepyDiscord::Message &)
{
	std::string add_pingedUser;
	std::string add_username;
	if (msgStream >> add_pingedUser >> add_username)
	{
		std::string add_discordID = extractDiscordID_fromPing(add_pingedUser);
		if (!add_discordID.empty())
		{
			if (userDatabase.addUser(add_discordID, add_username))
				return { true, "Added member ->   " + add_username };
			else
				return { true, "OMFG there is already a member with that discord ID u toxic fck!" };
		}
		else
			return { true, "Did you ping me the user like I told you to? Invalid DiscordID duuh..." };
	}
	return { true, "Invalid input. Syntax: add/addMember <@pingMember> <memberName>" };
}

MessageParser::Reply MessageParser::f_LvL_0_removeMember(std::stringstream &msgStream, SleepyDiscord::Message &)
{
	std::string rm_username;
	if (msgStream >> rm_username)
	{
		if (userDatabase.removeUser(rm_username))
			return { true, "Removed member ->   " + rm_username };
		else
			return { true, "No member with username \\\"" + rm_username + "\\\" in the database... noob :weary:" };
	}
	return { true, "Invalid input. Syntax: rm/removeMember <memberName>" };
}

MessageParser::Reply MessageParser::f_LvL_0_changeName(std::stringstream &msgStream, SleepyDiscord::Message &)
{
	std::string currentUsername;
	std::string newUsername;
	if (msgStream >> currentUsername >> newUsername)
	{
		if (userDatabase.changeUsername(currentUsername, newUsername))
			return{ true, "Renamed member: " + currentUsername + "  ->  " + newUsername };
		else
			return{ true, currentUsername + "is not a friend you have sry for u" };
	}
	else
		return{ true, "l2p faggi: ch <currentName> <newName>" };
}

MessageParser::Reply MessageParser::f_LvL_0_addAdmin(std::stringstream &msgStream, SleepyDiscord::Message &)
{
	std::string add_username;
	if (msgStream >> add_username)
	{
		if (userDatabase.addAdmin(add_username))
			return { true, "Added admin rights to ->   " + add_username };
		else
			return { true, "If you want to appoint admin to someone you should atleast be able to spell the username.. \\\""
					+ add_username + "\\\"? What is this? A imaginary friend of yours?" };
	}
	else
		return { true, "Invalid input. Syntax: addAdmin <memberName>" };
}

MessageParser::Reply MessageParser::f_LvL_0_removeAdmin(std::stringstream &msgStream, SleepyDiscord::Message &)
{
	std::string rm_username;
	if (msgStream >> rm_username)
	{
		if (userDatabase.removeAdmin(rm_username))
			return { true, "Removed admin rights from ->   " + rm_username };
		else
			return { true, "Are we a little paranoid today??? \\\""
					+ rm_username + "\\\"? Removing admin rights from people which do not even exist?" };
	}
	else 
		return { true, "Invalid input. Syntax: rmAdmin/removeAdmin <memberName>" };
}

MessageParser::Reply MessageParser::f_LvL_0_resetAttendanceList(std::stringstream &, SleepyDiscord::Message &)
{
	userDatabase.reset();
	return { true, "Performed reset on availability data of all users!"};
}
