#include "EngineDemoProcessor.h"
#include "EngineDemoEditor.h"

class EngineDemoContext final : public DMDA::MidiFileContext {
public:
	explicit EngineDemoContext(EngineDemoProcessor* parent)
		: parent(parent) {};

private:
	void handShakedStateChanged() override {
		if (auto editor =
			dynamic_cast<EngineDemoEditor*>(parent->getActiveEditor())) {
			if (this->isHandShaked()) {
				editor->setHandShaked(
					EngineDemoEditor::HandShakeStatus::Connected);
			}
			else {
				editor->setHandShaked(
					EngineDemoEditor::HandShakeStatus::Disconnected);
			}
		}
	};

private:
	EngineDemoProcessor* parent = nullptr;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EngineDemoContext)
};

EngineDemoProcessor::EngineDemoProcessor()
	: PluginProcessor(BusesProperties()
		.withOutput("Output", juce::AudioChannelSet::mono(), true)) {
	/** DMDA Context */
	this->initContext();

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
	if (!this->renderer->isRendered()) {
		if (auto editor = dynamic_cast<EngineDemoEditor*>(this->getActiveEditor())) {
			editor->setRendered(
				EngineDemoEditor::RenderStatus::Unrendered);
		}
	}
}

void EngineDemoProcessor::releaseResources() {
	this->renderer->releaseData();
	if (auto editor = dynamic_cast<EngineDemoEditor*>(this->getActiveEditor())) {
		editor->setRendered(
			EngineDemoEditor::RenderStatus::Unrendered);
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
	/** Check For Rendered */
	if (!this->renderer->isRendered()) {
		/** Get DMDA Context */
		if (auto DMDAContext =
			dynamic_cast<DMDA::MidiFileContext*>(this->getContext())) {
			/** Set Editor State */
			juce::MessageManager::callAsync(
				[editor = EngineDemoEditor::SafePointer(
					dynamic_cast<EngineDemoEditor*>(this->getActiveEditor()))] {
						if (editor) {
							editor->setRendered(
								EngineDemoEditor::RenderStatus::Rendering);
						}
				}
			);

			/** Get Context Data */
			juce::ScopedReadLock DMDALocker(DMDAContext->getLock());
			if (auto data = DMDAContext->getData()) {
				/** Context Info */
				{
					int trackNum = data->getNumTracks();
					double totalLength = data->getLastTimestamp();
					int totalEvents = 0;
					for (int i = 0; i < trackNum; i++) {
						auto track = data->getTrack(i);
						totalEvents += track->getNumEvents();
					}

					juce::MessageManager::callAsync(
						[trackNum, totalLength, totalEvents,
						editor = EngineDemoEditor::SafePointer(
							dynamic_cast<EngineDemoEditor*>(this->getActiveEditor()))] {
								if (editor) {
									editor->setMidiInfo(
										EngineDemoEditor::MidiInfo{
											trackNum, totalLength, totalEvents });
								}
						}
					);
				}

				/** Render */
				this->renderer->render(*data);

				/** Check Rendered */
				if (this->renderer->isRendered()) {
					juce::MessageManager::callAsync(
						[editor = EngineDemoEditor::SafePointer(
							dynamic_cast<EngineDemoEditor*>(this->getActiveEditor()))] {
								if (editor) {
									editor->setRendered(
										EngineDemoEditor::RenderStatus::Rendered);
								}
						}
					);
				}
				else {
					juce::MessageManager::callAsync(
						[editor = EngineDemoEditor::SafePointer(
							dynamic_cast<EngineDemoEditor*>(this->getActiveEditor()))] {
								if (editor) {
									editor->setRendered(
										EngineDemoEditor::RenderStatus::Unrendered);
								}
						}
					);
				}
			}
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
	return new EngineDemoContext(const_cast<EngineDemoProcessor*>(this));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
	return new EngineDemoProcessor();
}
