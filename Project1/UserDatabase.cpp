#include "UserDatabase.h"

#include <fstream>
#include <algorithm>
#include <sstream>
#include <assert.h>

UserDatabase::UserDatabase()
	:filename("userdata.dat")
{
	loadFile();
}

UserDatabase::~UserDatabase()
{
}

std::string UserDatabase::extendString(std::string s, unsigned int t)
{
	if (s.size() > t-3)
	{
		s.resize(t);
		s[t - 3] = '.';
		s[t - 2] = '.';
		s[t - 1] = ' ';
	}
	else
	{
		while (s.size()<t)
			s += ' ';
	}
	return s;
}

bool UserDatabase::changeAvailability_day(const std::string & discordID, dayIndex day, availableIndex status)
{
	auto it = std::find_if(user.begin(), user.end(), [&](const User& user) 
	{
		return user.discordID == discordID;
	});
	if (it != user.end())
	{
		it->availability[static_cast<int>(day)] = status;
		syncWithFile();
		return true;
	}
	else
		return false;
}

bool UserDatabase::changeAvailability_week(const std::string & discordID, std::vector<availableIndex> av)
{
	auto it = std::find_if(user.begin(), user.end(), [&](const User& user)
	{
		return user.discordID == discordID;
	});
	if (it != user.end())
	{
		it->availability = av;
		syncWithFile();
		return true;
	}
	else
		return false;
}

std::string UserDatabase::getFormatedAttendanceList()
{
	//Header 
	//name 16Slots + weekday10Slots
	int daybuffer = 10;
	int namebuffer = 18;
	std::string output =
		extendString("Member", namebuffer)
		+ extendString("Sunday", daybuffer)
		+ extendString("Monday", daybuffer)
		+ extendString("Tuesday", daybuffer)
		+ extendString("Wednesday", daybuffer)
		+ extendString("Thursday", daybuffer)
		+ extendString("Friday", daybuffer)
		+ extendString("Saturday", daybuffer) + "\\n\\n";

	std::sort(user.begin(), user.end(), [](User& user1, User& user2)
	{
		return user1.name < user2.name;
	});

	std::for_each(user.begin(), user.end(), [&](const User& user) 
	{
		output +=
			extendString(user.name, namebuffer);
		const std::vector<availableIndex>& av = user.availability;
		std::for_each(av.begin(), av.end(), [&](const availableIndex index) {
			switch (index)
			{
			case availableIndex::no:
				output += extendString("* no *", daybuffer);
				break;
			case availableIndex::yes:
				output += extendString("< yes >", daybuffer);
				break;
			case availableIndex::unsure:
				output += extendString("<mby>", daybuffer);
				break;
			default:
				output += extendString(".", daybuffer);
				break;
			}
		});
		output += "\\n";
	});

	return	output;
}

std::string UserDatabase::getFormatedAdminList()
{
	std::sort(user.begin(), user.end(), [](User& user1, User& user2)
	{
		return user1.name < user2.name;
	});

	std::string output("#Admin List:\\n\\n");
	std::for_each(user.begin(), user.end(), [&](const User& user)
	{
		if(user.isAdmin)
			output += (user.name + "\\n");
	});

	return ("```md\\n" + output + "```");
}

bool UserDatabase::isUser(const std::string & discordID) const
{
	return std::any_of(user.begin(), user.end(), [&](const User& user) 
	{
		return discordID == user.discordID;
	});
}

bool UserDatabase::isAdmin(const std::string & discordID) const
{
	auto it = std::find_if(user.begin(), user.end(), [&](const User& user)
	{
		return discordID == user.discordID;
	});
	if (it == user.end())
		return false;
	else
		return it->isAdmin;
}

bool UserDatabase::addUser(std::string discordID, std::string username)
{
	if (isUser(discordID))
		return false;
	else
	{
		user.emplace_back(username, discordID, 0, std::vector<availableIndex>(dayIndex::nCount, availableIndex::notFilled));
		syncWithFile();
		return true;
	}
}

bool UserDatabase::removeUser(std::string username)
{
	auto it = std::remove_if(user.begin(), user.end(), [&](User& user)
	{
		return user.name == username;
	});

	if (it != user.end())
	{
		user.erase(it, user.end());
		syncWithFile();
		return true;
	}
	else
		return false;
}

bool UserDatabase::addAdmin(std::string username)
{
	if (existsAsUser(username))
	{
		if (existsAsAdmin(username))
		{
			return true;
		}
		else
		{
			auto it = std::find_if(user.begin(), user.end(), [&](const User& user) 
			{
				return user.name == username;
			});
			if (it != user.end())
			{
				it->isAdmin = true;
				syncWithFile();
				return true;
			}
			else
				return false;
		}
	}
	return false;
}

bool UserDatabase::removeAdmin(std::string username)
{
	if (existsAsAdmin(username))
	{
		auto it = std::find_if(user.begin(), user.end(), [&](const User& user)
		{
			return user.name == username;
		});
		if (it != user.end())
		{
			it->isAdmin = false;
			syncWithFile();
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

std::ostream& operator <<(std::ostream& ofs, const std::vector<UserDatabase::availableIndex>& vec)
{
	for (UserDatabase::availableIndex i : vec)
		ofs << static_cast<int>(i) << ' ';
	return ofs;
}

std::istream& operator >>(std::istream& ifs, std::vector<UserDatabase::availableIndex>& vec)
{
	for (auto it = vec.begin(), e = vec.end(); it != e; ++it)
	{
		int a;
		ifs >> a;
		*it = static_cast<UserDatabase::availableIndex>(a);
	}
	return ifs;
}

void UserDatabase::loadFile()
{
	std::ifstream ifs(filename, std::ios_base::binary);
	if (!ifs.is_open())
		throw std::runtime_error("Userdata loadfile could not be opened!");
	while (!ifs.eof())
	{
		User u("temporary", "123");
		if (ifs >> u.name >> u.discordID >> u.isAdmin >> u.availability)
			user.emplace_back(std::move(u));
		else
			break;
	}
	ifs.close();
}

void UserDatabase::reset()
{
	for (auto it = user.begin(), e = user.end(); it != e; ++it)
	{
		it->availability = std::vector<availableIndex>(dayIndex::nCount, availableIndex::notFilled);
	}
	syncWithFile();
}

void UserDatabase::syncWithFile()
{
	std::ofstream ofs;
	ofs.open(filename, std::ofstream::out | std::ofstream::trunc | std::ios_base::binary);
	if (!ofs.is_open())
		throw std::runtime_error("Userdata output file (for sync) could not be opened!");
	std::sort(user.begin(), user.end(), [](User& user1, User& user2)
	{
		return user1.name < user2.name;
	});
	std::for_each(user.begin(), user.end(), [&](const User& user) 
	{
		ofs << user.name		<< ' '
			<< user.discordID	<< ' '
			<< user.isAdmin		<< ' '
			<< user.availability << ' ';
	});
	ofs.close();
}

bool UserDatabase::existsAsUser(std::string username) const
{
	return std::any_of(user.begin(), user.end(), [&](const User& user)
	{
		return (username == user.name);
	});
}

bool UserDatabase::existsAsAdmin(std::string username) const
{
	return std::any_of(user.begin(), user.end(), [&](const User& user)
	{
		return (user.isAdmin && (username == user.name));
	});
}
