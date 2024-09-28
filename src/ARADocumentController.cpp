#include "ARADocumentController.h"
#include "ARAArchieveIO.h"
#include "ARARenderer.h"
#include "ARAContextReader.h"
#include "ARADocument.h"
#include <ARASharedObject.h>

void ARADocumentController::willBeginEditing(juce::ARADocument* document) {
	if (auto ptr = dynamic_cast<ARADocument*>(document)) {
		ptr->stopRender();
	}
}

void ARADocumentController::didEndEditing(juce::ARADocument* document) {
	if (auto ptr = dynamic_cast<ARADocument*>(document)) {
		ptr->startRender();
	}
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

juce::ARADocument* ARADocumentController::doCreateDocument() {
	return new ARADocument{ this->getDocumentController() };
}

juce::ARAPlaybackRenderer* ARADocumentController::doCreatePlaybackRenderer() {
	return new ARAPlaybackRenderer{
		this->getDocumentController() };
}

juce::ARAEditorRenderer* ARADocumentController::doCreateEditorRenderer() {
	return new ARAEditorRenderer{
		this->getDocumentController() };
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

const ARA::ARAFactory* JUCE_CALLTYPE createARAFactory() {
	return juce::ARADocumentControllerSpecialisation::createARAFactory<ARADocumentController>();
}
