// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.2.1

// Freeze types in this file:
// name="Freeze::Catalog", key="string", value="Freeze::CatalogData"

#ifndef __Catalog_h__
#define __Catalog_h__

#include <Freeze/Map.h>
#include <Freeze/CatalogData.h>

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

class FREEZE_API CatalogKeyCodec
{
public:

    static void write(const ::std::string&, Freeze::Key&, const ::Ice::CommunicatorPtr&);
    static void read(::std::string&, const Freeze::Key&, const ::Ice::CommunicatorPtr&);
    static const std::string& typeId();
};

class FREEZE_API CatalogValueCodec
{
public:

    static void write(const ::Freeze::CatalogData&, Freeze::Value&, const ::Ice::CommunicatorPtr&);
    static void read(::Freeze::CatalogData&, const Freeze::Value&, const ::Ice::CommunicatorPtr&);
    static const std::string& typeId();
};

typedef Freeze::Map< ::std::string, ::Freeze::CatalogData, CatalogKeyCodec, CatalogValueCodec, Freeze::IceEncodingCompare > Catalog;

}

#endif
