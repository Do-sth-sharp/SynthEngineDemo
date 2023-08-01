#include "KarPlusStrong.h"

#include <random>

#define KPS_SAMPLERATE_MAX 192000
#define KPS_FREQ_MIN 8.2
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

bool KarPlusStrong::synth(juce::AudioBuffer<float>& buffer,
	int startSample, int length, juce::Array<double> freq, int frameSize) const {
	/** Check Size */
	if (static_cast<int64_t>(startSample) + length >= buffer.getNumSamples()) { 
		return false;
	}

	/** Prepare Temp */
	juce::Array<float> temp0, temp1;
	temp0.resize(this->noiseSource.size());
	temp1.resize(this->noiseSource.size());

	/** TODO Synth */

	return true;
}

void KarPlusStrong::prepareNextClip(float* source0, float* source1, int size) {
	jassert(source0 && source1);

	for (int i = 0; i < size; i++) {
		source1[i] = source0[i] * KPS_AVERAGE_R
			+ source0[(i == 0) ? (size - 1) : (i - 1)] * (1 - KPS_AVERAGE_R);
	}
}

void KarPlusStrong::copyClip(
	float* src, float* dst, int size) {
	jassert(src && dst);

	for (int i = 0; i < size; i++) {
		dst[i] += src[i];
	}
}
