#include <JuceHeader.h>

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

	/** TODO Read MIDI, synth and save wave */
}

int main(int argc, char* argv[]) {
	/** Application */
	juce::ConsoleApplication app;

	/** Default Commands */
	app.addVersionCommand("--version|-v",
		juce::String(ProjectInfo::projectName) + " " + juce::String(ProjectInfo::versionString));
	app.addHelpCommand("--help|-h",
		"Usage:\n"
		"\tTestKarPlusStrong [options] -i <MIDI File> <WAVE File>\n"
		"Or use default output file:\n"
		"\tTestKarPlusStrong [options] -i <MIDI File>\n", true);

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

