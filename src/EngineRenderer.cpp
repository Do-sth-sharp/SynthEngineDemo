#include "EngineRenderer.h"

EngineRenderer::EngineRenderer(int KPSSeed) {
	/** KPS Device */
	auto device = KarPlusStrong::Device::Normal;
	if (juce::SystemStats::hasSSE3()) {
		device = KarPlusStrong::Device::SSE3;
	}
	if (juce::SystemStats::hasAVX2()) {
		device = KarPlusStrong::Device::AVX2;
	}
	if (juce::SystemStats::hasAVX512F()) {
		device = KarPlusStrong::Device::AVX512;
	}

	/** KPS Renderer */
	this->kps = std::make_unique<KarPlusStrong>(KPSSeed, device);
}

void EngineRenderer::releaseData() {
	juce::ScopedWriteLock locker(this->bufferLock);
	this->buffer.clear();
	this->rendered = false;
}

bool EngineRenderer::isRendered() const {
	juce::ScopedReadLock locker(this->bufferLock);
	return this->rendered;
}

void EngineRenderer::prepare(double sampleRate) {
	juce::ScopedWriteLock locker(this->bufferLock);
	if (this->sampleRate != sampleRate) {
		this->sampleRate = sampleRate;
		this->releaseData();
	}
}

void EngineRenderer::render(const juce::MidiFile& context) {
	/** Locker */
	juce::ScopedWriteLock locker(this->bufferLock);

	/** Clear Data */
	this->releaseData();

	/** Init Buffer */
	double timeInSeconds = context.getLastTimestamp();
	int bufferSize = std::ceil(timeInSeconds * this->sampleRate);
	this->buffer.setSize(1, bufferSize);

	/** TODO Render For Each Track */
}

void EngineRenderer::getAudio(
	juce::AudioBuffer<float>& buffer, int64_t timeInSamples) const {
	/** TODO Get Audio Data */
}
