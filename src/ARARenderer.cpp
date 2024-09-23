#include "ARARenderer.h"

ARAPlaybackRenderer::ARAPlaybackRenderer(
	ARA::PlugIn::DocumentController* dc)
	: juce::ARAPlaybackRenderer::ARAPlaybackRenderer(dc) {}

void ARAPlaybackRenderer::prepareToPlay(double sampleRateIn,
	int maximumSamplesPerBlockIn,
	int numChannelsIn,
	juce::AudioProcessor::ProcessingPrecision precision,
	AlwaysNonRealtime alwaysNonRealtime) {
	/** TODO */
}

void ARAPlaybackRenderer::releaseResources() {
	/** TODO */
}

bool ARAPlaybackRenderer::processBlock(juce::AudioBuffer<float>& buffer,
	juce::AudioProcessor::Realtime realtime,
	const juce::AudioPlayHead::PositionInfo& positionInfo) noexcept {
	/** TODO */
	return true;
}
