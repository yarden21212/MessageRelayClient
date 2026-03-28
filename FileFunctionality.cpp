#include "FileFunctionality.h"

#include <windows.h>

/* Returns true or false in case a file exists in a given path  */
bool isFileExists(std::filesystem::path path) {


	/* --------------------- First implementation ----------------------------*/
	//Holds the file status to check if the file exists or not
	std::filesystem::file_status s = std::filesystem::file_status{};

	//Checks if the file exists in the given path
	if (std::filesystem::status_known(s) ? std::filesystem::exists(s) : std::filesystem::exists(path))
	{
		//std::cout << "File exists in location: " << path << std::endl;
		return true;
	}
	return false;
}

/*
	Finds the location of exe file. This is where the client's port resides
	GetModuleFileNameW() to determine where the .exe is: https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulefilenamew
	Convert wchar_t object to filesystem type: https://discord.com/channels/331718482485837825/1468520989511122955/1468552048403415172
*/
std::filesystem::path FindWhereEXEFileIs() {
	wchar_t buffer[MAX_PATH];

	DWORD len = GetModuleFileNameW(
		NULL,      // = this exe
		buffer,    // where to store the path
		MAX_PATH   // buffer size
	);
	
	std::filesystem::path path = buffer;

	//std::wcout << "Debugging(FileFunctionality): filesystem path is: " << path << std::endl;

	return path;
}

/* Replaces the original filename(client.exe) with the file we need(server.info.txt) in our case */ 
std::filesystem::path RemoveFilenameAndSeparator(std::filesystem::path path,std::string newFileName) {

	path.replace_filename(newFileName);

	//std::wcout << "Debbuging(FileFunctionality): filesystem path is: " << path << "\n\n";

	return path;
}

/* Creates a folder with a given name in the location: of the exe file */
void OpenFolderInsideExeFile(std::string folderName) {
	std::filesystem::path exePath = FindWhereEXEFileIs();
	std::filesystem::path pathWithoutExeSuffix = RemoveFilenameAndSeparator(exePath, "");
	try {
		if (std::filesystem::create_directory(pathWithoutExeSuffix / folderName)) {
			/*std::cout << "Folder " << folderName << "created" << std::endl;*/
		}

		//else
		//	std::cerr << "Folder already exists!\n";
	}
	catch (const std::exception& e) {
		std::cerr << "[ERROR]: during folder creation: " << e.what() << '\n';
	}
}

bool CheckIfFileAlreadyExists(const std::filesystem::path path) {
	if (std::filesystem::exists(path))
		return true;
	else
		return false;
}
bool CreateUserInfoFile(const std::string username, const std::string uniqueIdentifier, const std::string privateKey) {

	std::filesystem::path path = FindWhereEXEFileIs();
	try {
		std::string fileName = "Users\\" + username + ".info.txt";
		std::filesystem::path pathWithoutExeSuffix = RemoveFilenameAndSeparator(path, fileName);

		bool fileExists = CheckIfFileAlreadyExists(pathWithoutExeSuffix);

		//std::cout << "pathWithoutExeSuffix: " << pathWithoutExeSuffix << std::endl;
		//std::cout << "fileExists: " << fileExists << std::endl;

		if (!fileExists) {
			std::ofstream file(pathWithoutExeSuffix, std::ios::out | std::ios::binary | std::ios::trunc);

			if (!file.is_open())
				return false;

			file << username << '\n';
			file << uniqueIdentifier << '\n';
			file.write(privateKey.data(), privateKey.size());

			file.close();

			//std::cout << "Succeed to store the user info locally!\n";
			return true;
		}
		else {
			std::cout << "User already exists!\n";
			return false;
		}
	}
	catch (const std::exception& e) {
		std::cout << "Failed to access the folder!\n " << e.what();
		return false;
	}
}

/*
	Where the client.exe is stored, we have different folders&files,
	We want to check the folders or files exist before moving forward.
*/
bool checkIfFileOrFolderExistsInExeFolder(std::string folderName) {
	std::filesystem::path path = FindWhereEXEFileIs();
	std::string fileName = folderName;
	std::filesystem::path pathWithoutExeSuffix = RemoveFilenameAndSeparator(path, fileName);

	return std::filesystem::exists(pathWithoutExeSuffix);
}


//int main() {
//	/*RSAPrivateWrapper rsa;
//	std::string privateKey = rsa.getPrivateKey();*/
//
//	//OpenFolderInsideExeFile("Users");
//	//CreateUserInfoFile("Yarden", "55HJPP234!@#$%!@ASASVASV", privateKey);
//
//
//	std::cout << "Users:" << checkIfFileOrFolderExistsInExeFolder("Users") << std::endl;
//
//
//	std::vector<uint8_t> clientId = getUserId("Yarden");
//	std::cout << clientId.data() << std::endl;
//
//	std::system("pause");
//	return 0;
//}

