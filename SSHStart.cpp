#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <knownfolders.h>
#include "HostNames.h"
#include "ConfigFile.h"

using namespace std;

vector<ConfigFile*> files;
HostNames* hostNames;

string getChoice(string error = "") {
	system("cls");

	cout << "SSHStart" << endl << endl;

	for (ConfigFile* file : files)
		cout << file->editKey << ") " << "Edit " << file->type << " config" << endl;

	cout << endl;

	int i = 1;

	for (const string& host : hostNames->hosts) {
		cout << i << ") " << host << endl;
		i++;
	}

	cout << endl;

	if (error != "")
		cout << "Error: " << error << endl << endl;

	cout << "Choose: ";

	string line;
	getline(cin, line);

	return line;
}

string processChoice(string choice) {
	int index;
	
start:
	if (choice.empty()) {
		choice = getChoice();
		goto start;
	}

	for (ConfigFile* file : files) {
		if (choice.size() > 1 || choice[0] != file->editKey)
			continue;

		file->edit();
		hostNames->reload();

		choice = getChoice();
		goto start;
	}

	try {
		index = stoi(choice) - 1;
	} catch (invalid_argument) {
		choice = getChoice("Invalid choice");
		goto start;
	}

	if (index < 0 || index >= hostNames->hosts.size()) {
		choice = getChoice("Choice out of range");
		goto start;
	}

	return *next(hostNames->hosts.begin(), index);
}

int main(int argc, char* argv[]) {
	SetConsoleOutputCP(CP_UTF8);
	setlocale(LC_ALL, ".UTF8");

	files.push_back(new ConfigFile(FOLDERID_Profile, L"\\.ssh\\config", "user", 'e'));
	files.push_back(new ConfigFile(FOLDERID_ProgramData, L"\\ssh\\ssh_config", "global", 'g'));

	hostNames = new HostNames(&files);
	string choice = processChoice(argc > 1 ? argv[1] : getChoice());

	system("cls");
	system(("ssh " + choice).c_str());

	delete hostNames;

	for (ConfigFile* file : files)
		delete file;
}