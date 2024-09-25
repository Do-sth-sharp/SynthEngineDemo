﻿#pragma once

#include <JuceHeader.h>

class ARAContext final {
public:
	ARAContext() = default;

	void setData(const juce::ARAMusicalContext* context);
	const juce::MidiMessageSequence getData() const;

private:
	juce::ReadWriteLock contextLock;
	juce::MidiMessageSequence sequence;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ARAContext)
};
