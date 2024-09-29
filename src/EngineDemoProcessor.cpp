#include "EngineDemoProcessor.h"
#include "EngineDemoEditor.h"
#include "ARARenderer.h"
#include "ARARenderThread.h"
#include "ARADocument.h"

class RenderStateListener final : public juce::ChangeListener {
public:
	RenderStateListener() = delete;
	RenderStateListener(EditorStatusModel& model)
		: model(model) {};

private:
	void changeListenerCallback(juce::ChangeBroadcaster* source) override {
		if (auto renderer = dynamic_cast<ARARenderThread*>(source)) {
			if (renderer->getRunningFlag()) {
				this->model.setRendered(EditorStatusModel::RenderStatus::Rendering);
			}
			else {
				this->model.setRendered(
					renderer->getRendered()
					? EditorStatusModel::RenderStatus::Rendered
					: EditorStatusModel::RenderStatus::Unrendered);
			}
		}
	}

private:
	EditorStatusModel& model;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RenderStateListener)
};

class ContextInfoListener final : public juce::ARADocumentListener {
public:
	ContextInfoListener() = delete;
	ContextInfoListener(EngineDemoProcessor& processor)
		: processor(processor) {};

private:
	void didEndEditing(juce::ARADocument* document) override {
		this->updateDocument(document);
	}
	void willDestroyDocument(juce::ARADocument* document) override {
		this->updateDocument(document);
	}

private:
	EngineDemoProcessor& processor;

	void updateDocument(juce::ARADocument* /*document*/) {
		this->processor.updateContextInfo();
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ContextInfoListener)
};

EngineDemoProcessor::EngineDemoProcessor()
	: AudioProcessor(BusesProperties()
		.withInput("Mono Input", juce::AudioChannelSet::mono(), true)
		.withInput("Stereo Input", juce::AudioChannelSet::stereo(), false)
		.withOutput("Mono Output", juce::AudioChannelSet::mono(), true)
		.withOutput("Stereo Output", juce::AudioChannelSet::stereo(), false)) {
	this->renderStateListener = std::make_unique<RenderStateListener>(this->statusModel);
	this->contextInfoListener = std::make_unique<ContextInfoListener>(*this);

	/** Update Info */
	this->updateContextInfo();
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

	/** Prepare Real Time Synth */
	this->renderer.prepare(sampleRate, samplesPerBlock);

	/** Update Info */
	this->updateContextInfo();
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

	/** Release Real Time Synth */
	this->renderer.release();

	/** Update Info */
	this->updateContextInfo();
}

bool EngineDemoProcessor::isBusesLayoutSupported(
	const juce::AudioProcessor::BusesLayout& layouts) const {
	if (layouts.getMainInputChannelSet().size() > 2) {
		return false;
	}
	if (layouts.getMainOutputChannelSet().size() > 2) {
		return false;
	}
	return true;
}

void EngineDemoProcessor::processBlock(
	juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
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

	/** Real Time Synth */
	this->renderer.processBlock(buffer, midiMessages);
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

void EngineDemoProcessor::numChannelsChanged() {
	/** Update Info */
	this->updateContextInfo();
}

void EngineDemoProcessor::didBindToARA() noexcept {
	this->juce::AudioProcessorARAExtension::didBindToARA();
	this->updateARAStatus();

	if (auto document = dynamic_cast<ARADocument*>(this->getDocumentController()->getDocument())) {
		document->addRenderStateListener(this->renderStateListener.get());
		document->addListener(this->contextInfoListener.get());
	}
}

void EngineDemoProcessor::updateARAStatus() {
	bool hasPlaybackRenderer = (this->getPlaybackRenderer<ARAPlaybackRenderer>() != nullptr);
	bool hasEditorRenderer = (this->getEditorRenderer<ARAEditorRenderer>() != nullptr);

	this->statusModel.setARA((hasPlaybackRenderer || hasEditorRenderer)
		? EditorStatusModel::ARAStatus::Connected
		: EditorStatusModel::ARAStatus::Disconnected);
}

void EngineDemoProcessor::updateContextInfo() {
	EditorStatusModel::ContextInfo info{};

	if (auto dc = this->getDocumentController()) {
		if (auto document = dynamic_cast<ARADocument*>(dc->getDocument())) {
			auto& context = document->getContext();

			/** Regions */
			info.regions = context.getRegionList();

			/** Context */
			auto [notes, pitchs, length] = context.getContextData();
			info.noteNum = notes.size();
			info.pitchNum = pitchs.size();
			info.contextLength = length;
		}
	}

	/** Engine State */
	info.sampleRate = this->getSampleRate();
	info.blockSize = this->getBlockSize();
	info.channelNumInput = this->getChannelCountOfBus(true, 0);
	info.channelNumOutput = this->getChannelCountOfBus(false, 0);

	/** Set Info */
	this->statusModel.setContextInfo(info);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
	return new EngineDemoProcessor();
}
