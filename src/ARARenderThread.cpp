#include "ARARenderThread.h"

ARARenderThread::ARARenderThread(const ARAContext& context)
	: Thread("Synth Engine Demo ARA Renderer"), context(context) {
	/** Renderer */
	this->renderer = std::make_unique<EngineRenderer>();
}

ARARenderThread::~ARARenderThread() {
	this->stopInternal();
}

void ARARenderThread::setSampleRate(double sampleRate) {
	juce::GenericScopedLock locker(this->stateLock);

	bool isRunning = this->isThreadRunning();
	if (isRunning) {
		this->stopInternal();
	}

	this->renderer->prepare(sampleRate);

	if (isRunning) {
		this->startThread();
	}
}

double ARARenderThread::getSampleRate() const {
	return this->renderer->getSampleRate();
}

void ARARenderThread::releaseData() {
	juce::GenericScopedLock locker(this->stateLock);

	if (this->isThreadRunning()) {
		this->stopInternal();
	}

	this->renderer->releaseData();
}

void ARARenderThread::getAudioData(
	juce::AudioBuffer<float>& buffer, int64_t timeInSamples) const {
	this->renderer->getAudio(buffer, timeInSamples);
}

void ARARenderThread::run() {
	/** Get Context */
	auto [notes, pitchs, length] = this->context.getData();

	/** Synth */
	this->renderer->render(notes, pitchs, length);
}

void ARARenderThread::stopInternal() {
	juce::GenericScopedLock locker(this->stateLock);

	if (this->isThreadRunning()) {
		this->stopThread(30000);
	}
}
