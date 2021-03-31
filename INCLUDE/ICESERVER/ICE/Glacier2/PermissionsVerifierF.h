// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.2.1
// Generated from file `PermissionsVerifierF.ice'

#ifndef __Glacier2_PermissionsVerifierF_h__
#define __Glacier2_PermissionsVerifierF_h__

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

#ifndef GLACIER2_API
#   ifdef GLACIER2_API_EXPORTS
#       define GLACIER2_API ICE_DECLSPEC_EXPORT
#   else
#       define GLACIER2_API ICE_DECLSPEC_IMPORT
#   endif
#endif

namespace IceProxy
{

namespace Glacier2
{

class PermissionsVerifier;
GLACIER2_API bool operator==(const PermissionsVerifier&, const PermissionsVerifier&);
GLACIER2_API bool operator!=(const PermissionsVerifier&, const PermissionsVerifier&);
GLACIER2_API bool operator<(const PermissionsVerifier&, const PermissionsVerifier&);
GLACIER2_API bool operator<=(const PermissionsVerifier&, const PermissionsVerifier&);
GLACIER2_API bool operator>(const PermissionsVerifier&, const PermissionsVerifier&);
GLACIER2_API bool operator>=(const PermissionsVerifier&, const PermissionsVerifier&);

class SSLPermissionsVerifier;
GLACIER2_API bool operator==(const SSLPermissionsVerifier&, const SSLPermissionsVerifier&);
GLACIER2_API bool operator!=(const SSLPermissionsVerifier&, const SSLPermissionsVerifier&);
GLACIER2_API bool operator<(const SSLPermissionsVerifier&, const SSLPermissionsVerifier&);
GLACIER2_API bool operator<=(const SSLPermissionsVerifier&, const SSLPermissionsVerifier&);
GLACIER2_API bool operator>(const SSLPermissionsVerifier&, const SSLPermissionsVerifier&);
GLACIER2_API bool operator>=(const SSLPermissionsVerifier&, const SSLPermissionsVerifier&);

}

}

namespace Glacier2
{

class PermissionsVerifier;
GLACIER2_API bool operator==(const PermissionsVerifier&, const PermissionsVerifier&);
GLACIER2_API bool operator!=(const PermissionsVerifier&, const PermissionsVerifier&);
GLACIER2_API bool operator<(const PermissionsVerifier&, const PermissionsVerifier&);
GLACIER2_API bool operator<=(const PermissionsVerifier&, const PermissionsVerifier&);
GLACIER2_API bool operator>(const PermissionsVerifier&, const PermissionsVerifier&);
GLACIER2_API bool operator>=(const PermissionsVerifier&, const PermissionsVerifier&);

class SSLPermissionsVerifier;
GLACIER2_API bool operator==(const SSLPermissionsVerifier&, const SSLPermissionsVerifier&);
GLACIER2_API bool operator!=(const SSLPermissionsVerifier&, const SSLPermissionsVerifier&);
GLACIER2_API bool operator<(const SSLPermissionsVerifier&, const SSLPermissionsVerifier&);
GLACIER2_API bool operator<=(const SSLPermissionsVerifier&, const SSLPermissionsVerifier&);
GLACIER2_API bool operator>(const SSLPermissionsVerifier&, const SSLPermissionsVerifier&);
GLACIER2_API bool operator>=(const SSLPermissionsVerifier&, const SSLPermissionsVerifier&);

}

namespace IceInternal
{

GLACIER2_API void incRef(::Glacier2::PermissionsVerifier*);
GLACIER2_API void decRef(::Glacier2::PermissionsVerifier*);

GLACIER2_API void incRef(::IceProxy::Glacier2::PermissionsVerifier*);
GLACIER2_API void decRef(::IceProxy::Glacier2::PermissionsVerifier*);

GLACIER2_API void incRef(::Glacier2::SSLPermissionsVerifier*);
GLACIER2_API void decRef(::Glacier2::SSLPermissionsVerifier*);

GLACIER2_API void incRef(::IceProxy::Glacier2::SSLPermissionsVerifier*);
GLACIER2_API void decRef(::IceProxy::Glacier2::SSLPermissionsVerifier*);

}

namespace Glacier2
{

typedef ::IceInternal::Handle< ::Glacier2::PermissionsVerifier> PermissionsVerifierPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::Glacier2::PermissionsVerifier> PermissionsVerifierPrx;

GLACIER2_API void __write(::IceInternal::BasicStream*, const PermissionsVerifierPrx&);
GLACIER2_API void __read(::IceInternal::BasicStream*, PermissionsVerifierPrx&);
GLACIER2_API void __write(::IceInternal::BasicStream*, const PermissionsVerifierPtr&);
GLACIER2_API void __patch__PermissionsVerifierPtr(void*, ::Ice::ObjectPtr&);

GLACIER2_API void __addObject(const PermissionsVerifierPtr&, ::IceInternal::GCCountMap&);
GLACIER2_API bool __usesClasses(const PermissionsVerifierPtr&);
GLACIER2_API void __decRefUnsafe(const PermissionsVerifierPtr&);
GLACIER2_API void __clearHandleUnsafe(PermissionsVerifierPtr&);

typedef ::IceInternal::Handle< ::Glacier2::SSLPermissionsVerifier> SSLPermissionsVerifierPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::Glacier2::SSLPermissionsVerifier> SSLPermissionsVerifierPrx;

GLACIER2_API void __write(::IceInternal::BasicStream*, const SSLPermissionsVerifierPrx&);
GLACIER2_API void __read(::IceInternal::BasicStream*, SSLPermissionsVerifierPrx&);
GLACIER2_API void __write(::IceInternal::BasicStream*, const SSLPermissionsVerifierPtr&);
GLACIER2_API void __patch__SSLPermissionsVerifierPtr(void*, ::Ice::ObjectPtr&);

GLACIER2_API void __addObject(const SSLPermissionsVerifierPtr&, ::IceInternal::GCCountMap&);
GLACIER2_API bool __usesClasses(const SSLPermissionsVerifierPtr&);
GLACIER2_API void __decRefUnsafe(const SSLPermissionsVerifierPtr&);
GLACIER2_API void __clearHandleUnsafe(SSLPermissionsVerifierPtr&);

}

namespace Glacier2
{

}

namespace IceProxy
{

namespace Glacier2
{

}

}

namespace IceDelegate
{

namespace Glacier2
{

}

}

namespace IceDelegateM
{

namespace Glacier2
{

}

}

namespace IceDelegateD
{

namespace Glacier2
{

}

}

#endif
