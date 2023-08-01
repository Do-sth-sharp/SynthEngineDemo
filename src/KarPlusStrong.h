#pragma once

#include <JuceHeader.h>

class KarPlusStrong {
public:
	KarPlusStrong() = delete;
	KarPlusStrong(int seed = 1);

	bool synth(juce::AudioBuffer<float>& buffer, double sampleRate,
		int startSample, int length,
		juce::Array<double> freq, int frameLength, int frameDeviation) const;

private:
	juce::Array<float> noiseSource;

	static void prepareNextClip(
		const float* source0, float* source1, int size);
	static void copyClip(
		const float* src, float* dst,
		float startDecay, float endDecay,
		int size);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KarPlusStrong)
};
