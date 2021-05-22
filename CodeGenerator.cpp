#define _CRT_SECURE_NO_WARNINGS

#include "CodeGenerator.h"
#include <iostream>

void CodeGenerator(struct _signal_program* tree, string output_path)
{
	system_label_counter = 0;
	GetAllLabels(tree);
	GenerateCommands(tree);
	GenerateExit(tree);
	LoadAsmCode(output_path, asm_commands);
	LoadErrorLog(output_path);
	//PrintAsm();
	ClearGeneratorData();
}
void GenerateCommands(struct _signal_program* tree)
{
	auto program = tree->program;
	if (!program || !program->prcIdnt || !program->block || !program->block->declarations ||
		!program->block->declarations->labelDeclarations) return;
	auto program_name = program->prcIdnt->identifier->value;
	auto label_declarations = program->block->declarations->labelDeclarations;
	AddLine(program_name + " PROC");	// Main program start
	if (!label_declarations->isEmpty)	// labels declaration
	{
		AddLabel(label_declarations->uinteger->constant->value);
		auto labels_list = label_declarations->labelsList;
		if (!labels_list) return;
		while (!labels_list->isEmpty)
		{
			AddLabel(labels_list->uinteger->constant->value);
			labels_list = labels_list->labelsList;
		}
	}
	auto statementsList = program->block->statementsList;
	GenerateStatementList(statementsList);	// Stataments
}
void GenerateStatementList(_statements_list* statementsList)
{
	bool getNext = true;
	auto statements_list = statementsList;
	struct _statement* statement = new _statement();
	while (statements_list != nullptr && !statements_list->isEmpty)
	{
		if (getNext)
			statement = statements_list->statement;
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
					string error = "Undeclared label '" + uinteger->constant->value + "' was used";
					LogCodeGeneratorError(error, uinteger->line, uinteger->column);
					statement = statement->statement;
					continue;
				}
				if (IsLabelDefined(uinteger->constant->value))
				{
					LogCodeGeneratorError("Label re-definition is not allowed", uinteger->line, uinteger->column);
					statement = statement->statement;
					continue;
				}
				AddLine(statement->uinteger->constant->value + ":");
				AddDefinedLabel(statement->uinteger->constant->value);
				statement = statement->statement;
			}
			getNext = false;
			continue;
		case 2:
			uinteger = statement->uinteger;
			if (!CheckLabel(uinteger->constant->value))
			{
				string error = "Undeclared label '" + uinteger->constant->value + "' was used";
				LogCodeGeneratorError(error, uinteger->line, uinteger->column);
				break;
			}
			if (!IsLabelAvailable(uinteger->constant->value))
			{
				string error = "Undefined label '" + uinteger->constant->value + "' was used";
				LogCodeGeneratorError(error, uinteger->line, uinteger->column);
				break;
			}
			AddLine("JMP " + uinteger->constant->value);
			break;
		case 3:
			cond_expression = statement->condStatement->incompleteCondSt->condExpression;
			var = cond_expression->varIdnt->variable;
			number = cond_expression->uinteger;
			if (var->constant->type == "")
			{
				string error = "Undeclared variable '" + var->identifier->value + "' was used";
				LogCodeGeneratorError(error, cond_expression->varIdnt->line, cond_expression->varIdnt->column);
				break;
			}
			if (var->constant->type != number->constant->type)
			{
				if (number->constant->type == "")
					break;
				LogCodeGeneratorError("Incompatible types of variable '" + var->identifier->value + ":" +
					var->constant->type + "'" + " and constant '" + number->constant->type + "' in assignment",
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
				AddLine(system_label + ": NOP");
			}
			else
			{
				AddLine(system_label + ":");
				GenerateStatementList(statement->condStatement->alternativePart->statementsList);
			}
			break;
		case 4:
			AddLine("NOP");
			break;
		default:
			break;
		}
		statements_list = statements_list->statementsList;
		getNext = true;
	}
}
void GetAllLabels(struct _signal_program* tree)
{
	auto prevErrors = GetErrorList();
	GenerateCommands(tree);
	asm_commands.clear();
	labels_table.clear();
	available_labels = list<string>(defined_labels);
	defined_labels.clear();
	SetErrorList(prevErrors);
	system_label_counter = 0;
}
void GenerateExit(struct _signal_program* tree)
{
	if (tree->program && tree->program->prcIdnt)
	{
		auto program_name = tree->program->prcIdnt->identifier->value;
		AddLine(program_name + " ENDP");	// Main program endpoint
		AddLine("END " + program_name);		// Main program end
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

void AddDefinedLabel(string label_name)
{
	defined_labels.push_back(label_name);
}
bool IsLabelDefined(string label_name)
{
	for (auto& it : defined_labels)
		if (it == label_name)
			return true;
	return false;
}
bool IsLabelAvailable(string label_name)
{
	for (auto& i : available_labels)
		if (i == label_name)
			return true;
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
	for (auto& i : asm_commands)
		cout << i << endl;
}
void ClearGeneratorData()
{
	labels_table.clear();
	available_labels.clear();
	defined_labels.clear();
	asm_commands.clear();
	system_label_counter = 0;
	FreeTables();
}