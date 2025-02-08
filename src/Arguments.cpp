#include "Arguments.hpp"

#include <algorithm>

using Gigi::Arguments;

int Arguments::currentLang = 0;
string Arguments::program = "";

vector<pair<string, string>> Arguments::availableArgs {
	{ "language", "l" }, // che lingua scegliere
	{ "program", "P" }, // il nome/percorso del file da interpretare
	{ "input", "I" } // profilo input da utilizzare
};

void Arguments::processArgument(char** arguments, int argIndex) {

	// cerca a quale argomento si riferisce la stringa in arguments
	int index = -1;
	for (int i = 0; i < availableArgs.size(); i++) {
		if (string(arguments[argIndex]) == availableArgs[i].first || string(arguments[argIndex]) == availableArgs[i].second) {
			index = i;
			break;
		}
	}
	if (index < 0) throw 0b11101000; // 111010.. errore argomenti

	string sarg = string(arguments[argIndex + 1]);
	transform(sarg.begin(), sarg.end(), sarg.begin(), tolower);

	switch (index) {
	case 0:
		currentLang = -1;

		for (int i = 0; i < AVAILABLE_LANGUAGES; i++) {
			if (languages[i] == sarg) {
				currentLang = i;
				break;
			}
		}
		if (currentLang < 0) throw 0b11101000;

		break;
	
	case 1:
		program = sarg;
		break;
	default:
		throw 0b11101011; // ..10 errore generale
	}
}