#pragma once

#include <JuceHeader.h>
#include <ARASharedObject.h>

class ARAContext final {
public:
	ARAContext() = default;

	using NoteList = juce::Array<ARA::ARAContentNote>;
	using PitchList = juce::Array<ARAExtension::ARAContentIntParam>;
	void setData(
		ARA::PlugIn::ContentReader* noteReader,
		ARA::PlugIn::ContentReader* pitchReader);
	const std::tuple<NoteList, PitchList, double> getData() const;
	double getLength() const;

private:
	juce::ReadWriteLock contextLock;
	NoteList notes;
	PitchList pitchs;
	double totalLength = 0;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ARAContext)
};
