#pragma once
#include <string>
#include "SyntaxStructures.h"

using namespace std;

static list<string> syntax_tree;
static struct _signal_program* program_tree;
static Token LastToken;

struct _signal_program* Parser(string output_path);
bool SignalProgram(list<Token>::iterator& it, string indent, struct _signal_program* node);
bool Program(list<Token>::iterator& it, string indent, struct _program* node);
bool Block(list<Token>::iterator& it, string indent, struct _block* node);
bool Declarations(list<Token>::iterator& it, string indent, struct _declarations* node);
bool LabelDeclarations(list<Token>::iterator& it, string indent, struct _label_declarations* node);
bool LabelsList(list<Token>::iterator& it, string indent, struct _labels_list* node);
bool StatementsList(list<Token>::iterator& it, string indent, struct _statements_list* node);
bool Statement(list<Token>::iterator& it, string indent, bool &insideErr, struct _statement* node);
bool SubSt1(list<Token>::iterator& it, string indent, bool& insideErr, struct _statement* node);
bool SubSt2(list<Token>::iterator& it, string indent, bool& insideErr, struct _statement* node);
bool SubSt3(list<Token>::iterator& it, string indent, bool& insideErr, struct _statement* node);
bool ConditionStatement(list<Token>::iterator& it, string indent, struct _cond_statement* node);
bool IncompleteCondSt(list<Token>::iterator& it, string indent, struct _incomplete_cond_statement* node);
bool CondExpression(list<Token>::iterator& it, string indent, struct _cond_expression* node);
bool AlternativePart(list<Token>::iterator& it, string indent, struct _alt_part* node);
bool UnsignedInteger(list<Token>::iterator& it, string indent, struct _unsigned_integer* node);
bool VarIdentifier(list<Token>::iterator& it, string indent, struct _variable_identifier* node);
bool PrcIdentifier(list<Token>::iterator& it, string indent, struct _procedure_identifier* node);
bool IsConstant(Token token, string indent, Constant* node);
bool IsIdentifier(Token token, string indent, Identifier* node);

bool EndOfTokenList(list<Token>::iterator it);
bool EofErrorCheck(string expected, list<Token>::iterator &it, list<Token>::iterator prevIt);

void AddBranch(string line, string indent);
void RemoveLastBranch();

void LogParserError(string expected, Token token);