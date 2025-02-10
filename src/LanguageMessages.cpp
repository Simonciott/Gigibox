#include "LanguageManager.hpp"

map<string, vector<string>> Gigi::LanguageManager::messages = {
	{
		"logger.error",
		{
			"ERRORE GIGIBOX",
			"GIGIBOX ERROR"
		}
	},
	{
		"logger.in",
		{
			"IN",
			"IN"
		}
	},
	{
		"logger.error.code",
		{
			"CODICE ERRORE",
			"ERROR CODE"
		}
	},
	{
		"logger.error.description",
		{
			"DESCRIZIONE ERRORE (potrebbe non essere accurato)",
			"ERROR DESCRIPTION (could be inaccurate)"
		}
	},
	{
		"logger.procedures",
		{
			"PROCEDURE",
			"PROCEDURES"
		}
	},
	{
		"logger.error.assembly.illegalcode",
		{
			"E' stato interpretato codice invalido che ha causato errori fatali al programma",
			"Invalid code has been interpreted causing fatal errors to the program"
		}
	},
	{
		"logger.error.assembly.haltproc",
		{
			"L'interprete e' stato interrotto indefinitivamente. Si prega di riavviare GigiBox",
			"The interpreter has been put on halt indefenitely. Please restart Gigibox"
		}
	},
	{
		"logger.warning",
		{
			"AVVERTIMENTO GIGIBOX",
			"GIGIBOX WARNING"
		}
	},
	{
		"logger.warning.description",
		{
			"DESCRIZIONE AVVERTIMENTO",
			"WARNING DESCRIPTION"
		}
	},
	{
		"logger.description",
		{
			"DESCRIZIONE",
			"DESCRIPTION"
		}
	},
	{
		"logger.code",
		{
			"CODICE",
			"CODE"
		}
	},
	{
		"logger.warning.assembly.missingscript",
		{
			"Non sono state caricate alcune istruzioni al programma, causato probabilmente da script mancanti o inesistenti",
			"No instructions have been loaded in, possibly caused by missing scripts"
		}
	},
	{
		"logger.warning.assembly.noprogramrun",
		{
			"Non verra' eseguito alcun programma. Riavviare Gigibox se si desidera eseguirne uno",
			"No program will be run. Restart Gigibox if you wish to run one"
		}
	}
};