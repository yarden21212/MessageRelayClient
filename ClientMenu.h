#pragma once

#include <iostream>
#include <string_view>
#include <string> // For stoi
#include <charconv> // For std::from_chars

class ClientMenu {

public:
	/* Learn about scoped enums here: https://www.learncpp.com/cpp-tutorial/scoped-enumerations-enum-classes/ */
	enum class Menu {
		REGISTER = 110,
		CLIENTS_LIST = 120,
		GET_PUBLIC_KEY = 130,
		PULL_MESSAGES = 140,
		SEND_MESSAGE = 150,
		REQUEST_SYMMETRIC_KEY = 151,
		SEND_SYMMETRIC_KEY = 152,
		SEND_FILE = 153,
		EXIT = 0,
		OTHER = 999
	};

	std::string PickMenuOption(Menu option);
	void DisplayOptions();
	Menu ConvertToMenuOption(std::string_view userInput);
	std::string OptionSelected(ClientMenu& menu);
};

