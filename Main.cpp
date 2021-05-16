#include "Lexer.h"
#include "Parser.h"


using namespace std;

void print_file(string file_path);

int main(int argc, char* argv[])
{
	//if (argc != 2) {
	//	printf(" Please specify the path to a test folder as an argument\n Example: .\\Lab1.exe ..\\tests\\test01");
	//	system("pause");
	//	return 0;
	//}
	//string file_path = argv[1];
	string file_path = "tests/test07";
	ifstream test_file(file_path + "\\input.sig");
	if (!test_file.is_open())
	{
		printf(" Wrong test folder path");
		system("pause");
		return 0;
	}
	Lexer(test_file, file_path + "\\generated.txt");
	Parser(file_path + "\\generated.txt");
	print_file(file_path + "\\generated.txt");
	print_file(file_path + "\\input.sig");
	system("pause");
	return 0;
}
void print_file(string file_path)
{
	ifstream test_file(file_path);
	string line;
	while (getline(test_file, line))
	{
		cout << line << endl;
	}
}
