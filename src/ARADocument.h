#pragma once

#include <JuceHeader.h>
#include "ARAContext.h"
#include "ARARenderThread.h"

class ARADocument final : public juce::ARADocument {
public:
	explicit ARADocument(ARA::PlugIn::DocumentController* documentController);

	void stopRender();
	void startRender();

	void addRenderStateListener(juce::ChangeListener* listener);
	void removeRenderStateListener(juce::ChangeListener* listener);

private:
	friend class ARAPlaybackRenderer;
	friend class ARAEditorRenderer;
	ARAContext& getContext();
	ARARenderThread& getRenderer();

private:
	ARAContext context;
	std::unique_ptr<ARARenderThread> renderer = nullptr;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ARADocument)
};
