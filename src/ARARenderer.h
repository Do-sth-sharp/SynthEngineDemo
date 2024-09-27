#pragma once

#include <JuceHeader.h>
#include "ARAContext.h"
#include "ARARenderThread.h"

class ARAPlaybackRenderer final 
	: public juce::ARAPlaybackRenderer,
	public juce::ARADocumentListener {
public:
	ARAPlaybackRenderer(
		ARA::PlugIn::DocumentController* dc);

	void stopRender();
	void startRender(juce::ARADocument* document);

private:
	void willBeginEditing(juce::ARADocument* document) override;
	void didEndEditing(juce::ARADocument* document) override;

public:
	void prepareToPlay(double sampleRateIn,
		int maximumSamplesPerBlockIn,
		int numChannelsIn,
		juce::AudioProcessor::ProcessingPrecision precision,
		AlwaysNonRealtime alwaysNonRealtime) override;
	void releaseResources() override;
	bool processBlock(juce::AudioBuffer<float>& buffer,
		juce::AudioProcessor::Realtime realtime,
		const juce::AudioPlayHead::PositionInfo& positionInfo) noexcept override;

private:
	ARAContext context;
	std::unique_ptr<ARARenderThread> renderer = nullptr;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ARAPlaybackRenderer)
};
