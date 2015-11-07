#include <iostream>
#include <list>
#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>
#include <exception>
#include "Server.h"

void serverReaction(Server* server, ClientInfo* info) {

    ServerInfo* send = new ServerInfo();
    send->clientName = info->name;
    send->message = new std::string(*info->message);
    send->clientID = info->ID;
    send->message_type = NEW_MESSAGE;

    if (info->message->c_str() == SHUTDOWN_SIGNAL)
        server->setShutdownStatus(true);

    else
        server->sendToClient(send);

    delete send;
}

int main(int argc, char** argv) {

    if (SDLNet_Init() == -1)
        std::cout << "Error : " << SDLNet_GetError() << std::endl;

    Server *server = new Server(21225, 512, 5, serverReaction, "server");

    try {

        while (!server->getShutdownStatus()) {

            server->checkForConnections();
            server->checkForActivity();
        }

    }

    catch (std::exception& e) {

        std::cout << e.what();
    }

    return 0;
}
