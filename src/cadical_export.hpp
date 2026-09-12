#ifndef _cadical_export_hpp_INCLUDED
#define _cadical_export_hpp_INCLUDED

#if defined _WIN32
// dllexport only while building the DLL itself: users that see dllexport on a
// class try to define and export its members too.
#ifdef cadical_EXPORTS
#define CADICAL_API __declspec (dllexport)
#else
#define CADICAL_API
#endif
#else
#define CADICAL_API __attribute__ ((visibility ("default")))
#endif

#endif
