#pragma once

#include <list>
#include <string>
#include "Common.h"
using namespace std;

char* InitSymbolTable();
list<string> InitKeywords();
list<Variable> InitVariables();
list<Constant> InitConstants();