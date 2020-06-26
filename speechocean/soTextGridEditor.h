#ifndef _soTextGridEditor_h_
#define _soTextGridEditor_h_

#include "TextGridEditor.h"

Thing_define (SoTextGridEditor, TextGridEditor) 
{
	// EEG eeg;
	// GuiMenuItem extractSelectedEEGPreserveTimesButton, extractSelectedEEGTimeFromZeroButton;

	// bool v_hasPitch ()
	// 	override { return false; }
	// bool v_hasIntensity ()
	// 	override { return false; }
	// bool v_hasFormants ()
	// 	override { return false; }
	// bool v_hasPulses ()
	// 	override { return false; }
	// void v_createMenus ()
	// 	override;
	// void v_createHelpMenuItems (EditorMenu menu)
	// 	override;
	// conststring32 v_getChannelName (integer channelNumber)
	// 	override;
	// void v_createMenuItems_file_extract (EditorMenu menu)
	// 	override;
	// void v_updateMenuItems_file ()
	// 	override;
};

void SoTextGridEditor_init (SoTextGridEditor me, conststring32 title, TextGrid grid, 
    Sampled sound, 
    bool ownSound, 
    SpellingChecker spellingChecker, 
    conststring32 callbackSocket);

autoSoTextGridEditor SoTextGridEditor_create (conststring32 title, TextGrid grid,
	Sampled sound,   // either a Sound or a LongSound, or null
	bool ownSound,
	SpellingChecker spellingChecker,
	conststring32 callbackSocket);

#endif