#include <iostream>
#include <fstream>
#include <string>
#include<regex>
using namespace std;

ifstream file;

void readFile(string& code) {
	char ch;
	file.open("program.txt",ios::in);
	if (!file.is_open()){
		cout << "Error While Opening The File ...!!\n";
	}
	else {
		while (file >> noskipws >> ch) {
			if (ch == EOF)
				break;
			code += ch;
		}

	}
}
void removalOfComments(string& code) {
	regex singleLine("(//)([^\\\\\n]*)(\\\\n)");
	regex multiLine("/\\*[^*]*\\*+(?:[^/*][^*]*\\*+)*/");

	while (code.find("\n") < code.length()) {
		code.replace(code.find("\n"), 1, "\\n");
	}
	//cout << "1 File Code: \n" << code << endl;
	code = regex_replace(code, singleLine, "");
	//cout << "First Line Code: \n" << code << endl;
	code = regex_replace(code, multiLine, "");

	while (code.find("\\n") < code.length()) {
		code.replace(code.find("\\n"), 2, "\n");
	}
	//cout << "2 File Code: \n" << code << endl;
}
void removalOfWhite_spaces(string& code) {
	regex extraSpaces("[\\s]{2,}");
	code = regex_replace(code, extraSpaces, " ");
}
bool isSpace(string str) {
	regex space("\\s");
	return regex_match(str, space);
}
bool isKeyword(string lexeme) {
	regex keywords("base|bool|break|byte|case|catch|char|class|const|continue|default|delegate|double|explicit|event|extern|else|enum|false|finally|fixed|float|from|goto|if|in|int|interface|internal|long|new|null|namespace|operator|private|protected|public|readonly|ref|return|switch|struct|short|sizeof|static|string|this|throw|true|try|typeof|using|virtual|void|where|yield|main");
	return regex_match(lexeme, keywords);
}
bool isOperator(string lexeme) {
	regex operators("(\\+|-|\\*|/|>|<|=|>=|<=|\\+=|-=|\\*=|/=|\\+\\+|--|>>|<<|==|&|!|%)");
	return regex_match(lexeme, operators);
}
bool isSeparator(string lexeme) {
	regex separators("[(){},;]");
	return regex_match(lexeme, separators);
}
bool isNumeric_constant(string lexeme) {
	regex number("-?\\d+(\\.\\d+)?([Ee]-?\\d+)?");
	return regex_match(lexeme, number);
}
bool isString_constant(string lexeme) {
	regex literal("\".*\"");
	return regex_match(lexeme, literal);
}
bool isCharacter_constant(string lexeme) {
	regex character("'.?'");
	return regex_match(lexeme, character);
}
bool isConstant(string lexeme) {
	return isNumeric_constant(lexeme) || isString_constant(lexeme) || isCharacter_constant(lexeme);
}
bool isIdentifier(string lexeme) {
	regex identifier("^[a-zA-z_][a-zA-z0-9_]*$");
	return regex_match(lexeme, identifier);
}
void printRoleOfTokens(string lexeme) {
	if (isOperator(lexeme))
		cout << "< " << lexeme << " , operator >" << endl;
	else if (isSeparator(lexeme))
		cout << "< " << lexeme << " , separator >" << endl;
	else if (isKeyword(lexeme))
		cout << "< " << lexeme << " , keyword >" << endl;
	else if (isConstant(lexeme))
		cout << "< " << lexeme << " , Constant >" << endl;
	else if(isIdentifier(lexeme))
		cout << "< " << lexeme << " , Identifier >" << endl;
	else {
		cout << "Error ...!!!" << endl;
		cout << "< " << lexeme << " , Invalid Token >" << endl;
	}
}
void lexical_analyzer(string& code) {

	int i = 0;
	char ch;
	char qoute;
	string buffer = "";

	removalOfComments(code);
	cout << "Code: \n" << code << endl;
	removalOfWhite_spaces(code);
	cout << "Code Space: \n" << code << endl;

	while (i < code.length()) {
		ch = code.at(i++);

		if (ch == '\'' || ch == '\"') {
			if (ch == '\"')
				qoute = '\"';
			else
				qoute = '\'';

			do {
				buffer += ch;
				ch = code.at(i++);
			} while (ch != qoute);
			buffer += ch;
			continue;
		}
		else if (isSpace(string(1,ch))) {
			if (!buffer.empty()) {
				printRoleOfTokens(buffer);
				buffer = "";
			}
			continue;
		}
		else if (isSeparator(string(1, ch)))
		{
			if (!buffer.empty())
			{
				printRoleOfTokens(buffer);
				printRoleOfTokens(string(1, ch));
				buffer = "";
			}
			continue;
		}
		else if (isOperator(string(1, ch)) && !isOperator(buffer)) {
			if (!buffer.empty()) {
				printRoleOfTokens(buffer);
				buffer = "";
			}
		}
		else if (!isOperator(string(1, ch)) && isOperator(buffer)){
			printRoleOfTokens(buffer);
			buffer = "";
		}
		buffer += ch;
	}
}

int main() {
	string code;
	readFile(code);
	cout << "File Code: \n" << code << endl;
	lexical_analyzer(code);
	
	system("PAUSE");
} 