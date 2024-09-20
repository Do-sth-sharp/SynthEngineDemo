#pragma once

#include <JuceHeader.h>

class ARADocumentController final : public juce::ARADocumentControllerSpecialisation {
public:
	ARADocumentController(
		const ARA::PlugIn::PlugInEntry* entry,
		const ARA::ARADocumentControllerHostInstance* instance);

	bool doRestoreObjectsFromStream(
		juce::ARAInputStream& input,
		const juce::ARARestoreObjectsFilter* filter) override;
	bool doStoreObjectsToStream(
		juce::ARAOutputStream& output,
		const juce::ARAStoreObjectsFilter* filter) override;

	juce::ARAPlaybackRenderer* doCreatePlaybackRenderer() override;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ARADocumentController)
};
