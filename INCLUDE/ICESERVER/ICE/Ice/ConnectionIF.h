// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#ifndef ICE_CONNECTIONI_F_H
#define ICE_CONNECTIONI_F_H

#include <Ice/Handle.h>

namespace Ice
{

class ConnectionI;

}

namespace IceInternal
{

ICE_API void incRef(Ice::ConnectionI*);
ICE_API void decRef(Ice::ConnectionI*);

}

namespace Ice
{

typedef IceInternal::Handle<ConnectionI> ConnectionIPtr;

}

#endif
