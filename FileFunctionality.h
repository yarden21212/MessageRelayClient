#pragma once
#include <fstream>
#include <filesystem>
#include <iostream>

#include <windows.h>

bool isFileExists(std::filesystem::path path);
std::filesystem::path FindWhereEXEFileIs();
std::filesystem::path RemoveFilenameAndSeparator(std::filesystem::path path, std::string newFileName);