#include "ARAContext.h"

void ARAContext::setData(
	ARA::PlugIn::ContentReader* noteReader,
	ARA::PlugIn::ContentReader* pitchReader) {
	/** Lock Context */
	juce::ScopedWriteLock locker(this->contextLock);

	/** Clear Data */
	this->notes.clear();
	this->pitchs.clear();
	this->totalLength = 0;

	/** Get Note Data */
	if (noteReader) {
		auto noteNum = noteReader->getEventCount();

		for (int32_t i = 0; i < noteNum; i++) {
			if (auto ptrData = static_cast<const ARA::ARAContentNote*>(noteReader->getDataForEvent(i))) {
				this->notes.add(*ptrData);

				this->totalLength = std::max(
					this->totalLength, ptrData->startPosition + ptrData->noteDuration);
			}
		}
	}

	/** Get Pitch Data */
	if (pitchReader) {
		auto pitchNum = pitchReader->getEventCount();

		for (int32_t i = 0; i < pitchNum; i++) {
			if (auto ptrData = static_cast<const ARAExtension::ARAContentIntParam*>(pitchReader->getDataForEvent(i))) {
				this->pitchs.add(*ptrData);
			}
		}
	}
}

const std::tuple<ARAContext::NoteList, ARAContext::PitchList, double>
ARAContext::getData() const {
	juce::ScopedReadLock locker(this->contextLock);
	return { this->notes, this->pitchs, this->totalLength };
}

double ARAContext::getLength() const {
	juce::ScopedReadLock locker(this->contextLock);
	return this->totalLength;
}
