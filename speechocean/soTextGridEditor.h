#ifndef _soTextGridEditor_h_
#define _soTextGridEditor_h_

#include "TextGridEditor.h"

Thing_define (SoTextGridEditor, TextGridEditor) 
{
    autostring32 logFileName;
    bool isLogTime;
    autostring32 userId;

    void writeLog(conststring32 message);
};

void SoTextGridEditor_init (SoTextGridEditor me, conststring32 title, TextGrid grid, 
    Sampled sound, 
    bool ownSound, 
    SpellingChecker spellingChecker, 
    conststring32 callbackSocket,
    conststring32 logFileName,
    bool isLogTime,
    conststring32 userId);

autoSoTextGridEditor SoTextGridEditor_create (conststring32 title, TextGrid grid,
	Sampled sound,   // either a Sound or a LongSound, or null
	bool ownSound,
	SpellingChecker spellingChecker,
	conststring32 callbackSocket,
    conststring32 logFileName,
    bool isLogTime,
    conststring32 userId);

#define SO_LOG(...) \
    if(!str32cmp(my classInfo -> className, U"SoTextGridEditor")) \
	{ \
		SoTextGridEditor editor = (SoTextGridEditor)me; \
        char32 message[512]; \
        Melder_sprint(message, 512, __VA_ARGS__); \
		editor->writeLog(message); \
	} 
#endif