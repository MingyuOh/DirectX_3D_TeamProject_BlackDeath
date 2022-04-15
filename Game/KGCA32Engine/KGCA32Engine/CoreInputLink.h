#pragma once



#include "LACoreLinkStd.h"



#ifdef TOOL_MODE

#include "LAInput.h"
#define I_Input INSTACNE_DIRECT_INPUT 
typedef EDxInput keyState;

#else

#include "BInput.h"

#endif



//INSTACNE_DIRECT_INPUT.KeyCheck