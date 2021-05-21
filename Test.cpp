#include "Test.h"
#include <fstream>

void run_tests(string folder_name, string subfolder_name, unsigned int tests_number)
{
	for (unsigned int i = 1; i <= tests_number; i++)
	{
		string test_folder_name = folder_name + "/" + subfolder_name + ((i < 10) ? "0" + to_string(i) : to_string(i)) + "/";
		ifstream inputFile(test_folder_name + "input.sig");
		Lexer(inputFile, test_folder_name + "generated.txt");
		Parser(test_folder_name + "generated.txt");

		ifstream expectedFile(test_folder_name + "expected.txt"), generatedFile(test_folder_name + "generated.txt");
		bool test_failed = false;
		char ch1, ch2;
		expectedFile.get(ch1);
		generatedFile.get(ch2);

		while (!expectedFile.eof() && !generatedFile.eof())
		{
			if (ch1 != ch2)
			{
				test_failed = true;
				break;
			}
			expectedFile.get(ch1);
			generatedFile.get(ch2);
		}
		if (!test_failed && (!expectedFile.eof() || !generatedFile.eof()))
			test_failed = true;
		if (!test_failed)
			cout << test_folder_name.substr(0, test_folder_name.length() - 1) << " passed" << endl;
		else
			cout << test_folder_name.substr(0, test_folder_name.length() - 1) << " failed" << endl;
		FreeTables();
	}
}