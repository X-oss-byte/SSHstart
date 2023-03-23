#pragma once

#include <vector>
#include <algorithm>
#include <set>
#include <vector>
#include "ConfigFile.cpp"

using namespace std;

class HostNames {
public:
	set<string> hosts;

	HostNames(vector<ConfigFile*>* files) {
		this->files = files;

		reload();
	}

	void reload() {
		hosts.clear();

		for_each(files->begin(), files->end(), [=](ConfigFile* file) {
			hosts.merge(file->getHosts());
		});
	}

private:
	vector<ConfigFile*>* files;
};