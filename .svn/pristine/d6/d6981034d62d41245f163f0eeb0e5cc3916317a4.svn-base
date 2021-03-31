// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.2.1
// Generated from file `Properties.ice'

#ifndef __Ice_Properties_h__
#define __Ice_Properties_h__

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

#ifndef ICE_API
#   ifdef ICE_API_EXPORTS
#       define ICE_API ICE_DECLSPEC_EXPORT
#   else
#       define ICE_API ICE_DECLSPEC_IMPORT
#   endif
#endif

namespace Ice
{

class Properties;
ICE_API bool operator==(const Properties&, const Properties&);
ICE_API bool operator!=(const Properties&, const Properties&);
ICE_API bool operator<(const Properties&, const Properties&);
ICE_API bool operator<=(const Properties&, const Properties&);
ICE_API bool operator>(const Properties&, const Properties&);
ICE_API bool operator>=(const Properties&, const Properties&);

}

namespace IceInternal
{

ICE_API void incRef(::Ice::Properties*);
ICE_API void decRef(::Ice::Properties*);

}

namespace Ice
{

typedef ::IceInternal::Handle< ::Ice::Properties> PropertiesPtr;

}

namespace Ice
{

typedef ::std::map< ::std::string, ::std::string> PropertyDict;

}

namespace Ice
{

class ICE_API Properties : virtual public ::Ice::LocalObject
{
public:

    typedef PropertiesPtr PointerType;
    

    virtual ::std::string getProperty(const ::std::string&) = 0;

    virtual ::std::string getPropertyWithDefault(const ::std::string&, const ::std::string&) = 0;

    virtual ::Ice::Int getPropertyAsInt(const ::std::string&) = 0;

    virtual ::Ice::Int getPropertyAsIntWithDefault(const ::std::string&, ::Ice::Int) = 0;

    virtual ::Ice::PropertyDict getPropertiesForPrefix(const ::std::string&) = 0;

    virtual void setProperty(const ::std::string&, const ::std::string&) = 0;

    virtual ::Ice::StringSeq getCommandLineOptions() = 0;

    virtual ::Ice::StringSeq parseCommandLineOptions(const ::std::string&, const ::Ice::StringSeq&) = 0;

    virtual ::Ice::StringSeq parseIceCommandLineOptions(const ::Ice::StringSeq&) = 0;

    virtual void load(const ::std::string&) = 0;

    virtual ::Ice::PropertiesPtr clone() = 0;
};

}

#endif
