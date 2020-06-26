#ifndef _speechocean_h_
#define _speechocean_h_

#include "../sys/praat.h"

autostring32 get_current_time_string();

void Speechocean_Write_Log(conststring32 log_file_name, bool is_log_time, conststring32 user_id, conststring32 message);

void praat_speechocean_init();

#endif