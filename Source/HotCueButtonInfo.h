/*
  ==============================================================================

    HotCueButtonInfo.h
    Created: 20 Feb 2026 11:31:43pm
    Author:  German

  ==============================================================================
*/

#pragma once

class HotCueButtonInfo {
    
public:
    HotCueButtonInfo(int order, double positionInSeconds);
    
    int order;
    double positionInSeconds;
};
