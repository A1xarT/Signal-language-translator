#define _CRT_SECURE_NO_WARNINGS

#include "Lexer.h"

void Lexer(ifstream& test_file, string output_path)
{
	const char* symbols_table = InitSymbolTable();
	list<string> keywords_table = InitKeywords();
	unsigned int keyword_code = 401, constant_code = 501, identifier_code = 1001;

	unsigned int line = 1, column = 1;
	char c;
	test_file.get(c);
	while (!test_file.eof())
	{
		switch (symbols_table[c])
		{
		case 0:	// whitespaces
			do
			{
				if (c == 10)
				{
					line++;
					column = 1;
				}
				else column++;
				test_file.get(c);
			} while (!test_file.eof() && symbols_table[c] == 0);
			break;
		case 1:	// number
		{
			Token token = { line, column };
			string number = string(1, c);
			test_file.get(c);
			column++;
			while (!test_file.eof() && symbols_table[c] == 1)
			{
				number.push_back(c);
				test_file.get(c);
				column++;
			}
			token.value = number;
			token.code = FindToken(number);
			if (token.code == -1)
			{
				token.code = constant_code++;
			}
			AddToken(token);
			if (!FindConstant(token.value, "unsigned int"))
				AddConstant(Constant{ token.value, "unsigned int" });
			break;
		}
		case 2:	// identifier / keyword
		{
			Token token = { line, column };
			string idn = string(1, c);
			test_file.get(c);
			column++;
			while (!test_file.eof() && (symbols_table[c] == 1 || symbols_table[c] == 2))
			{
				idn.push_back(c);
				test_file.get(c);
				column++;
			}
			bool is_keyword = false;
			uint16_t keyword_id = 0;
			list<string>::iterator it;
			for (it = keywords_table.begin(); it != keywords_table.end(); it++, keyword_id++)
			{
				if (*it == idn)
				{
					is_keyword = true;
					break;
				}
			}

			token.value = idn;
			token.code = FindToken(idn);
			if (token.code == -1)
			{
				if (is_keyword)
				{
					token.code = keyword_code + keyword_id;
				}
				else
				{
					token.code = identifier_code++;
					if (!FindIdentifier(idn))
					{
						AddIdentifier(Identifier{ idn });
					}
				}
			}
			AddToken(token);
			break;
		}
		case 3:	// one-symbol delimiter
		{
			Token token{ line,column };
			token.value = string(1, c);
			token.code = c;
			AddToken(token);
			test_file.get(c);
			column++;
			break;
		}
		case 5:	// comment
		{
			unsigned int comm_line = line, comm_column = column;
			test_file.get(c);
			column++;
			if (test_file.eof() || c != '*')
			{
				LogError("Lexer", line, column - 1, "Illegal character '(' detected");
				break;
			}
			test_file.get(c);
			column++;
			if (test_file.eof())
			{
				LogError("Lexer", comm_line, comm_column, "Opened comment at the end of the file");
				break;
			}
			if (c == 10)
			{
				line++;
				column = 1;
			}
			else column++;
			char c_next;
			test_file.get(c_next);

			if (test_file.eof())
			{
				LogError("Lexer", comm_line, comm_column, "Opened comment at the end of the file");
				break;
			}
			while (true)
			{
				if (c == '*' && c_next == ')')
				{
					test_file.get(c);
					column++;
					break;
				}
				if (c_next == 10)
				{
					line++;
					column = 1;
				}
				else column++;
				c = c_next;
				test_file.get(c_next);
				if (test_file.eof())
				{
					if (c == '*')
						LogError("Lexer", comm_line, comm_column, "Opened comment at the end of the file");
					else
						LogError("Lexer", comm_line, comm_column, "Opened comment at the end of the file");
					break;
				}
			}
			break;
		}
		case 6:	// err symbol
			char buffer[255];
			sprintf(buffer, "Illegal character '%c' detected", c);
			LogError("Lexer", line, column, buffer);
			test_file.get(c);
			column++;
			break;
		}
	}
	ClearFile(output_path);
	LoadErrorLog(output_path);
	LoadTokenTable(output_path);
	LoadConstantTable(output_path);
	LoadIdentifierTable(output_path);
	return;
}