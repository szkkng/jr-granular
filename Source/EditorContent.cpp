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
   along with Simple Reverb. If not, see <http://www.gnu.org/licenses/>.

  ==============================================================================
*/

#include "EditorContent.h"
#include "ParamIDs.h"

EditorContent::EditorContent (juce::AudioProcessorValueTreeState& apvts,
                              juce::UndoManager& um)
    : intvDial  (*apvts.getParameter (ParamIDs::interval),  &um),
      pitchDial (*apvts.getParameter (ParamIDs::pitch),     &um),
      sizeDial  (*apvts.getParameter (ParamIDs::grainSize), &um),
      posDial   (*apvts.getParameter (ParamIDs::grainPos),  &um),
      widthDial (*apvts.getParameter (ParamIDs::width),     &um),
      mixDial   (*apvts.getParameter (ParamIDs::mix),       &um),
      gainDial  (*apvts.getParameter (ParamIDs::gain),      &um)
{
    setWantsKeyboardFocus (true);

    intvDial.setLabelText ("intv");
    mixDial.setLabelText  ("mix");
    posDial.setLabelText  ("pos");
    sizeDial.setLabelText ("size");
    gainDial.setLabelText ("vol");

    // Set interval of values changed by arrow keys or shift + arrow keys.
    intvDial.setInterval (5.0f);
    intvDial.setFineInterval (1.0f);
    sizeDial.setInterval (5.0f);
    sizeDial.setFineInterval (1.0f);
    posDial.setInterval (5.0f);
    posDial.setFineInterval (1.0f);
    pitchDial.setInterval (1.0f);
    pitchDial.setFineInterval (0.1f);
    gainDial.setInterval (1.0f);
    gainDial.setFineInterval (0.1f);

    addAndMakeVisible (sizeDial);
    addAndMakeVisible (posDial);
    addAndMakeVisible (intvDial);
    addAndMakeVisible (widthDial);
    addAndMakeVisible (pitchDial);
    addAndMakeVisible (mixDial);
    addAndMakeVisible (gainDial);
}

void EditorContent::resized()
{
    intvDial.setBounds  (30,  30,  80, 95);
    pitchDial.setBounds (130, 30,  80, 95);
    sizeDial.setBounds  (230, 30,  80, 95);
    posDial.setBounds   (330, 30,  80, 95);
    widthDial.setBounds (80,  155, 80, 95);
    mixDial.setBounds   (180, 155, 80, 95);
    gainDial.setBounds  (280, 155, 80, 95);
}
