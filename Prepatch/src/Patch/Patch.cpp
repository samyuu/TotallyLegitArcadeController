#include "Patch.h"
#include <fstream>
#include "../StringOperations/Operations.h"

namespace Patch
{
	Patch::Patch()
	{
	}

	Patch::~Patch()
	{
		for (auto& patchData : PatchDataVector)
			delete patchData;
	}

	bool Patch::ParseFile(const std::string& patchPath)
	{
		printf("Patch::ParseFile(): Opening patch file...\n");

		std::ifstream fileStream(patchPath);
	
		if (!fileStream.is_open())
		{
			printf("Patch::ParseFile(): Unable to open input file: %s\n", patchPath.c_str());
			return FALSE;
		}

		printf("Patch::ParseFile(): Parsing patch file...\n");

		std::string commentPrefix = "//";
		std::string headerPrefix = "#";
		std::string seperator = ":";
		std::string byteSeperator = " ";

		std::string line;
		while (getline(fileStream, line))
		{
			if (!line.compare(0, commentPrefix.size(), commentPrefix))
				continue;

			if (!line.compare(0, headerPrefix.size(), headerPrefix))
			{
				auto split = StringOperations::Split(line, seperator);

				if (split.size() != 2)
					continue;

				auto type = split[0].substr(1);
				auto value = split[1];
				StringOperations::Trim(type);
				StringOperations::Trim(value);

				if (type == "file")
					FileName = value;
				else if (type == "author")
					Author = value;
				else if (type == "version")
					Version = value;
				else if (type == "date")
					Date = value;
				else if (type == "image_base")
					ImageBase = _strtoi64(value.c_str(), nullptr, 16);

				continue;
			}

			auto valueSplit = StringOperations::Split(line, seperator);

			if (valueSplit.size() != 4)
				continue;
	
			for (auto& str : valueSplit)
				StringOperations::Trim(str);

			char* buffer;
			auto patchData = new PatchData();

			patchData->Address = _strtoi64(valueSplit[0].c_str(), &buffer, 16);
			if (buffer == nullptr)
				continue;
	
			patchData->Length = strtol(valueSplit[1].c_str(), &buffer, 16);
			if (buffer == nullptr)
				continue;

			auto originalBytesString = StringOperations::Split(valueSplit[2], byteSeperator);
			auto patchedBytesString  = StringOperations::Split(valueSplit[3], byteSeperator);

			patchData->OriginalBytes = new BYTE[patchData->Length];
			for (int i = 0; i < patchData->Length; i++)
				patchData->OriginalBytes[i] = strtol(originalBytesString[i].c_str(), nullptr, 16);

			patchData->PatchedBytes = new BYTE[patchData->Length];
			for (int i = 0; i < patchData->Length; i++)
				patchData->PatchedBytes[i] = strtol(patchedBytesString[i].c_str(), nullptr, 16);

			PatchDataVector.push_back(patchData);
		}

		return TRUE;
	}
}
