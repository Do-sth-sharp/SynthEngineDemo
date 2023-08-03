#include <JuceHeader.h>

#include "../KarPlusStrong.h"

#define KPS_SEED 114514191

inline void writeProgressBar(
	int totalEvents, int currentEvent, double timeSpent) {
	/** Get Progress Num */
	double percent = (totalEvents == 0)
		? 100 : (static_cast<double>(currentEvent) / totalEvents) * 100;
	double eventsPerSeconds = currentEvent / timeSpent;

	/** Get Current Bar */
	juce::String barStr;
	barStr += "    [\033[32m";
	barStr += juce::String{}.paddedRight('=', std::floor(percent / 2.5));
	barStr += juce::String{}.paddedRight(' ', 40 - std::floor(percent / 2.5));
	barStr += ("\033[0m] \033[33m"+juce::String(percent, 2) + "%\033[0m \033[32m"
		+ juce::String(currentEvent) + "\033[0m/" + juce::String(totalEvents)
		+ ", Time Spent: \033[35m" + juce::String(timeSpent, 3) + "\033[0ms"
		+ ", Speed: \033[36m" + juce::String(eventsPerSeconds, 2) + "\033[0meps");

	/** Clear Last Bar */
	static int lastStrLength = 0;
	int strLength = barStr.length();
	if (strLength < lastStrLength) {
		printf("\r");
		for (int i = 0; i < lastStrLength; i++) {
			printf(" ");
		}
	}
	lastStrLength = strLength;

	/** Print Progress Bar */
	printf("\r%s", barStr.toStdString().c_str());

	/** End Progress */
	if (percent == 100) {
		printf("\n");
	}

	/** Flush Output */
	fflush(stdout);
}

void synthTrack(const juce::MidiMessageSequence* track,
	juce::AudioSampleBuffer& buffer, int sampleRate) {
	/** Check Args */
	if (!track) { return; }

	/** Total Events */
	int totalEvents = track->getNumEvents();

	/** KPS Renderer */
	KarPlusStrong kps(KPS_SEED);

	/** Time Count */
	clock_t start = clock();

	/** Synth */
	for (int i = 0; i < totalEvents; i++) {
		/** Update Progress Bar */
		writeProgressBar(
			totalEvents, i, static_cast<double>(clock() - start) / CLOCKS_PER_SEC);

		/** Get Current Note */
		auto event = track->getEventPointer(i);
		if (!event->message.isNoteOn()) { continue; }
		auto endEvent = event->noteOffObject;

		/** Get Note Info */
		double startTime = event->message.getTimeStamp();
		double endTime =
			endEvent ? endEvent->message.getTimeStamp() : track->getEndTime();
		int startSample = startTime * sampleRate;
		int endSample = endTime * sampleRate;
		int noteNumber = event->message.getNoteNumber();

		/** Synth */
		if (startSample >= buffer.getNumSamples()) { continue; }
		int noteLength = std::min(
			endSample - startSample, buffer.getNumSamples() - 1 - startSample);
		double freq = 440 * std::pow(2, static_cast<double>(noteNumber - 69) / 12);
		kps.synth(buffer, sampleRate, startSample,
			noteLength, { freq }, noteLength, 0);
	}

	/** End Progress Bar */
	writeProgressBar(
		totalEvents, totalEvents, static_cast<double>(clock() - start) / CLOCKS_PER_SEC);
}

bool synth(const juce::File& input, const juce::File& output,
	int sampleRate, int bitDepth) {
	/** Read Midi */
	juce::MidiFile midi;
	{
		juce::FileInputStream inputStream(input);
		if (inputStream.failedToOpen()) {
			printf("Can't create input stream!\n");
			return false;
		}
		if (!midi.readFrom(inputStream)) {
			printf("Can't read MIDI file!\n");
			return false;
		}
	}

	/** Synth Info */
	midi.convertTimestampTicksToSeconds();
	double totalTime = midi.getLastTimestamp();
	int bufferSize = std::ceil(totalTime * sampleRate);
	printf("Total Time: %.3fs\n", totalTime);
	printf("MIDI Track Num: %d\n", midi.getNumTracks());
	printf("Buffer Size: %d\n", bufferSize);

	/** Audio Buffer */
	juce::AudioSampleBuffer buffer(1, bufferSize);
	for (int i = 0; i < bufferSize; i++) {
		buffer.setSample(0, i, 0);
	}

	/** Synth Each Track */
	for (int i = 0; i < midi.getNumTracks(); i++) {
		/** Get Track */
		auto track = midi.getTrack(i);

		/** Track Info */
		printf("MIDI Track %d:\n", i);
		printf("    MIDI Event Num: %d, Start Time: %.3fs, End Time: %.3fs\n",
			track->getNumEvents(), track->getStartTime(), track->getEndTime());

		/** Synth Track */
		synthTrack(track, buffer, sampleRate);
	}

	/** Save Wave File */
	{
		auto outputStream = std::make_unique<juce::FileOutputStream>(output);
		if (outputStream->failedToOpen()) {
			printf("Can't create output stream!\n");
			return false;
		}
		outputStream->setPosition(0);
		outputStream->truncate();
		
		juce::WavAudioFormat wavFormat;
		auto writer = std::unique_ptr<juce::AudioFormatWriter>(
			wavFormat.createWriterFor(outputStream.release(), sampleRate,
				juce::AudioChannelSet::mono(), bitDepth, juce::StringPairArray{}, 0));
		if (!writer) {
			printf("Can't create wave writer!\n");
			return false;
		}

		printf("Saving audio data...\n");
		if (!writer->writeFromAudioSampleBuffer(buffer, 0, bufferSize)) {
			printf("Can't write wave file!\n");
			return false;
		}
		printf("Audio data saved!\n");
	}

	return true;
}

