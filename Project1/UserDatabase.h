#pragma once

#include <string>
#include <vector>
#include <map>
#include <fstream>

class UserDatabase
{
public:
	enum dayIndex
	{
		sunday = 0,
		monday = 1,
		tuesday = 2,
		wednesday = 3,
		thursday = 4,
		friday = 5,
		saturday = 6,
		nCount = 7
	};

private:
	class User
	{
	public:
		bool isAdmin = false;
		std::string name;
		std::string discordID;
		std::vector<bool> availability;
	};

public:
	UserDatabase();
	~UserDatabase();

public:
	bool changeAvailability(const std::string& discordID, dayIndex day, bool isAvailable);
	std::string getFormatedList() const;

	bool isUser(const std::string& discordID) const;
	bool isAdmin(const std::string& discordID) const;
	bool loadFile() const;
	bool saveFile();

	void reset();

private:
	std::vector<User> user;
	std::fstream userList;
};

