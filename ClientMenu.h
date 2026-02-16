#pragma once

#include <iostream>

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
		EXIT = 0,
		OTHER = 999
	};

	std::string PickMenuOption(Menu option);
	void DisplayOptions();
	ClientMenu::Menu convertToMenuOption(const char* userInput);

};

