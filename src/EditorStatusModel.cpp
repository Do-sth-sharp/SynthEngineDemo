#include "EditorStatusModel.h"

void EditorStatusModel::setARA(ARAStatus status) {
	this->ara = status;
	this->sendChangeMessage();
}

void EditorStatusModel::setRendered(RenderStatus status) {
	this->rendered = status;
	this->sendChangeMessage();
}

void EditorStatusModel::setContextInfo(const ContextInfo& info) {
	this->infoStr.clear();

	/** Regions */
	this->infoStr += "Region Num: " + juce::String{ info.regions.size() } + "\n";
	this->infoStr += "Region List: \n";
	for (auto [startTime, endTime] : info.regions) {
		this->infoStr += "    " + juce::String{ startTime, 2 } + " - " + juce::String{ endTime, 2 } + "\n";
	}
	this->infoStr += "\n";

	/** Context */
	this->infoStr += "Context Length: " + juce::String{ info.contextLength } + "\n";
	this->infoStr += "Note Num: " + juce::String{ info.noteNum } + "\n";
	this->infoStr += "Pitch Num: " + juce::String{ info.pitchNum } + "\n";
	this->infoStr += "\n";

	/** Playback */
	this->infoStr += "Sample Rate: " + juce::String{ info.sampleRate, 0 } + "\n";
	this->infoStr += "Block Size: " + juce::String{ info.blockSize } + "\n";
	this->infoStr += "Input Channel Num: " + juce::String{ info.channelNumInput } + "\n";
	this->infoStr += "Output Channel Num: " + juce::String{ info.channelNumOutput } + "\n";
	this->infoStr += "\n";
}

void EditorStatusModel::clearContextInfo() {
	this->infoStr.clear();
}

EditorStatusModel::ARAStatus EditorStatusModel::getARA() const {
	return this->ara;
}

EditorStatusModel::RenderStatus EditorStatusModel::getRendered() const {
	return this->rendered;
}

juce::String EditorStatusModel::getContextInfo() const {
	return this->infoStr;
}
