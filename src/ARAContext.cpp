#include "ARAContext.h"

void ARAContext::setData(const juce::ARAMusicalContext* context) {
	if (!context) { return; }

	/** Lock Context */
	juce::ScopedWriteLock locker(this->contextLock);

	/** Clear Sequence */
	this->sequence.clear();

	/** TODO Get Data */
}

const juce::MidiMessageSequence ARAContext::getData() const {
	juce::ScopedReadLock locker(this->contextLock);
	return this->sequence;
}
