#define _CRT_SECURE_NO_WARNINGS

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
		for (auto& it : error_list)
			out << it << endl;
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
		for (auto& tk : token_list)
			out << left << "   " << setw(8) << tk.line << setw(10) << tk.column << "     " << setw(20) << tk.code << setw(12) << tk.value << endl;
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
		for (auto& i : constant_list)
			out << left << "   " << setw(8) << i.type << "          " << setw(15) << i.value << endl;
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
		for (auto& i : identifier_list)
			out << left << "         " << setw(15) << i.value << endl;
	}
}
void LoadSyntaxTree(string path, list<string> syntax_tree)
{
	if (syntax_tree.size() == 0) return;
	ofstream out;
	out.open(path, ios::app);
	if (out.is_open())
	{
		out << endl << setw(50) << "Syntax tree" << endl;
		for (auto& i : syntax_tree)
			out << i << endl;
	}
}
void LoadAsmCode(string path, list<string> asm_commands)
{
	if (asm_commands.size() == 0) return;
	ofstream out;
	out.open(path, ios::app);
	if (out.is_open())
	{
		out << endl << setw(50) << "Assembly code" << endl;
		list<string>::iterator it;
		for (auto& i : asm_commands)
			out << i << endl;
	}
}
int FindToken(string _value)
{
	list<Token>::iterator it;
	for (auto& tk : token_list)
	{
		if (tk.value == _value)
			return tk.code;
	}
	return -1;
}
bool FindIdentifier(string _value)
{
	list<Identifier>::iterator it;
	for (auto& it : identifier_list)
		if (it.value == _value)
			return true;
	return false;
}
bool FindConstant(string _value, string _type)
{
	list<Constant>::iterator it;
	for (auto& it : constant_list)
		if (it.value == _value && it.type == _type)
			return true;
	return false;
}
Variable FindVariable(string name)
{
	list<Variable>::iterator it;
	for (auto& it : variables_list)
		if (it.identifier->value == name)
			return it;
	return Variable{ new Identifier{""}, new Constant{"", ""} };
}
void FreeTables()
{
	token_list.clear();
	identifier_list.clear();
	variables_list.clear();
	constant_list.clear();
	error_list.clear();
}
void AddToken(Token token)
{
	token_list.push_back(token);
}
void AddIdentifier(Identifier identifier)
{
	if (!FindIdentifier(identifier.value))
		identifier_list.push_back(identifier);
}
void AddVariable(Variable variable)
{
	variables_list.push_back(variable);
	AddConstant(*variable.constant);
	AddIdentifier(*variable.identifier);
}
void AddConstant(Constant constant)
{
	if (!FindConstant(constant.value, constant.type))
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
list<Variable> GetVariablesList()
{
	return variables_list;
}