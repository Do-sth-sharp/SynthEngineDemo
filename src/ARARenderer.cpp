#include "ARARenderer.h"
#include "ARAContext.h"
#include "ARARenderThread.h"
#include "ARADocument.h"

void ARAPlaybackRenderer::prepareToPlay(double sampleRateIn,
	int /*maximumSamplesPerBlockIn*/,
	int /*numChannelsIn*/,
	juce::AudioProcessor::ProcessingPrecision /*precision*/,
	AlwaysNonRealtime /*alwaysNonRealtime*/) {
	if (auto document = dynamic_cast<ARADocument*>(this->getDocumentController()->getDocument())) {
		document->getRenderer().setSampleRate(sampleRateIn);
	}
}

void ARAPlaybackRenderer::releaseResources() {
	if (auto document = dynamic_cast<ARADocument*>(this->getDocumentController()->getDocument())) {
		document->getRenderer().releaseData();
	}
}

bool ARAPlaybackRenderer::processBlock(juce::AudioBuffer<float>& buffer,
	juce::AudioProcessor::Realtime /*realtime*/,
	const juce::AudioPlayHead::PositionInfo& positionInfo) noexcept {
	/** Check Document */
	auto document = dynamic_cast<ARADocument*>(this->getDocumentController()->getDocument());
	if (!document) { return false; }

	/** Check Playing */
	if (!positionInfo.getIsPlaying()) { return true; }

	/** Get Region List */
	double sampleRate = document->getRenderer().getSampleRate();
	double startTime = positionInfo.getTimeInSeconds().orFallback(0);
	auto regions = document->getContext().doMapTimeRealTime(
		startTime, buffer.getNumSamples() / sampleRate);

	/** Copy Regions Data */
	for (auto [seqStart, contextStart, length] : regions) {
		uint64_t bufferStartSample = (seqStart - startTime) * sampleRate;
		uint64_t contextStartSample = contextStart * sampleRate;
		uint64_t lengthInSample = std::ceil(length * sampleRate);

		document->getRenderer().getAudioData(buffer, contextStartSample, bufferStartSample, lengthInSample);
	}
	return true;
}

void ARAEditorRenderer::prepareToPlay(double sampleRateIn,
	int /*maximumSamplesPerBlockIn*/,
	int /*numChannelsIn*/,
	juce::AudioProcessor::ProcessingPrecision /*precision*/,
	AlwaysNonRealtime /*alwaysNonRealtime*/) {
	if (auto document = dynamic_cast<ARADocument*>(this->getDocumentController()->getDocument())) {
		document->getRenderer().setSampleRate(sampleRateIn);
	}
}

void ARAEditorRenderer::releaseResources() {
	if (auto document = dynamic_cast<ARADocument*>(this->getDocumentController()->getDocument())) {
		document->getRenderer().releaseData();
	}
}

bool ARAEditorRenderer::processBlock(juce::AudioBuffer<float>& buffer,
	juce::AudioProcessor::Realtime /*realtime*/,
	const juce::AudioPlayHead::PositionInfo& positionInfo) noexcept {
	/** Check Document */
	auto document = dynamic_cast<ARADocument*>(this->getDocumentController()->getDocument());
	if (!document) { return false; }

	/** Check Playing */
	if (!positionInfo.getIsPlaying()) { return true; }

	/** Get Region List */
	double sampleRate = document->getRenderer().getSampleRate();
	double startTime = positionInfo.getTimeInSeconds().orFallback(0);
	auto regions = document->getContext().doMapTimeRealTime(
		startTime, buffer.getNumSamples() / sampleRate);

	/** Copy Regions Data */
	for (auto [seqStart, contextStart, length] : regions) {
		uint64_t bufferStartSample = (seqStart - startTime) * sampleRate;
		uint64_t contextStartSample = contextStart * sampleRate;
		uint64_t lengthInSample = std::ceil(length * sampleRate);

		document->getRenderer().getAudioData(buffer, contextStartSample, bufferStartSample, lengthInSample);
	}
	return true;
}
