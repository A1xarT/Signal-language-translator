#pragma once

#include <string>
#include "SyntaxStructures.h"

using namespace std;

static unsigned int system_label_counter;

static list<string> asm_commands;
static list<string> labels_table;
static list<string> used_labels;

void CodeGenerator(struct _signal_program* tree, string output_path);
void GenerateCommands(struct _signal_program* tree, string output_path);
void GenerateStatementList(_statements_list* statementsList);

void AddLine(string line);
void AddLabel(string label_name);
void AddUsedLabel(string label_name);

bool CheckLabel(string label_name);
bool IsLabelUsed(string label_name);

string GenerateSystemLabel();

void LogCodeGeneratorError(string error, unsigned int line, unsigned int column);

void PrintAsm();
