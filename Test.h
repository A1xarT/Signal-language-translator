#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include "Lexer.h"
#include "Parser.h"
#include "CodeGenerator.h"
#include "Common.h"

using namespace std;

void run_tests(string folder_name, string subfolder_name, unsigned int tests_number);