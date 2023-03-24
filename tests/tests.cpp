#include <set>
#include <string>
#include "CppUnitTest.h"
#include <windows.foundation.collections.h>
#include "../ConfigFile.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace SSHStartTests {
	TEST_CLASS(TestConfigFile) {
	public:
		TEST_METHOD(getHosts) {
			string expectedHosts = "a..b..c..d..ee f..h..i..";

			ConfigFile configFile(wstring(L"../../tests/test_config"), "t", "test");
			string hostList;

			for (const string& host : configFile.getHosts())
				hostList += host + "..";

			Assert::AreEqual(expectedHosts, hostList);
		}
	};
}
