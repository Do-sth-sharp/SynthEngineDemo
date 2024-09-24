#include "ARAContext.h"

const juce::MidiMessageSequence ARAContext::getData() const {
	juce::ScopedReadLock locker(this->contextLock);
	return this->sequence;
}
