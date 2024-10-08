﻿#include "ARARenderThread.h"

ARARenderThread::ARARenderThread(const ARAContext& context)
	: Thread("Synth Engine Demo ARA Renderer"), context(context) {
	/** Renderer */
	this->renderer = std::make_unique<EngineRenderer>();
}

ARARenderThread::~ARARenderThread() {
	this->stopSafety();
}

void ARARenderThread::setSampleRate(double sampleRate) {
	juce::GenericScopedLock locker(this->stateLock);

	bool isRunning = this->isThreadRunning();
	if (isRunning) {
		this->stopSafety();
	}

	this->renderer->prepare(sampleRate);

	if (isRunning) {
		this->startThread();
	}

	this->sendChangeMessage();
}

double ARARenderThread::getSampleRate() const {
	return this->renderer->getSampleRate();
}

void ARARenderThread::releaseData() {
	juce::GenericScopedLock locker(this->stateLock);

	if (this->isThreadRunning()) {
		this->stopSafety();
	}

	this->renderer->releaseData();

	this->sendChangeMessage();
}

void ARARenderThread::getAudioData(
	juce::AudioBuffer<float>& buffer,
	int64_t timeInSamples,
	int64_t bufferPos,
	int64_t length) const {
	this->renderer->getAudio(buffer, timeInSamples, bufferPos, length);
}

void ARARenderThread::stopSafety() {
	juce::GenericScopedLock locker(this->stateLock);

	if (this->isThreadRunning()) {
		this->stopThread(30000);
	}
}

void ARARenderThread::startSafety() {
	juce::GenericScopedLock locker(this->stateLock);

	if (this->isThreadRunning()) {
		this->stopSafety();
	}

	this->startThread();
}

bool ARARenderThread::getRunningFlag() const {
	return this->runningFlag;
}

bool ARARenderThread::getRendered() const {
	return this->renderer->isRendered();
}

void ARARenderThread::run() {
	/** State Changed */
	this->runningFlag = true;
	this->sendChangeMessage();

	/** Get Context */
	auto [notes, pitchs, length] = this->context.getContextData();

	/** Synth */
	this->renderer->render(notes, pitchs, length);

	/** State Changed */
	this->runningFlag = false;
	this->sendChangeMessage();
}
