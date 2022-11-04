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

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ParamIDs.h"

//==============================================================================
JRGranularAudioProcessor::JRGranularAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
       apvts (*this, &undoManager, "Parameters", createParameterLayout())
{
    inputBuffers  = new RNBO::SampleValue* [coreObj.getNumInputChannels()];
    outputBuffers = new RNBO::SampleValue* [coreObj.getNumOutputChannels()];

    for (RNBO::Index i = 0; i < coreObj.getNumInputChannels(); i++)
        inputBuffers[i] = nullptr;

    for (RNBO::Index i = 0; i < coreObj.getNumOutputChannels(); i++)
        outputBuffers[i] = nullptr;

    for (RNBO::Index i = 0; i < coreObj.getNumParameters(); ++i)
    {
        RNBO::ParameterInfo info;
		coreObj.getParameterInfo(i, &info);

        if (info.visible)
        {
            auto name = juce::String (coreObj.getParameterName (i));
            jassert (apvts.getParameter (name) != nullptr);
            apvts.addParameterListener (name, this);
        }
    }
}

JRGranularAudioProcessor::~JRGranularAudioProcessor()
{
}

//==============================================================================
const juce::String JRGranularAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool JRGranularAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool JRGranularAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool JRGranularAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double JRGranularAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int JRGranularAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int JRGranularAudioProcessor::getCurrentProgram()
{
    return 0;
}

void JRGranularAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String JRGranularAudioProcessor::getProgramName (int index)
{
    return {};
}

void JRGranularAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void JRGranularAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    assureBufferSize (samplesPerBlock);
    coreObj.prepareToProcess (sampleRate, samplesPerBlock);
}

void JRGranularAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool JRGranularAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void JRGranularAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto bufferSize = buffer.getNumSamples();
    auto rnboNumInputChannels  = coreObj.getNumInputChannels();
    auto rnboNumOutputChannels = coreObj.getNumInputChannels();

    // Fill input buffers.
    for (int i = 0; i < rnboNumInputChannels; i++)
    {
        if (i < getTotalNumInputChannels())
        {
            for (int j = 0; j < bufferSize; j++)
                inputBuffers[i][j] = buffer.getReadPointer (i)[j];
        }
        else
        {
            memset (inputBuffers[i], 0, bufferSize * sizeof (double));
        }
    }

    coreObj.process (inputBuffers,
                     rnboNumInputChannels,
                     outputBuffers,
                     rnboNumOutputChannels,
                     bufferSize);

    // Fill output buffers.
    for (int i = 0; i < getTotalNumOutputChannels(); i++)
    {
        if (i < rnboNumOutputChannels)
        {
            for (int j = 0; j < bufferSize; j++)
                buffer.getWritePointer (i)[j] = outputBuffers[i][j];
        }
        else
        {
            buffer.clear (i, 0, bufferSize);
        }
    }
}

//==============================================================================
bool JRGranularAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* JRGranularAudioProcessor::createEditor()
{
    return new JRGranularAudioProcessorEditor (*this, apvts, undoManager);
    /* return new juce::GenericAudioProcessorEditor (*this); */
}

//==============================================================================
void JRGranularAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream mos (destData, true);
    apvts.state.writeToStream (mos);
}

void JRGranularAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto tree = juce::ValueTree::readFromData (data, sizeInBytes);

    if (tree.isValid())
        apvts.replaceState (tree);
}

void JRGranularAudioProcessor::parameterChanged (const juce::String& parameterID, float newValue)
{
    coreObj.setParameterValue (coreObj.getParameterIndexForID (parameterID.toRawUTF8()),
                               newValue);
}

void JRGranularAudioProcessor::assureBufferSize (int bufferSize)
{
    if (bufferSize > currentBufferSize)
    {
        for (int i = 0; i < coreObj.getNumInputChannels(); i++)
        {
            if (inputBuffers[i]) delete inputBuffers[i];

            inputBuffers[i] = new RNBO::SampleValue[bufferSize];
        }

        for (int i = 0; i < coreObj.getNumOutputChannels(); i++)
        {
            if (outputBuffers[i]) delete outputBuffers[i];

            outputBuffers[i] = new RNBO::SampleValue[bufferSize];
        }

        currentBufferSize = bufferSize;
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JRGranularAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout JRGranularAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    auto msFormat = [](float value, int)
    {
        if (value < 100.0f)
            return juce::String (value, 1) + " ms";
        else
            return juce::String (std::roundf (value)) + " ms";
    };

    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::interval, 1 },
                                                             ParamIDs::interval,
                                                             juce::NormalisableRange<float> (10.0f, 500.0f, 0.01f, 0.405f),
                                                             100.0f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             msFormat,
                                                             nullptr));

    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::pitch, 1 }, 
                                                             ParamIDs::pitch,
                                                             juce::NormalisableRange<float> (-12.0f, 12.0f, 0.1f, 1.0f),
                                                             0.0f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             [](float value, int) {
                                                                return juce::String (value, 1) + " st"; },
                                                             nullptr));

    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::grainPos, 1 }, 
                                                             ParamIDs::grainPos,
                                                             juce::NormalisableRange<float> (10.0f, 500.0f, 1.0f, 0.405f),
                                                             100.0f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             msFormat,
                                                             nullptr));

    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::grainSize, 1 }, 
                                                             ParamIDs::grainSize,
                                                             juce::NormalisableRange<float> (10.0f, 500.0f, 0.01f, 0.405f),
                                                             100.0f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             msFormat,
                                                             nullptr));

    auto convertToPercent = [](float value, int)
    {
         value *= 100;
         if (value < 10.0f)
             return juce::String (value, 2) + " %";
         else if (value < 100.0f)
             return juce::String (value, 1) + " %";
         else
             return juce::String (value, 0) + " %"; 
    };

    auto convertFromPercent = [](const juce::String& string)
    {
        return string.getFloatValue() * 0.01f;
    };

    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::mix, 1 }, 
                                                             ParamIDs::mix,
                                                             juce::NormalisableRange<float> (0.0f, 1.0f, 0.0001f, 1.0f),
                                                             0.5f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             convertToPercent,
                                                             convertFromPercent));

    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::width, 1 }, 
                                                             ParamIDs::width,
                                                             juce::NormalisableRange<float> (0.0f, 1.0f, 0.0001f, 1.0f),
                                                             0.5f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             convertToPercent,
                                                             convertFromPercent));

    layout.add (std::make_unique<juce::AudioParameterFloat> (juce::ParameterID { ParamIDs::gain, 1 }, 
                                                             ParamIDs::gain,
                                                             juce::NormalisableRange<float> (-36.0f, 12.0f, 0.1f, 2.4f),
                                                             0.0f,
                                                             juce::String(),
                                                             juce::AudioProcessorParameter::genericParameter,
                                                             [](float value, int) {
                                                             if (value <= -10.0f)
                                                                return juce::String (std::floorf (value), 0) + " dB";
                                                             else
                                                                 return juce::String (value, 1) + " dB";},
                                                             nullptr));

    return layout;
}

