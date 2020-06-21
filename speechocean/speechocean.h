#ifndef _speechocean_h_
#define _speechocean_h_

#include <fstream>
#include <chrono>
#include <ctime>
#include "../sys/praat.h"

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
	    //Melder_information (message);
    END
}

void praat_speechocean_init()
{
    praat_addMenuCommand(U"Objects", U"ApplicationHelp",U"-- speechocean --", nullptr, 0, nullptr);
	praat_addMenuCommand(U"Objects", U"ApplicationHelp",U"Write Log...", nullptr, 0, HELP_Write_Log);
}

#endif