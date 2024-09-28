#include "ARADocument.h"

ARADocument::ARADocument(
	ARA::PlugIn::DocumentController* documentController)
	: juce::ARADocument::ARADocument(documentController) {
	this->renderer = std::make_unique<ARARenderThread>(this->context);
}

void ARADocument::stopRender() {
	this->renderer->stopSafety();
}

void ARADocument::startRender() {
	/** Get Sequence */
	auto& sequences = this->getRegionSequences();

	/** Update Context */
	this->context.setSequenceData(
		(sequences.size() > 0) ? sequences.front() : nullptr);

	/** Render */
	this->renderer->startSafety();
}

void ARADocument::addRenderStateListener(juce::ChangeListener* listener) {
	this->renderer->addChangeListener(listener);
}

void ARADocument::removeRenderStateListener(juce::ChangeListener* listener) {
	this->renderer->removeChangeListener(listener);
}

ARAContext& ARADocument::getContext() {
	return this->context;
}

ARARenderThread& ARADocument::getRenderer() {
	return *(this->renderer);
}
