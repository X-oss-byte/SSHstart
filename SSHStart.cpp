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

void showChoices(string error = "") {
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
}

string selectChoice(string error = "") {
	showChoices(error);

	string line;
	getline(cin, line);

	for (ConfigFile* file : files) {
		if (line.size() > 1 || line[0] != file->editKey) continue;

		file->edit();
		hostNames->reload();

		return selectChoice();
	}

	int choice;
	
	try {
		choice = stoi(line) - 1;
	} catch (invalid_argument) {
		return selectChoice("Invalid choice");
	}

	if (choice < 0 || choice >= hostNames->hosts.size())
		return selectChoice("Choice out of range");

	return *next(hostNames->hosts.begin(), choice);
}

int main() {
	SetConsoleOutputCP(CP_UTF8);
	setlocale(LC_ALL, ".UTF8");

	files.push_back(new ConfigFile(FOLDERID_Profile, L"\\.ssh\\config", "user", 'e'));
	files.push_back(new ConfigFile(FOLDERID_ProgramData, L"\\ssh\\ssh_config", "global", 'g'));

	hostNames = new HostNames(&files);

	string choice = selectChoice();

	cout << endl;

	system("cls");
	system(("ssh " + choice).c_str());

	delete hostNames;

	for (ConfigFile* file : files)
		delete file;
}