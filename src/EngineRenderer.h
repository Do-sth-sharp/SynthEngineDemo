#pragma once

#include <JuceHeader.h>
#include "KarPlusStrong.h"

class EngineRenderer final {
public:
	EngineRenderer() = delete;
	explicit EngineRenderer(int KPSSeed = 114514191);

private:
	std::unique_ptr<KarPlusStrong> kps = nullptr;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EngineRenderer)
};
