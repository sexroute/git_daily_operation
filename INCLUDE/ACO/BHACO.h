// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.2.1
// Generated from file `BHACO.ice'

#ifndef __BHACO_h__
#define __BHACO_h__

#include <Ice/LocalObjectF.h>
#include <Ice/ProxyF.h>
#include <Ice/ObjectF.h>
#include <Ice/Exception.h>
#include <Ice/LocalObject.h>
#include <Ice/Proxy.h>
#include <Ice/Object.h>
#include <Ice/Outgoing.h>
#include <Ice/Incoming.h>
#include <Ice/Direct.h>
#include <Ice/UserExceptionFactory.h>
#include <Ice/FactoryTable.h>
#include <Ice/StreamF.h>
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

namespace BHAco
{

class Request;
bool operator==(const Request&, const Request&);
bool operator!=(const Request&, const Request&);
bool operator<(const Request&, const Request&);
bool operator<=(const Request&, const Request&);
bool operator>(const Request&, const Request&);
bool operator>=(const Request&, const Request&);

}

}

namespace BHAco
{

class Request;
bool operator==(const Request&, const Request&);
bool operator!=(const Request&, const Request&);
bool operator<(const Request&, const Request&);
bool operator<=(const Request&, const Request&);
bool operator>(const Request&, const Request&);
bool operator>=(const Request&, const Request&);

}

namespace IceInternal
{

void incRef(::BHAco::Request*);
void decRef(::BHAco::Request*);

void incRef(::IceProxy::BHAco::Request*);
void decRef(::IceProxy::BHAco::Request*);

}

namespace BHAco
{

typedef ::IceInternal::Handle< ::BHAco::Request> RequestPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::BHAco::Request> RequestPrx;

void __write(::IceInternal::BasicStream*, const RequestPrx&);
void __read(::IceInternal::BasicStream*, RequestPrx&);
void __write(::IceInternal::BasicStream*, const RequestPtr&);
void __patch__RequestPtr(void*, ::Ice::ObjectPtr&);

void __addObject(const RequestPtr&, ::IceInternal::GCCountMap&);
bool __usesClasses(const RequestPtr&);
void __decRefUnsafe(const RequestPtr&);
void __clearHandleUnsafe(RequestPtr&);

}

namespace BHAco
{

class GenericError : public ::Ice::UserException
{
public:

    GenericError() {}
    explicit GenericError(const ::std::string&);
    virtual ~GenericError() throw();

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

static GenericError __GenericError_init;

class PermissionDenied : public ::BHAco::GenericError
{
public:

    PermissionDenied() {}
    explicit PermissionDenied(const ::std::string&);
    virtual ~PermissionDenied() throw();

    virtual ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);

    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

class NameInUse : public ::BHAco::GenericError
{
public:

    NameInUse() {}
    explicit NameInUse(const ::std::string&);
    virtual ~NameInUse() throw();

    virtual ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);

    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

class NoSuchName : public ::BHAco::GenericError
{
public:

    NoSuchName() {}
    explicit NoSuchName(const ::std::string&);
    virtual ~NoSuchName() throw();

    virtual ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);

    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

enum EFieldType
{
    FTFloat,
    FTInt,
    FTString,
    FTDateTime,
    FTBytes,
    FTBool,
    FTChar
};

void __write(::IceInternal::BasicStream*, EFieldType);
void __read(::IceInternal::BasicStream*, EFieldType&);

typedef ::std::vector< ::Ice::Byte> Buffers;

class __U__Buffers { };

struct SFieldContent
{
    ::BHAco::EFieldType fType;
    ::std::string strValue;
    ::Ice::Int iValue;
    ::Ice::Float fValue;
    ::BHAco::Buffers buf;

