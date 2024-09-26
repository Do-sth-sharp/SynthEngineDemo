#include "EngineRenderer.h"

#define PITCH_ACCURACY 100
#define MIDI_PITCH_MAX 8191
#define MIDI_PITCH_MIN -8192

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

void EngineRenderer::render(
	const juce::Array<ARA::ARAContentNote>& notes,
	const juce::Array<ARAExtension::ARAContentIntParam>& pitchs,
	double totalLength) {
	/** Locker */
	juce::ScopedWriteLock locker(this->bufferLock);

	/** Clear Data */
	this->releaseData();

	/** Init Buffer */
	int bufferSize = std::ceil(totalLength * this->sampleRate);
	this->buffer.setSize(1, bufferSize);

	/** Render For Track */
	{

		/** Param Temp */
		juce::Array<double> pitchTemp;
		pitchTemp.resize(bufferSize / PITCH_ACCURACY);

		/** Get Pitch */
		{
			int lastParamPlace = 0;
			double lastPitchData = 0;
			for (int j = 0; j < pitchs.size(); j++) {
				/** Get Current Param */
				auto event = pitchs.getUnchecked(j);

				/** Get Event Time */
				double time = event.timeSec;
				int samplePlace = time * this->sampleRate;
				int paramPlace = samplePlace / PITCH_ACCURACY;

				/** Get Pitch Value */
				int pitchValue = event.value;
				double pitchData = (pitchValue >= 0)
					? (static_cast<double>(pitchValue) / MIDI_PITCH_MAX)
					: (static_cast<double>(pitchValue) / MIDI_PITCH_MIN);
				pitchData *= 2;

				/** Store Last Value */
				for (int k = lastParamPlace; k < paramPlace; k++) {
					pitchTemp.setUnchecked(k, lastPitchData);
				}

				/** Value Temp */
				lastParamPlace = paramPlace;
				lastPitchData = pitchData;
			}

			/** Store Last Value */
			for (int k = lastParamPlace; k < pitchTemp.size(); k++) {
				pitchTemp.setUnchecked(k, lastPitchData);
			}
		}

		/** Synth */
		for (int j = 0; j < notes.size(); j++) {
			/** Check For Stop */
			if (juce::Thread::currentThreadShouldExit()) {
				this->releaseData();
				return;
			}

			/** Get Current Note */
			auto event = notes.getUnchecked(j);

			/** Get Note Info */
			double startTime = event.startPosition;
			double endTime = startTime + event.noteDuration;
			int startSample = startTime * sampleRate;
			int endSample = endTime * sampleRate;
			int noteNumber = std::round(std::log2(event.frequency / 440) * 12) + 69;

			/** Pitch Param */
			int paramStartPlace = startSample / PITCH_ACCURACY;
			int paramEndPlace = endSample / PITCH_ACCURACY;
			int paramOffset = startSample - paramStartPlace * PITCH_ACCURACY;
			
			/** Caculate Freq */
			juce::Array<double> freqTemp;
			freqTemp.resize(paramEndPlace - paramStartPlace);
			for (int k = 0; k < freqTemp.size(); k++) {
				double pitchData = pitchTemp.getUnchecked(paramStartPlace + k);
				double freq =
					440 * std::pow(2, (noteNumber + pitchData - 69) / 12);
				freqTemp.setUnchecked(k, freq);
			}

			/** Synth */
			if (startSample >= this->buffer.getNumSamples()) { continue; }
			int noteLength = std::min(endSample - startSample,
				this->buffer.getNumSamples() - 1 - startSample);
			this->kps->synth(this->buffer, this->sampleRate, startSample,
				noteLength, freqTemp, PITCH_ACCURACY, paramOffset);
		}
	}

	/** Set Rendered Flag */
	this->rendered = true;
}

void EngineRenderer::getAudio(
	juce::AudioBuffer<float>& buffer, int64_t timeInSamples) const {
	/** Lock */
	juce::ScopedTryReadLock locker(this->bufferLock);
	if (!locker.isLocked()) { return; }

	/** Check Is Rendered */
	if (!this->rendered) { return; }

	/** Check Size */
	int bufferSize = buffer.getNumSamples();
	int tempSize = this->buffer.getNumSamples();
	if (static_cast<int64_t>(tempSize) - bufferSize <= timeInSamples) {
		bufferSize = std::max(tempSize - timeInSamples, (int64_t)0);
	}

	/** Copy Data */
	int channelNum = buffer.getNumChannels();
	for (int i = 0; i < channelNum; i++) {
		buffer.copyFrom(
			i, 0, &((this->buffer.getReadPointer(0))[timeInSamples]), bufferSize);
	}
}

double EngineRenderer::getSampleRate() const {
	return this->sampleRate;
}
