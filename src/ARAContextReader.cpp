#include "ARAContextReader.h"

ARAContextReaderBase::ARAContextReaderBase(
	const juce::ARAMusicalContext* context,
	ARAExtension::ARAContentType type)
	: context(context->getHostRef()), controller(context->getDocument()->getDocumentController()->getHostContentAccessController()),
	available(this->controller ? this->controller->isMusicalContextContentAvailable(this->context, type) : false),
	grade(this->available ? this->controller->getMusicalContextContentGrade(this->context, type) : ARA::kARAContentGradeInitial),
	reader(this->available ? this->controller->createMusicalContextContentReader(this->context, type, nullptr) : nullptr),
	count(this->available ? this->controller->getContentReaderEventCount(this->reader) : 0) {}

int32_t ARAContextReaderBase::getEventCount() noexcept {
	return this->count;
}

const void* ARAContextReaderBase::getDataForEvent(int32_t eventIndex) noexcept {
	ARA_INTERNAL_ASSERT(eventIndex < this->count);

	return this->controller->getContentReaderDataForEvent(this->reader, eventIndex);
}

ARAContextNoteReader::ARAContextNoteReader(const juce::ARAMusicalContext* context)
	: ARAContextReaderBase(context, ARAExtension::ARAContentTypeNote) {}

ARAContextPitchReader::ARAContextPitchReader(const juce::ARAMusicalContext* context)
	: ARAContextReaderBase(context, ARAExtension::ARAContentTypePitchWheel) {}
