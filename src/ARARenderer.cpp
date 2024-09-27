#include "ARARenderer.h"

ARAPlaybackRenderer::ARAPlaybackRenderer(
	ARA::PlugIn::DocumentController* dc)
	: juce::ARAPlaybackRenderer::ARAPlaybackRenderer(dc) {
	/** Renderer */
	this->renderer = std::make_unique<ARARenderThread>(this->context);
}

void ARAPlaybackRenderer::stopRender() {
	this->renderer->stopSafety();
}

void ARAPlaybackRenderer::startRender(juce::ARADocument* document) {
	/** Get Sequence */
	auto& sequences = document->getRegionSequences();

	/** Update Context */
	this->context.setSequenceData(
		(sequences.size() > 0) ? sequences.front() : nullptr);

	/** Render */
	this->renderer->startSafety();
}

void ARAPlaybackRenderer::willBeginEditing(juce::ARADocument* /*document*/) {
	this->stopRender();
}

void ARAPlaybackRenderer::didEndEditing(juce::ARADocument* document) {
	this->startRender(document);
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
	/** Get Region List */
	double sampleRate = this->renderer->getSampleRate();
	double startTime = positionInfo.getTimeInSeconds().orFallback(0);
	auto regions = this->context.doMapTimeRealTime(
		startTime, buffer.getNumSamples() / sampleRate);

	/** Copy Regions Data */
	for (auto [seqStart, contextStart, length] : regions) {
		uint64_t bufferStartSample = (seqStart - startTime) * sampleRate;
		uint64_t contextStartSample = contextStart * sampleRate;
		uint64_t lengthInSample = std::ceil(length * sampleRate);

		this->renderer->getAudioData(buffer, contextStartSample, bufferStartSample, lengthInSample);
	}
	return true;
}
