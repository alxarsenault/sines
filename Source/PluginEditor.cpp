/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
SinesAudioProcessorEditor::SinesAudioProcessorEditor (SinesAudioProcessor& p)
    : easy::AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    addSlider("Gain");
}

SinesAudioProcessorEditor::~SinesAudioProcessorEditor()
{
}

void SinesAudioProcessorEditor::paint(Graphics& g)
{
    g.fillAll ({180, 180, 180});
}

void SinesAudioProcessorEditor::resized()
{
    resize();
}
