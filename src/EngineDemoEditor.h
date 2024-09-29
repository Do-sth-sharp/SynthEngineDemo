#pragma once

#include <JuceHeader.h>

class EditorStatusModel;

class EngineDemoEditor final
	: public juce::AudioProcessorEditor,
	public juce::AudioProcessorEditorARAExtension,
	private juce::ChangeListener {
public:
	explicit EngineDemoEditor(
		juce::AudioProcessor& processor,
		EditorStatusModel& model);

	void resized() override;
	void paint(juce::Graphics& g) override;

private:
	EditorStatusModel& model;
	std::unique_ptr<juce::TextEditor> infoEditor = nullptr;
	std::unique_ptr<juce::TextEditor> dataEditor = nullptr;

	void changeListenerCallback(juce::ChangeBroadcaster* source) override;

	void onDataTextChanged();

	const juce::Rectangle<int> getContentArea() const;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EngineDemoEditor)
};
