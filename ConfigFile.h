#pragma once

#include <string>
#include <set>
#include <fstream>
#include <windows.h>
#include <ShlObj.h>

using namespace std;

class ConfigFile {
public:
	wstring path;
	string type;
	char editKey;

	ConfigFile(wstring path, string type, char editKey);
	ConfigFile(REFKNOWNFOLDERID folder, LPCWSTR file, string type, char editKey);

	set<string> getHosts();
	void edit();

private:
	const char* whitespace = "\t ";
	const char* separator = "\t =";
};
