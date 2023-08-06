#include "KarPlusStrong.h"

#include <random>

#define KPS_SAMPLERATE_MAX 192000
#define KPS_FREQ_MIN 8.2
#define KPS_FREQ_MAX 12543.9
#define KPS_AVERAGE_R 0.5

KarPlusStrong::KarPlusStrong(int seed, KarPlusStrong::Device device) {
	/** Init Random Engine */
	std::mt19937 randomEngine(seed);
	std::uniform_real_distribution<float> randomDistribution(-1, 1);

	/** Create Noise Array */
	const int KPSNoiseLength =
		std::ceil((1 / KPS_FREQ_MIN) * KPS_SAMPLERATE_MAX);
	this->noiseSource.resize(KPSNoiseLength);
	for (int i = 0; i < this->noiseSource.size(); i++) {
		this->noiseSource.getReference(i) = randomDistribution(randomEngine);
	}

	/** Set Device */
	switch (device) {
	case KarPlusStrong::Device::SSE3:
		this->prepareNextClipFunc = KarPlusStrong::prepareNextClipSSE;
		this->copyClipFunc = KarPlusStrong::copyClipSSE;
		break;
	case KarPlusStrong::Device::AVX2:
		this->prepareNextClipFunc = KarPlusStrong::prepareNextClipAVX;
		this->copyClipFunc = KarPlusStrong::copyClipAVX;
		break;
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
	for (int start = 0, clipLength = 0; start < length - 1; start += clipLength) {
		/** Get Clip Length */
		int frameNum = (start + frameDeviation) / frameLength;
		if (frameNum < freq.size()) {
			double freqTemp = freq.getUnchecked(frameNum);
			if (freqTemp >= KPS_FREQ_MIN && freqTemp <= KPS_FREQ_MAX) {
				currentFreq = freqTemp;
			}
		}
		clipLength = (1 / currentFreq) * sampleRate;
		if (start + clipLength >= length) {
			clipLength = length - 1 - start;
		}

		/** Select Buffer */
		const float* ptr0 = (start == 0)
			? this->noiseSource.data()
			: (tempReverseFlag ? temp1.data() : temp0.data());
		float* ptr1 = tempReverseFlag ? temp0.data() : temp1.data();
		tempReverseFlag = !tempReverseFlag;

		/** Prepare Clip */
		this->prepareNextClipFunc(ptr0, ptr1, this->noiseSource.size());

		/** Get Decay */
		float startDecay = 1 - static_cast<float>(start) / length;
		float endDecay = 1 - static_cast<float>(start + clipLength - 1) / length;

		/** Copy Data */
		this->copyClipFunc(ptr1,
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
		dst[i] = dst[i] + (src[i] * decay);
	}
}

void KarPlusStrong::prepareNextClipSSE(
	const float* source0, float* source1, int size) {
	jassert(source0 && source1);

	/** Average Coef */
	__m128 src0Coef = _mm_set1_ps(KPS_AVERAGE_R);
	__m128 src1Coef = _mm_set1_ps(1 - KPS_AVERAGE_R);

	/** Caculate Start Point */
	source1[0] = source0[0] * KPS_AVERAGE_R
		+ source0[size - 1] * (1 - KPS_AVERAGE_R);

	/** Caculate Loop */
	for (int i = 1; (i + 4) <= size; i += 4) {
		__m128 src0Data = _mm_loadu_ps(&source0[i]);
		__m128 src0Res = _mm_mul_ps(src0Data, src0Coef);

		__m128 src1Data = _mm_loadu_ps(&source0[i - 1]);
		__m128 src1Res = _mm_mul_ps(src1Data, src1Coef);

		__m128 dstData = _mm_add_ps(src0Res, src1Res);
		_mm_storeu_ps(&source1[i], dstData);
	}

	/** Caculate End */
	if (int remainder = ((size - 1) % 4)) {
		for (int i = size - remainder; i < size; i++) {
			source1[i] = source0[i] * KPS_AVERAGE_R
				+ source0[i - 1] * (1 - KPS_AVERAGE_R);
		}
	}
}

void KarPlusStrong::copyClipSSE(
	const float* src, float* dst,
	float startDecay, float endDecay,
	int size) {
	jassert(src && dst);

	/** Decay Memory */
	const float decayDiffer = endDecay - startDecay;
	__m128 decayDifferData = _mm_set1_ps(decayDiffer);
	__m128 sizeM1Data = _mm_set1_ps(size - 1);
	__m128 startDecayData = _mm_set1_ps(startDecay);
	_MM_ALIGN16 float incrementalCore[4] = { 0, 1, 2, 3 };
	__m128 incrementalCoreData = _mm_load_ps(incrementalCore);

	/** Caculate Loop */
	for (int i = 0; i + 4 <= size; i += 4) {
		/** Get Decay */
		__m128 decayBase = _mm_set1_ps(i);
		__m128 decayArg = _mm_add_ps(decayBase, incrementalCoreData);
		__m128 decayProportion = _mm_div_ps(decayArg, sizeM1Data);
		__m128 decaySize = _mm_mul_ps(decayProportion, decayDifferData);
		__m128 decayRes = _mm_add_ps(startDecayData, decaySize);
		
		/** Add Data To Dst */
		__m128 srcData = _mm_loadu_ps(&src[i]);
		__m128 srcRes = _mm_mul_ps(srcData, decayRes);
		__m128 dstData = _mm_loadu_ps(&dst[i]);
		__m128 dstRes = _mm_add_ps(dstData, srcRes);
		_mm_storeu_ps(&dst[i], dstRes);
	}

	/** Caculate End */
	if (int remainder = (size % 4)) {
		for (int i = size - remainder; i < size; i++) {
			float decay =
				startDecay + static_cast<float>(i) / (size - 1) * decayDiffer;
			dst[i] = dst[i] + (src[i] * decay);
		}
	}
}

void KarPlusStrong::prepareNextClipAVX(
	const float* source0, float* source1, int size) {
	jassert(source0 && source1);

	/** Average Coef */
	__m256 src0Coef = _mm256_set1_ps(KPS_AVERAGE_R);
	__m256 src1Coef = _mm256_set1_ps(1 - KPS_AVERAGE_R);

	/** Caculate Start Point */
	source1[0] = source0[0] * KPS_AVERAGE_R
		+ source0[size - 1] * (1 - KPS_AVERAGE_R);

	/** Caculate Loop */
	for (int i = 1; (i + 8) <= size; i += 8) {
		__m256 src0Data = _mm256_loadu_ps(&source0[i]);
		__m256 src0Res = _mm256_mul_ps(src0Data, src0Coef);

		__m256 src1Data = _mm256_loadu_ps(&source0[i - 1]);
		__m256 src1Res = _mm256_mul_ps(src1Data, src1Coef);

		__m256 dstData = _mm256_add_ps(src0Res, src1Res);
		_mm256_storeu_ps(&source1[i], dstData);
	}

	/** Caculate End */
	if (int remainder = ((size - 1) % 8)) {
		for (int i = size - remainder; i < size; i++) {
			source1[i] = source0[i] * KPS_AVERAGE_R
				+ source0[i - 1] * (1 - KPS_AVERAGE_R);
		}
	}
}

void KarPlusStrong::copyClipAVX(
	const float* src, float* dst,
	float startDecay, float endDecay,
	int size) {
	jassert(src && dst);

	/** Decay Memory */
	const float decayDiffer = endDecay - startDecay;
	__m256 decayDifferData = _mm256_set1_ps(decayDiffer);
	__m256 sizeM1Data = _mm256_set1_ps(size - 1);
	__m256 startDecayData = _mm256_set1_ps(startDecay);
	float incrementalCore[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };
	__m256 incrementalCoreData = _mm256_loadu_ps(incrementalCore);

	/** Caculate Loop */
	for (int i = 0; i + 8 <= size; i += 8) {
		/** Get Decay */
		__m256 decayBase = _mm256_set1_ps(i);
		__m256 decayArg = _mm256_add_ps(decayBase, incrementalCoreData);
		__m256 decayProportion = _mm256_div_ps(decayArg, sizeM1Data);
		__m256 decaySize = _mm256_mul_ps(decayProportion, decayDifferData);
		__m256 decayRes = _mm256_add_ps(startDecayData, decaySize);

		/** Add Data To Dst */
		__m256 srcData = _mm256_loadu_ps(&src[i]);
		__m256 srcRes = _mm256_mul_ps(srcData, decayRes);
		__m256 dstData = _mm256_loadu_ps(&dst[i]);
		__m256 dstRes = _mm256_add_ps(dstData, srcRes);
		_mm256_storeu_ps(&dst[i], dstRes);
	}

	/** Caculate End */
	if (int remainder = (size % 8)) {
		for (int i = size - remainder; i < size; i++) {
			float decay =
				startDecay + static_cast<float>(i) / (size - 1) * decayDiffer;
			dst[i] = dst[i] + (src[i] * decay);
		}
	}
}
