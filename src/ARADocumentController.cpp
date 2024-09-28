#include "ARADocumentController.h"
#include "ARAArchieveIO.h"
#include "ARARenderer.h"
#include "ARAContextReader.h"
#include <ARASharedObject.h>

ARADocumentController::ARADocumentController(
	const ARA::PlugIn::PlugInEntry* entry,
	const ARA::ARADocumentControllerHostInstance* instance)
	: ARADocumentControllerSpecialisation(entry, instance) {
	this->renderer = std::make_unique<ARARenderThread>(this->context);
}

void ARADocumentController::willBeginEditing(juce::ARADocument* /*document*/) {
	this->stopRender();
}

void ARADocumentController::didEndEditing(juce::ARADocument* document) {
	this->startRender(document);
}

bool ARADocumentController::doRestoreObjectsFromStream(
	juce::ARAInputStream& input,
	const juce::ARARestoreObjectsFilter* filter) {
	auto reportProgress = [archivingController = this->getDocumentController()->getHostArchivingController()](float p) {
		archivingController->notifyDocumentUnarchivingProgress(p);
		};

	ARAArchieveReader reader(filter, reportProgress);
	return reader.read(input);
}

bool ARADocumentController::doStoreObjectsToStream(
	juce::ARAOutputStream& output,
	const juce::ARAStoreObjectsFilter* filter) {
	auto reportProgress = [archivingController = this->getDocumentController()->getHostArchivingController()](float p) {
			archivingController->notifyDocumentArchivingProgress(p);
		};

	ARAArchieveWriter writer(filter, reportProgress);
	return writer.write(output);
}

juce::ARAPlaybackRenderer* ARADocumentController::doCreatePlaybackRenderer() {
	return new ARAPlaybackRenderer{
		this->getDocumentController(),
		this->context, *(this->renderer) };
}

juce::ARAEditorRenderer* ARADocumentController::doCreateEditorRenderer() {
	return new ARAEditorRenderer{
		this->getDocumentController(),
		this->context, *(this->renderer) };
}

bool ARADocumentController::doIsPlaybackRegionContentAvailable(
	const ARA::PlugIn::PlaybackRegion* /*playbackRegion*/,
	ARA::ARAContentType type) {
	return type == ARAExtension::ARAContentTypeNote
		|| type == ARAExtension::ARAContentTypePitchWheel;
}

ARA::ARAContentGrade ARADocumentController::doGetPlaybackRegionContentGrade(
	const ARA::PlugIn::PlaybackRegion* /*playbackRegion*/,
	ARA::ARAContentType /*type*/) {
	return ARA::kARAContentGradeInitial;
}

ARA::PlugIn::ContentReader* ARADocumentController::doCreatePlaybackRegionContentReader(
	ARA::PlugIn::PlaybackRegion* playbackRegion,
	ARA::ARAContentType type, const ARA::ARAContentTimeRange* /*range*/) {
	switch (type) {
	case ARAExtension::ARAContentTypeNote:
		return new ARAContextNoteReader{
			playbackRegion->getRegionSequence()->getMusicalContext<juce::ARAMusicalContext>() };
	case ARAExtension::ARAContentTypePitchWheel:
		return new ARAContextPitchReader{
			playbackRegion->getRegionSequence()->getMusicalContext<juce::ARAMusicalContext>() };
	}

	return nullptr;
}

void ARADocumentController::doGetPlaybackRegionHeadAndTailTime(
	const ARA::PlugIn::PlaybackRegion* playbackRegion,
	ARA::ARATimeDuration* headTime, ARA::ARATimeDuration* tailTime) {
	(*headTime) = playbackRegion->getStartInPlaybackTime();
	(*tailTime) = playbackRegion->getEndInPlaybackTime();
}

void ARADocumentController::stopRender() {
	this->renderer->stopSafety();
}

void ARADocumentController::startRender(juce::ARADocument* document) {
	/** Get Sequence */
	auto& sequences = document->getRegionSequences();

	/** Update Context */
	this->context.setSequenceData(
		(sequences.size() > 0) ? sequences.front() : nullptr);

	/** Render */
	this->renderer->startSafety();
}

const ARA::ARAFactory* JUCE_CALLTYPE createARAFactory() {
	return juce::ARADocumentControllerSpecialisation::createARAFactory<ARADocumentController>();
}
