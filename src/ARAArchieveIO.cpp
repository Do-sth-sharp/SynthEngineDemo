#include "ARAArchieveIO.h"

ARAArchieveWriter::ARAArchieveWriter(
	const juce::ARAStoreObjectsFilter* filter,
	const ProgressFunc& reportProgress)
	: filter(filter), reportProgress(reportProgress) {}

bool ARAArchieveWriter::write(juce::ARAOutputStream& stream) {
	/** Get Source List */
	auto &audioSources = this->filter->getAudioSourcesToStore<juce::ARAAudioSource>();
	auto &audioModifications = this->filter->getAudioModificationsToStore<juce::ARAAudioModification>();

	/** Scope Guard */
	juce::ScopeGuard guard([reportProgress = this->reportProgress] { reportProgress(1.0f); });

	/** Size */
	if (!stream.writeInt64((int64_t)audioSources.size())) { return false; }
	if (!stream.writeInt64((int64_t)audioModifications.size())) { return false; }

	/** Audio Sources */
	for (int i = 0; i < audioSources.size(); i++) {
		if (!stream.writeString(audioSources[i]->getPersistentID())) { return false; }

		/** Progress */
		const auto progressVal = (float)i / (float)(audioSources.size() + audioModifications.size());
		this->reportProgress(progressVal);
	}

	/** Audio Modifications */
	for (int i = 0; i < audioModifications.size(); i++) {
		if (!stream.writeString(audioModifications[i]->getPersistentID())) { return false; }

		/** Progress */
		const auto progressVal = (float)(audioSources.size() + i) / (float)(audioSources.size() + audioModifications.size());
		this->reportProgress(progressVal);
	}

	return true;
}

ARAArchieveReader::ARAArchieveReader(
	const juce::ARARestoreObjectsFilter* filter,
	const ProgressFunc& reportProgress)
	: filter(filter), reportProgress(reportProgress) {}

bool ARAArchieveReader::read(juce::ARAInputStream& stream) {
	/** Scope Guard */
	juce::ScopeGuard guard([reportProgress = this->reportProgress] { reportProgress(1.0f); });

	/** Size */
	uint64_t sourceNum = stream.readInt64();
	uint64_t modificationNum = stream.readInt64();
	
	/** Audio Sources */
	for (int i = 0; i < sourceNum; i++) {
		juce::String id = stream.readString();
		auto audioSource = this->filter->
			getAudioSourceToRestoreStateWithID<juce::ARAAudioSource>(id.getCharPointer());
		juce::ignoreUnused(audioSource);

		/** Progress */
		const auto progressVal = (float)i / (float)(sourceNum + modificationNum);
		this->reportProgress(progressVal);
	}

	/** Audio Modifications */
	for (int i = 0; i < modificationNum; i++) {
		juce::String id = stream.readString();
		auto audioModification = this->filter->
			getAudioModificationToRestoreStateWithID<juce::ARAAudioModification>(id.getCharPointer());
		juce::ignoreUnused(audioModification);

		/** Progress */
		const auto progressVal = (float)(sourceNum + i) / (float)(sourceNum + modificationNum);
		this->reportProgress(progressVal);
	}

	return true;
}
