#pragma once
#include <fstream>
#include <filesystem>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <locale>
#include <codecvt>

bool isFileExists(std::filesystem::path path);
std::filesystem::path FindWhereEXEFileIs();
std::filesystem::path RemoveFilenameAndSeparator(std::filesystem::path path, std::string newFileName);
void OpenFolderInsideExeFile(std::string folderName);
bool CheckIfFileAlreadyExists(const std::filesystem::path path);
bool CreateUserInfoFile(const std::string username, const std::string uniqueIdentifier, const std::string privateKey);