#pragma once

#include "Arguments.hpp"

namespace Gigi {
	struct LanguageManager {
		static string getMessage(string key) {
			vector<string>& message = messages[key];
			if (Arguments::currentLang >= message.size())
				return "[UNAVAILABLE TEXT]";
			return message[Arguments::currentLang];
		}

	private:
		static map<string, vector<string>> messages;
	};
}