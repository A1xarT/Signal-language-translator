#pragma once
#include "Common.h"
#include <string>

using namespace std;

void Parser(string output_path);
bool SignalProgram(list<Token>::iterator& it, string indent);
bool Program(list<Token>::iterator& it, string indent);
bool Block(list<Token>::iterator& it, string indent);
bool Declarations(list<Token>::iterator& it, string indent);
bool LabelDeclarations(list<Token>::iterator& it, string indent);
bool ConstantDeclarations(list<Token>::iterator& it, string indent);
bool ConstantDeclarationList(list<Token>::iterator& it, string indent);
bool ConstantDeclaration(list<Token>::iterator& it, string indent, bool& insideErr);
bool LabelsList(list<Token>::iterator& it, string indent);
bool StatementsList(list<Token>::iterator& it, string indent);
bool Statement(list<Token>::iterator& it, string indent, bool &insideErr);
bool SubSt1(list<Token>::iterator& it, string indent, bool& insideErr);
bool SubSt2(list<Token>::iterator& it, string indent, bool& insideErr);
bool SubSt3(list<Token>::iterator& it, string indent, bool& insideErr);
bool ConditionStatement(list<Token>::iterator& it, string indent);
bool IncompleteCondSt(list<Token>::iterator& it, string indent);
bool CondExpression(list<Token>::iterator& it, string indent);
bool AlternativePart(list<Token>::iterator& it, string indent);
bool UnsignedInteger(list<Token>::iterator& it, string indent);
bool ConstIdentifier(list<Token>::iterator& it, string indent);
bool VarIdentifier(list<Token>::iterator& it, string indent);
bool PrcIdentifier(list<Token>::iterator& it, string indent);
bool IsConstant(Token token, string indent);
bool IsIdentifier(Token token, string indent);

bool EndOfTokenList(list<Token>::iterator it);
bool EofErrorCheck(string expected, list<Token>::iterator &it, list<Token>::iterator prevIt);

static list<string> syntax_tree;
void AddBranch(string line, string indent);
void RemoveLastBranch();

void LogParserError(string expected, Token token);

static Token LastToken;