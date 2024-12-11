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

#include "EditorContent.h"
#include "../ParamIDs.h"

EditorContent::EditorContent (juce::AudioProcessorValueTreeState& apvts, juce::UndoManager& um)
    : intvDial (*apvts.getParameter (param_ids::interval), &um)
    , pitchDial (*apvts.getParameter (param_ids::pitch), &um)
    , sizeDial (*apvts.getParameter (param_ids::grainSize), &um)
    , posDial (*apvts.getParameter (param_ids::grainPos), &um)
    , widthDial (*apvts.getParameter (param_ids::width), &um)
    , mixDial (*apvts.getParameter (param_ids::mix), &um)
    , gainDial (*apvts.getParameter (param_ids::gain), &um)
{
    setWantsKeyboardFocus (true);
    setFocusContainerType (FocusContainerType::keyboardFocusContainer);

    // Set interval of values changed by arrow keys or shift + arrow keys.
    intvDial.setInterval (5.0f);
    intvDial.setFineInterval (1.0f);
    pitchDial.setInterval (1.0f);
    pitchDial.setFineInterval (0.1f);
    sizeDial.setInterval (5.0f);
    sizeDial.setFineInterval (1.0f);
    posDial.setInterval (5.0f);
    posDial.setFineInterval (1.0f);
    gainDial.setInterval (1.0f);
    gainDial.setFineInterval (0.1f);

    addAndMakeVisible (intvDial);
    addAndMakeVisible (pitchDial);
    addAndMakeVisible (sizeDial);
    addAndMakeVisible (posDial);
    addAndMakeVisible (widthDial);
    addAndMakeVisible (mixDial);
    addAndMakeVisible (gainDial);
}

void EditorContent::resized()
{
    const auto topDialBounds = juce::Rectangle { 0, 30, 80, 95 };
    intvDial.setBounds (topDialBounds.withX (30));
    pitchDial.setBounds (topDialBounds.withX (130));
    sizeDial.setBounds (topDialBounds.withX (230));
    posDial.setBounds (topDialBounds.withX (330));

    const auto bottomDialBounds = topDialBounds.withY (155);
    widthDial.setBounds (bottomDialBounds.withX (80));
    mixDial.setBounds (bottomDialBounds.withX (180));
    gainDial.setBounds (bottomDialBounds.withX (280));
}

bool EditorContent::keyPressed (const juce::KeyPress& k)
{
    if (k.isKeyCode (juce::KeyPress::tabKey) && hasKeyboardFocus (false))
    {
        intvDial.grabKeyboardFocus();
        return true;
    }

    return false;
}
