#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <juce_audio_basics/juce_audio_basics.h>
//==============================================================================
AudioPluginAudioProcessor::AudioPluginAudioProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), parameters(*this, nullptr, "PARAMETERS", createParameterLayout() )
{
    mpeInstrument.enableLegacyMode(24);
    mpeInstrument.addListener(&mpeInstrumentListener);
}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor()
{
}

//==============================================================================
const juce::String AudioPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AudioPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AudioPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioPluginAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String AudioPluginAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void AudioPluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void AudioPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{

    juce::ignoreUnused (sampleRate, samplesPerBlock);
}

void AudioPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void AudioPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();


    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());


    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        juce::ignoreUnused (channelData);
    }

    for (const auto metadata : midiMessages)
    {
        auto message = metadata.getMessage();

        mpeInstrument.processNextMidiEvent(message);
    }
    loopsCounter++;
    if (loopsCounter > 5)
    {
        std::unique_lock<std::mutex> lock(noteEventVectorMutex);
        loopsCounter = 0;
        noteEventVector = mpeInstrumentListener.createNoteEventVector();
    }



    positionChangeCounter++;
    double ppqStart = 0;
    double ppqEnd = 0;
    double ppqPosition = 0;

    if (juce::JUCEApplicationBase::isStandaloneApp() )
        return;
    // Get the current position information from the host
    if (auto* playHead = getPlayHead())
    {

        juce::Optional<juce::AudioPlayHead::PositionInfo> positionInfo = playHead->getPosition();


        if (isRecording()  && !positionInfo->getIsPlaying() && playHead->canControlTransport())
            playHead->transportPlay(true);


        if (positionInfo->getIsLooping()) {
            ppqStart = positionInfo->getLoopPoints()->ppqStart;
            ppqEnd = positionInfo->getLoopPoints()->ppqEnd;
        }
        ppqPosition = *positionInfo->getPpqPosition();
        if (!positionInfo->getIsPlaying())
            ppqPosition = 0;

        dawTransportData.setPpqPositionNotSynced(ppqPosition);

        if (positionChangeCounter > 5 &&
            dawTransportData.changed(ppqPosition, ppqStart, ppqEnd))
        {
            {
                std::lock_guard<std::mutex> lock(dawTransportDataMutex);
                dawTransportData.set(ppqPosition, ppqStart, ppqEnd);
            }
            sendChangeMessage();
            positionChangeCounter = 0;
        }


        if (dawTransportData.changed(positionInfo->getTimeSignature()->numerator, positionInfo->getTimeSignature()->denominator))
        {
            {
                std::lock_guard<std::mutex> lock(dawTransportDataMutex);
                dawTransportData.set(positionInfo->getTimeSignature()->numerator,
                                     positionInfo->getTimeSignature()->denominator);
            }
            sendChangeMessage();
        }

        if (dawTransportData.bpmChanged(*positionInfo->getBpm()))
        {
            {
                std::lock_guard<std::mutex> lock(dawTransportDataMutex);
                dawTransportData.setBpm(*positionInfo->getBpm());
            }
            sendChangeMessage();
        }


        double maxPpq = (double) dawTransportData.getNumerator() * 4 / dawTransportData.getDenominator();
        const bool cursorReachedLoopEnd = positionInfo->getIsPlaying() && positionInfo->getIsLooping() && ceil(ppqPosition*64.0)/64.0 >= ppqEnd;
        const bool cursorReachedTheEndOfTheBar = positionInfo->getIsPlaying() && !positionInfo->getIsLooping() && ceil(ppqPosition*64.0)/64.0 >= maxPpq;
        if (cursorReachedLoopEnd || cursorReachedTheEndOfTheBar)
            mpeInstrumentListener.resetPlayedNotes();
        if (isRecording() && (cursorReachedLoopEnd || cursorReachedTheEndOfTheBar) && !mpeInstrumentListener.isJustStartedRecording())
            toggleRecording();
        prevPpqPosition = ppqPosition;
        mpeInstrumentListener.updateNotes(ppqPosition);
    }
}

//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AudioPluginAudioProcessor::createEditor()
{
    return new AudioPluginAudioProcessorEditor (*this);
}

//==============================================================================
void AudioPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void AudioPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState != nullptr)
        if (xmlState->hasTagName (parameters.state.getType()))
            parameters.replaceState (juce::ValueTree::fromXml (*xmlState));
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPluginAudioProcessor();
}


DAWTransportData AudioPluginAudioProcessor::getDAWTransportData()
{
    std::lock_guard<std::mutex> lock{dawTransportDataMutex};
    return dawTransportData;
}

std::vector<MPENoteEvent> AudioPluginAudioProcessor::getNoteEventVector() {
    std::lock_guard<std::mutex> lock{noteEventVectorMutex};
    return noteEventVector;
}

void AudioPluginAudioProcessor::toggleRecording() {
    mpeInstrumentListener.toggleRecording();
}

bool AudioPluginAudioProcessor::isRecording() const {
    return mpeInstrumentListener.isRecording();
}

juce::AudioProcessorValueTreeState::ParameterLayout AudioPluginAudioProcessor::createParameterLayout()
{
    return
            {
                    std::make_unique<juce::AudioParameterFloat>("windowWidth", "Window Width", 0.0f, 3440.0f, 1200.0f),
            };
}

int AudioPluginAudioProcessor::getWindowWidth() const
{
    return static_cast<int>(std::round(*parameters.getRawParameterValue("windowWidth")));
}

void AudioPluginAudioProcessor::setWindowWidth(int width)
{
    parameters.getParameter("windowWidth")->setValueNotifyingHost(static_cast<float>(width) / 3440.0f);
}

