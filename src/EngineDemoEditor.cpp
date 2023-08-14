#include "EngineDemoEditor.h"

EngineDemoEditor::EngineDemoEditor(juce::AudioProcessor& processor)
	: AudioProcessorEditor(processor) {
	/** Info Editor */
	this->infoEditor = std::make_unique<juce::TextEditor>();
	this->infoEditor->setMultiLine(true);
	this->infoEditor->setReadOnly(true);
	this->infoEditor->setWantsKeyboardFocus(false);
	this->infoEditor->setTabKeyUsedAsCharacter(true);
	this->infoEditor->setTextToShowWhenEmpty("No Info",
		juce::LookAndFeel::getDefaultLookAndFeel().findColour(
			juce::TextEditor::ColourIds::textColourId));
	this->addAndMakeVisible(this->infoEditor.get());

	/** Min Size */
	this->setResizeLimits(480, 270, INT_MAX, INT_MIN);
	this->setSize(480, 270);
}

void EngineDemoEditor::resized() {
	/** Content Area */
	auto area = this->getContentArea();
	bool contentVertical = area.getHeight() >= area.getWidth();

	/** Info Size */
	juce::Rectangle<int> infoArea = contentVertical
		? area.withTrimmedTop(area.getHeight() / 2)
		: area.withTrimmedLeft(area.getWidth() / 2);
	if (this->infoEditor) {
		this->infoEditor->setBounds(infoArea);
	}
}

void EngineDemoEditor::paint(juce::Graphics& g) {
	/** LAF */
	auto& laf = juce::LookAndFeel::getDefaultLookAndFeel();

	/** Content Area */
	auto area = this->getContentArea();
	bool contentVertical = area.getHeight() >= area.getWidth();

	/** Fill A EngineDemoEditor::rea */
	g.setColour(laf.findColour(juce::ResizableWindow::ColourIds::backgroundColourId));
	g.fillRect(area);

	/** Text Area */
	juce::Rectangle<int> textArea = contentVertical
		? area.withTrimmedBottom(area.getHeight() / 2)
		: area.withTrimmedRight(area.getWidth() / 2);
}

void EngineDemoEditor::setWaveShaked(bool waveShaked) {
	this->waveShaked = waveShaked;
	this->repaint();
}

void EngineDemoEditor::setRendered(bool rendered) {
	this->rendered = rendered;
	this->repaint();
}

void EngineDemoEditor::setMidiInfo(const EngineDemoEditor::MidiInfo& info) {
	/** TODO Update Info */
}

const juce::Rectangle<int> EngineDemoEditor::getContentArea() const {
	int width = this->getWidth(), height = this->getHeight();
	if (width > 2 * height) {
		width = 2 * height;
	}
	else if (height > 2 * width) {
		height = 2 * width;
	}
	return this->getLocalBounds().withSizeKeepingCentre(width, height);
}
