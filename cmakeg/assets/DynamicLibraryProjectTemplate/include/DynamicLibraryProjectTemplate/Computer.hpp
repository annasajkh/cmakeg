#pragma once

#if defined(_WIN32) && defined(DynamicLibraryProjectTemplate_SHARED)
  #ifdef DynamicLibraryProjectTemplate_EXPORTS
    #define DynamicLibraryProjectTemplate_API __declspec(dllexport)
  #else
    #define DynamicLibraryProjectTemplate_API __declspec(dllimport)
  #endif
#else
  #define DynamicLibraryProjectTemplate_API
#endif


namespace computer
{
    DynamicLibraryProjectTemplate_API int getTheMeaningOfLife();
}