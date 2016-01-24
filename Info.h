#pragma once
#include <string>

enum server_message_type {NEW_CLIENT, NEW_MESSAGE, CLIENT_DISCONNECTED, CLIENTS_CONNECTED};

class ServerInfo {

public:



	server_message_type message_type;
	int clientID;
	std::string clientName;
	std::string* message;
	float servoDegree;
	int speed;

	ServerInfo() {

		message = nullptr;
	}

	ServerInfo(char* info) {

		std::string* values = new std::string(info);
		int letter = 0;

		int begin = 0;

		std::string tmp;

		for (int i = 0; i < 4; ++i) {

			while (values->at(letter) != '\n') ++letter;

			for (int k = begin; k < letter; ++k)
				tmp.push_back(values->at(k));

			begin = ++letter;

			switch (i) {

				case 0:

					message_type = server_message_type(SDL_atoi(tmp.c_str()));
					break;

				case 1:

					clientID = SDL_atoi(tmp.c_str());
					break;
				case 2:
					clientName = std::string(tmp);
					break;

				case 3:

					if (tmp == "NULL")
						message = nullptr;
					else
						message = new std::string(tmp);
					break;
				case 4: 
					servoDegree = SDL_atof(tmp.c_str());
					break;
				case 5 :
					speed = SDL_atoi(tmp.c_str());
					break;
			}
			tmp.clear();
		}
		delete values;
	}

	~ServerInfo() {

		delete message;
	}

	virtual void convertToString(std::string &returnValue) {

		char chr[10];

		returnValue += SDL_itoa(message_type, chr, 10);
		returnValue.push_back('\n');

		returnValue += SDL_itoa(clientID, chr, 10);
		returnValue.push_back('\n');

		returnValue += clientName;
		returnValue.push_back('\n');

		if (message != nullptr)
			returnValue += *message;
		else
			returnValue.append("NULL");
		returnValue.push_back('\n');
		
		returnValue += std::to_string(servoDegree);
		returnValue.push_back('\n');

		returnValue += std::to_string(speed);
		returnValue.push_back('\n');
	}
};

class ClientInfo {

public:

	int ID;
	std::string name;
	std::string* message;
	float servoDegree;
	int speed;
	ClientInfo() {

		message = nullptr;
	}

	ClientInfo(char* info) {

		std::string* values = new std::string(info);
		int letter = 0;

		int begin = 0;
		std::string tmp;

		for (int i = 0; i < 3; ++i) {

			while (values->at(letter) != '\n') ++letter;

			for (int k = begin; k < letter; ++k)
				tmp.push_back(values->at(k));

			begin = ++letter;

			switch (i) {

				case 0:

					ID = SDL_atoi(tmp.c_str());
					break;

				case 1:
					name = std::string(tmp);
					break;

				case 2:

					if (tmp == "NULL")
						message = nullptr;
					else
						message = new std::string(tmp);
					break;

				case 3:
                                        servoDegree = SDL_atof(tmp.c_str());
                                        break;
                                case 4:
                                        speed = SDL_atoi(tmp.c_str());
					break;

			}
			tmp.clear();
		}
		delete values;
	}

	~ClientInfo() {

		delete message;
	}

	virtual void convertToString(std::string &returnValue) {


		char chr[10];

		returnValue += SDL_itoa(ID, chr, 10);
		returnValue.push_back('\n');

		returnValue += name;
		returnValue.push_back('\n');

		if (message != nullptr)
			returnValue += *message;
		else
			returnValue.append("NULL");
		returnValue.push_back('\n');

                returnValue += std::to_string(servoDegree);
                returnValue.push_back('\n');

                returnValue += std::to_string(speed);
                returnValue.push_back('\n');

	}
};
