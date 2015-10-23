#pragma once
#include <string>

class ServerInfo {

public:

	std::string clientName;
	std::string* message;

	ServerInfo() {

	}

	ServerInfo(const char* info) {

		std::string values(info);
		std::string tmp = values;
		int numberValue = 1;
		int letter = 0;
		bool ok = false;

		while (!ok) {

			while (tmp[letter] != '\n') ++letter;

			tmp.erase(letter, tmp.size());
			values.erase(0, letter + 1);

			switch (numberValue) {

			case 1:
				clientName = std::string(tmp);
				break;

			case 2:

				if (tmp == "NULL")
					message = nullptr;
				else
					message = new std::string(tmp);
				break;
			}
			++numberValue;
			letter = 0;

			if (values.size() > 0)
				tmp = values;
			else
				ok = true;

		}
	}

	virtual void convertToString(std::string &returnValue) {

		returnValue += clientName;
		returnValue.push_back('\n');

		if (message != nullptr)
			returnValue += *message;
		else
			returnValue.append("NULL");
		returnValue.push_back('\n');
	}
};

class ClientInfo {

public:

	std::string name;
	std::string* message;

	ClientInfo() {

	}

	ClientInfo(const char* info) {

		std::string values(info);
		std::string tmp = values;
		int numberValue = 1;
		int letter = 0;
		bool ok = false;

		while (!ok) {

			while (tmp[letter] != '\n') ++letter;

			tmp.erase(letter, tmp.size());
			values.erase(0, letter + 1);

			switch (numberValue) {

			case 1:
				name = std::string(tmp);
				break;

			case 2:

				if (tmp == "NULL")
					message = nullptr;
				else
					message = new std::string(tmp);
				break;
			}
			++numberValue;
			letter = 0;

			if (values.size() > 0)
				tmp = values;
			else
				ok = true;
		}
	}

	virtual void convertToString(std::string &returnValue) {

		returnValue += name;
		returnValue.push_back('\n');

		if (message != nullptr)
			returnValue += *message;
		else
			returnValue.append("NULL");
		returnValue.push_back('\n');

	}
};