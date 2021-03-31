#if !defined(GUIFUNC_ALREADY_LOAD)
#define GUIFUNC_ALREADY_LOAD

#if defined(_DEBUG) 
    #if defined(_KAIDA_BASE)
        #pragma comment(lib,"GuiFuncD.lib") 
        #pragma message("Automatically linking with GuiFuncD.dll") 
    #else
        #pragma comment(lib,"GuiFunc.lib") 
        #pragma message("Automatically linking with GuiFunc.dll") 
    #endif        
#else 
  #pragma comment(lib,"GuiFunc.lib") 
  #pragma message("Automatically linking with GuiFunc.dll") 
#endif 

#include "Customlistctrlex.h"
#include "Customtreectrl.h"
#include "CustomComboBox.h"
#include "Custommenu.h"
#include "dib256.h"     //CDIBitmap
#include "CustomLabel.h"
#include "Customdialog.h"
#include "CustomFlatBtn.h"

#endif