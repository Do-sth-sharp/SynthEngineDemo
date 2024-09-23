#pragma once

#include <JuceHeader.h>

class ARAPlaybackRenderer final : public juce::ARAPlaybackRenderer {
public:
	ARAPlaybackRenderer(
		ARA::PlugIn::DocumentController* dc);

private:
	void prepareToPlay(double sampleRateIn,
		int maximumSamplesPerBlockIn,
		int numChannelsIn,
		juce::AudioProcessor::ProcessingPrecision precision,
		AlwaysNonRealtime alwaysNonRealtime) override;
	void releaseResources() override;
	bool processBlock(juce::AudioBuffer<float>& buffer,
		juce::AudioProcessor::Realtime realtime,
		const juce::AudioPlayHead::PositionInfo& positionInfo) noexcept override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ARAPlaybackRenderer)
};
