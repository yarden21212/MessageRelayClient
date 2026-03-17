#include <sstream>
#include <string>

#include <fstream>

#include "FileFunctionality.h"
#include "ClientMenu.h"
#include "ClientConnection.h"
#include "NetworkBelonging.h"



// How to handle files in c++: https://www.geeksforgeeks.org/cpp/file-handling-c-classes/
int main(int argc, char* argv[]) {

	// Reach the location of server.info.exe file inside the file's location of client.exe (where the file gets its ip:port info)
	std::filesystem::path pathWithEXE = FindWhereEXEFileIs();
	// Replaces the client.exe suffix with server.info.txt suffix (the files that contains the socket address we need
	std::filesystem::path requestedPath = RemoveFilenameAndSeparator(pathWithEXE, "server.info.txt");

	bool fileExists = isFileExists(requestedPath);
	if (fileExists)
	{
		std::fstream fileReadStream(requestedPath, std::ios::in);
		std::string networkInfoTemp;
		getline(fileReadStream, networkInfoTemp);

		char* networkInfoPtr = networkInfoTemp.data();
		//std::cout << "File's content: " << networkInfoPtr << std::endl;


		std::string ip;
		std::string port;
		try {
			ip = networkPart(networkInfoPtr, "ip");
			port = networkPart(networkInfoPtr, "port");
		}
		catch (const std::invalid_argument& e) {
			std::cout << "[Invalid_argument ERROR]: " << e.what() << std::endl;
		}
		catch (const std::exception& e) {
			std::cout << "[Invalid_argument ERROR]: " << e.what() << std::endl;
		}

		TCPConnector(ip, port);

	}
	//else {
	//	//std::fstream contactFile(path, std::ios::out | std::ios::app);

	//	//if (!contactFile.is_open()) {
	//	//	std::cout << "Contact has failed to be created, maybe contact already exists? " << path << std::endl;
	//	//	return;
	//	//}
	//	std::cout << "File doesn't exist!" << std::endl;
	//	std::cin.get();
	//}

	////TCPConnector(argc, argv);
	//TCPConnector(networkInfo);
	
	std::system("pause");
	return 0;
}


