// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.2.1
// Generated from file `FileParser.ice'

#ifndef __IceGrid_FileParser_h__
#define __IceGrid_FileParser_h__

#include <Ice/LocalObjectF.h>
#include <Ice/ProxyF.h>
#include <Ice/ObjectF.h>
#include <Ice/Exception.h>
#include <Ice/LocalObject.h>
#include <Ice/Proxy.h>
#include <Ice/Object.h>
#include <Ice/Outgoing.h>
#include <Ice/OutgoingAsync.h>
#include <Ice/Incoming.h>
#include <Ice/IncomingAsync.h>
#include <Ice/Direct.h>
#include <Ice/UserExceptionFactory.h>
#include <Ice/FactoryTable.h>
#include <Ice/StreamF.h>
#include <IceGrid/Admin.h>
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

namespace IceProxy
{

namespace IceGrid
{

class FileParser;
bool operator==(const FileParser&, const FileParser&);
bool operator!=(const FileParser&, const FileParser&);
bool operator<(const FileParser&, const FileParser&);
bool operator<=(const FileParser&, const FileParser&);
bool operator>(const FileParser&, const FileParser&);
bool operator>=(const FileParser&, const FileParser&);

}

}

namespace IceGrid
{

class FileParser;
bool operator==(const FileParser&, const FileParser&);
bool operator!=(const FileParser&, const FileParser&);
bool operator<(const FileParser&, const FileParser&);
bool operator<=(const FileParser&, const FileParser&);
bool operator>(const FileParser&, const FileParser&);
bool operator>=(const FileParser&, const FileParser&);

}

namespace IceInternal
{

void incRef(::IceGrid::FileParser*);
void decRef(::IceGrid::FileParser*);

void incRef(::IceProxy::IceGrid::FileParser*);
void decRef(::IceProxy::IceGrid::FileParser*);

}

namespace IceGrid
{

typedef ::IceInternal::Handle< ::IceGrid::FileParser> FileParserPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::IceGrid::FileParser> FileParserPrx;

void __write(::IceInternal::BasicStream*, const FileParserPrx&);
void __read(::IceInternal::BasicStream*, FileParserPrx&);
void __write(::IceInternal::BasicStream*, const FileParserPtr&);
void __patch__FileParserPtr(void*, ::Ice::ObjectPtr&);

void __addObject(const FileParserPtr&, ::IceInternal::GCCountMap&);
bool __usesClasses(const FileParserPtr&);
void __decRefUnsafe(const FileParserPtr&);
void __clearHandleUnsafe(FileParserPtr&);

}

namespace IceGrid
{

class ParseException : public ::Ice::UserException
{
public:

    ParseException() {}
    explicit ParseException(const ::std::string&);
    virtual ~ParseException() throw();

    virtual ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    ::std::string reason;

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);

    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

static ParseException __ParseException_init;

}

namespace IceAsync
{

}

namespace IceProxy
{

namespace IceGrid
{

class FileParser : virtual public ::IceProxy::Ice::Object
{
public:

    ::IceGrid::ApplicationDescriptor parse(const ::std::string& xmlFile, const ::IceGrid::AdminPrx& adminProxy)
    {
        return parse(xmlFile, adminProxy, 0);
    }
    ::IceGrid::ApplicationDescriptor parse(const ::std::string& xmlFile, const ::IceGrid::AdminPrx& adminProxy, const ::Ice::Context& __ctx)
    {
        return parse(xmlFile, adminProxy, &__ctx);
    }
    
private:

    ::IceGrid::ApplicationDescriptor parse(const ::std::string&, const ::IceGrid::AdminPrx&, const ::Ice::Context*);
    
public:
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

}

}

namespace IceDelegate
{

namespace IceGrid
{

class FileParser : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual ::IceGrid::ApplicationDescriptor parse(const ::std::string&, const ::IceGrid::AdminPrx&, const ::Ice::Context*) = 0;
};

}

}

namespace IceDelegateM
{

namespace IceGrid
{

class FileParser : virtual public ::IceDelegate::IceGrid::FileParser,
                   virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual ::IceGrid::ApplicationDescriptor parse(const ::std::string&, const ::IceGrid::AdminPrx&, const ::Ice::Context*);
};

}

}

namespace IceDelegateD
{

namespace IceGrid
{

class FileParser : virtual public ::IceDelegate::IceGrid::FileParser,
                   virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual ::IceGrid::ApplicationDescriptor parse(const ::std::string&, const ::IceGrid::AdminPrx&, const ::Ice::Context*);
};

}

}

namespace IceGrid
{

class FileParser : virtual public ::Ice::Object
{
public:

    typedef FileParserPrx ProxyType;
    typedef FileParserPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::IceGrid::ApplicationDescriptor parse(const ::std::string&, const ::IceGrid::AdminPrx&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___parse(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void __patch__FileParserPtr(void*, ::Ice::ObjectPtr&);

}

#endif
