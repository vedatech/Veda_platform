#ifndef LIBVEDAWALLET_GLOBAL_H
#define LIBVEDAWALLET_GLOBAL_H


#ifndef __linux__
#if defined( VEDALIB_EXPORTS )
#define DLL_IMPORT __declspec(dllexport)
#else // !BUILDING_DLL
#define DLL_IMPORT __declspec(dllimport)
#endif // BUILDING_DLL
#else
#define DLL_IMPORT extern "C"
#endif // __linux__


#endif // LIBVEDAWALLET_GLOBAL_H
