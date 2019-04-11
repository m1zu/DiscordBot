
/*If you are using uWebSockets,
  remember to change this to uwebsockets_websocket.h*/

#include "Botclient.h"

#include <iostream>
#include <fstream>
#include <exception>

int main() 
{
	try
	{
		std::string token;
		{
			std::ifstream tokenFile("token.txt", std::ios_base::binary);
			if (!tokenFile)
				throw std::runtime_error("Cannot load tokenfile!");

			tokenFile >> token;
			if (!tokenFile)
				throw std::runtime_error("Cannot read token from file: File is emtpy?");
		}
		BotClient client(token);
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
		return -1;
	}
}