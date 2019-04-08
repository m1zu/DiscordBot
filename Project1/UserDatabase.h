#pragma once

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <ostream>

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
	enum availableIndex
	{
		notFilled = 0,
		no = 1,
		yes = 2,
		unsure = 3
	};

private:
	class User
	{
	public:
		User(std::string name, std::string discordID)
			:
			name(name),
			discordID(discordID),
			availability((int)dayIndex::nCount, availableIndex::notFilled),
			isAdmin(false)
		{}
		User(std::string name, std::string discordID, bool isAdmin, std::vector<availableIndex> availability)
			:
			name(name),
			discordID(discordID),
			availability(availability),
			isAdmin(isAdmin)
		{}
		User(User&& u)
		{
			*this = std::move(u);
		}
		User& operator=(User&& rhs) 
		{
			name = std::move(rhs.name);
			discordID = std::move(rhs.discordID);
			isAdmin = std::move(rhs.isAdmin);
			availability = std::move(rhs.availability);
			return *this;
		}
		User& operator =(const User& rhs)
		{
			name = rhs.name;
			isAdmin = rhs.isAdmin;
			discordID = rhs.discordID;
			availability = rhs.availability;
			return *this;
		}
		User(const User& user)
		{
			*this = user;
		}
		~User() noexcept {}
	public:
		std::string name;
		std::string discordID;
		bool isAdmin = false;
		std::vector<availableIndex> availability;
	};

public:
	UserDatabase();
	~UserDatabase() noexcept;

public:
	bool changeAvailability_day(const std::string& discordID, dayIndex day, bool isAvailable);
	bool changeAvailability_week(const std::string& discordID, std::vector<availableIndex> av);
	std::string getFormatedList();

	bool isUser(const std::string& discordID) const;
	bool isAdmin(const std::string& discordID) const;

	bool addUser(std::string discordID, std::string username);
	bool removeUser(std::string username);
	void reset();

private:
	void loadFile();
	void syncWithFile();

private:
	std::string filename;
	std::vector<User> user;
};

