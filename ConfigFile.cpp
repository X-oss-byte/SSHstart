#pragma once
#include <string>
#include <set>
#include <fstream>
#include <windows.h>
#include <ShlObj.h>

using namespace std;

class ConfigFile {
public:
	string editKey;
	wstring path;
	string type;
	bool adminRequired;

	ConfigFile(REFKNOWNFOLDERID folder, LPCWSTR file, string editKey, string type, bool adminRequired = false) {
		PWSTR folderPath;

		SHGetKnownFolderPath(folder, NULL, NULL, &folderPath);

		this->editKey = editKey;
		this->path = wstring(folderPath) + wstring(file);
		this->type = type;
		this->adminRequired = adminRequired;
	}

	set<string> getHosts() {
		set<string> hosts;

		ifstream file(this->path.c_str());

		if (!file.good())
			return hosts;

		string line;

		while (getline(file, line)) {
			size_t first = line.find_first_not_of(' ');
			size_t last = line.find_last_not_of(' ');
			line = line.substr(first, last - first + 1);

			if (line.rfind("Host ", 0) != 0)
				continue;

			hosts.insert(line.substr(5));
		}

		return hosts;
	}

	void edit() {
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
};