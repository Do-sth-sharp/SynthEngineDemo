#pragma once

#include <JuceHeader.h>

class EngineDemoEditor final : public juce::AudioProcessorEditor {
public:
	explicit EngineDemoEditor(juce::AudioProcessor& processor);

	void resized() override;
	void paint(juce::Graphics& g) override;

	using MidiInfo = std::tuple<int, double, int>;
	void setWaveShaked(bool waveShaked);
	void setRendered(bool rendered);
	void setMidiInfo(const MidiInfo& info);

private:
	std::unique_ptr<juce::TextEditor> infoEditor = nullptr;
	bool waveShaked = false, rendered = false;

	const juce::Rectangle<int> getContentArea() const;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EngineDemoEditor)
};
