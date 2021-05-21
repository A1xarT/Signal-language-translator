#pragma once
#include "Common.h"

using namespace std;

struct _signal_program {
	struct _program* program;
};

struct _program {
	struct _procedure_identifier* prcIdnt;
	struct _block* block;
};

struct _block {
	struct _declarations* declarations;
	struct _statements_list* statementsList;
};

struct _declarations {
	struct _label_declarations* labelDeclarations;
};

struct _label_declarations {
	struct _unsigned_integer* uinteger;
	struct _labels_list* labelsList;
	bool isEmpty;
};

struct _labels_list {
	struct _unsigned_integer* uinteger;
	struct _labels_list* labelsList;
	bool isEmpty;
};

struct _statements_list {
	struct _statement* statement;
	struct _statements_list* statementList;
	bool isEmpty;
};

struct _statement {
	unsigned int statementType;	// 1 2 3 4
	// type 1 <unsigned integer>:<statement>
	struct _unsigned_integer* uinteger;
	struct _statement* statement;
	// type 2 GOTO <unsigned-integer>;
	//
	// type 3 <condition-statement> ENDIF;
	struct _cond_statement* condStatement;
	// type 3 ;
	//
};

struct _cond_statement {
	struct _incomplete_cond_statement* incompleteCondSt;
	struct _alt_part* alternativePart;
};
struct _incomplete_cond_statement {
	struct _cond_expression* condExpression;
	struct _statements_list* statementList;

};
struct _cond_expression {
	struct _variable_identifier* varIdnt;
	struct _unsigned_integer* uinteger;
};
struct _alt_part {
	struct _statements_list* statementList;
	bool isEmpty;
};

struct _procedure_identifier {
	Identifier* value;
};

struct _variable_identifier {
	Identifier* value;
};

struct _unsigned_integer {
	Constant* value;
};