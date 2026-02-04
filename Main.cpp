#include <format>
#include <string>

#include <fstream>

#include "FileFunctionality.h"


// How to handle files in c++: https://www.geeksforgeeks.org/cpp/file-handling-c-classes/
int main() {

	// Reach the location of server.info.exe file inside the file's location of client.exe (where the file gets its ip:port info)
	std::filesystem::path pathWithEXE = FindWhereEXEFileIs();
	std::filesystem::path finalPath = RemoveFilenameAndSeparator(pathWithEXE, "server.info.txt");

	bool fileExists = isFileExists(finalPath);
	if (fileExists)
	{
		std::fstream fileReadStream(finalPath, std::ios::in);
		std::string readLine;
		getline(fileReadStream, readLine);

		std::cout << "File's content: " << readLine << std::endl;
		std::cout << "Debugging(Main): Hello World!" << std::endl;
		std::cin.get();
		return 0; //File exists -> Contact already exists -> Cancel the process
	}
	else {
		//std::fstream contactFile(path, std::ios::out | std::ios::app);

		//if (!contactFile.is_open()) {
		//	std::cout << "Contact has failed to be created, maybe contact already exists? " << path << std::endl;
		//	return;
		//}
		std::cout << "File doesn't exist!" << std::endl;
		std::cin.get();
		return 0;
	}

	
	//std::cin.get();
	//return 0;
}


