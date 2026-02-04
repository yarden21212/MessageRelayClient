#include "FileFunctionality.h"

//#include <iostream>
//#include <windows.h>
#include <stdlib.h>
#include <string>


#include <locale>
#include <codecvt>

/* Returns true or false in case a file exists in a given path  */
bool isFileExists(std::filesystem::path path) {


	/* --------------------- First implementation ----------------------------*/
	//Holds the file status to check if the file exists or not
	std::filesystem::file_status s = std::filesystem::file_status{};

	//Checks if the file exists in the given path
	if (std::filesystem::status_known(s) ? std::filesystem::exists(s) : std::filesystem::exists(path))
	{
		std::cout << "File exists in location: " << path << std::endl;
		return true;
	}
	return false;


	/* ----------------------- Second implementation -----------------------------*/
	/*std::ifstream file(path);

	if (file) {
		std::cout << " Contact already exists " << std::endl;
		return true;
	}
	return false;*/
}

/*
	Find the location of exe file, this is where the client's port resides
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

	std::wcout << "Debugging(FileFunctionality): filesystem path is: " << path << std::endl;

	return path;
}

/* Replaces the original filename(client.exe) with the file we need(server.info.txt) in our case */ 
std::filesystem::path RemoveFilenameAndSeparator(std::filesystem::path path,std::string newFileName) {

	path.replace_filename(newFileName);

	std::wcout << "Debbuging(FileFunctionality): filesystem path is: " << path << "\n\n";

	return path;
}