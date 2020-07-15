/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
Svf_v2AudioProcessorEditor::Svf_v2AudioProcessorEditor (Svf_v2AudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (500, 300);
    reduction = 5;
    
    String cutoffParamID;
    int cutoffIndex;
    cutoffParamID = "cutoff";
    cutoffIndex = 1;
    initSlider(cutoffParamID, cutoffIndex, &cutoffAttachment);
    
    String qParamID;
    int qIndex;
    qParamID = "q";
    qIndex = 2;
    initSlider(qParamID, qIndex, &qAttachment);
    
    // define state knob
    String typeParamID;
    int typeIndex;
    typeParamID = "type";
    typeIndex = 3;
    initSlider(typeParamID, typeIndex, &typeAttachment);
    
    String gainParamID;
    int gainIndex;
    gainParamID = "gain";
    gainIndex = 0;
    initSlider(gainParamID, gainIndex, &gainAttachment);

    calculateBounds();
}

Svf_v2AudioProcessorEditor::~Svf_v2AudioProcessorEditor()
{
}

//==============================================================================
void Svf_v2AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colours::black);
    g.setColour (Colours::yellow);
    
    g.setFont (Font ("Monospaced", titleFontSize, Font::plain));
    g.drawRect(title);
    for (int i = 0; i < knobArraySize; i++)
    {
        knobArray[i].drawOutline(g);
    }
    
    g.setColour(Colours::white);
    g.setFont (Font ("Monospaced", titleFontSize, Font::plain));
    g.drawFittedText ("SVF V_2", title, Justification::centred, 1);
}

void Svf_v2AudioProcessorEditor::sliderValueChanged(Slider *slider)
{
    if (slider == &knobArray[knobMap["cutoff"]])
    {
        setSliderText("cutoff", 1.0f);
    }
    if (slider == &knobArray[knobMap["q"]])
    {
        setSliderText("q", 0.1f);
    }
    if (slider == &knobArray[knobMap["type"]])
    {
        setSliderText("type", -1.0f);
    }
    if (slider == &knobArray[knobMap["gain"]])
    {
        setSliderText("gain", 1.0f);
    }
}

void Svf_v2AudioProcessorEditor::resized()
{
    this->calculateBounds();
}

void Svf_v2AudioProcessorEditor::setSliderText(String paramID, float resolution)
{
    if (paramID.equalsIgnoreCase("type"))
    {
        changeTypeText(knobArray[knobMap[paramID]].getValue());
    }
    else
    {
        
        float value = knobArray[knobMap[paramID]].getValue();
        value = round(value * (1.0f / resolution)) * resolution;
        knobArray[knobMap[paramID]].value.setText((String) value , dontSendNotification);
    }
}

void Svf_v2AudioProcessorEditor::calculateBounds()
{
    // rectangle containing local bounds
    Rectangle<int> r = getLocalBounds();
    
    titleHeight = r.getHeight() / 4;
    
    title = r.removeFromTop(titleHeight);
    title.reduce(reduction, reduction);
    titleFontSize = title.getHeight() * 0.5;
    
    int knobWidth = r.getWidth() / knobArraySize;
    
    for (int i = 0; i < knobArraySize; i++)
    {
        Rectangle<int> thisRect;
        if (r.getWidth() > knobWidth)
        {
            thisRect = r.removeFromLeft(knobWidth);
        }
        else
        {
            thisRect = r;
        }
        thisRect.reduce(reduction, reduction);
        knobArray[i].setComponentBounds(thisRect);
    }
}


void Svf_v2AudioProcessorEditor::initSlider(String paramID, int index, std::unique_ptr<SliderAttachment> * attachment)
{
    addAndMakeVisible(knobArray[index]);
    addAndMakeVisible(knobArray[index].value);
    knobMap.set(paramID, index);
    *attachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.apvts, paramID, knobArray[index]);
    knobArray[index].init(paramID);
    knobArray[index].addListener(this);
    setSliderText(paramID, 1.0f);
}

void Svf_v2AudioProcessorEditor::changeTypeText(int typeValue)
{
    switch(typeValue)
    {
        case 0:
            knobArray[knobMap["type"]].value.setText("low", dontSendNotification);
            break;
        case 1:
            knobArray[knobMap["type"]].value.setText("band", dontSendNotification);
            break;
        case 2:
            knobArray[knobMap["type"]].value.setText("notch", dontSendNotification);
            break;
        case 3:
            knobArray[knobMap["type"]].value.setText("high", dontSendNotification);
            break;
        default:
            knobArray[knobMap["type"]].value.setText("low", dontSendNotification);
    }
}
