#pragma once

#include "LanguageManager.hpp"
#include "CommonFunctions.hpp"
#include "ErrorCodes.hpp"

#include <iostream>

#define NEWL '\n'
#define TAB '\t'
#define INDRES ':' + TAB // indented result

#define LOGGER_PROCEDURES 1
#define LOGGER_DESCRIPTION 2
#define LOGGER_CODE 4
#define LOGGER_IN 8
#define LOGGER_ERROR 16 // se non è incluso allora l'errore non è visibile
#define LOGGER_WARNING 32

#define LOGGER_ALL (LOGGER_ERROR | LOGGER_WARNING | LOGGER_IN | LOGGER_DESCRIPTION | LOGGER_PROCEDURES)


using std::cout;

typedef Gigi::LanguageManager LM;

namespace Gigi {
	struct Logger {
		static void logError(string in, string description, string procedure, int errorcode = 0, int showcode = LOGGER_ALL) {
			if (!areBitsOn(showcode, LOGGER_ERROR)) return;
			string message =
				LM::getMessage("logger.error") + NEWL
				+ (areBitsOn(showcode, LOGGER_IN) ? (TAB + LM::getMessage("logger.in") + INDRES + in + NEWL) : "")
				+ (areBitsOn(showcode, LOGGER_CODE) ? (TAB + LM::getMessage("logger.error.code") + INDRES + std::to_string(errorcode) + NEWL) : "")
				+ (areBitsOn(showcode, LOGGER_DESCRIPTION) ? (TAB + LM::getMessage("logger.error.description") + INDRES + LM::getMessage(description) + NEWL) : "")
				+ (areBitsOn(showcode, LOGGER_PROCEDURES) ? (TAB + LM::getMessage("logger.procedures") + INDRES + LM::getMessage(procedure) + NEWL) : "")
				+ NEWL;
			cout << message;
		}
		static void logWarning(string in, string description, string procedure, int warncode = 0, int showcode = (LOGGER_ALL | LOGGER_CODE ^ LOGGER_CODE)) {
			if (!areBitsOn(showcode, LOGGER_WARNING)) return;
			string message =
				LM::getMessage("logger.warning") + NEWL
				+ (areBitsOn(showcode, LOGGER_IN) ? (TAB + LM::getMessage("logger.in") + INDRES + in + NEWL) : "")
				+ (areBitsOn(showcode, LOGGER_CODE) ? (TAB + LM::getMessage("logger.code") + INDRES + std::to_string(warncode) + NEWL) : "")
				+ (areBitsOn(showcode, LOGGER_DESCRIPTION) ? (TAB + LM::getMessage("logger.warning.description") + INDRES + LM::getMessage(description) + NEWL) : "")
				+ (areBitsOn(showcode, LOGGER_PROCEDURES) ? (TAB + LM::getMessage("logger.procedures") + INDRES + LM::getMessage(procedure) + NEWL) : "")
				+ NEWL;
			cout << message;
		}
	};
}