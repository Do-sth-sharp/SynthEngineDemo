#pragma once

#include <JuceHeader.h>
#include "KarPlusStrong.h"

class EngineRenderer final {
public:
	explicit EngineRenderer(int KPSSeed = 114514191);

	void releaseData();
	bool isRendered() const;
	void prepare(double sampleRate);
	void render(const juce::MidiFile& context);
	void getAudio(juce::AudioBuffer<float>& buffer, int64_t timeInSamples) const;

private:
	std::unique_ptr<KarPlusStrong> kps = nullptr;

	juce::ReadWriteLock bufferLock;
	juce::AudioBuffer<float> buffer;
	bool rendered = false;
	double sampleRate = 48000;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EngineRenderer)
};
