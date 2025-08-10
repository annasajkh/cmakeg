#pragma once

#if defined(_WIN32) && defined(DynamicLibraryProjectWithoutWorkspaceTemplate_SHARED)
  #ifdef DynamicLibraryProjectWithoutWorkspaceTemplate_EXPORTS
    #define DynamicLibraryProjectWithoutWorkspaceTemplate_API __declspec(dllexport)
  #else
    #define DynamicLibraryProjectWithoutWorkspaceTemplate_API __declspec(dllimport)
  #endif
#else
  #define DynamicLibraryProjectWithoutWorkspaceTemplate_API
#endif


namespace computer
{
    DynamicLibraryProjectWithoutWorkspaceTemplate_API int getTheMeaningOfLife();
}