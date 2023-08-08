#include "EngineRenderer.h"

EngineRenderer::EngineRenderer(int KPSSeed) {
	/** KPS Device */
	auto device = KarPlusStrong::Device::Normal;
	if (juce::SystemStats::hasSSE3()) {
		device = KarPlusStrong::Device::SSE3;
	}
	if (juce::SystemStats::hasAVX2()) {
		device = KarPlusStrong::Device::AVX2;
	}
	if (juce::SystemStats::hasAVX512F()) {
		device = KarPlusStrong::Device::AVX512;
	}

	/** KPS Renderer */
	this->kps = std::make_unique<KarPlusStrong>(KPSSeed, device);
}
