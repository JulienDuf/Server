#pragma once
#include "Info.h"
#include <sstream>
#include <iostream>
#include <time.h>

#define SERVER_NOT_FULL "OK"
#define SERVER_FULL "FULL"
#define SHUTDOWN_SIGNAL "/shutdown"
#define CONNECTION_SUCCESSFUL "/success"

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

	std::string* clientNames;
	TCPsocket* clientSocket;
	bool* socketIsFree;

	SDLNet_SocketSet socketSet;
	unsigned int clientCount;
	bool shutdownServer;

	void(*activityReaction)(Server*, ClientInfo*);

public:

	Server(unsigned int port, unsigned int bufferSize, unsigned int maxSockets, void reaction(Server*, ClientInfo*), std::string hostName) {

		shutdownServer = false;

		this->port = port;
		this->bufferSize = bufferSize;
		this->maxSockets = maxSockets;
		this->maxClients = maxSockets - 1;
		this->activityReaction = reaction;

		this->clientSocket = new TCPsocket[maxClients];
		this->socketIsFree = new bool[maxClients];
		this->clientNames = new std::string[maxClients];
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
		delete[] clientNames;
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

				std::cout << "New client" << std::endl;

				std::string bufferString;

				clientSocket[freeSpot] = SDLNet_TCP_Accept(serverSocket);
				SDLNet_TCP_AddSocket(socketSet, clientSocket[freeSpot]);
				++clientCount;
				ServerInfo* info = new ServerInfo();
				info->clientName = "New client";
				info->message = new std::string(SERVER_NOT_FULL);
				info->message_type = NEW_CLIENT;
				info->clientID = freeSpot;
				info->convertToString(bufferString);
				const char* bufferTmp = bufferString.c_str();
				int msgLength = strlen(bufferTmp) + 1;
				SDLNet_TCP_Send(clientSocket[freeSpot], (void*)bufferTmp, msgLength);
				delete info;

				//while (SDLNet_SocketReady(clientSocket[freeSpot]) == 0);

				char* buffer = new char[bufferSize];

				int receivedByteCount = SDLNet_TCP_Recv(clientSocket[freeSpot], buffer, bufferSize);

				if (receivedByteCount > 0) {

					ClientInfo* clientInfo = new ClientInfo(buffer);
					delete[] buffer;

					if (*clientInfo->message == CONNECTION_SUCCESSFUL) {

						ServerInfo* info = new ServerInfo();
						info->message = new std::string("New connection");
						info->clientID = clientInfo->ID;
						info->clientName = clientInfo->name;
						info->message_type = NEW_CLIENT;
						clientNames[freeSpot] = clientInfo->name;

						sendToClient(info, freeSpot);
						delete info;
					}

					delete clientInfo;

				}

				for (unsigned int k = 0; k < maxClients; ++k) {

					if (k != freeSpot && !socketIsFree[k]) {

						std::string str;

						info = new ServerInfo();
						info->clientName = clientNames[k];
						info->message = new std::string("One of the clients");
						info->message_type = CLIENTS_CONNECTED;
						info->clientID = k;
						info->convertToString(str);
						bufferTmp = str.c_str();
						msgLength = strlen(bufferTmp) + 1;
						SDLNet_TCP_Send(clientSocket[freeSpot], (void*)bufferTmp, msgLength);
						delete info;

						SDL_Delay(100);

					}
				}

			}
			else {

				TCPsocket tempSock = SDLNet_TCP_Accept(serverSocket);

				char* buffer = new char[bufferSize];

#if defined(_WIN32)
				strcpy_s(buffer, sizeof(char*), SERVER_FULL);
#else
				strcpy(buffer, SERVER_FULL);
#endif
				int msgLength = strlen(buffer) + 1;
				SDLNet_TCP_Send(tempSock, (void *)buffer, msgLength);
				SDLNet_TCP_Close(tempSock);
				delete[] buffer;
			}
		} 
	}

	void checkForActivity() {

		for (unsigned int clientNumber = 0; clientNumber < maxClients; ++clientNumber) {

			int clientSocketActivity = SDLNet_SocketReady(clientSocket[clientNumber]);

			if (clientSocketActivity != 0) {

				char* buffer = new char[bufferSize];
				int receivedByteCount = SDLNet_TCP_Recv(clientSocket[clientNumber], buffer, bufferSize);

				if (receivedByteCount <= 0) {

					SDLNet_TCP_DelSocket(socketSet, clientSocket[clientNumber]);
					SDLNet_TCP_Close(clientSocket[clientNumber]);
					clientSocket[clientNumber] = NULL;
					socketIsFree[clientNumber] = true;

					ServerInfo* info = new ServerInfo();
					info->clientName = clientNames[clientNumber];
					info->message = new std::string("A client quit");
					info->clientID = clientNumber;
					info->message_type = CLIENT_DISCONNECTED;
					sendToClient(info);
					delete info;

					clientNames[clientNumber].clear();
					--clientCount;
				}

				else {
					ClientInfo* clientInfo = new ClientInfo(buffer);
					activityReaction(this, clientInfo);
					delete clientInfo;
				}

				delete[] buffer;
			}
		}
	}

	void sendToClient(ServerInfo* info) {

		std::string str;
		info->convertToString(str);
		const char* message = str.c_str();
		unsigned int msgLength = strlen(message) + 1;

		for (unsigned int i = 0; i < maxClients; ++i) {

			if (msgLength > 1 && !socketIsFree[i])
				SDLNet_TCP_Send(clientSocket[i], (void*)message, msgLength);
		}
	}

	void sendToClient(ServerInfo* info, int exception) {

		std::string str;
		info->convertToString(str);
		const char* message = str.c_str();
		unsigned int msgLength = strlen(message) + 1;

		for (unsigned int i = 0; i < maxClients; ++i) {

			if (msgLength > 1 && !socketIsFree[i] && i != exception)
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
