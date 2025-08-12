#include "ReferenceCommandHandler.hpp"
#include <iostream>
#include "../utils/filesystem_utils.hpp"
#include <boost/algorithm/string.hpp>

using namespace cmakeg::commands;

ReferenceCommandHandler::ReferenceCommandHandler()
{

}

void ReferenceCommandHandler::execute()
{
    std::vector<std::string> libraryProjectIdentifiers = {
        "# STATIC LIBRARY PROJECT IDENTIFIER DON'T REMOVE THIS COMMENT IF YOU WANT TO KEEP USING CMAKEG TO MANAGE THIS LIBRARY",
        "# DYNAMIC LIBRARY PROJECT IDENTIFIER DON'T REMOVE THIS COMMENT IF YOU WANT TO KEEP USING CMAKEG TO MANAGE THIS LIBRARY"
    };

    ProjectIntegrityStatus libraryProjectIntegrityStatus = checkProjectIntegrity(librarySourceName, libraryProjectIdentifiers);

    switch (libraryProjectIntegrityStatus)
    {
    case ProjectIntegrityStatus::Skip:
        return;
    case ProjectIntegrityStatus::NotExist:
        std::cout << "Cannot find the library project source\n";
        return;
    case ProjectIntegrityStatus::ContainsCMakeButNotIdentifier:
        std::cout << "Cannot find the library project identifier but found CMakeList.txt, is library project identifier get removed?\n";
        return;
    }

    std::vector<std::string> executableProjectIdentifiers = {
        "# STATIC LIBRARY PROJECT IDENTIFIER DON'T REMOVE THIS COMMENT IF YOU WANT TO KEEP USING CMAKEG TO MANAGE THIS LIBRARY",
        "# DYNAMIC LIBRARY PROJECT IDENTIFIER DON'T REMOVE THIS COMMENT IF YOU WANT TO KEEP USING CMAKEG TO MANAGE THIS LIBRARY",
        "# EXECUTABLE PROJECT IDENTIFIER DON'T REMOVE THIS COMMENT IF YOU WANT TO KEEP USING CMAKEG TO MANAGE THIS PROJECT"
    };

    ProjectIntegrityStatus projectIntegrityStatus = checkProjectIntegrity(projectDestinationName, executableProjectIdentifiers);

    switch (projectIntegrityStatus)
    {
    case ProjectIntegrityStatus::Skip:
        return;
    case ProjectIntegrityStatus::NotExist:
        std::cout << "Cannot find the project destination\n";
        return;
    case ProjectIntegrityStatus::ContainsCMakeButNotIdentifier:
        std::cout << "Cannot find the project destination but found CMakeList.txt, is project identifier get removed?\n";
        return;
    }

    std::string staticLibraryIdentifier = "# STATIC LIBRARY PROJECT IDENTIFIER DON'T REMOVE THIS COMMENT IF YOU WANT TO KEEP USING CMAKEG TO MANAGE THIS LIBRARY";

    std::string staticLibraryAddReference = "\ntarget_link_libraries(${PROJECT_NAME} PRIVATE librarySourceName)";
    std::string dynamicLibraryAddReference = R"cmake(
target_link_libraries(${PROJECT_NAME} PRIVATE librarySourceName)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:librarySourceName>
    $<TARGET_FILE_DIR:${PROJECT_NAME}>
))cmake";

    std::string librarySourceCMake = filesystem_utils::fileReadText(boost::filesystem::current_path() / librarySourceName / "CMakeLists.txt");
    std::string projectDestinationCMake = filesystem_utils::fileReadText(boost::filesystem::current_path() / projectDestinationName / "CMakeLists.txt");

    if (boost::algorithm::contains(librarySourceCMake, staticLibraryIdentifier))
    {
        boost::algorithm::replace_all(staticLibraryAddReference, "librarySourceName", librarySourceName);

        filesystem_utils::fileWriteText(boost::filesystem::current_path() / projectDestinationName / "CMakeLists.txt", projectDestinationCMake + staticLibraryAddReference);
    }
    else
    {
        boost::algorithm::replace_all(dynamicLibraryAddReference, "librarySourceName", librarySourceName);

        filesystem_utils::fileWriteText(boost::filesystem::current_path() / projectDestinationName / "CMakeLists.txt", projectDestinationCMake + dynamicLibraryAddReference);
    }
}
