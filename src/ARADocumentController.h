#pragma once

#include <JuceHeader.h>
#include "ARAContext.h"
#include "ARARenderThread.h"

class ARADocumentController final : public juce::ARADocumentControllerSpecialisation {
public:
	ARADocumentController(
		const ARA::PlugIn::PlugInEntry* entry,
		const ARA::ARADocumentControllerHostInstance* instance);

	void willBeginEditing(juce::ARADocument* document) override;
	void didEndEditing(juce::ARADocument* document) override;

	bool doRestoreObjectsFromStream(
		juce::ARAInputStream& input,
		const juce::ARARestoreObjectsFilter* filter) override;
	bool doStoreObjectsToStream(
		juce::ARAOutputStream& output,
		const juce::ARAStoreObjectsFilter* filter) override;

	juce::ARAPlaybackRenderer* doCreatePlaybackRenderer() override;
	juce::ARAEditorRenderer* doCreateEditorRenderer() override;

	bool doIsPlaybackRegionContentAvailable(
		const ARA::PlugIn::PlaybackRegion* playbackRegion,
		ARA::ARAContentType type) override;
	ARA::ARAContentGrade doGetPlaybackRegionContentGrade(
		const ARA::PlugIn::PlaybackRegion* playbackRegion,
		ARA::ARAContentType type) override;
	ARA::PlugIn::ContentReader* doCreatePlaybackRegionContentReader(
		ARA::PlugIn::PlaybackRegion* playbackRegion,
		ARA::ARAContentType type, const ARA::ARAContentTimeRange* range) override;
	void doGetPlaybackRegionHeadAndTailTime(
		const ARA::PlugIn::PlaybackRegion* playbackRegion,
		ARA::ARATimeDuration* headTime, ARA::ARATimeDuration* tailTime) override;

private:
	ARAContext context;
	std::unique_ptr<ARARenderThread> renderer = nullptr;

	void stopRender();
	void startRender(juce::ARADocument* document);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ARADocumentController)
};
