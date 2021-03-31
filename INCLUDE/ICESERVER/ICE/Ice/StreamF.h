// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#ifndef ICE_STREAM_F_H
#define ICE_STREAM_F_H

#include <Ice/Handle.h>

namespace Ice
{

class InputStream;
class OutputStream;

}

namespace IceInternal
{

ICE_API void incRef(::Ice::InputStream*);
ICE_API void decRef(::Ice::InputStream*);
ICE_API void incRef(::Ice::OutputStream*);
ICE_API void decRef(::Ice::OutputStream*);

}

namespace Ice
{

typedef IceInternal::Handle< InputStream > InputStreamPtr;
typedef IceInternal::Handle< OutputStream > OutputStreamPtr;

}

#endif
