#include "soTextGridEditor.h"
#include "speechocean.h"

Thing_implement (SoTextGridEditor, TextGridEditor, 0);

void SoTextGridEditor_init (SoTextGridEditor me, conststring32 title, TextGrid grid, 
    Sampled sound, 
    bool ownSound, 
    SpellingChecker spellingChecker, 
    conststring32 callbackSocket,
    conststring32 logFileName,
    bool isLogTime,
    conststring32 userId)
{
    my logFileName = Melder_dup(logFileName);
    my isLogTime = isLogTime;
    my userId = Melder_dup(userId);
    TextGridEditor_init (me, title, grid, sound, ownSound, spellingChecker, callbackSocket);
    // Melder_information(my logFileName.get(), U",", my isLogTime, U",", my userId.get());
}

autoSoTextGridEditor SoTextGridEditor_create (conststring32 title, TextGrid grid, 
    Sampled sound, 
    bool ownSound, 
    SpellingChecker spellingChecker, 
    conststring32 callbackSocket,
    conststring32 logFileName,
    bool isLogTime,
    conststring32 userId)
{
    try {
		autoSoTextGridEditor me = Thing_new (SoTextGridEditor);
		SoTextGridEditor_init (me.get(), title, grid, sound, ownSound, spellingChecker, callbackSocket, logFileName, isLogTime, userId);
		return me;
	} catch (MelderError) {
		Melder_throw (U"SoTextGrid window not created.");
	}
}

void structSoTextGridEditor::writeLog(conststring32 message)
{
    Speechocean_Write_Log(logFileName.get(), isLogTime, userId.get(), message);
}
