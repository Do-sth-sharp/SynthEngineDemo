#pragma once

#include <JuceHeader.h>

class ARAArchieveWriter final {
public:
	using ProgressFunc = std::function<void(float)>;
	ARAArchieveWriter(
		const juce::ARAStoreObjectsFilter* filter,
		const ProgressFunc& reportProgress);

	bool write(juce::ARAOutputStream& stream);

private:
	const juce::ARAStoreObjectsFilter* const filter;
	const ProgressFunc reportProgress;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ARAArchieveWriter)
};

class ARAArchieveReader final {
public:
	using ProgressFunc = std::function<void(float)>;
	ARAArchieveReader(
		const juce::ARARestoreObjectsFilter* filter,
		const ProgressFunc& reportProgress);

	bool read(juce::ARAInputStream& stream);

private:
	const juce::ARARestoreObjectsFilter* const filter;
	const ProgressFunc reportProgress;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ARAArchieveReader)
};
