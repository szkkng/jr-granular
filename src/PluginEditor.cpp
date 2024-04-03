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

#include "PluginEditor.h"
#include "PluginProcessor.h"
#include "ui/MyColours.h"

PluginEditor::PluginEditor (PluginProcessor& p, juce::AudioProcessorValueTreeState& apvts, juce::UndoManager& um)
    : AudioProcessorEditor (&p)
    , undoManager (um)
    , editorContent (apvts, um)
{
    constexpr auto ratio = static_cast<double> (defaultWidth) / defaultHeight;
    setResizable (false, true);
    getConstrainer()->setFixedAspectRatio (ratio);
    getConstrainer()->setSizeLimits (defaultWidth / 2, defaultHeight / 2, defaultWidth * 2, defaultHeight * 2);
    setSize (defaultWidth, defaultHeight);
    editorContent.setSize (defaultWidth, defaultHeight);

    addAndMakeVisible (editorContent);
}

void PluginEditor::paint (juce::Graphics& g) { g.fillAll (MyColours::black); }

void PluginEditor::resized()
{
    const auto factor = static_cast<float> (getWidth()) / defaultWidth;
    editorContent.setTransform (juce::AffineTransform::scale (factor));
}

bool PluginEditor::keyPressed (const juce::KeyPress& key)
{
    if (const auto cmdZ = juce::KeyPress { 'z', juce::ModifierKeys::commandModifier, 0 };
        key == cmdZ && undoManager.canUndo())
    {
        undoManager.undo();
        return true;
    }

    if (const auto cmdShiftZ =
            juce::KeyPress { 'z', juce::ModifierKeys::commandModifier | juce::ModifierKeys::shiftModifier, 0 };
        key == cmdShiftZ && undoManager.canRedo())
    {
        undoManager.redo();
        return true;
    }

    return false;
}
