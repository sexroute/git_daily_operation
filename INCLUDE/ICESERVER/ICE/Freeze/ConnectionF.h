// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.2.1
// Generated from file `ConnectionF.ice'

#ifndef __Freeze_ConnectionF_h__
#define __Freeze_ConnectionF_h__

#include <Ice/LocalObjectF.h>
#include <Ice/ProxyF.h>
#include <Ice/ObjectF.h>
#include <Ice/Exception.h>
#include <Ice/LocalObject.h>
#include <Ice/UndefSysMacros.h>

#ifndef ICE_IGNORE_VERSION
#   if ICE_INT_VERSION / 100 != 302
#       error Ice version mismatch!
#   endif
#   if ICE_INT_VERSION % 100 > 50
#       error Beta header file detected
#   endif
#   if ICE_INT_VERSION % 100 < 1
#       error Ice patch level mismatch!
#   endif
#endif

#ifndef FREEZE_API
#   ifdef FREEZE_API_EXPORTS
#       define FREEZE_API ICE_DECLSPEC_EXPORT
#   else
#       define FREEZE_API ICE_DECLSPEC_IMPORT
#   endif
#endif

namespace Freeze
{

class Connection;
FREEZE_API bool operator==(const Connection&, const Connection&);
FREEZE_API bool operator!=(const Connection&, const Connection&);
FREEZE_API bool operator<(const Connection&, const Connection&);
FREEZE_API bool operator<=(const Connection&, const Connection&);
FREEZE_API bool operator>(const Connection&, const Connection&);
FREEZE_API bool operator>=(const Connection&, const Connection&);

}

namespace IceInternal
{

FREEZE_API void incRef(::Freeze::Connection*);
FREEZE_API void decRef(::Freeze::Connection*);

}

namespace Freeze
{

typedef ::IceInternal::Handle< ::Freeze::Connection> ConnectionPtr;

}

namespace Freeze
{

}

#endif
