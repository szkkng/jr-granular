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

#include "PluginProcessor.h"
#include "ParamIDs.h"
#include "PluginEditor.h"

static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    auto msFormat = [] (float value, int)
    {
        if (value < 100.0f)
            return juce::String { std::floor (value * 10.0f) / 10.0f, 1 } + " ms";

        return juce::String { std::floor (value) } + " ms";
    };

    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::interval, 1 },
                                                             "intv",
                                                             juce::NormalisableRange { 10.0f, 500.0f, 0.01f, 0.405f },
                                                             100.0f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             msFormat,
                                                             nullptr));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParamIDs::pitch, 1 },
        ParamIDs::pitch,
        juce::NormalisableRange { -12.0f, 12.0f, 0.1f, 1.0f },
        0.0f,
        juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [] (float value, int) {
            return juce::String { value, 1 } + " st";
        },
        nullptr));

    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::grainPos, 1 },
                                                             "pos",
                                                             juce::NormalisableRange { 10.0f, 500.0f, 1.0f, 0.405f },
                                                             100.0f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             msFormat,
                                                             nullptr));

    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::grainSize, 1 },
                                                             "size",
                                                             juce::NormalisableRange { 10.0f, 500.0f, 0.01f, 0.405f },
                                                             100.0f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             msFormat,
                                                             nullptr));

    auto percentFormat = [] (float value, int)
    {
        if (value < 10.0f)
            return juce::String { value, 2 } + " %";

        if (value < 100.0f)
            return juce::String { std::floor (value * 10.0f) / 10.0f, 1 } + " %";

        return juce::String { value } + " %";
    };

    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::mix, 1 },
                                                             ParamIDs::mix,
                                                             juce::NormalisableRange { 0.0f, 100.0f, 0.01f, 1.0f },
                                                             50.0f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             percentFormat,
                                                             nullptr));

    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::width, 1 },
                                                             ParamIDs::width,
                                                             juce::NormalisableRange { 0.0f, 100.0f, 0.01f, 1.0f },
                                                             50.0,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             percentFormat,
                                                             nullptr));

    layout.add (std::make_unique<juce::AudioParameterFloat> (
        juce::ParameterID { ParamIDs::gain, 1 },
        "vol",
        juce::NormalisableRange { -36.0f, 12.0f, 0.1f, 2.4f },
        0.0f,
        juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [] (float value, int)
        {
            if (-10.0f < value && value < 10.0f)
                return juce::String { value, 1 } + " dB";

            return juce::String { std::floor (value) } + " dB";
        },
        nullptr));

    return layout;
}

PluginProcessor::PluginProcessor()
    : AudioProcessor (BusesProperties()
                          .withInput ("Input", juce::AudioChannelSet::stereo(), true)
                          .withOutput ("Output", juce::AudioChannelSet::stereo(), true))
    , apvts (*this, &undoManager, "Parameters", createParameterLayout())
{
    for (RNBO::ParameterIndex i = 0; i < rnboObject.getNumParameters(); ++i)
    {
        RNBO::ParameterInfo info;
        rnboObject.getParameterInfo (i, &info);

        if (info.visible)
        {
            auto paramID = juce::String (rnboObject.getParameterId (i));

            // Each apvts parameter id and range must be the same as the rnbo param object's.
            // If you hit this assertion then you need to fix the incorrect id in ParamIDs.h.
            jassert (apvts.getParameter (paramID) != nullptr);

            // If you hit these assertions then you need to fix the incorrect apvts
            // parameter range in createParameterLayout().
            jassert (info.min == apvts.getParameterRange (paramID).start);
            jassert (info.max == apvts.getParameterRange (paramID).end);

            apvtsParamIdToRnboParamIndex[paramID] = i;

            apvts.addParameterListener (paramID, this);
            rnboObject.setParameterValue (i, apvts.getRawParameterValue (paramID)->load());
        }
    }
}

PluginProcessor::~PluginProcessor() {}

const juce::String PluginProcessor::getName() const { return JucePlugin_Name; }

bool PluginProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool PluginProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool PluginProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double PluginProcessor::getTailLengthSeconds() const { return 0.0; }

int PluginProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
        // so this should be at least 1, even if you're not really implementing programs.
}

int PluginProcessor::getCurrentProgram() { return 0; }

void PluginProcessor::setCurrentProgram (int index) { juce::ignoreUnused (index); }

const juce::String PluginProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void PluginProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

void PluginProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    rnboObject.prepareToProcess (sampleRate, static_cast<size_t> (samplesPerBlock));
}

void PluginProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool PluginProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainInputChannelSet() == juce::AudioChannelSet::disabled()
        || layouts.getMainOutputChannelSet() == juce::AudioChannelSet::disabled())
        return false;

    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    return layouts.getMainInputChannelSet() == layouts.getMainOutputChannelSet();
}

void PluginProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);
    rnboObject.process (buffer.getArrayOfWritePointers(),
                        static_cast<RNBO::Index> (buffer.getNumChannels()),
                        buffer.getArrayOfWritePointers(),
                        static_cast<RNBO::Index> (buffer.getNumChannels()),
                        static_cast<RNBO::Index> (buffer.getNumSamples()));
}

bool PluginProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PluginProcessor::createEditor() { return new PluginEditor (*this, apvts, undoManager); }

void PluginProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream mos (destData, true);
    apvts.state.writeToStream (mos);
}

void PluginProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (const auto tree = juce::ValueTree::readFromData (data, static_cast<size_t> (sizeInBytes)); tree.isValid())
        apvts.replaceState (tree);
}

void PluginProcessor::parameterChanged (const juce::String& parameterID, float newValue)
{
    rnboObject.setParameterValue (apvtsParamIdToRnboParamIndex[parameterID], newValue);
}

// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new PluginProcessor(); }
