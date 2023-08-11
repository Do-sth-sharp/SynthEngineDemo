#pragma once

#include <JuceHeader.h>

class EngineDemoEditor final : public juce::AudioProcessorEditor {
public:
	explicit EngineDemoEditor(juce::AudioProcessor& processor);

	void resized() override;
	void paint(juce::Graphics& g) override;

private:
	std::unique_ptr<juce::TextEditor> infoEditor = nullptr;

	const juce::Rectangle<int> getContentArea() const;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EngineDemoEditor)
};
