/*
  ==============================================================================

   Copyright 2022 Suzuki Kengo

   JR-Granular is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
 
   JR-Granular is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.
 
   You should have received a copy of the GNU General Public License
   along with JR-Granular. If not, see <http://www.gnu.org/licenses/>.

  ==============================================================================
*/

#pragma once

#include "EditorContent.h"
#include "GUI/EditorLnf.h"
#include "PluginProcessor.h"
#include <JuceHeader.h>

class JRGranularAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    JRGranularAudioProcessorEditor (JRGranularAudioProcessor& p,
                                    juce::AudioProcessorValueTreeState& apvts,
                                    juce::UndoManager& um);

    void paint (juce::Graphics&) override;
    void resized() override;

    bool keyPressed (const juce::KeyPress& key) override;

private:
    JRGranularAudioProcessor& audioProcessor;

    juce::UndoManager& undoManager;

    EditorContent editorContent;

    static constexpr int defaultWidth { 440 };
    static constexpr int defaultHeight { 280 };

    struct SharedLnf
    {
        SharedLnf() { juce::LookAndFeel::setDefaultLookAndFeel (&editorLnf); }
        ~SharedLnf() { juce::LookAndFeel::setDefaultLookAndFeel (nullptr); }

        EditorLnf editorLnf;
    };

    SharedResourcePointer<SharedLnf> lnf;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JRGranularAudioProcessorEditor)
};
