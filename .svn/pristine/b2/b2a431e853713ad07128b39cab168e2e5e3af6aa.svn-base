#ifndef __BH5000MACRO_H__
#define __BH5000MACRO_H__

#ifndef BH5000_API_EXPORT
	#define BH5000_API_EXPORT __declspec(dllexport)
#endif

#ifndef BH5000_API_IMPORT
	#define BH5000_API_IMPORT __declspec(dllimport)
#endif

#ifdef _BH5000EXT
	#define BH5000_EXT_CLASS BH5000_API_EXPORT
#else
	#define BH5000_EXT_CLASS BH5000_API_IMPORT
#endif

#ifdef _BH5000EXT
	#define BH5000_EXT_FUNC BH5000_API_EXPORT
#else
	#define BH5000_EXT_FUNC BH5000_API_IMPORT
#endif

#define TIMER_WIND_DEMO  (3000)

#endif /* __BH5000MACRO_H__ */