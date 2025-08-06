#pragma once

#include <boost/filesystem.hpp>

namespace filesystem_utils
{
	void findAndReplaceTextFile(boost::filesystem::path pathToFile, std::string textToReplace, std::string textToReplaceWith);

	std::string fileReadText(boost::filesystem::path pathToFile);
	void fileWriteText(boost::filesystem::path pathToFile, std::string text);
}