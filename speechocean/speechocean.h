#ifndef _speechocean_h_
#define _speechocean_h_

#include "../sys/praat.h"

FORM (HELP_Write_Log, U"SpeechOcean Write Log", nullptr)
{
    SENTENCE(message, U"Log Message", U"Log Test, Speechocean4.5")
    OK
    DO
	    Melder_information (message);
    END
}

void praat_speechocean_init()
{
    praat_addMenuCommand(U"Objects", U"ApplicationHelp",U"-- speechocean --", nullptr, 0, nullptr);
	praat_addMenuCommand(U"Objects", U"ApplicationHelp",U"Write Log...", nullptr, 0, HELP_Write_Log);
}

#endif