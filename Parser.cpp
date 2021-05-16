#define _CRT_SECURE_NO_WARNINGS
#include "Parser.h"
void Parser(string output_path)
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
	SignalProgram(token, "");
	LoadErrorLog(output_path);
	LoadSyntaxTree(output_path, syntax_tree);
}

bool SignalProgram(list<Token>::iterator& it, string indent)
{
	AddBranch("<signal-program>", indent);
	list<Token>::iterator prevIt = it;
	if (Program(it, indent + ".."))
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
bool Program(list<Token>::iterator& it, string indent)
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
	if (!PrcIdentifier(it, indent + ".."))
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
	if (!Block(it, indent + ".."))
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
bool Block(list<Token>::iterator& it, string indent)
{
	AddBranch("<block>", indent);
	list<Token>::iterator prevIt = it;
	if (EofErrorCheck("<declarations>", it, prevIt))
		return false;
	if (!Declarations(it, indent + ".."))
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
	if (!StatementsList(it, indent + ".."))
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
bool Declarations(list<Token>::iterator& it, string indent)
{
	AddBranch("<declarations>", indent);
	if (!LabelDeclarations(it, indent + ".."))
		return false;
	if (!ConstantDeclarations(it, indent + ".."))
		return false;
	return true;
}
bool LabelDeclarations(list<Token>::iterator& it, string indent)
{
	AddBranch("<label-declarations>", indent);
	list<Token>::iterator prevIt = it;
	if (EndOfTokenList(it) || (*it).code != 404)
	{
		AddBranch("<empty>", indent + "..");
		return true;
	}
	AddBranch(to_string((*it).code) + " " + (*it).value, indent + "..");
	it++;
	if (EofErrorCheck("<unsigned-integer>", it, prevIt))
		return false;
	if (!UnsignedInteger(it, indent + ".."))
	{
		it = prevIt;
		return false;
	}
	if (!LabelsList(it, indent + ".."))
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
bool ConstantDeclarations(list<Token>::iterator& it, string indent)
{
	AddBranch("<constant-declarations>", indent);
	list<Token>::iterator prevIt = it;
	if (EndOfTokenList(it) || (*it).code!= 410)
	{
		AddBranch("<empty>", indent + "..");
		return true;
	}
	AddBranch(to_string((*it).code) + " " + (*it).value, indent + "..");
	it++;

	if (!ConstantDeclarationList(it, indent + ".."))
		return false;
	return true;
}
bool ConstantDeclarationList(list<Token>::iterator& it, string indent)
{
	AddBranch("<constant-declarations-list>", indent);
	list<Token>::iterator prevIt = it;
	list<string> prevErrList(GetErrorList());
	list<string> prevTree(syntax_tree);

	bool check = false;
	if (!ConstantDeclaration(it, indent + "..", check))
	{
		if (check == true)
		{
			syntax_tree = prevTree;
			SetErrorList(prevErrList);
			AddBranch("<empty>", indent + "..");
			return true;
		}
		it = prevIt;
		return false;
	}
	if (!ConstantDeclarationList(it, indent + ".."))
		return false;
	return true;
}
bool ConstantDeclaration(list<Token>::iterator& it, string indent, bool& insideErr)
{
	AddBranch("<constant-declaration>", indent);
	list<Token>::iterator prevIt = it;
	if (EofErrorCheck("<constant-identifier>", it, prevIt) || !ConstIdentifier(it, indent + ".."))
	{
		insideErr = !insideErr;
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
	if (!UnsignedInteger(it, indent + ".."))
		return false;
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
bool LabelsList(list<Token>::iterator& it, string indent)
{
	AddBranch("<labels-list>", indent);
	list<Token>::iterator prevIt = it;
	if (EndOfTokenList(it) || (*it).code != ',')
	{
		AddBranch("<empty>", indent + "..");
		return true;
	}
	AddBranch(to_string((*it).code) + " " + (*it).value, indent + "..");
	it++;
	if (EofErrorCheck("<unsigned-integer>", it, prevIt))
		return false;
	if (!UnsignedInteger(it, indent + ".."))
	{
		it = prevIt;
		return false;
	}
	if (!LabelsList(it, indent + ".."))
	{
		it = prevIt;
		return false;
	}
	return true;
}
bool StatementsList(list<Token>::iterator& it, string indent)
{
	AddBranch("<statements-list>", indent);

	list<Token>::iterator prevIt = it;
	list<string> prevErrList(GetErrorList());
	list<string> prevTree(syntax_tree);

	if (EndOfTokenList(it))
	{
		AddBranch("<empty>", indent + "..");
		return true;
	}
	bool insideError = true;
	if (!Statement(it, indent + "..", insideError))
	{
		if (!insideError)
		{
			syntax_tree = prevTree;
			SetErrorList(prevErrList);
			AddBranch("<empty>", indent + "..");
			return true;
		}
		it = prevIt;
		return false;
	}
	if (!StatementsList(it, indent + ".."))
	{
		it = prevIt;
		return false;
	}
	return true;
}
bool Statement(list<Token>::iterator& it, string indent, bool& insideErr)
{
	AddBranch("<statement>", indent);

	list<Token>::iterator prevIt = it;
	list<string> prevTree(syntax_tree);
	list<string> prevErrList(GetErrorList());
	if (EndOfTokenList(it) || !UnsignedInteger(it, indent + ".."))
	{
		SetErrorList(prevErrList);
		return SubSt1(it, indent, insideErr);
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
	if (!Statement(it, indent + "..", tmp))
	{
		it = prevIt;
		return false;
	}
	return true;
}
bool SubSt1(list<Token>::iterator& it, string indent, bool& insideErr)
{
	list<Token>::iterator prevIt = it;
	if (EndOfTokenList(it) || (*it).code != 405)
	{
		return SubSt2(it, indent, insideErr);
	}
	AddBranch(to_string((*it).code) + " " + (*it).value, indent + "..");
	it++;
	if (EofErrorCheck("<unsigned-integer>", it, prevIt))
		return false;
	if (!UnsignedInteger(it, indent + ".."))
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
bool SubSt2(list<Token>::iterator& it, string indent, bool& insideErr)
{
	list<Token>::iterator prevIt = it;
	list<string> prevTree(syntax_tree);
	list<string> prevErrList(GetErrorList());

	if (EndOfTokenList(it) || !ConditionStatement(it, indent + ".."))
	{
		syntax_tree = prevTree;
		SetErrorList(prevErrList);
		it = prevIt;
		return SubSt3(it, indent, insideErr);
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
bool SubSt3(list<Token>::iterator& it, string indent, bool& insideErr)
{
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
bool ConditionStatement(list<Token>::iterator& it, string indent)
{
	AddBranch("<condition-statement>", indent);
	list<Token>::iterator prevIt = it;
	if (EofErrorCheck("<incomplete-condition-statement>", it, prevIt))
		return false;
	if (!IncompleteCondSt(it, indent + ".."))
	{
		it = prevIt;
		return false;
	}
	if (!AlternativePart(it, indent + ".."))
	{
		it = prevIt;
		return false;
	}
	return true;
}
bool IncompleteCondSt(list<Token>::iterator& it, string indent)
{
	AddBranch("<incomplete-condition-statement>", indent);
	list<Token>::iterator prevIt = it;
	if (EofErrorCheck("IF", it, prevIt))
		return false;
	if ((*it).code != 406)
	{
		LogParserError("IF", *it);
		it = prevIt;
		return false;
	}
	AddBranch(to_string((*it).code) + " " + (*it).value, indent + "..");
	it++;
	if (EofErrorCheck("<condition-expression>", it, prevIt))
		return false;
	if (!CondExpression(it, indent + ".."))
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
	if (!StatementsList(it, indent + ".."))
	{
		it = prevIt;
		return false;
	}
	return true;
}
bool CondExpression(list<Token>::iterator& it, string indent)
{
	AddBranch("<conditional-expression>", indent);
	list<Token>::iterator prevIt = it;
	if (EofErrorCheck("<variable-identifier>", it, prevIt))
		return false;
	if (!VarIdentifier(it, indent + ".."))
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
	if (!UnsignedInteger(it, indent + ".."))
	{
		it = prevIt;
		return false;
	}
	return true;
}
bool AlternativePart(list<Token>::iterator& it, string indent)
{
	AddBranch("<alternative-part>", indent);
	list<Token>::iterator prevIt = it;
	if (EndOfTokenList(it) || (*it).code != 408)
	{
		AddBranch("<empty>", indent + "..");
		return true;
	}
	AddBranch(to_string((*it).code) + " " + (*it).value, indent + "..");
	it++;
	if (!StatementsList(it, indent + ".."))
	{
		it = prevIt;
		return false;
	}
	return true;
}
bool UnsignedInteger(list<Token>::iterator& it, string indent)
{
	AddBranch("<unsigned-integer>", indent);
	if (IsConstant(*it, indent + ".."))
	{
		it++;
		return true;
	}
	RemoveLastBranch();
	return false;
}
bool ConstIdentifier(list<Token>::iterator& it, string indent)
{
	AddBranch("<constant-identifier>", indent);
	if (IsIdentifier(*it, indent + ".."))
	{
		it++;
		return true;
	}
	RemoveLastBranch();
	return false;
}
bool VarIdentifier(list<Token>::iterator& it, string indent)
{
	AddBranch("<variable-identifier>", indent);
	if (IsIdentifier(*it, indent + ".."))
	{
		it++;
		return true;
	}
	RemoveLastBranch();
	return false;
}
bool PrcIdentifier(list<Token>::iterator& it, string indent)
{
	AddBranch("<procedure-identifier>", indent);
	if (IsIdentifier(*it, indent + ".."))
	{
		it++;
		return true;
	}
	RemoveLastBranch();
	return false;
}
bool IsConstant(Token token, string indent) {
	if (token.code > 500 && token.code <= 1000)
	{
		AddBranch(to_string(token.code) + " " + token.value, indent);
		return true;
	}
	LogParserError("<unsigned-integer>", token);
	return false;
}
bool IsIdentifier(Token token, string indent)
{
	if (token.code > 1000)
	{
		AddBranch("<identifier>", indent);
		AddBranch(to_string(token.code) + " " + token.value, indent + "..");
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