#include "EngineDemoProcessor.h"
#include "EngineDemoEditor.h"
#include "ARARenderer.h"

EngineDemoProcessor::EngineDemoProcessor()
	: AudioProcessor(BusesProperties()
		.withOutput("Output", juce::AudioChannelSet::mono(), true)) {}

EngineDemoProcessor::~EngineDemoProcessor() {}

const juce::String EngineDemoProcessor::getName() const {
	return JucePlugin_Name;
}

bool EngineDemoProcessor::acceptsMidi() const {
	return true;
}

bool EngineDemoProcessor::producesMidi() const {
	return false;
}

bool EngineDemoProcessor::isMidiEffect() const {
	return false;
}

double EngineDemoProcessor::getTailLengthSeconds() const {
	return 0.0;
}

int EngineDemoProcessor::getNumPrograms() {
	return 1;
}

int EngineDemoProcessor::getCurrentProgram() {
	return 0;
}

void EngineDemoProcessor::setCurrentProgram(int /*index*/) {
}

const juce::String EngineDemoProcessor::getProgramName(int /*index*/) {
	return {};
}

void EngineDemoProcessor::changeProgramName(
	int /*index*/, const juce::String& /*newName*/) {
}

void EngineDemoProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
	/** ARA Playback Renderer */
	if (auto playbackRenderer = this->getPlaybackRenderer<ARAPlaybackRenderer>()) {
		playbackRenderer->prepareToPlay(sampleRate, samplesPerBlock,
			this->getChannelCountOfBus(true, 0), ProcessingPrecision::singlePrecision,
			juce::ARARenderer::AlwaysNonRealtime::no);
	}

	/** TODO Prepare Normal Synth */
}

void EngineDemoProcessor::releaseResources() {
	/** ARA Playback Renderer */
	if (auto playbackRenderer = this->getPlaybackRenderer<ARAPlaybackRenderer>()) {
		playbackRenderer->releaseResources();
	}
}

bool EngineDemoProcessor::isBusesLayoutSupported(
	const juce::AudioProcessor::BusesLayout& layouts) const {
	if (layouts.getMainInputChannelSet().size() != 0) {
		return false;
	}
	if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()) {
		return false;
	}
	return true;
}

void EngineDemoProcessor::processBlock(
	juce::AudioBuffer<float>& buffer, juce::MidiBuffer& /*midiMessages*/) {
	/** ARA Playback Renderer */
	if (auto playbackRenderer = this->getPlaybackRenderer<ARAPlaybackRenderer>()) {
		if (playbackRenderer->processBlock(buffer, Realtime::yes,
			this->getPlayHead()->getPosition().orFallback(juce::AudioPlayHead::PositionInfo{}))) {
			return;
		}
	}

	/** TODO Normal Synth */
}

bool EngineDemoProcessor::hasEditor() const {
	return true;
}

juce::AudioProcessorEditor* EngineDemoProcessor::createEditor() {
	return new EngineDemoEditor(*this);
}

void EngineDemoProcessor::getStateInformation(juce::MemoryBlock& /*destData*/) {
}

void EngineDemoProcessor::setStateInformation(
	const void* /*data*/, int /*sizeInBytes*/) {
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
	return new EngineDemoProcessor();
}
