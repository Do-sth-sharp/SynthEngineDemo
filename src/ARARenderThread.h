#pragma once

#include <JuceHeader.h>
#include "ARAContext.h"
#include "EngineRenderer.h"

class ARARenderThread final
	: public juce::Thread,
	public juce::ChangeBroadcaster {
public:
	ARARenderThread(const ARAContext& context);
	~ARARenderThread();

	void setSampleRate(double sampleRate);
	double getSampleRate() const;

	void releaseData();

	void getAudioData(
		juce::AudioBuffer<float>& buffer,
		int64_t timeInSamples,
		int64_t bufferPos,
		int64_t length) const;

	void stopSafety();
	void startSafety();

	bool getRunningFlag() const;
	bool getRendered() const;

private:
	void run() override;

private:
	const ARAContext& context;
	std::unique_ptr<EngineRenderer> renderer = nullptr;

	juce::CriticalSection stateLock;

	std::atomic_bool runningFlag = false;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ARARenderThread)
};
