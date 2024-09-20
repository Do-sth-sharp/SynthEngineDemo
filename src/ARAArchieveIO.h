#pragma once

#include <JuceHeader.h>

class ARAArchieveWriter final {
public:
	ARAArchieveWriter(const juce::ARAStoreObjectsFilter* filter);

	bool write(juce::ARAOutputStream& stream);

private:
	const juce::ARAStoreObjectsFilter* const filter;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ARAArchieveWriter)
};

class ARAArchieveReader final {
public:
	ARAArchieveReader(const juce::ARARestoreObjectsFilter* filter);

	bool read(juce::ARAInputStream& stream);

private:
	const juce::ARARestoreObjectsFilter* const filter;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ARAArchieveReader)
};
