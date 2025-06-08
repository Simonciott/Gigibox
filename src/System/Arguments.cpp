#include "Arguments.hpp"

#include "ErrorCodes.hpp"

#include <algorithm>
#include <stdexcept>

using Gigi::Arguments;

string Gigi::languages[AVAILABLE_LANGUAGES] = {
	"italian",
	"english"
};

int Arguments::currentLang = 0;
string Arguments::program = "./dvd.gasm";
string Arguments::directory = "";

vector<pair<string, string>> Arguments::availableArgs {
	{ "language", "l" }, // che lingua scegliere
	{ "program", "P" }, // il nome/percorso del file da interpretare
	{ "input", "I" } // profilo input da utilizzare
};

void Arguments::processArgument(const char** arguments, int argIndex) {

	string sarg;
	// cerca a quale argomento si riferisce la stringa in arguments
	int index = -1;

	string buffer;
	try {
		buffer = arguments[argIndex];
	}
	catch (const std::out_of_range& err) {
		throw CODE_ERROR_ARGUMENT | CODE_ERROR_ABSENCE;
	}
	int firstsecond = -1; // determina se si accede al nome argomento intero o l'acronimo ((bool)firstsecond ? availableArgs[x].second : availableArgs[x].first) se negativo dà un errore

	// rimuove dash
	int dashindex = buffer.find_first_of("-");
	while (dashindex == 0) {
		firstsecond++;
		buffer.erase(0, 1);
		dashindex = buffer.find_first_of("-");
	}
	if (firstsecond < 0) {
		index = 1;
		sarg = buffer;
	}

	if(sarg != buffer) // esegui for solo se sarg non è stato assegnato dall'if giusto sopra questo
	for (int i = 0; i < availableArgs.size(); i++) {
		// assegna nome argomento appropriato
		string argname;
		// eliminato try catch inutile

		// compara
		if (buffer == argname) {
			index = i;
			break;
		}
	}
	if (index < 0) throw CODE_ERROR_ARGUMENT; // 111010.. errore argomenti


	if(sarg != buffer) sarg = arguments[argIndex + 1];
	if(index != 1) transform(sarg.begin(), sarg.end(), sarg.begin(), tolower);


	switch (index) {
	case 0:
		currentLang = -1;

		for (int i = 0; i < AVAILABLE_LANGUAGES; i++) {
			if (languages[i] == sarg) {
				currentLang = i;
				break;
			}
		}
		if (currentLang < 0) throw CODE_ERROR_ARGUMENT;

		break;
	case 1:
		program = sarg;
		break;
	default:
		throw CODE_ERROR_ARGUMENT | CODE_ERROR_GENERAL; // ..10 errore generale
	}

}