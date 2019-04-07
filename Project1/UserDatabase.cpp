#include "UserDatabase.h"

#include <fstream>

UserDatabase::UserDatabase()
{
	loadFile("userdata.dat");
}

UserDatabase::~UserDatabase()
{
}

std::ostream& operator <<(std::ostream& ofs, const std::vector<bool>& vec)
{
	for (bool b : vec)
		ofs << b << ' ';
	return ofs;
}

std::istream& operator >>(std::istream& ifs, std::vector<bool>& vec)
{
	for (auto it = vec.begin(), e = vec.end(); it != e; ++it)
	{
		bool a;
		ifs >> a;
		*it = a;
	}
	return ifs;
}

void UserDatabase::loadFile(const std::string& filename)
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
}
