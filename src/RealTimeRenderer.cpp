#include "RealTimeRenderer.h"

#define MAX_VOICES 10

#define ATTACK_TIME 0.05
#define RELEASE_TIME 0.25
#define MAX_NOTE_LENGTH 10.0

#define SAMPLE_NOTE 69
#define SAMPLE_SAMPLERATE 48000
#define SAMPLE_LENGTH 2.0

#define PI 3.14159265//3589793238462643383279502884197169399375105

class SinWaveReader final : public juce::AudioFormatReader {
public:
	SinWaveReader(
		int noteID = SAMPLE_NOTE,
		double sampleRate = SAMPLE_SAMPLERATE,
		double length = SAMPLE_LENGTH)
	: AudioFormatReader(nullptr, "Static Sin Wave") {
		/** Attributes */
		this->sampleRate = sampleRate;
		this->bitsPerSample = 24;
		this->lengthInSamples = sampleRate * length;
		this->numChannels = 1;
		this->usesFloatingPointData = false;

		/** Buffer */
		this->buffer.setSize(this->numChannels, this->lengthInSamples);

		double freq = 440 * std::pow(2, (noteID - 69) / 12.0);
		double t = 1.0 / freq;
		for (int i = 0; i < this->buffer.getNumSamples(); i++) {
			float data = std::sin(((i / sampleRate) / t) * PI * 2);
			for (int j = 0; j < this->buffer.getNumChannels(); j++) {
				this->buffer.setSample(j, i, data);
			}
		}
	}

	void readMaxLevels(
		juce::int64 /*startSample*/, juce::int64 /*numSamples*/,
		juce::Range<float>* results, int /*numChannelsToRead*/) override {
		if (results) {
			results->setStart(-1.0f);
			results->setEnd(1.0f);
		}
	};
	void readMaxLevels(
		juce::int64 startSample, juce::int64 numSamples,
		float& lowestLeft, float& highestLeft,
		float& lowestRight, float& highestRight) override {
		lowestLeft = lowestRight = -1.0f;
		highestLeft = highestRight = 1.0f;
	};

	juce::AudioChannelSet getChannelLayout() override {
		return juce::AudioChannelSet::mono();
	}

	bool readSamples(
		int* const* destChannels, int numDestChannels,
		int startOffsetInDestBuffer, juce::int64 startSampleInFile, int numSamples) override {
		for (int i = 0; i < numDestChannels; i++) {
			/** Get Buffer Pointer */
			int bufferChannel = i % this->buffer.getNumChannels();
			auto ptr = this->buffer.getReadPointer(bufferChannel);
			auto dstPtr = reinterpret_cast<float*>(destChannels[i]);

			/** Copy Data */
			std::memcpy(&dstPtr[startOffsetInDestBuffer], &ptr[startSampleInFile], numSamples * sizeof(float));
		}
		return true;
	}

private:
	juce::AudioSampleBuffer buffer;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SinWaveReader)
};

RealTimeRenderer::RealTimeRenderer() {
	/** Init Synthesiser */
	this->synth = std::make_unique<juce::Synthesiser>();
	for (int i = 0; i < MAX_VOICES; i++) {
		this->synth->addVoice(new juce::SamplerVoice{});
	}

	/** Create Synth Sound */
	this->createSound();
}

void RealTimeRenderer::prepare(double sampleRate, int /*blockSize*/) {
	juce::GenericScopedLock locker(this->lock);
	this->synth->setCurrentPlaybackSampleRate(sampleRate);
}

void RealTimeRenderer::release() {
	juce::GenericScopedLock locker(this->lock);
	this->synth->allNotesOff(0, false);
}

void RealTimeRenderer::processBlock(
	juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
	juce::GenericScopedTryLock locker(this->lock);
	if (!locker.isLocked()) { return; }

	this->synth->renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

void RealTimeRenderer::createSound() {
	juce::GenericScopedLock locker(this->lock);

	auto formatReader = std::make_unique<SinWaveReader>();

	juce::BigInteger midiNotes;
	midiNotes.setRange(0, 128, true);
	juce::SynthesiserSound::Ptr newSound = new juce::SamplerSound{
		"Voice", *formatReader, midiNotes,
		SAMPLE_NOTE, ATTACK_TIME, RELEASE_TIME, MAX_NOTE_LENGTH };

	this->synth->removeSound(0);

	sound = newSound;

	this->synth->addSound(sound);
}
