#if !defined(BASEFUNC_ALREADY_LOAD)
#define BASEFUNC_ALREADY_LOAD

__declspec(dllimport)  void SplitStrByChar( const char * buf , char ch , CStringArray &arOut);
__declspec(dllimport)  char * GetIniSection(char * instr,int index);
__declspec(dllimport)  char * Trim(char * buffer);

__declspec(dllimport) void RC5_Encrypt(const char *input,char *output,const char *key);
__declspec(dllimport) void RC5_Decrypt(const char *input,char *output,const char *key);

__declspec(dllimport) void des3_ecb_encrypt(const char *key,const char *cleartext,char *ciphertext);
__declspec(dllimport) void des3_ecb_decrypt(const char *key,const char *ciphertext,char *cleartext);


#if defined(_DEBUG) 
    #if defined(_KAIDA_BASE)
        #pragma comment(lib,"BaseFunc.lib") 
        #pragma message("Automatically linking with BaseFuncD.dll") 
    #else
        #pragma comment(lib,"BaseFunc.lib") 
        #pragma message("Automatically linking with BaseFunc.dll") 
    #endif        
#else 
    #pragma comment(lib,"BaseFunc.lib") 
    #pragma message("Automatically linking with BaseFunc.dll") 
#endif 

#include "CustomIni.h"
#include "CustomReg.h"

#endif