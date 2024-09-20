#include "ARADocumentController.h"
#include "ARAArchieveIO.h"

ARADocumentController::ARADocumentController(
	const ARA::PlugIn::PlugInEntry* entry,
	const ARA::ARADocumentControllerHostInstance* instance)
	: ARADocumentControllerSpecialisation(entry, instance) {};

bool ARADocumentController::doRestoreObjectsFromStream(
	juce::ARAInputStream& input,
	const juce::ARARestoreObjectsFilter* filter) {
	ARAArchieveReader reader(filter);
	return reader.read(input);
}

bool ARADocumentController::doStoreObjectsToStream(
	juce::ARAOutputStream& output,
	const juce::ARAStoreObjectsFilter* filter) {
	ARAArchieveWriter writer(filter);
	return writer.write(output);
}

juce::ARAPlaybackRenderer* ARADocumentController::doCreatePlaybackRenderer() {
	/** TODO */
	return nullptr;
}

const ARA::ARAFactory* JUCE_CALLTYPE createARAFactory() {
	return juce::ARADocumentControllerSpecialisation::createARAFactory<ARADocumentController>();
}