void parseCommand(const juce::ArgumentList& args) {
	/** Check Arg */
	args.failIfOptionIsMissing("-i");

	/** Get IO Arg */
	int argSize = args.size();
	int inputIndex = args.indexOfOption("-i");
	juce::File inputFile = args.getFileForOption("-i");
	juce::File outputFile(inputFile.getFullPathName() + ".wav");
	if (argSize - 1 > inputIndex + 1) {
		auto lastArg = args[argSize - 1];
		outputFile = lastArg.resolveAsFile();
	}

	/** Get Options */
	juce::WavAudioFormat format;
	int sampleRate = 48000, bitDepth = 24;
	auto availableSampleRates = format.getPossibleSampleRates();
	auto availableBitDepths = format.getPossibleBitDepths();
	int sampleRateValue = args.getValueForOption("--samplerate").getIntValue();
	int bitDepthValue = args.getValueForOption("--bitdepth").getIntValue();
	if (availableSampleRates.contains(sampleRateValue)) {
		sampleRate = sampleRateValue;
	}
	if (availableBitDepths.contains(bitDepthValue)) {
		bitDepth = bitDepthValue;
	}

	/** Check File Name */
	if (!inputFile.existsAsFile()) {
		juce::ConsoleApplication::fail(
			"Unexisting input file: " + inputFile.getFullPathName());
	}
	if (inputFile.getFileExtension() != ".mid") {
		juce::ConsoleApplication::fail(
			"Input file is not MIDI: " + inputFile.getFullPathName());
	}
	if (outputFile.getFileExtension() != ".wav") {
		juce::ConsoleApplication::fail(
			"Output file is not wave: " + outputFile.getFullPathName());
	}

	/** Print Informations */
	printf("Input File: %s\n", inputFile.getFullPathName().toStdString().c_str());
	printf("Output File: %s\n", outputFile.getFullPathName().toStdString().c_str());
	printf("Sample Rate: %d\n", sampleRate);
	printf("Bit Depth: %d\n", bitDepth);

	/** Read MIDI, synth and save wave */
	printf("=====Synth=====\n");
	if (!synth(inputFile, outputFile, sampleRate, bitDepth)) {
		printf("=====Synth=====\n");
		juce::ConsoleApplication::fail("Synth Error!");
	}
}

int main(int argc, char* argv[]) {
	/** Application */
	juce::ConsoleApplication app;

	/** Default Commands */
	app.addVersionCommand("--version|-v",
		juce::String(ProjectInfo::projectName) + " " + juce::String(ProjectInfo::versionString));
	app.addHelpCommand("--help|-h",
		"Usage:\n"
		"    TestKarPlusStrong [options] -i <MIDI File> <WAVE File>\n"
		"Or use default output file:\n"
		"    TestKarPlusStrong [options] -i <MIDI File>\n", true);

	/** Command */
	app.addCommand({ "-i",
		"-i <MIDI File>",
		"Set MIDI file input.",
		"Set MIDI file input.",
		::parseCommand
		});
	app.addCommand({ "--samplerate",
		"--samplerate=<Sample Rate>",
		"Set audio sample rate.",
		"Set audio sample rate. Default is 48000(Hz).\n"
		"Available sample rates: 8000, 11025, 12000, 16000,  22050,  32000,  44100, "
		"48000, 88200, 96000, 176400, 192000, 352800, 384000",
		::parseCommand
		});
	app.addCommand({ "--bitdepth",
		"--bitdepth=<Bit Depth>",
		"Set audio bit depth.",
		"Set audio bit depth. Default is 24(bit).\n"
		"Available bit depths: 8, 16, 24, 32",
		::parseCommand
		});

	/** Run Application */
	return app.findAndRunCommand(argc, argv);
}

