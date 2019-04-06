
/*If you are using uWebSockets,
  remember to change this to uwebsockets_websocket.h*/

#include "Botclient.h"

int main() {
	BotClient client("NTU2MjE1OTgwNDk0MjkwOTc3.D22mEg.zCGW1IwEoqS4YPHb69hzYBswtig", 2);
	client.run();
}