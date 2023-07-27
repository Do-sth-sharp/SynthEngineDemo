#pragma once

#include <JuceHeader.h>

class EngineDemoEditor final : public juce::AudioProcessorEditor {
public:
	EngineDemoEditor() = delete;
	explicit EngineDemoEditor(juce::AudioProcessor& processor);

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EngineDemoEditor)
};
