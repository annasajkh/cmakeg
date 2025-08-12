#include <boost/dll.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>
#include "filesystem_utils.hpp"

namespace filesystem_utils
{
	void findAndReplaceTextFile(boost::filesystem::path pathToFile, std::string textToReplace, std::string textToReplaceWith)
	{
		std::string fileData = fileReadText(pathToFile);

		// Replace all textToReplace in fileData with textToReplaceWith
		boost::algorithm::replace_all(fileData, textToReplace, textToReplaceWith);

		fileWriteText(pathToFile, fileData);
	}

	std::string fileReadText(boost::filesystem::path pathToFile)
	{
		boost::filesystem::ifstream readFileHandler;
		readFileHandler.open(pathToFile);
		std::string fileData(std::istreambuf_iterator<char>{readFileHandler}, {});
		readFileHandler.close();

		return fileData;
	}

	void fileWriteText(boost::filesystem::path pathToFile, std::string text)
	{
		boost::filesystem::ofstream writeFileHandler;
		writeFileHandler.open(pathToFile);
		writeFileHandler << text;
		writeFileHandler.close();
	}
}