#include "speechocean.h"

#include "soTextGridEditor.h"
#include "SpectrumEditor.h"

#include <fstream>
#include <chrono>
#include <ctime>

autostring32 get_current_time_string()
{
    std::chrono::high_resolution_clock::time_point nowTime = std::chrono::high_resolution_clock::now();
    int milli = static_cast<int>(
        std::chrono::duration_cast<std::chrono::milliseconds>(nowTime.time_since_epoch()).count() % 1000);
	std::time_t nowTimeT = std::chrono::high_resolution_clock::to_time_t(nowTime);

	char buffer[50];
	std::strftime(buffer, 200, "%Y-%m-%dT%H:%M:%S", std::localtime(&nowTimeT));
	sprintf(buffer, "%s.%03d", buffer, milli);
	
    return Melder_8to32(buffer);
}

void Speechocean_Write_Log(conststring32 log_file_name, bool is_log_time, conststring32 user_id, conststring32 message)
{
    structMelderFile _file { };
    MelderFile file = &_file;        

    Melder_relativePathToFile(log_file_name, &_file);
    if(is_log_time)
    {
        autostring32 timeStr=get_current_time_string();
        MelderFile_appendText(file, timeStr.get());
        MelderFile_appendText(file, U"\t");
    }

    MelderFile_appendText(file, user_id);
    MelderFile_appendText(file, U"\t");

    MelderFile_appendText(file, message);
    MelderFile_appendText(file, U"\n");

    MelderFile_close(file);
}

FORM (HELP_Write_Log, U"SpeechOcean Write Log", nullptr)
{
    SENTENCE(log_file_name, U"Log File Name", U"log.txt")
    BOOLEAN(is_log_time, U"Log Time", true)
    SENTENCE(user_id, U"Speaker ID", U"Undefined")
    SENTENCE(message, U"Log Message", U"Log Test, Speechocean")
    OK
    DO
        Speechocean_Write_Log(log_file_name, is_log_time, user_id, message);
	    // Melder_information (message);
    END
}

static void cb_SoTextGridEditor_publication (Editor /* editor */, autoDaata publication) {
	/*
	 * Keep the gate for error handling.
	 */
	try {
		bool isaSpectralSlice = Thing_isa (publication.get(), classSpectrum) && str32equ (Thing_getName (publication.get()), U"slice");
		praat_new (publication.move());
		praat_updateSelection ();
		if (isaSpectralSlice) {
			int IOBJECT;
			FIND_ONE_WITH_IOBJECT (Spectrum)
			autoSpectrumEditor editor2 = SpectrumEditor_create (ID_AND_FULL_NAME, me);
			praat_installEditor (editor2.get(), IOBJECT);
			editor2.releaseToUser();
		}
	} catch (MelderError) {
		Melder_flushError ();
	}
}

FORM (SoTextGrid_viewAndEditWithLog, U"SoTextGrid: View & Edit with log", nullptr) 
{
	SENTENCE(log_file_name, U"Log File Name", U"log.txt")
    BOOLEAN(is_log_time, U"Log Time", true)
    SENTENCE(user_id, U"Speaker ID", U"Undefined")
	OK
    DO
        FIND_TWO_WITH_IOBJECT (TextGrid, Sound)   // Sound may not be NULL
        autoSoTextGridEditor editor = SoTextGridEditor_create(ID_AND_FULL_NAME, me, you, true, nullptr, nullptr, log_file_name, is_log_time, user_id);
        if (theCurrentPraatApplication -> batch) Melder_throw (U"Cannot view or edit a TextGrid from batch.");
        Editor_setPublicationCallback (editor.get(), cb_SoTextGridEditor_publication);
		praat_installEditor (editor.get(), IOBJECT);
		editor.releaseToUser();
    END
}

void praat_speechocean_init()
{
    praat_addMenuCommand(U"Objects", U"ApplicationHelp",U"-- speechocean --", nullptr, 0, nullptr);
	praat_addMenuCommand(U"Objects", U"ApplicationHelp",U"Write Log...", nullptr, 0, HELP_Write_Log);
    	praat_addAction2 (classSound, 1, classTextGrid, 1, U"SpeechOcean Special", nullptr, 0, nullptr);
        praat_addAction2 (classSound, 1, classTextGrid, 1, U"Edit with log...", nullptr, praat_ATTRACTIVE, SoTextGrid_viewAndEditWithLog);

}