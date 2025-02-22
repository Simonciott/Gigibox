#pragma once

#include <string>
#include <vector>
#include <map>
#include <functional>

#define AVAILABLE_LANGUAGES 2

using std::string, std::vector, std::pair, std::map;

namespace Gigi {
	extern string languages[AVAILABLE_LANGUAGES];

	struct Arguments {

		static int currentLang;
		static string program;
		static string directory;

		// argomenti disponibili. prima stringa è l'argomento acronomizzato, seconda è il nome intero
		static vector<pair<string, string>> availableArgs;



		static void processArgument(const char** arguments, int argIndex);
	};
}