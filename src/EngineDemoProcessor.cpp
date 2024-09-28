#include "EngineDemoProcessor.h"
#include "EngineDemoEditor.h"
#include "ARARenderer.h"

EngineDemoProcessor::EngineDemoProcessor()
	: AudioProcessor(BusesProperties()
		.withInput("Mono Input", juce::AudioChannelSet::mono(), true)
		.withInput("Stereo Input", juce::AudioChannelSet::stereo(), false)
		.withOutput("Mono Output", juce::AudioChannelSet::mono(), true)
		.withOutput("Stereo Output", juce::AudioChannelSet::stereo(), false)) {}

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
	/** ARA Editor Renderer */
	else if (auto editorRenderer = this->getEditorRenderer<ARAEditorRenderer>()) {
		editorRenderer->prepareToPlay(sampleRate, samplesPerBlock,
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
	/** ARA Editor Renderer */
	else if (auto editorRenderer = this->getEditorRenderer<ARAEditorRenderer>()) {
		editorRenderer->releaseResources();
	}
}

bool EngineDemoProcessor::isBusesLayoutSupported(
	const juce::AudioProcessor::BusesLayout& layouts) const {
	if (layouts.getMainInputChannelSet().size() > 2) {
		return false;
	}
	if (layouts.getMainOutputChannelSet().size()  > 2) {
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
	/** ARA Editor Renderer */
	else if (auto editorRenderer = this->getEditorRenderer<ARAEditorRenderer>()) {
		if (editorRenderer->processBlock(buffer, Realtime::yes,
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
	return new EngineDemoEditor(*this, this->statusModel);
}

void EngineDemoProcessor::getStateInformation(juce::MemoryBlock& /*destData*/) {
}

void EngineDemoProcessor::setStateInformation(
	const void* /*data*/, int /*sizeInBytes*/) {
}

void EngineDemoProcessor::didBindToARA() noexcept {
	this->juce::AudioProcessorARAExtension::didBindToARA();
	this->updateARAStatus();
}

void EngineDemoProcessor::updateARAStatus() {
	bool hasPlaybackRenderer = (this->getPlaybackRenderer<ARAPlaybackRenderer>() != nullptr);
	bool hasEditorRenderer = (this->getEditorRenderer<ARAEditorRenderer>() != nullptr);

	this->statusModel.setARA((hasPlaybackRenderer || hasEditorRenderer)
		? EditorStatusModel::ARAStatus::Connected
		: EditorStatusModel::ARAStatus::Disconnected);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
	return new EngineDemoProcessor();
}
