#include "KarPlusStrong.h"

#include <random>

#define KPS_SAMPLERATE_MAX 192000
#define KPS_FREQ_MIN 8.2
#define KPS_FREQ_MAX 12543.9
#define KPS_AVERAGE_R 0.5

KarPlusStrong::KarPlusStrong(int seed) {
	/** Init Random Engine */
	std::default_random_engine randomEngine(seed);
	std::uniform_real_distribution<float> randomDistribution(-1, 1);

	/** Create Noise Array */
	const int KPSNoiseLength =
		std::ceil((1 / KPS_FREQ_MIN) * KPS_SAMPLERATE_MAX);
	this->noiseSource.resize(KPSNoiseLength);
	for (int i = 0; i < this->noiseSource.size(); i++) {
		this->noiseSource.getReference(i) = randomDistribution(randomEngine);
	}
}

bool KarPlusStrong::synth(juce::AudioBuffer<float>& buffer, double sampleRate,
	int startSample, int length,
	juce::Array<double> freq, int frameLength, int frameDeviation) const {
	/** Check Size */
	if (buffer.getNumChannels() < 1) { return false; }
	if (static_cast<int64_t>(startSample) + length >= buffer.getNumSamples()) { 
		return false;
	}

	/** Prepare Temp */
	juce::Array<float> temp0, temp1;
	temp0.resize(this->noiseSource.size());
	temp1.resize(this->noiseSource.size());

	/** Synth */
	double currentFreq = 440;
	bool tempReverseFlag = false;
	for (int start = 0, clipLength = 0; start < length; start += clipLength) {
		/** Get Clip Length */
		int frameNum = (start + frameDeviation) / frameLength;
		if (frameNum < freq.size()) {
			double freqTemp = freq.getUnchecked(frameNum);
			if (freqTemp >= KPS_FREQ_MIN && freqTemp <= KPS_FREQ_MAX) {
				currentFreq = freqTemp;
			}
		}
		clipLength = (1 / currentFreq) * sampleRate;

		/** Select Buffer */
		const float* ptr0 = (start == 0)
			? this->noiseSource.data()
			: (tempReverseFlag ? temp1.data() : temp0.data());
		float* ptr1 = tempReverseFlag ? temp0.data() : temp1.data();

		/** Prepare Clip */
		KarPlusStrong::prepareNextClip(ptr0, ptr1, this->noiseSource.size());

		/** Get Decay */
		float startDecay = 1 - static_cast<float>(start) / length;
		float endDecay = 1 - static_cast<float>(start + clipLength - 1) / length;

		/** Copy Data */
		KarPlusStrong::copyClip(ptr1,
			&(buffer.getWritePointer(0))[startSample + start],
			startDecay, endDecay, clipLength);
	}

	return true;
}

void KarPlusStrong::prepareNextClip(const float* source0, float* source1, int size) {
	jassert(source0 && source1);

	for (int i = 0; i < size; i++) {
		source1[i] = source0[i] * KPS_AVERAGE_R
			+ source0[(i == 0) ? (size - 1) : (i - 1)] * (1 - KPS_AVERAGE_R);
	}
}

void KarPlusStrong::copyClip(
	const float* src, float* dst,
	float startDecay, float endDecay,
	int size) {
	jassert(src && dst);

	for (int i = 0; i < size; i++) {
		float decay = startDecay + static_cast<float>(i) / (size - 1) * (endDecay - startDecay);
		dst[i] += (src[i] * decay);
	}
}
