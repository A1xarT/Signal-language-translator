#pragma once

#include <string>
#include "SyntaxStructures.h"

using namespace std;

static unsigned int system_label_counter;

static list<string> asm_commands;

static list<string> labels_table;
static list<string> available_labels;
static list<string> defined_labels;

void CodeGenerator(struct _signal_program* tree, string output_path);
void GenerateCommands(struct _signal_program* tree);
void GenerateStatementList(_statements_list* statementsList);
void GenerateExit(struct _signal_program* tree);

void AddLine(string line);
void AddLabel(string label_name);
void AddDefinedLabel(string label_name);
void GetAllLabels(struct _signal_program* tree);

bool CheckLabel(string label_name);
bool IsLabelDefined(string label_name);
bool IsLabelAvailable(string label_name);

string GenerateSystemLabel();

void LogCodeGeneratorError(string error, unsigned int line, unsigned int column);

void PrintAsm();
void ClearGeneratorData();