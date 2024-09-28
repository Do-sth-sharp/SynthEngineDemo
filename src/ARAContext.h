#pragma once

#include <JuceHeader.h>
#include <ARASharedObject.h>

class ARAContext final {
public:
	ARAContext() = default;

	using NoteList = juce::Array<ARA::ARAContentNote>;
	using PitchList = juce::Array<ARAExtension::ARAContentIntParam>;
	using ContentReaderPtr = std::unique_ptr<ARA::PlugIn::ContentReader, std::function<void(ARA::PlugIn::ContentReader*)>>;
	void setContextData(ContentReaderPtr noteReader, ContentReaderPtr pitchReader);
	const std::tuple<NoteList, PitchList, double> getContextData() const;
	double getContextLength() const;

	void setSequenceData(
		const juce::ARARegionSequence* sequence);

	/** StartTimeInSequence, StartTimeInContext, Length */
	using TimeRangeMap = std::tuple<double, double, double>;
	using TimeRangeList = juce::Array<TimeRangeMap>;
	const TimeRangeList doMapTime(double startTime, double length) const;
	const TimeRangeList doMapTimeRealTime(double startTime, double length) const;

private:
	mutable juce::ReadWriteLock contextLock;
	NoteList notes;
	PitchList pitchs;
	double contextLength = 0;

	struct Region final {
		double startInSeq, endInSeq;
		double startInContext, endInContext;
	};
	using RegionList = juce::Array<Region>;
	RegionList regions;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ARAContext)
};
