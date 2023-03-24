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

	ConfigFile(wstring path, string editKey, string type, bool adminRequired = false);
	ConfigFile(REFKNOWNFOLDERID folder, LPCWSTR file, string editKey, string type, bool adminRequired = false);

	set<string> getHosts();
	void edit();

private:
	const char* whitespace = "\t ";
};
