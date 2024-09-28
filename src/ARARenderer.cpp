#include "ARARenderer.h"
#include "ARAContext.h"
#include "ARARenderThread.h"

ARAPlaybackRenderer::ARAPlaybackRenderer(
	ARA::PlugIn::DocumentController* dc,
	const ARAContext& context,
	ARARenderThread& renderer)
	: juce::ARAPlaybackRenderer::ARAPlaybackRenderer(dc),
	context(context), renderer(renderer) {}

void ARAPlaybackRenderer::prepareToPlay(double sampleRateIn,
	int /*maximumSamplesPerBlockIn*/,
	int /*numChannelsIn*/,
	juce::AudioProcessor::ProcessingPrecision /*precision*/,
	AlwaysNonRealtime /*alwaysNonRealtime*/) {
	this->renderer.setSampleRate(sampleRateIn);
}

void ARAPlaybackRenderer::releaseResources() {
	this->renderer.releaseData();
}

bool ARAPlaybackRenderer::processBlock(juce::AudioBuffer<float>& buffer,
	juce::AudioProcessor::Realtime /*realtime*/,
	const juce::AudioPlayHead::PositionInfo& positionInfo) noexcept {
	/** Get Region List */
	double sampleRate = this->renderer.getSampleRate();
	double startTime = positionInfo.getTimeInSeconds().orFallback(0);
	auto regions = this->context.doMapTimeRealTime(
		startTime, buffer.getNumSamples() / sampleRate);

	/** Copy Regions Data */
	for (auto [seqStart, contextStart, length] : regions) {
		uint64_t bufferStartSample = (seqStart - startTime) * sampleRate;
		uint64_t contextStartSample = contextStart * sampleRate;
		uint64_t lengthInSample = std::ceil(length * sampleRate);

		this->renderer.getAudioData(buffer, contextStartSample, bufferStartSample, lengthInSample);
	}
	return true;
}

ARAEditorRenderer::ARAEditorRenderer(
	ARA::PlugIn::DocumentController* dc,
	const ARAContext& context,
	ARARenderThread& renderer)
	: juce::ARAEditorRenderer::ARAEditorRenderer(dc),
	context(context), renderer(renderer) {}

void ARAEditorRenderer::prepareToPlay(double sampleRateIn,
	int /*maximumSamplesPerBlockIn*/,
	int /*numChannelsIn*/,
	juce::AudioProcessor::ProcessingPrecision /*precision*/,
	AlwaysNonRealtime /*alwaysNonRealtime*/) {
	this->renderer.setSampleRate(sampleRateIn);
}

void ARAEditorRenderer::releaseResources() {
	this->renderer.releaseData();
}

bool ARAEditorRenderer::processBlock(juce::AudioBuffer<float>& buffer,
	juce::AudioProcessor::Realtime /*realtime*/,
	const juce::AudioPlayHead::PositionInfo& positionInfo) noexcept {
	/** Get Region List */
	double sampleRate = this->renderer.getSampleRate();
	double startTime = positionInfo.getTimeInSeconds().orFallback(0);
	auto regions = this->context.doMapTimeRealTime(
		startTime, buffer.getNumSamples() / sampleRate);

	/** Copy Regions Data */
	for (auto [seqStart, contextStart, length] : regions) {
		uint64_t bufferStartSample = (seqStart - startTime) * sampleRate;
		uint64_t contextStartSample = contextStart * sampleRate;
		uint64_t lengthInSample = std::ceil(length * sampleRate);

		this->renderer.getAudioData(buffer, contextStartSample, bufferStartSample, lengthInSample);
	}
	return true;
}
