#include "EngineDemoProcessor.h"
#include "EngineDemoEditor.h"

EngineDemoProcessor::EngineDemoProcessor()
	: PluginProcessor(BusesProperties()
		.withOutput("Output", juce::AudioChannelSet::mono(), true)) {
	/** Renderer */
	this->renderer = std::make_unique<EngineRenderer>();
}

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

void EngineDemoProcessor::prepareToPlay(double sampleRate, int /*samplesPerBlock*/) {
	this->renderer->prepare(sampleRate);
}

void EngineDemoProcessor::releaseResources() {
	this->renderer->releaseData();
}

bool EngineDemoProcessor::isBusesLayoutSupported(
	const juce::AudioProcessor::BusesLayout& layouts) const {
	if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()) {
		return false;
	}
	return true;
}

void EngineDemoProcessor::processBlock(
	juce::AudioBuffer<float>& buffer, juce::MidiBuffer& /*midiMessages*/) {
	/** Check For Rendered */
	if (!this->renderer->isRendered()) {
		/** Get DMDA Context */
		if (auto DMDAContext =
			dynamic_cast<DMDA::MidiFileContext*>(this->getContext())) {
			juce::ScopedReadLock DMDALocker(DMDAContext->getLock());
			this->renderer->render(DMDAContext->getData());
		}
	}

	/** Get Audio Data */
	if (auto playHead = this->getPlayHead()) {
		auto position = playHead->getPosition();
		int64_t timeInSamples = position->getTimeInSamples().orFallback(0);
		this->renderer->getAudio(buffer, timeInSamples);
	}
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

DMDA::Context* EngineDemoProcessor::createContext() const {
	return new DMDA::MidiFileContext;
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
	return new EngineDemoProcessor();
}
