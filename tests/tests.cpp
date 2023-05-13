#include <set>
#include <string>
#include "CppUnitTest.h"
#include "../ConfigFile.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace SSHStartTests {
	TEST_CLASS(TestConfigFile) {
	public:
		TEST_METHOD(getHosts) {
			string expectedHosts = u8"a..b..c..d..ee..f..h..i..j..k..l l..m..验🍍..";

			ConfigFile configFile(wstring(L"../../tests"), wstring(L"../../tests/test_config"), "test", 't');
			string hostList;

			for (const string& host : configFile.getHosts())
				hostList += host + "..";

			Assert::AreEqual(expectedHosts, hostList);
		}
	};
}
