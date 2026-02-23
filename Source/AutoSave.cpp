/*
  ==============================================================================

    AutoSave.cpp
    Created: 20 Feb 2026 10:46:56pm
    Author:  German

  ==============================================================================
*/

#include "AutoSave.h"

AutoSave::AutoSave() {
    startTimer(delay);
}

void AutoSave::timerCallback() {
    if (saveInProgress) {
        return;
    }
    saveInProgress = true;
    saveProject();
    saveInProgress = false;
}
