#pragma once
#include "ServerClient.h"
#include "Info.h"
#include <sstream>

#define SERVER_NOT_FULL "OK"
#define SERVER_FULL "FULL"
#define SHUTDOWN_SIGNAL "/shutdown"

template<class T>
std::string toString(const T& thing) {
	std::ostringstream stream;
	stream << thing;
	return stream.str();
}

class Server {

	unsigned int port;
	unsigned int maxSockets;
	unsigned int bufferSize;
	unsigned int maxClients;

	IPaddress serverIP; 
	TCPsocket serverSocket;
	std::string hostName;

	TCPsocket* clientSocket;
	bool* socketIsFree;
	char* buffer;

	SDLNet_SocketSet socketSet;
	unsigned int clientCount;
	bool shutdownServer;

	void(*activityReaction)(Server*, ClientInfo);

public:

	Server(unsigned int port, unsigned int bufferSize, unsigned int maxSockets, void reaction(Server*, ClientInfo), std::string hostName) {

		shutdownServer = false;

		this->port = port;
		this->bufferSize = bufferSize;
		this->maxSockets = maxSockets;
		this->maxClients = maxSockets - 1;
		this->activityReaction = reaction;

		this->clientSocket = new TCPsocket[maxClients];
		this->socketIsFree = new bool[maxClients];
		this->buffer = new char[bufferSize];
		this->hostName = hostName;

		clientCount = 0;

		socketSet = SDLNet_AllocSocketSet(maxSockets);

		if (socketSet == NULL)
			std::cout << "Failed to allocate the socket set : " << SDLNet_GetError() << std::endl;

		int hostResolved = SDLNet_ResolveHost(&serverIP, NULL, port);

		if (hostResolved == -1)
			std::cout << "Failed to resolve server host : " << SDLNet_GetError() << std::endl;

		for (unsigned int i = 0; i < maxClients; ++i) {
			clientSocket[i] = NULL;
			socketIsFree[i] = true;
		}

		serverSocket = SDLNet_TCP_Open(&serverIP);

		if (!serverSocket)
			std::cout << "Failed to open server socket : " << SDLNet_GetError() << std::endl;

		SDLNet_TCP_AddSocket(socketSet, serverSocket);
	}

	~Server() {
		
		for (unsigned int i = 0; i < maxClients; ++i) {
			if (!socketIsFree[i]) {
				SDLNet_TCP_Close(clientSocket[i]);
				socketIsFree[i] = true;
			}
		}

		SDLNet_TCP_Close(serverSocket);
		SDLNet_FreeSocketSet(socketSet);

		delete[] clientSocket;
		delete[] socketIsFree;
		delete[] buffer;
	}

	void checkForConnections(){
		
		int numActiveSockets = SDLNet_CheckSockets(socketSet, 1);
		int serverSocketActivity = SDLNet_SocketReady(serverSocket);

		if (serverSocketActivity != 0) {
			if (clientCount < maxClients) {

				
				int freeSpot = -99;
				for (unsigned int i = 0; i < maxClients; ++i) {
					if (socketIsFree[i]) {

						socketIsFree[i] = false;
						freeSpot = i;
						break;
					}
				}

				std::cout << "New Client" << std::endl;

				std::string bufferString;

				clientSocket[freeSpot] = SDLNet_TCP_Accept(serverSocket);
				SDLNet_TCP_AddSocket(socketSet, clientSocket[freeSpot]);
				++clientCount;
				ServerInfo info;
				info.clientName = "New client";
				info.message = new std::string(SERVER_NOT_FULL);
				info.convertToString(bufferString);
				buffer = (char*)bufferString.c_str();
				int msgLength = strlen(buffer) + 1;
				SDLNet_TCP_Send(clientSocket[freeSpot], (void*)buffer, msgLength);
			}
			else {

				TCPsocket tempSock = SDLNet_TCP_Accept(serverSocket);

				strcpy(buffer, SERVER_FULL);
				int msgLength = strlen(buffer) + 1;
				SDLNet_TCP_Send(tempSock, (void *)buffer, msgLength);
				SDLNet_TCP_Close(tempSock);
			}
		} 
	}

	void checkForActivity() {
		
		for (unsigned int clientNumber = 0; clientNumber < maxClients; ++clientNumber) {

			int clientSocketActivity = SDLNet_SocketReady(clientSocket[clientNumber]);

			if (clientSocketActivity != 0) {

				int receivedByteCount = SDLNet_TCP_Recv(clientSocket[clientNumber], buffer, bufferSize);

				if (receivedByteCount <= 0) {

					SDLNet_TCP_DelSocket(socketSet, clientSocket[clientNumber]);
					SDLNet_TCP_Close(clientSocket[clientNumber]);
					clientSocket[clientNumber] = NULL;
					socketIsFree[clientNumber] = true;
					--clientCount;
				}
				else 
					activityReaction(this, ClientInfo(buffer));
			}
		}
	}

	void sendToClient(ServerInfo info) {

		std::string str;
		info.convertToString(str);
		const char* message = str.c_str();
		unsigned int msgLength = strlen(message) + 1;

		for (unsigned int i = 0; i < maxClients; ++i) {

			if (msgLength > 1 && !socketIsFree[i])
				SDLNet_TCP_Send(clientSocket[i], (void*)message, msgLength);
		}
	}

	bool getShutdownStatus() {
		return shutdownServer;
	}

	void setShutdownStatus(bool shutdown) {

		shutdownServer = shutdown;
	}

	std::string getHostName() {
		return hostName;
	}
};
