#pragma once

#include <string>
#include <vector>
#include <map>
#include <functional>

#define AVAILABLE_LANGUAGES 2

using std::string, std::vector, std::pair, std::map;

namespace Gigi {
	string languages[AVAILABLE_LANGUAGES];

	struct Arguments {

		static int currentLang;
		static string program;

		// argomenti disponibili. prima stringa � l'argomento acronomizzato, seconda � il nome intero
		static vector<pair<string, string>> availableArgs;



		static void processArgument(char** arguments, int argIndex);
	};
}