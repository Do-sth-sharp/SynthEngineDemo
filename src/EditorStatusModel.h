#pragma once

#include <JuceHeader.h>

class EditorStatusModel final
	: public juce::ChangeBroadcaster {
public:
	EditorStatusModel() = default;

	enum class ARAStatus {
		Disconnected = 0,
		Connected = 1
	};
	enum class RenderStatus {
		Unrendered = 0,
		Rendering = 1,
		Rendered = 2
	};

	struct ContextInfo {
		/** StartTimeInSequence, StartTimeInContext, Length */
		using TimeRangeMap = std::tuple<double, double, double>;
		juce::Array<TimeRangeMap> regions;

		double contextLength;
		int noteNum;
		int pitchNum;

		double sampleRate;
		int blockSize;
		int channelNumInput;
		int channelNumOutput;
	};

	void setARA(ARAStatus status);
	void setRendered(RenderStatus status);
	void setContextInfo(const ContextInfo& info);
	void clearContextInfo();

	ARAStatus getARA() const;
	RenderStatus getRendered() const;
	juce::String getContextInfo() const;

private:
	ARAStatus ara = ARAStatus::Disconnected;
	RenderStatus rendered = RenderStatus::Unrendered;
	juce::String infoStr;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EditorStatusModel)
};
