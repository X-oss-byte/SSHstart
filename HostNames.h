#pragma once

#include <vector>
#include <algorithm>
#include <set>
#include <string>
#include "ConfigFile.h"

using namespace std;

class HostNames {
public:
	set<string> hosts;

	HostNames(vector<ConfigFile*>* files);

	void reload();

private:
	vector<ConfigFile*>* files;
};