// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.2.1
// Generated from file `Evictor.ice'

#ifndef __Freeze_Evictor_h__
#define __Freeze_Evictor_h__

#include <Ice/LocalObjectF.h>
#include <Ice/ProxyF.h>
#include <Ice/ObjectF.h>
#include <Ice/Exception.h>
#include <Ice/LocalObject.h>
#include <Ice/Proxy.h>
#include <Ice/ObjectAdapterF.h>
#include <Ice/ServantLocator.h>
#include <Freeze/Exception.h>
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

class ServantInitializer;
FREEZE_API bool operator==(const ServantInitializer&, const ServantInitializer&);
FREEZE_API bool operator!=(const ServantInitializer&, const ServantInitializer&);
FREEZE_API bool operator<(const ServantInitializer&, const ServantInitializer&);
FREEZE_API bool operator<=(const ServantInitializer&, const ServantInitializer&);
FREEZE_API bool operator>(const ServantInitializer&, const ServantInitializer&);
FREEZE_API bool operator>=(const ServantInitializer&, const ServantInitializer&);

class EvictorIterator;
FREEZE_API bool operator==(const EvictorIterator&, const EvictorIterator&);
FREEZE_API bool operator!=(const EvictorIterator&, const EvictorIterator&);
FREEZE_API bool operator<(const EvictorIterator&, const EvictorIterator&);
FREEZE_API bool operator<=(const EvictorIterator&, const EvictorIterator&);
FREEZE_API bool operator>(const EvictorIterator&, const EvictorIterator&);
FREEZE_API bool operator>=(const EvictorIterator&, const EvictorIterator&);

class Evictor;
FREEZE_API bool operator==(const Evictor&, const Evictor&);
FREEZE_API bool operator!=(const Evictor&, const Evictor&);
FREEZE_API bool operator<(const Evictor&, const Evictor&);
FREEZE_API bool operator<=(const Evictor&, const Evictor&);
FREEZE_API bool operator>(const Evictor&, const Evictor&);
FREEZE_API bool operator>=(const Evictor&, const Evictor&);

}

namespace IceInternal
{

FREEZE_API void incRef(::Freeze::ServantInitializer*);
FREEZE_API void decRef(::Freeze::ServantInitializer*);

FREEZE_API void incRef(::Freeze::EvictorIterator*);
FREEZE_API void decRef(::Freeze::EvictorIterator*);

FREEZE_API void incRef(::Freeze::Evictor*);
FREEZE_API void decRef(::Freeze::Evictor*);

}

namespace Freeze
{

typedef ::IceInternal::Handle< ::Freeze::ServantInitializer> ServantInitializerPtr;

typedef ::IceInternal::Handle< ::Freeze::EvictorIterator> EvictorIteratorPtr;

typedef ::IceInternal::Handle< ::Freeze::Evictor> EvictorPtr;

}

namespace Freeze
{

class FREEZE_API NoSuchElementException : public ::Ice::LocalException
{
public:

    NoSuchElementException(const char*, int);
    virtual ~NoSuchElementException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;
};

class FREEZE_API EvictorDeactivatedException : public ::Ice::LocalException
{
public:

    EvictorDeactivatedException(const char*, int);
    virtual ~EvictorDeactivatedException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;
};

}

namespace Freeze
{

class FREEZE_API ServantInitializer : virtual public ::Ice::LocalObject
{
public:

    typedef ServantInitializerPtr PointerType;
    

    virtual void initialize(const ::Ice::ObjectAdapterPtr&, const ::Ice::Identity&, const ::std::string&, const ::Ice::ObjectPtr&) = 0;
};

class FREEZE_API EvictorIterator : virtual public ::Ice::LocalObject
{
public:

    typedef EvictorIteratorPtr PointerType;
    

    virtual bool hasNext() = 0;

    virtual ::Ice::Identity next() = 0;
};

class FREEZE_API Evictor : virtual public ::Ice::ServantLocator
{
public:

    typedef EvictorPtr PointerType;
    

    virtual void setSize(::Ice::Int) = 0;

    virtual ::Ice::Int getSize() = 0;

    virtual ::Ice::ObjectPrx add(const ::Ice::ObjectPtr&, const ::Ice::Identity&) = 0;

    virtual ::Ice::ObjectPrx addFacet(const ::Ice::ObjectPtr&, const ::Ice::Identity&, const ::std::string&) = 0;

    virtual ::Ice::ObjectPtr remove(const ::Ice::Identity&) = 0;

    virtual ::Ice::ObjectPtr removeFacet(const ::Ice::Identity&, const ::std::string&) = 0;

    virtual void keep(const ::Ice::Identity&) = 0;

    virtual void keepFacet(const ::Ice::Identity&, const ::std::string&) = 0;

    virtual void release(const ::Ice::Identity&) = 0;

    virtual void releaseFacet(const ::Ice::Identity&, const ::std::string&) = 0;

    virtual bool hasObject(const ::Ice::Identity&) = 0;

    virtual bool hasFacet(const ::Ice::Identity&, const ::std::string&) = 0;

    virtual ::Freeze::EvictorIteratorPtr getIterator(const ::std::string&, ::Ice::Int) = 0;
};

}

#endif
