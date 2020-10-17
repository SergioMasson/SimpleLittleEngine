#pragma once

#if (defined WIN32 || defined _WIN32 || defined WINCE) && defined SLEAPI_EXPORTS
# define SLE_EXPORTS __declspec(dllexport)
#else
# define SLE_EXPORTS __declspec(dllimport)
#endif