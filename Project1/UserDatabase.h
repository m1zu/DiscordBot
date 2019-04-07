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
		User(std::string name, std::string discordID)
			:
			name(name),
			discordID(discordID),
			availability((int)dayIndex::nCount, false),
			isAdmin(false)
		{}
		User(std::string name, std::string discordID, bool isAdmin, std::vector<bool> availability)
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
			isAdmin = rhs.isAdmin;
			availability = rhs.availability;
			return *this;
		}
		~User() noexcept {}
	public:
		bool isAdmin = false;
		std::string name;
		std::string discordID;
		std::vector<bool> availability;
	};

public:
	UserDatabase();
	~UserDatabase() noexcept;

public:
	bool changeAvailability(const std::string& discordID, dayIndex day, bool isAvailable);
	std::string getFormatedList() const;

	bool isUser(const std::string& discordID) const;
	bool isAdmin(const std::string& discordID) const;
	bool addUser(const std::string& pingedUserInput);
	bool removeUser(const std::string& pingedUserInput);

private:
	void loadFile(const std::string& filename);
	void reset();
	void add(User&& user);
	void remove(User&& user);

private:
	std::vector<User> user;
};

