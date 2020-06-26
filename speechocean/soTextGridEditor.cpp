#include "soTextGridEditor.h"

Thing_implement (SoTextGridEditor, TextGridEditor, 0);

void SoTextGridEditor_init (SoTextGridEditor me, conststring32 title, TextGrid grid, 
    Sampled sound, 
    bool ownSound, 
    SpellingChecker spellingChecker, 
    conststring32 callbackSocket)
{
    TextGridEditor_init (me, title, grid, sound, ownSound, spellingChecker, callbackSocket);
}

autoSoTextGridEditor SoTextGridEditor_create (conststring32 title, TextGrid grid, 
    Sampled sound, 
    bool ownSound, 
    SpellingChecker spellingChecker, 
    conststring32 callbackSocket)
{
    try {
		autoSoTextGridEditor me = Thing_new (SoTextGridEditor);
		SoTextGridEditor_init (me.get(), title, grid, sound, ownSound, spellingChecker, callbackSocket);
		return me;
	} catch (MelderError) {
		Melder_throw (U"SoTextGrid window not created.");
	}
}