/*
  ==============================================================================

    SimpleKnob.h
    Created: 26 Jun 2020 1:32:51pm
    Author:  Admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SimpleLookAndFeel.h"

class SimpleKnob : public Slider
{
public:
    Label value;
    
    void init(String name);
    void drawOutline (Graphics& g);
    void setComponentBounds (Rectangle<int> bounds);
    
private:
    SimpleLookAndFeel lookAndFeel;
    String name;
    HashMap<String, Rectangle<int>> boundMap;
    int paramTextHeight, nameFontSize;
};
