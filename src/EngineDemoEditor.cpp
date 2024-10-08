﻿#include "EngineDemoEditor.h"
#include "EditorStatusModel.h"

EngineDemoEditor::EngineDemoEditor(
	juce::AudioProcessor& processor,
	EditorStatusModel& model)
	: AudioProcessorEditor(processor),
	AudioProcessorEditorARAExtension(&processor), model(model) {
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

	/** Data Editor */
	this->dataEditor = std::make_unique<juce::TextEditor>();
	this->dataEditor->setMultiLine(true);
	this->dataEditor->setWantsKeyboardFocus(true);
	this->dataEditor->setTabKeyUsedAsCharacter(true);
	this->dataEditor->setReturnKeyStartsNewLine(true);
	this->dataEditor->onTextChange = [this] { this->onDataTextChanged(); };
	this->addAndMakeVisible(this->dataEditor.get());

	/** Listener */
	model.addChangeListener(this);
	model.sendChangeMessage();

	/** Min Size */
	this->setResizable(true, false);
	this->setResizeLimits(480, 270, INT_MAX, INT_MAX);
	this->setSize(480, 270);
}

void EngineDemoEditor::resized() {
	/** Content Area */
	auto area = this->getContentArea();
	bool contentVertical = area.getHeight() >= area.getWidth();

	/** Data Size */
	juce::Rectangle<int> statusArea = contentVertical
		? area.withTrimmedBottom(area.getHeight() / 2)
		: area.withTrimmedRight(area.getWidth() / 2);
	juce::Rectangle<int> dataArea = statusArea.withTrimmedTop(statusArea.getHeight() / 2);
	this->dataEditor->setBounds(dataArea);

	/** Info Size */
	juce::Rectangle<int> infoArea = contentVertical
		? area.withTrimmedTop(area.getHeight() / 2)
		: area.withTrimmedLeft(area.getWidth() / 2);
	this->infoEditor->setBounds(infoArea);
}

void EngineDemoEditor::paint(juce::Graphics& g) {
	/** LAF */
	auto& laf = juce::LookAndFeel::getDefaultLookAndFeel();

	/** Content Area */
	auto area = this->getContentArea();
	bool contentVertical = area.getHeight() >= area.getWidth();

	/** Fill Content Area */
	g.setColour(laf.findColour(juce::ResizableWindow::ColourIds::backgroundColourId));
	g.fillRect(area);

	/** Text Area */
	juce::Rectangle<int> textArea = contentVertical
		? area.withTrimmedBottom(area.getHeight() / 2)
		: area.withTrimmedRight(area.getWidth() / 2);

	/** Text Size */
	int textLineAreaHeight = textArea.getHeight() / 10;
	{
		juce::Label label;
		juce::Font labelFont = label.getFont();
		int fontHeight = labelFont.getHeight();
		textLineAreaHeight = fontHeight * 1.5;
	}

	/** ARA Status Area */
	juce::Rectangle<int> ARAStatusArea = textArea.withHeight(textLineAreaHeight);
	{
		juce::String ARAStatusStr = "ARA Disconnected";
		juce::Colour ARAStatusColour = juce::Colours::red;
		switch (this->model.getARA()){
		case EditorStatusModel::ARAStatus::Connected:
			ARAStatusStr = "ARA Connected";
			ARAStatusColour = juce::Colours::green;
			break;
		}

		g.setColour(ARAStatusColour);
		g.drawFittedText(
			ARAStatusStr, ARAStatusArea, juce::Justification::centred, 1, 0.5);
	}

	/** Render Status Area */
	juce::Rectangle<int> renderStatusArea
		= textArea.withTrimmedTop(
			ARAStatusArea.getHeight()).withHeight(textLineAreaHeight);
	if (this->model.getARA() == EditorStatusModel::ARAStatus::Connected) {
		juce::String renderStatusStr = "Unrendered";
		switch (this->model.getRendered()) {
		case EditorStatusModel::RenderStatus::Rendering:
			renderStatusStr = "Rendering";
			break;
		case EditorStatusModel::RenderStatus::Rendered:
			renderStatusStr = "Rendered";
			break;
		}

		g.setColour(laf.findColour(juce::Label::ColourIds::textColourId));
		g.drawFittedText(
			renderStatusStr, renderStatusArea, juce::Justification::centred, 1, 0.5);
	}
}

void EngineDemoEditor::changeListenerCallback(juce::ChangeBroadcaster* /*source*/) {
	/** Set Text */
	this->infoEditor->setReadOnly(false);
	this->infoEditor->setText(this->model.getContextInfo(), false);
	this->infoEditor->setReadOnly(true);

	this->dataEditor->setText(this->model.getData(), false);

	/** Repaint */
	this->repaint();
}

void EngineDemoEditor::onDataTextChanged() {
	this->model.setData(this->dataEditor->getText(), false);
}

const juce::Rectangle<int> EngineDemoEditor::getContentArea() const {
	/*int width = this->getWidth(), height = this->getHeight();
	if (width > 2 * height) {
		width = 2 * height;
	}
	else if (height > 2 * width) {
		height = 2 * width;
	}
	return this->getLocalBounds().withSizeKeepingCentre(width, height);*/
	return this->getLocalBounds();
}
