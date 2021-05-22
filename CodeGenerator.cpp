#define _CRT_SECURE_NO_WARNINGS

#include "CodeGenerator.h"
#include <iostream>

void CodeGenerator(struct _signal_program* tree, string output_path)
{
	GenerateCommands(tree, output_path);
	PrintAsm();
	LoadErrorLog(output_path);
}
void GenerateCommands(struct _signal_program* tree, string output_path)
{
	auto program = tree->program;
	auto program_name = program->prcIdnt->identifier->value;
	auto label_declarations = program->block->declarations->labelDeclarations;
	AddLine(program_name + " proc");	// Main program start
	if (!label_declarations->isEmpty)	// labels declaration
	{
		AddLabel(label_declarations->uinteger->constant->value);
		auto labels_list = label_declarations->labelsList;
		while (!labels_list->isEmpty)
		{
			AddLabel(labels_list->uinteger->constant->value);
			labels_list = labels_list->labelsList;
		}
	}
	auto statementsList = program->block->statementsList;
	GenerateStatementList(statementsList);	// Stataments
	AddLine(program_name + " endp");	// Main program endpoint
	AddLine("end " + program_name);		// Main program end
}
void GenerateStatementList(_statements_list* statementsList)
{
	while (!statementsList->isEmpty)
	{
		auto statement = statementsList->statement;
		struct _unsigned_integer* uinteger;
		struct _cond_expression* cond_expression;
		struct Variable* var;
		struct _unsigned_integer* number;
		string system_label;
		switch (statement->statementType)
		{
		case 1:
			while (statement->statementType == 1)
			{
				uinteger = statement->uinteger;
				if (!CheckLabel(uinteger->constant->value))
				{
					LogCodeGeneratorError("Undeclared label used", uinteger->line, uinteger->column);
					return;
				}
				if (IsLabelUsed(uinteger->constant->value))
				{
					LogCodeGeneratorError("Label re-definition is not allowed", uinteger->line, uinteger->column);
					return;
				}
				AddLine(statement->uinteger->constant->value + ":");
				AddUsedLabel(statement->uinteger->constant->value);
				statement = statement->statement;
			}
			statementsList->statement = statement;
			continue;
		case 2:
			uinteger = statement->uinteger;
			if (!CheckLabel(uinteger->constant->value))
			{
				LogCodeGeneratorError("Undefined label used", uinteger->line, uinteger->column);
				break;
			}
			AddLine("jmp " + uinteger->constant->value);
			break;
		case 3:
			cond_expression = statement->condStatement->incompleteCondSt->condExpression;
			var = cond_expression->varIdnt->variable;
			number = cond_expression->uinteger;
			if (var->constant->type != number->constant->type)
			{
				LogCodeGeneratorError("Incompatible types of variables 'string' and 'unsigned integer' in assignment",
					cond_expression->varIdnt->line, cond_expression->varIdnt->column);
				break;
			}
			AddLine("MOV AX, " + var->identifier->value);
			AddLine("MOV BX, " + number->constant->value);
			AddLine("CMP AX, BX");
			system_label = GenerateSystemLabel();
			AddLine("JNE " + system_label);
			GenerateStatementList(statement->condStatement->incompleteCondSt->statementsList);
			if (statement->condStatement->alternativePart->isEmpty)
			{
				AddLine(system_label + ":	NOP");
			}
			else
			{
				AddLine(system_label + ":");
				GenerateStatementList(statement->condStatement->alternativePart->statementsList);
			}
			break;
		case 4:
			AddLine("nop");
			break;
		default:
			break;
		}
		statementsList = statementsList->statementsList;
	}
}
void AddLine(string line)
{
	asm_commands.push_back(line);
}

void AddLabel(string label_name)
{
	labels_table.push_back(label_name);
}

void AddUsedLabel(string label_name)
{
	used_labels.push_back(label_name);
}
bool IsLabelUsed(string label_name)
{
	list<string>::iterator it = used_labels.begin();
	for (; it != used_labels.end(); it++)
	{
		if (*it == label_name)
			return true;
	}
	return false;
}
bool CheckLabel(string label_name)
{
	list<string>::iterator it = labels_table.begin();
	for (; it != labels_table.end(); it++)
	{
		if (*it == label_name)
			return true;
	}
	return false;
}

string GenerateSystemLabel()
{
	return "?L" + to_string(system_label_counter++);
}

void LogCodeGeneratorError(string error, unsigned int line, unsigned int column)
{
	char buf[100];
	sprintf(buf, error.c_str());
	LogError("Code Generator", line, column, buf);
}

void PrintAsm()
{
	list<string>::iterator it = asm_commands.begin();
	for (; it != asm_commands.end(); it++)
	{
		cout << *it << endl;
	}
}