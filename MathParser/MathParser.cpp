// MathParser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Parser.h"


int _tmain(int argc, _TCHAR* argv[])
{
	if (argc == 2 || argc == 3) {
		Parser leParser;
		std::unique_ptr<ExpressionNode> leResultat(leParser.parse(argv[1]));

		//output format options.
		if (argc == 3) {
			int forwardSlash = (argv[2][0] == '/') ? 1 : 0,
				outputReal = std::count(argv[2], argv[2]+_tcslen(argv[2]), 'R') == 1 ? 1 << 1 : 0,
				outputImaginary = std::count(argv[2], argv[2] + _tcslen(argv[2]), 'I') == 1 ? 1 << 2 : 0;

			switch (forwardSlash + outputReal + outputImaginary) {
				case 3:
					std::tcout << leResultat->getValue().real();
				break;

				case 5:
					std::tcout << leResultat->getValue().imag();
				break;

				case 7:
					std::tcout << leResultat->getValue();
				break;

				default:
					std::tcout << TEXT("Unrecognized options");
			}
		}
		else {
			std::tcout << leResultat->getValue();
		}
	}
	else {
		_tprintf(TEXT("1 or 2 input string(s) expected, %d found\n"), argc - 1);
	}
	return EXIT_SUCCESS;
}