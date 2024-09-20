#include "ARAArchieveIO.h"

ARAArchieveWriter::ARAArchieveWriter(
	const juce::ARAStoreObjectsFilter* filter)
	: filter(filter) {}

bool ARAArchieveWriter::write(juce::ARAOutputStream& stream) {
	/** TODO */
	return false;
}

ARAArchieveReader::ARAArchieveReader(
	const juce::ARARestoreObjectsFilter* filter)
	: filter(filter) {}

bool ARAArchieveReader::read(juce::ARAInputStream& stream) {
	/** TODO */
	return false;
}
