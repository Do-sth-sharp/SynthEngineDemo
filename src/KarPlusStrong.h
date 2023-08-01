#pragma once

#include <JuceHeader.h>

class KarPlusStrong {
public:
	KarPlusStrong() = delete;
	KarPlusStrong(int seed = 1);

	bool synth(juce::AudioBuffer<float>& buffer,
		int startSample, int length, juce::Array<double> freq, int frameSize) const;

private:
	juce::Array<float> noiseSource;

	static void prepareNextClip(
		float* source0, float* source1, int size);
	static void copyClip(
		float* src, float* dst, int size);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KarPlusStrong)
};
