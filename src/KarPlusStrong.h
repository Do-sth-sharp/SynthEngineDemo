#pragma once

#include <JuceHeader.h>

class KarPlusStrong {
public:
	enum class Device {
		Normal,
		SSE3,
		AVX2,
		AVX512
	};

	KarPlusStrong() = delete;
	KarPlusStrong(int seed = 1, Device device = Device::Normal);

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

	static void prepareNextClipSSE3(
		const float* source0, float* source1, int size);
	static void copyClipSSE3(
		const float* src, float* dst,
		float startDecay, float endDecay,
		int size);

	static void prepareNextClipAVX2(
		const float* source0, float* source1, int size);
	static void copyClipAVX2(
		const float* src, float* dst,
		float startDecay, float endDecay,
		int size);

	static void prepareNextClipAVX512(
		const float* source0, float* source1, int size);
	static void copyClipAVX512(
		const float* src, float* dst,
		float startDecay, float endDecay,
		int size);

	typedef void(*PrepareNextClipFuncType)(const float*, float*, int);
	typedef void(*CopyClipFuncType)(const float*, float*, float, float, int);
	PrepareNextClipFuncType prepareNextClipFunc = prepareNextClip;
	CopyClipFuncType copyClipFunc = copyClip;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KarPlusStrong)
};
