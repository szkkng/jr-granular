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

#include <juce_gui_basics/juce_gui_basics.h>

class EditorLnf final : public juce::LookAndFeel_V4
{
public:
    EditorLnf();

    void drawCornerResizer (juce::Graphics& g, int w, int h, bool isMouseOver, bool isMouseDragging) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditorLnf)
};
