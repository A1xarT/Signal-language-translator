#include "Init.h"

char* InitSymbolTable()
{
	char* table = new char[256];
	for (int i = 0; i < 256; i++)	// err
		table[i] = 6;
	for (int i = 8; i <= 13; i++)	// whitespaces
		table[i] = 0;
	table[32] = 0;
	for (int i = 48; i <= 57; i++)	// 0-9
		table[i] = 1;
	for (int i = 65; i <= 90; i++)	// A-Z
		table[i] = 2;
	table[40] = 5;					// (
	table[44] = table[46] = table[58] = table[59] = table[61] = 3;	// , . : ; =
	return table;
}
list<string> InitKeywords()
{
	list<string> lst = { "PROGRAM", "BEGIN", "END", "LABEL", "GOTO", "IF", "THEN", "ELSE" , "ENDIF", "CONST"}; // 401 ...
	return lst;
}