    bool operator==(const SFieldContent&) const;
    bool operator!=(const SFieldContent&) const;
    bool operator<(const SFieldContent&) const;
    bool operator<=(const SFieldContent& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const SFieldContent& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const SFieldContent& __rhs) const
    {
        return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

typedef ::std::map< ::Ice::Int, ::BHAco::SFieldContent> RowDetail;

class __U__RowDetail { };
void __write(::IceInternal::BasicStream*, const RowDetail&, __U__RowDetail);
void __read(::IceInternal::BasicStream*, RowDetail&, __U__RowDetail);

typedef ::std::vector< ::BHAco::RowDetail> PackageDetail;

class __U__PackageDetail { };
void __write(::IceInternal::BasicStream*, const ::BHAco::RowDetail*, const ::BHAco::RowDetail*, __U__PackageDetail);
void __read(::IceInternal::BasicStream*, PackageDetail&, __U__PackageDetail);

typedef ::std::vector< ::std::string> Warnings;

class __U__Warnings { };

struct SMessageHeader
{
    ::Ice::Short nServiceCode;
    ::Ice::Byte nVersion;
    ::Ice::Short bodylength;
    ::std::string sessionid;

    bool operator==(const SMessageHeader&) const;
    bool operator!=(const SMessageHeader&) const;
    bool operator<(const SMessageHeader&) const;
    bool operator<=(const SMessageHeader& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const SMessageHeader& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const SMessageHeader& __rhs) const
    {
        return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

struct SDataPacket
{
    ::BHAco::SMessageHeader msgHeader;
    ::BHAco::PackageDetail packet;
    ::BHAco::Warnings curWarnings;
    ::Ice::Int nRetCode;

    bool operator==(const SDataPacket&) const;
    bool operator!=(const SDataPacket&) const;
    bool operator<(const SDataPacket&) const;
    bool operator<=(const SDataPacket& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    bool operator>(const SDataPacket& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    bool operator>=(const SDataPacket& __rhs) const
    {
        return !operator<(__rhs);
    }

    void __write(::IceInternal::BasicStream*) const;
    void __read(::IceInternal::BasicStream*);
};

}

namespace IceProxy
{

namespace BHAco
{

class Request : virtual public ::IceProxy::Ice::Object
{
public:

    ::BHAco::SDataPacket RequestData(const ::BHAco::SDataPacket& dp)
    {
        return RequestData(dp, 0);
    }
    ::BHAco::SDataPacket RequestData(const ::BHAco::SDataPacket& dp, const ::Ice::Context& __ctx)
    {
        return RequestData(dp, &__ctx);
    }
    
private:

    ::BHAco::SDataPacket RequestData(const ::BHAco::SDataPacket&, const ::Ice::Context*);
    
public:

    void sayHello(::Ice::Int delay)
    {
        sayHello(delay, 0);
    }
    void sayHello(::Ice::Int delay, const ::Ice::Context& __ctx)
    {
        sayHello(delay, &__ctx);
    }
    
private:

    void sayHello(::Ice::Int, const ::Ice::Context*);
    
public:

    void shutdown()
    {
        shutdown(0);
    }
    void shutdown(const ::Ice::Context& __ctx)
    {
        shutdown(&__ctx);
    }
    
private:

    void shutdown(const ::Ice::Context*);
    
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

namespace BHAco
{

class Request : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual ::BHAco::SDataPacket RequestData(const ::BHAco::SDataPacket&, const ::Ice::Context*) = 0;

    virtual void sayHello(::Ice::Int, const ::Ice::Context*) = 0;

    virtual void shutdown(const ::Ice::Context*) = 0;
};

}

}

namespace IceDelegateM
{

namespace BHAco
{

class Request : virtual public ::IceDelegate::BHAco::Request,
                virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual ::BHAco::SDataPacket RequestData(const ::BHAco::SDataPacket&, const ::Ice::Context*);

    virtual void sayHello(::Ice::Int, const ::Ice::Context*);

    virtual void shutdown(const ::Ice::Context*);
};

}

}

namespace IceDelegateD
{

namespace BHAco
{

class Request : virtual public ::IceDelegate::BHAco::Request,
                virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual ::BHAco::SDataPacket RequestData(const ::BHAco::SDataPacket&, const ::Ice::Context*);

    virtual void sayHello(::Ice::Int, const ::Ice::Context*);

    virtual void shutdown(const ::Ice::Context*);
};

}

}

namespace BHAco
{

class Request : virtual public ::Ice::Object
{
public:

    typedef RequestPrx ProxyType;
    typedef RequestPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::BHAco::SDataPacket RequestData(const ::BHAco::SDataPacket&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___RequestData(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void sayHello(::Ice::Int, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___sayHello(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual void shutdown(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___shutdown(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void __patch__RequestPtr(void*, ::Ice::ObjectPtr&);

}

#endif
