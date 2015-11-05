#pragma once
#include <iostream>

#define QUIT_SIGNAL "/quit"
#define CONNECTION_TIMEOUT_PERIOD 5000
#define SOCKET_SET_POLL_PERIOD 10
#define CONNECTION_SUCCESSFUL "/success"
#define SHUTDOWN_SIGNAL "/shutdown"

class ServerClient {
private:

	unsigned int serverPort;
	unsigned int bufferSize;

	std::string serverHostname;
	IPaddress serverIP;
	int ID;
	std::string clientName;
	TCPsocket clientSocket;

	SDLNet_SocketSet socketSet;
	bool shutdownClient;

	void(*newMessageReaction)(ServerInfo*);

public:

	ServerClient(std::string serverAddress, unsigned int serverPort, unsigned int bufferSize, void reaction(ServerInfo*), std::string name) {

		this->shutdownClient = false;
		this->serverHostname = serverAddress;
		newMessageReaction = reaction;

		this->serverPort = serverPort;
		this->bufferSize = bufferSize;
		clientName = name;

		socketSet = SDLNet_AllocSocketSet(2);

		if (socketSet == NULL)
			std::cout << "Failed to allocate the socket set : " << SDLNet_GetError() << std::endl;
	}

	~ServerClient(){

		SDLNet_TCP_Close(clientSocket);
		SDLNet_FreeSocketSet(socketSet);

	}

	void connectToServer(){

		int hostResolved = SDLNet_ResolveHost(&serverIP, serverHostname.c_str(), serverPort);
		
		clientSocket = SDLNet_TCP_Open(&serverIP);
		if (clientSocket) {

			SDLNet_TCP_AddSocket(socketSet, clientSocket);
			int activeSockets = SDLNet_CheckSockets(socketSet, CONNECTION_TIMEOUT_PERIOD);

			int gotServerResponse = SDLNet_SocketReady(clientSocket);

			if (gotServerResponse != 0) {

				char* buffer = new char[bufferSize];
				int serverResponseByteCount = SDLNet_TCP_Recv(clientSocket, buffer, bufferSize - 1);

                ServerInfo* serverInfo = new ServerInfo(buffer);
                ID = serverInfo->clientID;
				delete[] buffer;

                ClientInfo* info = new ClientInfo();
				info->ID = ID;
                info->message = new std::string(CONNECTION_SUCCESSFUL);
                info->name = clientName;

                sendToServer(info);

				delete info;
				delete serverInfo;
			}

			else
				shutdownClient = true;
		}
		else {
			shutdownClient = true;
		}
	}

	void checkForIncomingMessages(){

		ServerInfo* info;

		int activeSockets = SDLNet_CheckSockets(socketSet, SOCKET_SET_POLL_PERIOD);

		if (activeSockets != 0) {

			int gotMessage = SDLNet_SocketReady(clientSocket);

			if (gotMessage != 0) {

				char* buffer = new char[bufferSize];
				int serverResponseByteCount = SDLNet_TCP_Recv(clientSocket, buffer, bufferSize - 1);

				if (serverResponseByteCount != 0) {

                    info = new ServerInfo(buffer);

					if (*info->message == SHUTDOWN_SIGNAL)
						shutdownClient = true;

					newMessageReaction(info);
					delete info;
				}
				delete[] buffer;
			}
		}
	}

	void sendToServer(ClientInfo* info) {

		std::string str;
		info->convertToString(str);
		const char* message = str.c_str();

		unsigned int msgLength = strlen(message);

		SDLNet_TCP_Send(clientSocket, (void *)message, msgLength);
	}

	bool getShutdownStatus() {
		return shutdownClient;
	}

	std::string getClientName() {
		return clientName;
	}

	int getClientID() {

		return ID;
	}
};