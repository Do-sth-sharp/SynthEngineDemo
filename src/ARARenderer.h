#pragma once

#include <JuceHeader.h>

class ARAContext;
class ARARenderThread;

class ARAPlaybackRenderer final 
	: public juce::ARAPlaybackRenderer {
public:
	using juce::ARAPlaybackRenderer::ARAPlaybackRenderer;

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
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ARAPlaybackRenderer)
};

class ARAEditorRenderer final
	: public juce::ARAEditorRenderer {
public:
	using juce::ARAEditorRenderer::ARAEditorRenderer;

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
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ARAEditorRenderer)
};
