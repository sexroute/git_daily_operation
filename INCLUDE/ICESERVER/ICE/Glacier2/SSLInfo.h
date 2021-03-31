// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.2.1
// Generated from file `SSLInfo.ice'

#ifndef __Glacier2_SSLInfo_h__
#define __Glacier2_SSLInfo_h__

#include <Ice/LocalObjectF.h>
#include <Ice/ProxyF.h>
#include <Ice/ObjectF.h>
#include <Ice/Exception.h>
#include <Ice/LocalObject.h>
#include <Ice/Proxy.h>
#include <Ice/BuiltinSequences.h>
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

namespace Glacier2
{

struct SSLInfo
{
    ::std::string remoteHost;
    ::Ice::Int remotePort;
    ::std::string localHost;
    ::Ice::Int localPort;
    ::std::string cipher;
    ::Ice::StringSeq certs;

    GLACIER2_API bool operator==(const SSLInfo&) const;
    GLACIER2_API bool operator!=(const SSLInfo&) const;
    GLACIER2_API bool operator<(const SSLInfo&) const;
    GLACIER2_API bool operator<=(const SSLInfo& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    GLACIER2_API bool operator>(const SSLInfo& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    GLACIER2_API bool operator>=(const SSLInfo& __rhs) const
    {
        return !operator<(__rhs);
    }

    GLACIER2_API void __write(::IceInternal::BasicStream*) const;
    GLACIER2_API void __read(::IceInternal::BasicStream*);
};

}

#endif
