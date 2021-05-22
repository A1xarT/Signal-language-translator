#define _CRT_SECURE_NO_WARNINGS

#include "Parser.h"
struct _signal_program* Parser(string output_path)
{
	token_list = GetTokenList();
	list<Token>::iterator token = token_list.begin();

	if (token == token_list.end())
	{
		LastToken = { 0, 0, '#',  "EndOfFile" };
	}
	else
	{
		LastToken = { token_list.back().line, 0, '#', "EndOfFile" };
		LastToken.column = token_list.back().value.length() + token_list.back().column;
	}
	program_tree = new _signal_program();
	SignalProgram(token, "", program_tree);
	//LoadErrorLog(output_path);
	LoadSyntaxTree(output_path, syntax_tree);
	syntax_tree.clear();
	return program_tree;
}

bool SignalProgram(list<Token>::iterator& it, string indent, struct _signal_program* node)
{
	AddBranch("<signal-program>", indent);
	list<Token>::iterator prevIt = it;
	node->program = new _program();
	if (Program(it, indent + "..", node->program))
	{
		if (it != token_list.end())
		{
			LogParserError("EndOfFile", *it);
			return false;
		}
	}
	it = prevIt;
	return false;
}
bool Program(list<Token>::iterator& it, string indent, struct _program* node)
{
	AddBranch("<program>", indent);
	list<Token>::iterator prevIt = it;
	if (EofErrorCheck("PROGRAM", it, prevIt))
		return false;
	if ((*it).code != 401)
	{
		LogParserError("PROGRAM", *it);
		it = prevIt;
		return false;
	}
	AddBranch(to_string((*it).code) + " " + (*it).value, indent + "..");
	it++;
	if (EofErrorCheck("<procedure-identifier>", it, prevIt))
		return false;
	node->prcIdnt = new _procedure_identifier();
	if (!PrcIdentifier(it, indent + "..", node->prcIdnt))
	{
		it = prevIt;
		return false;
	}
	if (EofErrorCheck(";", it, prevIt))
		return false;
	if ((*it).code != ';')
	{
		LogParserError(";", *it);
		it = prevIt;
		return false;
	}
	AddBranch(to_string((*it).code) + " " + (*it).value, indent + "..");
	it++;
	if (EofErrorCheck("<block>", it, prevIt))
		return false;
	node->block = new _block();
	if (!Block(it, indent + "..", node->block))
	{
		it = prevIt;
		return false;
	}
	if (EofErrorCheck(".", it, prevIt))
		return false;
	if ((*it).code != '.')
	{
		LogParserError(".", *it);
		it = prevIt;
		return false;
	}
	AddBranch(to_string((*it).code) + " " + (*it).value, indent + "..");
	it++;
	return true;
}
bool Block(list<Token>::iterator& it, string indent, struct _block* node)
{
	AddBranch("<block>", indent);
	list<Token>::iterator prevIt = it;
	if (EofErrorCheck("<declarations>", it, prevIt))
		return false;
	node->declarations = new _declarations();
	if (!Declarations(it, indent + "..", node->declarations))
	{
		it = prevIt;
		return false;
	}
	if (EofErrorCheck("BEGIN", it, prevIt))
		return false;
	if ((*it).code != 402)
	{
		LogParserError("BEGIN", *it);
		it = prevIt;
		return false;
	}
	AddBranch(to_string((*it).code) + " " + (*it).value, indent + "..");
	it++;
	node->statementsList = new _statements_list();
	if (!StatementsList(it, indent + "..", node->statementsList))
	{
		it = prevIt;
		return false;
	}
	if (EofErrorCheck("END", it, prevIt))
		return false;
	if ((*it).code != 403)
	{
		LogParserError("END", *it);
		it = prevIt;
		return false;
	}
	AddBranch(to_string((*it).code) + " " + (*it).value, indent + "..");
	it++;
	return true;
}
bool Declarations(list<Token>::iterator& it, string indent, struct _declarations* node)
{
	AddBranch("<declarations>", indent);
	node->labelDeclarations = new _label_declarations();
	if (!LabelDeclarations(it, indent + "..", node->labelDeclarations))
		return false;
	return true;
}
bool LabelDeclarations(list<Token>::iterator& it, string indent, struct _label_declarations* node)
{
	AddBranch("<label-declarations>", indent);
	list<Token>::iterator prevIt = it;
	if (EndOfTokenList(it) || (*it).code != 404)
	{
		AddBranch("<empty>", indent + "..");
		node->isEmpty = true;
		return true;
	}
	AddBranch(to_string((*it).code) + " " + (*it).value, indent + "..");
	it++;
	if (EofErrorCheck("<unsigned-integer>", it, prevIt))
		return false;
	node->uinteger = new _unsigned_integer();
	if (!UnsignedInteger(it, indent + "..", node->uinteger))
	{
		it = prevIt;
		return false;
	}
	node->labelsList = new _labels_list();
	if (!LabelsList(it, indent + "..", node->labelsList))
	{
		it = prevIt;
		return false;
	}
	if (EofErrorCheck(";", it, prevIt))
		return false;
	if ((*it).code != ';')
	{
		LogParserError(";", *it);
		it = prevIt;
		return false;
	}
	AddBranch(to_string((*it).code) + " " + (*it).value, indent + "..");
	it++;
	node->isEmpty = false;
	return true;
}
bool LabelsList(list<Token>::iterator& it, string indent, struct _labels_list* node)
{
	AddBranch("<labels-list>", indent);
	list<Token>::iterator prevIt = it;
	if (EndOfTokenList(it) || (*it).code != ',')
	{
		AddBranch("<empty>", indent + "..");
		node->isEmpty = true;
		return true;
	}
	AddBranch(to_string((*it).code) + " " + (*it).value, indent + "..");
	it++;
	if (EofErrorCheck("<unsigned-integer>", it, prevIt))
		return false;
	node->uinteger = new _unsigned_integer();
	if (!UnsignedInteger(it, indent + "..", node->uinteger))
	{
		it = prevIt;
		return false;
	}
	node->labelsList = new _labels_list();
	if (!LabelsList(it, indent + "..", node->labelsList))
	{
		it = prevIt;
		return false;
	}
	node->isEmpty = false;
	return true;
}
bool StatementsList(list<Token>::iterator& it, string indent, struct _statements_list* node)
{
	AddBranch("<statements-list>", indent);

	list<Token>::iterator prevIt = it;
	list<string> prevErrList(GetErrorList());
	list<string> prevTree(syntax_tree);

	if (EndOfTokenList(it))
	{
		AddBranch("<empty>", indent + "..");
		node->isEmpty = true;
		return true;
	}
	bool insideError = true;
	node->statement = new _statement();
	if (!Statement(it, indent + "..", insideError, node->statement))
	{
		if (!insideError)
		{
			syntax_tree = prevTree;
			SetErrorList(prevErrList);
			AddBranch("<empty>", indent + "..");
			node->isEmpty = true;
			return true;
		}
		it = prevIt;
		return false;
	}
	node->statementsList = new _statements_list();
	if (!StatementsList(it, indent + "..", node->statementsList))
	{
		it = prevIt;
		return false;
	}
	node->isEmpty = false;
	return true;
}
bool Statement(list<Token>::iterator& it, string indent, bool& insideErr, struct _statement* node)
{
	AddBranch("<statement>", indent);

	list<Token>::iterator prevIt = it;
	list<string> prevTree(syntax_tree);
	list<string> prevErrList(GetErrorList());
	node->statementType = 1;
	node->uinteger = new _unsigned_integer();
	if (EndOfTokenList(it) || !UnsignedInteger(it, indent + "..", node->uinteger))
	{
		SetErrorList(prevErrList);
		return SubSt1(it, indent, insideErr, node);
	}
	if (EofErrorCheck(":", it, prevIt))
		return false;
	if ((*it).code != ':')
	{
		LogParserError(":", *it);
		syntax_tree = prevTree;
		it = prevIt;
		return false;
	}
	AddBranch(to_string((*it).code) + " " + (*it).value, indent + "..");
	it++;
	if (EofErrorCheck("<statement>", it, prevIt))
		return false;
	bool tmp = false;
	node->statement = new _statement();
	if (!Statement(it, indent + "..", tmp, node->statement))
	{
		it = prevIt;
		return false;
	}
	return true;
}
bool SubSt1(list<Token>::iterator& it, string indent, bool& insideErr, struct _statement* node)
{
	list<Token>::iterator prevIt = it;
	node->statementType = 2;
	if (EndOfTokenList(it) || (*it).code != 405)
	{
		return SubSt2(it, indent, insideErr, node);
	}
	AddBranch(to_string((*it).code) + " " + (*it).value, indent + "..");
	it++;
	if (EofErrorCheck("<unsigned-integer>", it, prevIt))
		return false;
	node->uinteger = new _unsigned_integer();
	if (!UnsignedInteger(it, indent + "..", node->uinteger))
	{
		it = prevIt;
		return false;
	}
	if (EofErrorCheck(";", it, prevIt))
		return false;
	if ((*it).code != ';')
	{
		LogParserError(";", *it);
		it = prevIt;
		return false;
	}
	AddBranch(to_string((*it).code) + " " + (*it).value, indent + "..");
	it++;
	return true;
}
bool SubSt2(list<Token>::iterator& it, string indent, bool& insideErr, struct _statement* node)
{
	list<Token>::iterator prevIt = it;
	list<string> prevTree(syntax_tree);
	list<string> prevErrList(GetErrorList());

	node->condStatement = new _cond_statement();
	node->statementType = 3;
	if (EndOfTokenList(it))
	{
		return SubSt3(it, indent, insideErr, node);
	}
	insideErr = true;
	if (!ConditionStatement(it, indent + "..", insideErr, node->condStatement))
	{
		if (!insideErr)
		{
			syntax_tree = prevTree;
			SetErrorList(prevErrList);
			it = prevIt;
			return SubSt3(it, indent, insideErr, node);
		}
		return false;
	}
	if (EofErrorCheck("ENDIF", it, prevIt))
		return false;
	if ((*it).code != 409)
	{
		LogParserError("ENDIF", *it);
		it = prevIt;
		return false;
	}
	AddBranch(to_string((*it).code) + " " + (*it).value, indent + "..");
	it++;
	if (EofErrorCheck(";", it, prevIt))
		return false;
	if ((*it).code != ';')
	{
		LogParserError(";", *it);
		it = prevIt;
		return false;
	}
	AddBranch(to_string((*it).code) + " " + (*it).value, indent + "..");
	it++;
	return true;
}
bool SubSt3(list<Token>::iterator& it, string indent, bool& insideErr, struct _statement* node)
{
	node->statementType = 4;
	if (EofErrorCheck(";", it, it))
		return false;
	if ((*it).code != ';')
	{
		LogParserError(";", *it);
		insideErr = false;
		return false;
	}
	AddBranch(to_string((*it).code) + " " + (*it).value, indent + "..");
	it++;
	return true;
}
bool ConditionStatement(list<Token>::iterator& it, string indent, bool& insideErr, struct _cond_statement* node)
{
	AddBranch("<condition-statement>", indent);
	list<Token>::iterator prevIt = it;
	if (EofErrorCheck("<incomplete-condition-statement>", it, prevIt))
	{
		return false;
		insideErr = false;
	}
	node->incompleteCondSt = new _incomplete_cond_statement();
	if (!IncompleteCondSt(it, indent + "..", insideErr, node->incompleteCondSt))
	{
		it = prevIt;
		return false;
	}
	node->alternativePart = new _alt_part();
	if (!AlternativePart(it, indent + "..", node->alternativePart))
	{
		it = prevIt;
		return false;
	}
	return true;
}
bool IncompleteCondSt(list<Token>::iterator& it, string indent, bool& insideErr, struct _incomplete_cond_statement* node)
{
	AddBranch("<incomplete-condition-statement>", indent);
	list<Token>::iterator prevIt = it;
	if (EofErrorCheck("IF", it, prevIt))
	{
		insideErr = false;
		return false;
	}
	if ((*it).code != 406)
	{
		LogParserError("IF", *it);
		it = prevIt;
		insideErr = false;
		return false;
	}
	AddBranch(to_string((*it).code) + " " + (*it).value, indent + "..");
	it++;
	if (EofErrorCheck("<condition-expression>", it, prevIt))
		return false;
	node->condExpression = new _cond_expression();
	if (!CondExpression(it, indent + "..", node->condExpression))
	{
		it = prevIt;
		return false;
	}
	if (EofErrorCheck("THEN", it, prevIt))
		return false;
	if ((*it).code != 407)
	{
		LogParserError("THEN", *it);
		it = prevIt;
		return false;
	}
	AddBranch(to_string((*it).code) + " " + (*it).value, indent + "..");
	it++;
	node->statementsList = new _statements_list();
	if (!StatementsList(it, indent + "..", node->statementsList))
	{
		it = prevIt;
		return false;
	}
	return true;
}
bool CondExpression(list<Token>::iterator& it, string indent, struct _cond_expression* node)
{
	AddBranch("<conditional-expression>", indent);
	list<Token>::iterator prevIt = it;
	if (EofErrorCheck("<variable-identifier>", it, prevIt))
		return false;
	node->varIdnt = new _variable_identifier();
	if (!VarIdentifier(it, indent + "..", node->varIdnt))
	{
		it = prevIt;
		return false;
	}
	if (EofErrorCheck("=", it, prevIt))
		return false;
	if ((*it).code != '=')
	{
		LogParserError("=", *it);
		it = prevIt;
		return false;
	}
	AddBranch(to_string((*it).code) + " " + (*it).value, indent + "..");
	it++;
	if (EofErrorCheck("<unsigned-integer>", it, prevIt))
		return false;
	node->uinteger = new _unsigned_integer();
	if (!UnsignedInteger(it, indent + "..", node->uinteger))
	{
		it = prevIt;
		return false;
	}
	return true;
}
bool AlternativePart(list<Token>::iterator& it, string indent, struct _alt_part* node)
{
	AddBranch("<alternative-part>", indent);
	list<Token>::iterator prevIt = it;
	if (EndOfTokenList(it) || (*it).code != 408)
	{
		AddBranch("<empty>", indent + "..");
		node->isEmpty = true;
		return true;
	}
	AddBranch(to_string((*it).code) + " " + (*it).value, indent + "..");
	it++;
	node->statementsList = new _statements_list();
	if (!StatementsList(it, indent + "..", node->statementsList))
	{
		it = prevIt;
		return false;
	}
	node->isEmpty = false;
	return true;
}
bool UnsignedInteger(list<Token>::iterator& it, string indent, struct _unsigned_integer* node)
{
	AddBranch("<unsigned-integer>", indent);
	node->constant = new Constant();
	node->line = (*it).line;
	node->column = (*it).column;
	if (IsConstant(*it, indent + "..", node->constant))
	{
		it++;
		return true;
	}
	RemoveLastBranch();
	return false;
}
bool VarIdentifier(list<Token>::iterator& it, string indent, struct _variable_identifier* node)
{
	AddBranch("<variable-identifier>", indent);
	node->variable = new Variable();
	node->variable->identifier = new Identifier();
	if (IsIdentifier(*it, indent + "..", node->variable->identifier))
	{
		node->variable->constant = FindVariable(node->variable->identifier->value).constant;
		node->line = (*it).line;
		node->column = (*it).column;
		it++;
		return true;
	}
	RemoveLastBranch();
	return false;
}
bool PrcIdentifier(list<Token>::iterator& it, string indent, struct _procedure_identifier* node)
{
	AddBranch("<procedure-identifier>", indent);
	node->identifier = new Identifier();
	if (IsIdentifier(*it, indent + "..", node->identifier))
	{
		node->line = (*it).line;
		node->column = (*it).column;
		it++;
		return true;
	}
	RemoveLastBranch();
	return false;
}
bool IsConstant(Token token, string indent, struct Constant* node)
{
	if (token.code > 500 && token.code <= 1000)
	{
		AddBranch(to_string(token.code) + " " + token.value, indent);
		node->type = "integer";
		node->value = token.value;
		return true;
	}
	LogParserError("<unsigned-integer>", token);
	return false;
}
bool IsIdentifier(Token token, string indent, struct Identifier* node)
{
	if (token.code > 1000)
	{
		AddBranch("<identifier>", indent);
		AddBranch(to_string(token.code) + " " + token.value, indent + "..");
		node->value = token.value;
		return true;
	}
	LogParserError("<identifier>", token);
	return false;
}
void AddBranch(string line, string indent)
{
	syntax_tree.push_back(indent + line);
}
void RemoveLastBranch()
{
	syntax_tree.pop_back();
}
void LogParserError(string expected, Token token)
{
	char buf[100];
	sprintf(buf, "'%s' expected but '%s' found", expected.c_str(), token.value.c_str());
	LogError("Parser", token.line, token.column, buf);
}
bool EndOfTokenList(list<Token>::iterator it)
{
	return it == token_list.end();
}
bool EofErrorCheck(string expected, list<Token>::iterator& it, list<Token>::iterator prevIt)
{
	if (EndOfTokenList(it))
	{
		LogParserError(expected, LastToken);
		it = prevIt;
		return true;
	}
	return false;
}