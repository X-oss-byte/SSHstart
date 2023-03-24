#pragma once

#include <string>
#include <set>
#include <fstream>
#include <windows.h>
#include <ShlObj.h>
#include "ConfigFile.h"

using namespace std;

ConfigFile::ConfigFile(wstring cPath, string cEditKey, string cType, bool cAdminRequired)
	: path{ cPath }, editKey{ cEditKey }, type{ cType }, adminRequired{ cAdminRequired } { }

ConfigFile::ConfigFile(REFKNOWNFOLDERID folder, LPCWSTR file, string cEditKey, string cType, bool cAdminRequired)
	: editKey{ cEditKey }, type{ cType }, adminRequired{ cAdminRequired } {
	PWSTR folderPath;

	SHGetKnownFolderPath(folder, NULL, NULL, &folderPath);

	path = wstring(folderPath);
	path.append(file);

	CoTaskMemFree(folderPath);
}

set<string> ConfigFile::getHosts() {
	set<string> hosts;

	ifstream file(this->path.c_str());

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

			if (line.at(0) == '"') {
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
	SHELLEXECUTEINFO execInfo{};
	execInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	execInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	execInfo.lpVerb = this->adminRequired ? L"runas" : L"open";
	execInfo.lpFile = L"notepad";
	execInfo.lpParameters = this->path.c_str();
	execInfo.nShow = SW_SHOW;

	ShellExecuteEx(&execInfo);
	WaitForSingleObject(execInfo.hProcess, INFINITE);
	CloseHandle(execInfo.hProcess);
}