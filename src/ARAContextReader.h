#pragma once

#include <JuceHeader.h>
#include <ARASharedObject.h>

class ARAContextReaderBase : public ARA::PlugIn::ContentReader {
public:
    ARAContextReaderBase() = delete;
    ARAContextReaderBase(
        const juce::ARAMusicalContext* context,
        ARAExtension::ARAContentType type);
    virtual ~ARAContextReaderBase() = default;

    int32_t getEventCount() noexcept override;
    const void* getDataForEvent(int32_t eventIndex) noexcept override;

private:
    const ARA::ARAMusicalContextHostRef context;
    ARA::PlugIn::HostContentAccessController* const controller;

    const bool available;
    const ARA::ARAContentGrade grade;
    const ARA::ARAContentReaderHostRef reader;
    const int32_t count;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ARAContextReaderBase)
};

class ARAContextNoteReader final : public ARAContextReaderBase {
public:
    ARAContextNoteReader() = delete;
    ARAContextNoteReader(const juce::ARAMusicalContext* context);

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ARAContextNoteReader)
};

class ARAContextPitchReader final : public ARAContextReaderBase {
public:
    ARAContextPitchReader() = delete;
    ARAContextPitchReader(const juce::ARAMusicalContext* context);

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ARAContextPitchReader)
};
