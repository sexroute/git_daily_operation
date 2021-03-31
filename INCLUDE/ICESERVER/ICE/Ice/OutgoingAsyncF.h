// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

#ifndef ICE_OUTGOING_ASYNC_F_H
#define ICE_OUTGOING_ASYNC_F_H

#include <Ice/Handle.h>

namespace IceInternal
{

class OutgoingAsync;
ICE_API void incRef(OutgoingAsync*);
ICE_API void decRef(OutgoingAsync*);
typedef IceInternal::Handle<OutgoingAsync> OutgoingAsyncPtr;

}

namespace Ice
{

class AMI_Object_ice_invoke;
class AMI_Array_Object_ice_invoke;

}

namespace IceInternal
{

ICE_API void incRef(::Ice::AMI_Object_ice_invoke*);
ICE_API void decRef(::Ice::AMI_Object_ice_invoke*);
ICE_API void incRef(::Ice::AMI_Array_Object_ice_invoke*);
ICE_API void decRef(::Ice::AMI_Array_Object_ice_invoke*);

}

namespace Ice
{

typedef IceInternal::Handle<AMI_Object_ice_invoke> AMI_Object_ice_invokePtr;
typedef IceInternal::Handle<AMI_Array_Object_ice_invoke> AMI_Array_Object_ice_invokePtr;

}

#endif
