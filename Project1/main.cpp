
/*If you are using uWebSockets,
  remember to change this to uwebsockets_websocket.h*/

#include "Botclient.h"

#include <iostream>
#include <fstream>
#include <exception>

int main(int argc, char** argv) 
{
	if (argc != 2)
	{
		std::cout << "Usage of executable:\n ./executable  path/to/tokenfile" << std::endl;
		return -1;
	}

	try
	{
		std::string tokenFileName(argv[1]);
		std::ifstream tokenFile(tokenFileName, std::ios_base::binary);
		if (!tokenFile)
			throw std::runtime_error("Could not load tokenfile: " + tokenFileName);

		std::string token;
		tokenFile >> token;
		if (!tokenFile)
			throw std::runtime_error("Cannot read token from file: " + tokenFileName);

		BotClient client(token, 2);
		client.run();
	}
	catch (std::runtime_error& err)
	{
		std::cerr << "Catched runtime exception in main():\n";
		std::cerr << err.what();
		return -1;
	}
	catch (...)
	{
		std::cerr << "Catched some unidentified exception in main()!";
	}
}