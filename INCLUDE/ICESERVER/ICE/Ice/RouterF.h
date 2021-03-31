// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.2.1
// Generated from file `RouterF.ice'

#ifndef __Ice_RouterF_h__
#define __Ice_RouterF_h__

#include <Ice/LocalObjectF.h>
#include <Ice/ProxyF.h>
#include <Ice/ObjectF.h>
#include <Ice/Exception.h>
#include <Ice/LocalObject.h>
#include <Ice/Proxy.h>
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

#ifndef ICE_API
#   ifdef ICE_API_EXPORTS
#       define ICE_API ICE_DECLSPEC_EXPORT
#   else
#       define ICE_API ICE_DECLSPEC_IMPORT
#   endif
#endif

namespace IceProxy
{

namespace Ice
{

class Router;
ICE_API bool operator==(const Router&, const Router&);
ICE_API bool operator!=(const Router&, const Router&);
ICE_API bool operator<(const Router&, const Router&);
ICE_API bool operator<=(const Router&, const Router&);
ICE_API bool operator>(const Router&, const Router&);
ICE_API bool operator>=(const Router&, const Router&);

}

}

namespace Ice
{

class Router;
ICE_API bool operator==(const Router&, const Router&);
ICE_API bool operator!=(const Router&, const Router&);
ICE_API bool operator<(const Router&, const Router&);
ICE_API bool operator<=(const Router&, const Router&);
ICE_API bool operator>(const Router&, const Router&);
ICE_API bool operator>=(const Router&, const Router&);

}

namespace IceInternal
{

ICE_API void incRef(::Ice::Router*);
ICE_API void decRef(::Ice::Router*);

ICE_API void incRef(::IceProxy::Ice::Router*);
ICE_API void decRef(::IceProxy::Ice::Router*);

}

namespace Ice
{

typedef ::IceInternal::Handle< ::Ice::Router> RouterPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::Ice::Router> RouterPrx;

ICE_API void __write(::IceInternal::BasicStream*, const RouterPrx&);
ICE_API void __read(::IceInternal::BasicStream*, RouterPrx&);
ICE_API void __write(::IceInternal::BasicStream*, const RouterPtr&);
ICE_API void __patch__RouterPtr(void*, ::Ice::ObjectPtr&);

ICE_API void __addObject(const RouterPtr&, ::IceInternal::GCCountMap&);
ICE_API bool __usesClasses(const RouterPtr&);
ICE_API void __decRefUnsafe(const RouterPtr&);
ICE_API void __clearHandleUnsafe(RouterPtr&);

}

namespace Ice
{

}

namespace IceProxy
{

namespace Ice
{

}

}

namespace IceDelegate
{

namespace Ice
{

}

}

namespace IceDelegateM
{

namespace Ice
{

}

}

namespace IceDelegateD
{

namespace Ice
{

}

}

#endif
