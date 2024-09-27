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

	/** Generate Text */
	auto& [trackNum, totalLength, totalEvents] = info;
	this->infoStr += "Track Num: " + juce::String(trackNum) + "\n";
	this->infoStr += "Total Length: " + juce::String(totalLength, 3) + "s\n";
	this->infoStr += "Total Events: " + juce::String(totalEvents) + "\n";
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
