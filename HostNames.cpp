#pragma once

#include <vector>
#include <algorithm>
#include <set>
#include "HostNames.h"
#include "ConfigFile.h"

using namespace std;

HostNames::HostNames(vector<ConfigFile*>* files) {
	this->files = files;

	reload();
}

void HostNames::reload() {
	hosts.clear();

	for_each(files->begin(), files->end(), [=](ConfigFile* file) {
		hosts.merge(file->getHosts());
	});
}