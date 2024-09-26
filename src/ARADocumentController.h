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
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ARADocumentController)
};
