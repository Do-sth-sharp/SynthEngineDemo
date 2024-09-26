#include "ARARenderer.h"

ARAPlaybackRenderer::ARAPlaybackRenderer(
	ARA::PlugIn::DocumentController* dc)
	: juce::ARAPlaybackRenderer::ARAPlaybackRenderer(dc) {
	/** Renderer */
	this->renderer = std::make_unique<ARARenderThread>(this->context);
}

void ARAPlaybackRenderer::prepareToPlay(double sampleRateIn,
	int /*maximumSamplesPerBlockIn*/,
	int /*numChannelsIn*/,
	juce::AudioProcessor::ProcessingPrecision /*precision*/,
	AlwaysNonRealtime /*alwaysNonRealtime*/) {
	this->renderer->setSampleRate(sampleRateIn);
}

void ARAPlaybackRenderer::releaseResources() {
	this->renderer->releaseData();
}

bool ARAPlaybackRenderer::processBlock(juce::AudioBuffer<float>& buffer,
	juce::AudioProcessor::Realtime /*realtime*/,
	const juce::AudioPlayHead::PositionInfo& positionInfo) noexcept {
	/** TODO Get Region Data */
	//this->renderer->getAudioData(buffer, positionInfo.getTimeInSamples().orFallback(0));
	return true;
}
