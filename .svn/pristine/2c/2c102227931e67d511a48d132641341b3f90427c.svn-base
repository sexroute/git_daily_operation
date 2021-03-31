#if !defined(DBFUNC_ALREADY_LOAD)
#define DBFUNC_ALREADY_LOAD

#if defined(_DEBUG) 
    #if defined(_KAIDA_BASE)
        #pragma comment(lib,"DbFuncD.lib") 
        #pragma message("Automatically linking with DbFuncD.dll") 
    #else
        #pragma comment(lib,"DbFunc.lib") 
        #pragma message("Automatically linking with DbFunc.dll") 
    #endif        
#else 
    #pragma comment(lib,"DbFunc.lib") 
    #pragma message("Automatically linking with DbFunc.dll") 
#endif 

#include "customdb.h"
#include "simado.h"
#include "xbase.h"

#endif