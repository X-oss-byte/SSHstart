#pragma once

#include <string>
#include <set>
#include <fstream>
#include <windows.h>
#include <ShlObj.h>
#include "ConfigFile.h"

using namespace std;

ConfigFile::ConfigFile(wstring cFolderPath, wstring cFilePath, string cType, char cEditKey)
	: folderPath{ cFolderPath }, filePath{ cFilePath }, type{ cType }, editKey{ cEditKey } { }

ConfigFile::ConfigFile(REFKNOWNFOLDERID knownFolderID, LPCWSTR folder, LPCWSTR file, string cType, char cEditKey)
	: type{ cType }, editKey{ cEditKey } {

	LPWSTR knownFolder;
	SHGetKnownFolderPath(knownFolderID, NULL, NULL, &knownFolder);

	folderPath = wstring(knownFolder) + L"\\" + folder;
	filePath = folderPath + L"\\" + file;

	CoTaskMemFree(knownFolder);
}

set<string> ConfigFile::getHosts() {
	set<string> hosts;

	ifstream file(filePath.c_str());

	if (!file.good())
		return hosts;

	string line;

	while (getline(file, line)) {
		// Remove whitespace
		line.erase(0, line.find_first_not_of(whitespace));
		line.erase(line.find_last_not_of(whitespace) + 1);

		// Check for and remove "Host"
		if (lstrcmpiA("HOST", line.substr(0, 4).c_str()) != 0)
			continue;

		line.erase(0, 4);

		// Check for and remove separator
		size_t nonSepPos = line.find_first_not_of(separator);

		if (nonSepPos == 0)
			continue;

		line.erase(0, nonSepPos);

		// Iterate host names
		while (!line.empty()) {
			size_t end;

			if (line[0] == '"') {
				// Find and remove quotes
				line.erase(0, 1);
				end = line.find('"');

				if (end == string::npos) break;
				if (end == 0) continue;

				line.erase(end, 1);
			} else {
				end = line.find_first_of(whitespace);
			}

			// Insert and remove the host name
			hosts.insert(line.substr(0, end));
			line.erase(0, line.find_first_not_of(whitespace, end));
		}
	}

	return hosts;
}

void ConfigFile::edit() {
	if (GetFileAttributes(folderPath.c_str()) == INVALID_FILE_ATTRIBUTES)
		CreateDirectory(folderPath.c_str(), NULL);

	SHELLEXECUTEINFO execInfo{};
	execInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	execInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	execInfo.lpVerb = L"open";
	execInfo.lpFile = L"notepad";
	execInfo.lpParameters = filePath.c_str();
	execInfo.nShow = SW_SHOW;
	ShellExecuteEx(&execInfo);

	WaitForSingleObject(execInfo.hProcess, INFINITE);
	CloseHandle(execInfo.hProcess);
}