#pragma once

#include <JuceHeader.h>
#include <ARASharedObject.h>
#include "KarPlusStrong.h"

class EngineRenderer final {
public:
	explicit EngineRenderer(int KPSSeed = 114514191);

	void releaseData();
	bool isRendered() const;
	void prepare(double sampleRate);
	void render(
		const juce::Array<ARA::ARAContentNote>& notes,
		const juce::Array<ARAExtension::ARAContentIntParam>& pitchs,
		double totalLength);
	void getAudio(juce::AudioBuffer<float>& buffer, int64_t timeInSamples) const;

	double getSampleRate() const;

private:
	std::unique_ptr<KarPlusStrong> kps = nullptr;
		
	mutable juce::ReadWriteLock bufferLock;
	juce::AudioBuffer<float> buffer;
	bool rendered = false;
	double sampleRate = 48000;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EngineRenderer)
};
