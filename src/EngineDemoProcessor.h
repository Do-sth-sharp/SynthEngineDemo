#pragma once

#include <JuceHeader.h>
#include "EditorStatusModel.h"

class EngineDemoProcessor final
	: public juce::AudioProcessor,
	public juce::AudioProcessorARAExtension {
public:
	EngineDemoProcessor();
	~EngineDemoProcessor();

	const juce::String getName() const override;
	bool acceptsMidi() const override;
	bool producesMidi() const override;
	bool isMidiEffect() const override;
	double getTailLengthSeconds() const override;

	int getNumPrograms() override;
	int getCurrentProgram() override;
	void setCurrentProgram(int index) override;
	const juce::String getProgramName(int index) override;
	void changeProgramName(int index, const juce::String& newName) override;

	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void releaseResources() override;
	bool isBusesLayoutSupported(
		const juce::AudioProcessor::BusesLayout& layouts) const override;
	void processBlock(
		juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;

	bool hasEditor() const override;
	juce::AudioProcessorEditor* createEditor() override;

	void getStateInformation(juce::MemoryBlock& destData) override;
	void setStateInformation(const void* data, int sizeInBytes) override;

	void numChannelsChanged() override;

private:
	EditorStatusModel statusModel;
	std::unique_ptr<juce::ChangeListener> renderStateListener = nullptr;
	std::unique_ptr<juce::ARADocumentListener> contextInfoListener = nullptr;

	void didBindToARA() noexcept override;

	friend class ContextInfoListener;
	void updateARAStatus();
	void updateContextInfo();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EngineDemoProcessor)
};
