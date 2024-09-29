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
	for (auto [seqStartTime, contextStartTime, length] : info.regions) {
		this->infoStr += "    (" + juce::String{ seqStartTime, 2 } + ", " + juce::String{ seqStartTime + length, 2 }
			+ ") - (" + juce::String{ contextStartTime, 2 } + ", " + juce::String{ contextStartTime + length, 2 } + ")\n";
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

	this->sendChangeMessage();
}

void EditorStatusModel::clearContextInfo() {
	this->infoStr.clear();
	this->sendChangeMessage();
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
