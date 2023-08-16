#pragma once

#include <JuceHeader.h>

class EngineDemoEditor final : public juce::AudioProcessorEditor {
public:
	explicit EngineDemoEditor(juce::AudioProcessor& processor);

	void resized() override;
	void paint(juce::Graphics& g) override;

	enum class HandShakeStatus {
		Disconnected = 0,
		Connected = 1
	};
	enum class RenderStatus {
		Unrendered = 0,
		Rendering = 1,
		Rendered = 2
	};
	using MidiInfo = std::tuple<int, double, int>;

	void setHandShaked(HandShakeStatus status);
	void setRendered(RenderStatus status);
	void setMidiInfo(const MidiInfo& info);
	void clearMidiInfo();

private:
	std::unique_ptr<juce::TextEditor> infoEditor = nullptr;
	HandShakeStatus handShaked = HandShakeStatus::Disconnected;
	RenderStatus rendered = RenderStatus::Unrendered;

	const juce::Rectangle<int> getContentArea() const;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EngineDemoEditor)
};
