#pragma once

#include <JuceHeader.h>

class RealTimeRenderer final {
public:
	RealTimeRenderer();

	void prepare(double sampleRate, int blockSize);
	void release();
	void processBlock(
		juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);

private:
	juce::CriticalSection lock;
	std::unique_ptr<juce::Synthesiser> synth;
	juce::SynthesiserSound::Ptr sound;

	void createSound();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RealTimeRenderer)
};
