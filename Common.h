#pragma once

#include <list>
#include <fstream>
#include <iomanip>

using namespace std;

typedef struct {
	unsigned int line;
	unsigned int column;
	int code;
	string value;
}Token;

typedef struct {
	string value;
}Identifier;

typedef struct {
	string value;
	string type;
}Constant;

static list<Token> token_list;
static list<Identifier> identifier_list;
static list<Constant> constant_list;
static list<string> error_list;

void SetErrorList(list<string> newList);

void LogError(const char* dest, int line, int column, const char* description);
void ClearFile(string path);
void LoadErrorLog(string path);
void LoadTokenTable(string path);
void LoadConstantTable(string path);
void LoadIdentifierTable(string path);
void LoadSyntaxTree(string path, list<string> syntax_tree);

int FindToken(string _value);
bool FindIdentifier(string _value);
bool FindConstant(string _value, string _type);

void AddToken(Token token);
void AddIdentifier(Identifier identifier);
void AddConstant(Constant constant);

void FreeTables();

list<Token> GetTokenList();
list<Identifier> GetIdentifierList();
list<Constant> GetConstantList();
list<string> GetErrorList();