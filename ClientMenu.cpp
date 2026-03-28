#include "ClientMenu.h"
#include "GeneralFunctions.h"
#include <string>
#include <iostream> // Also needed for std::cin and std::cout


/*
	Outputs the menu: what different options available to interact with the server
*/
void ClientMenu::DisplayOptions(){

	std::cout << "MessageU client at your service." << std::endl;

	std::cout <<
		"110) Register\n"
		"120) Request for clients list\n"
		"130) Request for public key\n"
		"140) Request for waiting messages\n"
		"150) Send a text message\n"
		"151) Send a request for symmetric key\n"
		"152) Send your symmetric key\n"
		"153) Send a file\n"
		"0) Exit client\n"
		"?"
		<<std::endl;

}

/* 
	Menu for the client to choose how to interact with the server:

	-> User enters string as input 
	-> Input is converted into int which the switch can work with
	-> Output will be one of the options represented as enum, more logical for the human brain
*/
ClientMenu::Menu ClientMenu::ConvertToMenuOption(std::string_view userInput) {
	using enum ClientMenu::Menu;

	int stringToInt;

	// If the input contains any digit, then it's illegal input. Send it as default!
	if (!(general::isNumber(userInput))) {
		stringToInt = 999;
	}
	else {
		//Switch statement can't receive strings
		std::from_chars(userInput.data(), userInput.data() + userInput.size(), stringToInt);
	}


	switch (stringToInt) {

	case 110:
		return ClientMenu::Menu::REGISTER;
	case 120:
		return ClientMenu::Menu::CLIENTS_LIST;
	case 130:
		return ClientMenu::Menu::GET_PUBLIC_KEY;
	case 140:
		return ClientMenu::Menu::PULL_MESSAGES;
	case 150:
		return ClientMenu::Menu::SEND_MESSAGE;
	case 151:
		return ClientMenu::Menu::REQUEST_SYMMETRIC_KEY;
	case 152:
		return ClientMenu::Menu::SEND_SYMMETRIC_KEY;
	case 153:
		return ClientMenu::Menu::SEND_FILE;
	case 0:
		return ClientMenu::Menu::EXIT;

	default:
		return ClientMenu::Menu::OTHER;
	}


}

/*
	Menu's output for the client's decision.

	-> Get the user's input's decision and output whatever necessary
*/
std::string ClientMenu::PickMenuOption(Menu option) {

	using enum ClientMenu::Menu;

	switch (option) {

	case Menu::REGISTER:
		//std::cout << "Register was chosen!" << std::endl;
		return "register";

	case Menu::CLIENTS_LIST:
		//std::cout << "Client list was chosen!" << std::endl;
		return "client list";

	case Menu::GET_PUBLIC_KEY:
		//std::cout << "Public key was chosen!" << std::endl;
		return "public key";

	case Menu::PULL_MESSAGES:
		//std::cout << "Pull messages was chosen!" << std::endl;
		return "pull messages";

	case Menu::SEND_MESSAGE:
		//std::cout << "Send a message was chosen!" << std::endl;
		return "send message";

	case Menu::REQUEST_SYMMETRIC_KEY:
		//std::cout << "Request symmetric key was chosen!" << std::endl;
		return "request symmetric key";

	case Menu::SEND_SYMMETRIC_KEY:
		//std::cout << "Send symmetric key was chosen!" << std::endl;
		return "send symmetric key";
	case Menu::SEND_FILE:
		//std::cout << "Send file was chosen!" << std::endl;
		return "send a file";
	case Menu::EXIT:
		std::cout << "Exit was chosen!" << std::endl;
		return "exit";

	default: /* Other */
		std::cout << "Wrong input was given! try again" << std::endl;
		return "bad input";
	}
}
std::string ClientMenu::OptionSelected(ClientMenu& menu) {
	std::string option;

	std::getline(std::cin, option);
	option = general::reduce(option); // Deletes extra name-spaces
	ClientMenu::Menu optionLegalityCheck = ConvertToMenuOption(option);
	std::string legalOption = menu.PickMenuOption(optionLegalityCheck);
	//std::cout << "Option chosen: " << legalOption << std::endl;

	return legalOption;
}