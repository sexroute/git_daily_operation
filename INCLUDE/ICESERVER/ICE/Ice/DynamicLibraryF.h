// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#ifndef ICE_DYNAMIC_LIBRARY_F_H
#define ICE_DYNAMIC_LIBRARY_F_H

#include <Ice/Handle.h>

namespace IceInternal
{

class DynamicLibrary;
ICE_API void incRef(DynamicLibrary*);
ICE_API void decRef(DynamicLibrary*);
typedef Handle<DynamicLibrary> DynamicLibraryPtr;

class DynamicLibraryList;
ICE_API void incRef(DynamicLibraryList*);
ICE_API void decRef(DynamicLibraryList*);
typedef Handle<DynamicLibraryList> DynamicLibraryListPtr;

}

#endif
