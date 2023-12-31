#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "processing/DAWTransportData.h"
#include "processing/MyMPEInstrumentListener.h"


//==============================================================================
class AudioPluginAudioProcessor  : public juce::AudioProcessor, public juce::ChangeBroadcaster
{
public:
    //==============================================================================
    AudioPluginAudioProcessor();
    ~AudioPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    DAWTransportData getDAWTransportData() ;

    std::vector<MPENoteEvent> getNoteEventVector();

    void toggleRecording();
    bool isRecording() const;

    int getWindowWidth() const;
    void setWindowWidth(int width);
    int getWindowHeight() const;
    void setWindowHeight(int height);

private:
    juce::AudioProcessorValueTreeState parameters;
    DAWTransportData dawTransportData;
    std::mutex dawTransportDataMutex;
    juce::MPEInstrument mpeInstrument;
    MyMPEInstrumentListener mpeInstrumentListener{&dawTransportData};

    std::vector<MPENoteEvent> noteEventVector;
    std::mutex noteEventVectorMutex;
    size_t loopsCounter = 0;
    int positionChangeCounter = 0;
    double prevPpqPosition = 0;

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessor)
};
