﻿#include "ARAContext.h"

void ARAContext::setContextData(
	ARA::PlugIn::ContentReader* noteReader,
	ARA::PlugIn::ContentReader* pitchReader) {
	/** Lock Context */
	juce::ScopedWriteLock locker(this->contextLock);

	/** Clear Data */
	this->notes.clear();
	this->pitchs.clear();
	this->contextLength = 0;

	/** Get Note Data */
	if (noteReader) {
		auto noteNum = noteReader->getEventCount();

		for (int32_t i = 0; i < noteNum; i++) {
			if (auto ptrData = static_cast<const ARA::ARAContentNote*>(noteReader->getDataForEvent(i))) {
				this->notes.add(*ptrData);

				this->contextLength = std::max(
					this->contextLength, ptrData->startPosition + ptrData->noteDuration);
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
ARAContext::getContextData() const {
	juce::ScopedReadLock locker(this->contextLock);
	return { this->notes, this->pitchs, this->contextLength };
}

double ARAContext::getContextLength() const {
	juce::ScopedReadLock locker(this->contextLock);
	return this->contextLength;
}

void ARAContext::setSequenceData(
	const juce::ARARegionSequence* sequence) {
	/** Lock Context */
	juce::ScopedWriteLock locker(this->contextLock);

	/** Clear Data */
	this->regions.clear();

	/** Get Regions List */
	auto& regions = sequence->getPlaybackRegions();
	for (auto i : regions) {
		this->regions.add({
			.startInSeq = i->getStartInPlaybackTime(),
			.endInSeq = i->getEndInPlaybackTime(),
			.startInContext = i->getStartInAudioModificationTime(),
			.endInContext = i->getEndInAudioModificationTime()
			});
	}

	/** Create Context Reader */
	auto noteReader = (regions.size() > 0)
		? sequence->getDocumentController()->createPlaybackRegionContentReader(
			ARA::PlugIn::toRef(regions.at(0)), ARAExtension::ARAContentTypeNote, nullptr) : nullptr;
	auto pitchReader = (regions.size() > 0)
		? sequence->getDocumentController()->createPlaybackRegionContentReader(
			ARA::PlugIn::toRef(regions.at(0)), ARAExtension::ARAContentTypePitchWheel, nullptr) : nullptr;
	this->setContextData(
		ARA::PlugIn::fromRef(noteReader),
		ARA::PlugIn::fromRef(pitchReader));
}

const ARAContext::TimeRangeList ARAContext::doMapTime(double startTime, double length) const {
	juce::ScopedReadLock locker(this->contextLock);

	double endTime = startTime + length;

	TimeRangeList result;
	for (auto& region : this->regions) {
		if (region.endInSeq > startTime && region.startInSeq < endTime) {
			double realStartTime = std::max(region.startInSeq, startTime);
			double realEndTime = std::min(region.endInSeq, endTime);
			double realLength = realEndTime - realStartTime;

			double contextStartTime = realStartTime + (region.startInContext - region.startInSeq);

			result.add({ realStartTime, contextStartTime, realLength });
		}
	}

	return result;
}

const ARAContext::TimeRangeList ARAContext::doMapTimeRealTime(double startTime, double length) const {
	/** Try Lock */
	juce::ScopedTryReadLock locker(this->contextLock);
	if (!locker.isLocked()) { return {}; }

	/** Get List */
	return this->doMapTime(startTime, length);
}
