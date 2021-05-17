#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <iomanip>
#include "Common.h"

void LogError(const char* dest, int line, int column, const char* description)
{
	char buffer[255];
	sprintf(buffer, "%s: Error (line %d, column %d): %s", dest, line, column, description);
	error_list.push_back(buffer);
}
list<string> GetErrorList()
{
	return error_list;
}
void SetErrorList(list<string> newList)
{
	list<string> lst(newList);
	error_list = lst;
}
void ClearFile(string path)
{
	ofstream out;
	out.open(path);
	out.close();
}
void LoadErrorLog(string path)
{
	ofstream out;
	out.open(path, ios::app);
	if (out.is_open())
	{
		list<string>::iterator it;
		for (it = error_list.begin(); it != error_list.end(); it++)
		{
			out << *it << endl;
		}
	}
}
void LoadTokenTable(string path)
{
	if (token_list.size() == 0) return;
	ofstream out;
	out.open(path, ios::app);
	if (out.is_open())
	{
		out << endl << setw(50) << "Token table" << endl;
		out << setw(8) << " Row  |" << setw(10) << " Column |" << setw(22) << " Token identifier |" << setw(18) << "  Token value  " << endl;
		list<Token>::iterator it;
		for (it = token_list.begin(); it != token_list.end(); it++)
		{
			Token tk = *it;
			out << left << "   " << setw(8) << tk.line << setw(10) << tk.column << "     " << setw(20) << tk.code << setw(12) << tk.value << endl;
		}
	}
}
void LoadConstantTable(string path)
{
	if (constant_list.size() == 0) return;
	ofstream out;
	out.open(path, ios::app);
	if (out.is_open())
	{
		out << endl << setw(50) << "Constant table" << endl;
		out << setw(8) << "Type" << "          " << setw(15) << "Value" << endl;
		list<Constant>::iterator it;
		for (it = constant_list.begin(); it != constant_list.end(); it++)
		{
			Constant cons = *it;
			out << left << "   " << setw(8) << cons.type << "          " << setw(15) << cons.value << endl;
		}
	}
}
void LoadIdentifierTable(string path)
{
	if (identifier_list.size() == 0) return;
	ofstream out;
	out.open(path, ios::app);
	if (out.is_open())
	{
		out << endl << setw(50) << "Identifier table" << endl;
		out << setw(15) << "Value" << endl;
		list<Identifier>::iterator it;
		for (it = identifier_list.begin(); it != identifier_list.end(); it++)
		{
			Identifier idn = *it;
			out << left << "         " << setw(15) << idn.value << endl;
		}
	}
}
void LoadSyntaxTree(string path, list<string> syntax_tree)
{
	if (syntax_tree.size() == 0) return;
	ofstream out;
	out.open(path, ios::app);
	if(out.is_open())
	{
		out << endl << setw(50) << "Syntax tree" << endl;
		list<string>::iterator it;
		for (it = syntax_tree.begin(); it != syntax_tree.end(); it++)
		{
			string str = *it;
			out << str << endl;
		}
	}
}
int FindToken(string _value)
{
	list<Token>::iterator it;
	for (it = token_list.begin(); it != token_list.end(); it++)
	{
		Token tk = *it;
		if (tk.value == _value)
		{
			return tk.code;
		}
	}
	return -1;
}
bool FindIdentifier(string _value)
{
	list<Identifier>::iterator it;
	for (it = identifier_list.begin(); it != identifier_list.end(); it++)
	{
		Identifier idt = *it;
		if (idt.value == _value)
		{
			return true;
		}
	}
	return false;
}
bool FindConstant(string _value, string _type)
{
	list<Constant>::iterator it;
	for (it = constant_list.begin(); it != constant_list.end(); it++)
	{
		Constant con = *it;
		if (con.value == _value && con.type == _type)
		{
			return true;
		}
	}
	return false;
}
void FreeTables()
{
	token_list.clear();
	identifier_list.clear();
	constant_list.clear();
	error_list.clear();
}
void AddToken(Token token)
{
	token_list.push_back(token);
}
void AddIdentifier(Identifier identifier)
{
	identifier_list.push_back(identifier);
}
void AddConstant(Constant constant)
{
	constant_list.push_back(constant);
}
list<Token> GetTokenList()
{
	return token_list;
}
list<Identifier> GetIdentifierList()
{
	return identifier_list;
}
list<Constant> GetConstantList()
{
	return constant_list;
}