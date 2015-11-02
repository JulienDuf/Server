#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include "Server.h"

void serverReaction(Server* server, ClientInfo info) {

    ServerInfo send;
    send.clientName = info.name;
    send.message = info.message;

    if (info.message->c_str() == SHUTDOWN_SIGNAL)
        server->setShutdownStatus(true);

    else
        server->sendToClient(send);
}

int main(int argc, char** argv) {

   Server* server = new Server(21255, 512, 5, serverReaction, "server");

   if (SDLNet_Init() == -1)
	std::cout << "Error : " << SDLNet_GetError() << std::endl;

    while (!server->getShutdownStatus()){

        server->checkForConnections();
        server->checkForActivity();
    }

    return 0;
}
