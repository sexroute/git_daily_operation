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

#include <BHACO.h>
#include <Ice/LocalException.h>
#include <Ice/ObjectFactory.h>
#include <Ice/BasicStream.h>
#include <Ice/Object.h>
#include <IceUtil/Iterator.h>
#include <IceUtil/ScopedArray.h>

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

static const ::std::string __BHAco__Request__RequestData_name = "RequestData";

static const ::std::string __BHAco__Request__sayHello_name = "sayHello";

static const ::std::string __BHAco__Request__shutdown_name = "shutdown";

void
IceInternal::incRef(::BHAco::Request* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::BHAco::Request* p)
{
    p->__decRef();
}

void
IceInternal::incRef(::IceProxy::BHAco::Request* p)
{
    p->__incRef();
}

void
IceInternal::decRef(::IceProxy::BHAco::Request* p)
{
    p->__decRef();
}

void
BHAco::__write(::IceInternal::BasicStream* __os, const ::BHAco::RequestPrx& v)
{
    __os->write(::Ice::ObjectPrx(v));
}

void
BHAco::__read(::IceInternal::BasicStream* __is, ::BHAco::RequestPrx& v)
{
    ::Ice::ObjectPrx proxy;
    __is->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::BHAco::Request;
        v->__copyFrom(proxy);
    }
}

void
BHAco::__write(::IceInternal::BasicStream* __os, const ::BHAco::RequestPtr& v)
{
    __os->write(::Ice::ObjectPtr(v));
}

BHAco::GenericError::GenericError(const ::std::string& __ice_reason) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    UserException(),
#else
    ::Ice::UserException(),
#endif
    reason(__ice_reason)
{
}

BHAco::GenericError::~GenericError() throw()
{
}

static const char* __BHAco__GenericError_name = "BHAco::GenericError";

::std::string
BHAco::GenericError::ice_name() const
{
    return __BHAco__GenericError_name;
}

::Ice::Exception*
BHAco::GenericError::ice_clone() const
{
    return new GenericError(*this);
}

void
BHAco::GenericError::ice_throw() const
{
    throw *this;
}

void
BHAco::GenericError::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::BHAco::GenericError"), false);
    __os->startWriteSlice();
    __os->write(reason);
    __os->endWriteSlice();
}

void
BHAco::GenericError::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->read(myId, false);
    }
    __is->startReadSlice();
    __is->read(reason);
    __is->endReadSlice();
}

void
BHAco::GenericError::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception BHAco::GenericError was not generated with stream support";
    throw ex;
}

void
BHAco::GenericError::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception BHAco::GenericError was not generated with stream support";
    throw ex;
}

struct __F__BHAco__GenericError : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
        throw ::BHAco::GenericError();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__BHAco__GenericError__Ptr = new __F__BHAco__GenericError;

const ::IceInternal::UserExceptionFactoryPtr&
BHAco::GenericError::ice_factory()
{
    return __F__BHAco__GenericError__Ptr;
}

class __F__BHAco__GenericError__Init
{
public:

    __F__BHAco__GenericError__Init()
    {
        ::IceInternal::factoryTable->addExceptionFactory("::BHAco::GenericError", ::BHAco::GenericError::ice_factory());
    }

    ~__F__BHAco__GenericError__Init()
    {
        ::IceInternal::factoryTable->removeExceptionFactory("::BHAco::GenericError");
    }
};

static __F__BHAco__GenericError__Init __F__BHAco__GenericError__i;

#ifdef __APPLE__
extern "C" { void __F__BHAco__GenericError__initializer() {} }
#endif

BHAco::PermissionDenied::PermissionDenied(const ::std::string& __ice_reason) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    GenericError(__ice_reason)
#else
    ::BHAco::GenericError(__ice_reason)
#endif
{
}

BHAco::PermissionDenied::~PermissionDenied() throw()
{
}

static const char* __BHAco__PermissionDenied_name = "BHAco::PermissionDenied";

::std::string
BHAco::PermissionDenied::ice_name() const
{
    return __BHAco__PermissionDenied_name;
}

::Ice::Exception*
BHAco::PermissionDenied::ice_clone() const
{
    return new PermissionDenied(*this);
}

void
BHAco::PermissionDenied::ice_throw() const
{
    throw *this;
}

void
BHAco::PermissionDenied::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::BHAco::PermissionDenied"), false);
    __os->startWriteSlice();
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    GenericError::__write(__os);
#else
    ::BHAco::GenericError::__write(__os);
#endif
}

void
BHAco::PermissionDenied::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->read(myId, false);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    GenericError::__read(__is, true);
#else
    ::BHAco::GenericError::__read(__is, true);
#endif
}

void
BHAco::PermissionDenied::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception BHAco::PermissionDenied was not generated with stream support";
    throw ex;
}

void
BHAco::PermissionDenied::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception BHAco::PermissionDenied was not generated with stream support";
    throw ex;
}

struct __F__BHAco__PermissionDenied : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
        throw ::BHAco::PermissionDenied();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__BHAco__PermissionDenied__Ptr = new __F__BHAco__PermissionDenied;

const ::IceInternal::UserExceptionFactoryPtr&
BHAco::PermissionDenied::ice_factory()
{
    return __F__BHAco__PermissionDenied__Ptr;
}

class __F__BHAco__PermissionDenied__Init
{
public:

    __F__BHAco__PermissionDenied__Init()
    {
        ::IceInternal::factoryTable->addExceptionFactory("::BHAco::PermissionDenied", ::BHAco::PermissionDenied::ice_factory());
    }

    ~__F__BHAco__PermissionDenied__Init()
    {
        ::IceInternal::factoryTable->removeExceptionFactory("::BHAco::PermissionDenied");
    }
};

static __F__BHAco__PermissionDenied__Init __F__BHAco__PermissionDenied__i;

#ifdef __APPLE__
extern "C" { void __F__BHAco__PermissionDenied__initializer() {} }
#endif

BHAco::NameInUse::NameInUse(const ::std::string& __ice_reason) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    GenericError(__ice_reason)
#else
    ::BHAco::GenericError(__ice_reason)
#endif
{
}

BHAco::NameInUse::~NameInUse() throw()
{
}

static const char* __BHAco__NameInUse_name = "BHAco::NameInUse";

::std::string
BHAco::NameInUse::ice_name() const
{
    return __BHAco__NameInUse_name;
}

::Ice::Exception*
BHAco::NameInUse::ice_clone() const
{
    return new NameInUse(*this);
}

void
BHAco::NameInUse::ice_throw() const
{
    throw *this;
}

void
BHAco::NameInUse::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::BHAco::NameInUse"), false);
    __os->startWriteSlice();
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    GenericError::__write(__os);
#else
    ::BHAco::GenericError::__write(__os);
#endif
}

void
BHAco::NameInUse::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->read(myId, false);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    GenericError::__read(__is, true);
#else
    ::BHAco::GenericError::__read(__is, true);
#endif
}

void
BHAco::NameInUse::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception BHAco::NameInUse was not generated with stream support";
    throw ex;
}

void
BHAco::NameInUse::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception BHAco::NameInUse was not generated with stream support";
    throw ex;
}

struct __F__BHAco__NameInUse : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
        throw ::BHAco::NameInUse();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__BHAco__NameInUse__Ptr = new __F__BHAco__NameInUse;

const ::IceInternal::UserExceptionFactoryPtr&
BHAco::NameInUse::ice_factory()
{
    return __F__BHAco__NameInUse__Ptr;
}

class __F__BHAco__NameInUse__Init
{
public:

    __F__BHAco__NameInUse__Init()
    {
        ::IceInternal::factoryTable->addExceptionFactory("::BHAco::NameInUse", ::BHAco::NameInUse::ice_factory());
    }

    ~__F__BHAco__NameInUse__Init()
    {
        ::IceInternal::factoryTable->removeExceptionFactory("::BHAco::NameInUse");
    }
};

static __F__BHAco__NameInUse__Init __F__BHAco__NameInUse__i;

#ifdef __APPLE__
extern "C" { void __F__BHAco__NameInUse__initializer() {} }
#endif

BHAco::NoSuchName::NoSuchName(const ::std::string& __ice_reason) :
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    GenericError(__ice_reason)
#else
    ::BHAco::GenericError(__ice_reason)
#endif
{
}

BHAco::NoSuchName::~NoSuchName() throw()
{
}

static const char* __BHAco__NoSuchName_name = "BHAco::NoSuchName";

::std::string
BHAco::NoSuchName::ice_name() const
{
    return __BHAco__NoSuchName_name;
}

::Ice::Exception*
BHAco::NoSuchName::ice_clone() const
{
    return new NoSuchName(*this);
}

void
BHAco::NoSuchName::ice_throw() const
{
    throw *this;
}

void
BHAco::NoSuchName::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(::std::string("::BHAco::NoSuchName"), false);
    __os->startWriteSlice();
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    GenericError::__write(__os);
#else
    ::BHAco::GenericError::__write(__os);
#endif
}

void
BHAco::NoSuchName::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->read(myId, false);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    GenericError::__read(__is, true);
#else
    ::BHAco::GenericError::__read(__is, true);
#endif
}

void
BHAco::NoSuchName::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception BHAco::NoSuchName was not generated with stream support";
    throw ex;
}

void
BHAco::NoSuchName::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "exception BHAco::NoSuchName was not generated with stream support";
    throw ex;
}

struct __F__BHAco__NoSuchName : public ::IceInternal::UserExceptionFactory
{
    virtual void
    createAndThrow()
    {
        throw ::BHAco::NoSuchName();
    }
};

static ::IceInternal::UserExceptionFactoryPtr __F__BHAco__NoSuchName__Ptr = new __F__BHAco__NoSuchName;

const ::IceInternal::UserExceptionFactoryPtr&
BHAco::NoSuchName::ice_factory()
{
    return __F__BHAco__NoSuchName__Ptr;
}

class __F__BHAco__NoSuchName__Init
{
public:

    __F__BHAco__NoSuchName__Init()
    {
        ::IceInternal::factoryTable->addExceptionFactory("::BHAco::NoSuchName", ::BHAco::NoSuchName::ice_factory());
    }

    ~__F__BHAco__NoSuchName__Init()
    {
        ::IceInternal::factoryTable->removeExceptionFactory("::BHAco::NoSuchName");
    }
};

static __F__BHAco__NoSuchName__Init __F__BHAco__NoSuchName__i;

#ifdef __APPLE__
extern "C" { void __F__BHAco__NoSuchName__initializer() {} }
#endif

void
BHAco::__write(::IceInternal::BasicStream* __os, ::BHAco::EFieldType v)
{
    __os->write(static_cast< ::Ice::Byte>(v));
}

void
BHAco::__read(::IceInternal::BasicStream* __is, ::BHAco::EFieldType& v)
{
    ::Ice::Byte val;
    __is->read(val);
    v = static_cast< ::BHAco::EFieldType>(val);
}

bool
BHAco::SFieldContent::operator==(const SFieldContent& __rhs) const
{
    return !operator!=(__rhs);
}

bool
BHAco::SFieldContent::operator!=(const SFieldContent& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(fType != __rhs.fType)
    {
        return true;
    }
    if(strValue != __rhs.strValue)
    {
        return true;
    }
    if(iValue != __rhs.iValue)
    {
        return true;
    }
    if(fValue != __rhs.fValue)
    {
        return true;
    }
    if(buf != __rhs.buf)
    {
        return true;
    }
    return false;
}

bool
BHAco::SFieldContent::operator<(const SFieldContent& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(fType < __rhs.fType)
    {
        return true;
    }
    else if(__rhs.fType < fType)
    {
        return false;
    }
    if(strValue < __rhs.strValue)
    {
        return true;
    }
    else if(__rhs.strValue < strValue)
    {
        return false;
    }
    if(iValue < __rhs.iValue)
    {
        return true;
    }
    else if(__rhs.iValue < iValue)
    {
        return false;
    }
    if(fValue < __rhs.fValue)
    {
        return true;
    }
    else if(__rhs.fValue < fValue)
    {
        return false;
    }
    if(buf < __rhs.buf)
    {
        return true;
    }
    else if(__rhs.buf < buf)
    {
        return false;
    }
    return false;
}

void
BHAco::SFieldContent::__write(::IceInternal::BasicStream* __os) const
{
    ::BHAco::__write(__os, fType);
    __os->write(strValue);
    __os->write(iValue);
    __os->write(fValue);
    if(buf.size() == 0)
    {
        __os->writeSize(0);
    }
    else
    {
        __os->write(&buf[0], &buf[0] + buf.size());
    }
}

void
BHAco::SFieldContent::__read(::IceInternal::BasicStream* __is)
{
    ::BHAco::__read(__is, fType);
    __is->read(strValue);
    __is->read(iValue);
    __is->read(fValue);
    ::std::pair<const ::Ice::Byte*, const ::Ice::Byte*> ___buf;
    __is->read(___buf);
    ::std::vector< ::Ice::Byte>(___buf.first, ___buf.second).swap(buf);
}

void
BHAco::__write(::IceInternal::BasicStream* __os, const ::BHAco::RowDetail& v, ::BHAco::__U__RowDetail)
{
    __os->writeSize(::Ice::Int(v.size()));
    ::BHAco::RowDetail::const_iterator p;
    for(p = v.begin(); p != v.end(); ++p)
    {
        __os->write(p->first);
        p->second.__write(__os);
    }
}

void
BHAco::__read(::IceInternal::BasicStream* __is, ::BHAco::RowDetail& v, ::BHAco::__U__RowDetail)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    while(sz--)
    {
        ::std::pair<const  ::Ice::Int, ::BHAco::SFieldContent> pair;
        __is->read(const_cast< ::Ice::Int&>(pair.first));
        ::BHAco::RowDetail::iterator __i = v.insert(v.end(), pair);
        __i->second.__read(__is);
    }
}

void
BHAco::__write(::IceInternal::BasicStream* __os, const ::BHAco::RowDetail* begin, const ::BHAco::RowDetail* end, ::BHAco::__U__PackageDetail)
{
    ::Ice::Int size = static_cast< ::Ice::Int>(end - begin);
    __os->writeSize(size);
    for(int i = 0; i < size; ++i)
    {
        ::BHAco::__write(__os, begin[i], ::BHAco::__U__RowDetail());
    }
}

void
BHAco::__read(::IceInternal::BasicStream* __is, ::BHAco::PackageDetail& v, ::BHAco::__U__PackageDetail)
{
    ::Ice::Int sz;
    __is->readSize(sz);
    __is->startSeq(sz, 1);
    v.resize(sz);
    for(int i = 0; i < sz; ++i)
    {
        ::BHAco::__read(__is, v[i], ::BHAco::__U__RowDetail());
        __is->checkSeq();
        __is->endElement();
    }
    __is->endSeq(sz);
}

bool
BHAco::SMessageHeader::operator==(const SMessageHeader& __rhs) const
{
    return !operator!=(__rhs);
}

bool
BHAco::SMessageHeader::operator!=(const SMessageHeader& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(nServiceCode != __rhs.nServiceCode)
    {
        return true;
    }
    if(nVersion != __rhs.nVersion)
    {
        return true;
    }
    if(bodylength != __rhs.bodylength)
    {
        return true;
    }
    if(sessionid != __rhs.sessionid)
    {
        return true;
    }
    return false;
}

bool
BHAco::SMessageHeader::operator<(const SMessageHeader& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(nServiceCode < __rhs.nServiceCode)
    {
        return true;
    }
    else if(__rhs.nServiceCode < nServiceCode)
    {
        return false;
    }
    if(nVersion < __rhs.nVersion)
    {
        return true;
    }
    else if(__rhs.nVersion < nVersion)
    {
        return false;
    }
    if(bodylength < __rhs.bodylength)
    {
        return true;
    }
    else if(__rhs.bodylength < bodylength)
    {
        return false;
    }
    if(sessionid < __rhs.sessionid)
    {
        return true;
    }
    else if(__rhs.sessionid < sessionid)
    {
        return false;
    }
    return false;
}

void
BHAco::SMessageHeader::__write(::IceInternal::BasicStream* __os) const
{
    __os->write(nServiceCode);
    __os->write(nVersion);
    __os->write(bodylength);
    __os->write(sessionid);
}

void
BHAco::SMessageHeader::__read(::IceInternal::BasicStream* __is)
{
    __is->read(nServiceCode);
    __is->read(nVersion);
    __is->read(bodylength);
    __is->read(sessionid);
}

bool
BHAco::SDataPacket::operator==(const SDataPacket& __rhs) const
{
    return !operator!=(__rhs);
}

bool
BHAco::SDataPacket::operator!=(const SDataPacket& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(msgHeader != __rhs.msgHeader)
    {
        return true;
    }
    if(packet != __rhs.packet)
    {
        return true;
    }
    if(curWarnings != __rhs.curWarnings)
    {
        return true;
    }
    if(nRetCode != __rhs.nRetCode)
    {
        return true;
    }
    return false;
}

bool
BHAco::SDataPacket::operator<(const SDataPacket& __rhs) const
{
    if(this == &__rhs)
    {
        return false;
    }
    if(msgHeader < __rhs.msgHeader)
    {
        return true;
    }
    else if(__rhs.msgHeader < msgHeader)
    {
        return false;
    }
    if(packet < __rhs.packet)
    {
        return true;
    }
    else if(__rhs.packet < packet)
    {
        return false;
    }
    if(curWarnings < __rhs.curWarnings)
    {
        return true;
    }
    else if(__rhs.curWarnings < curWarnings)
    {
        return false;
    }
    if(nRetCode < __rhs.nRetCode)
    {
        return true;
    }
    else if(__rhs.nRetCode < nRetCode)
    {
        return false;
    }
    return false;
}

void
BHAco::SDataPacket::__write(::IceInternal::BasicStream* __os) const
{
    msgHeader.__write(__os);
    if(packet.size() == 0)
    {
        __os->writeSize(0);
    }
    else
    {
        ::BHAco::__write(__os, &packet[0], &packet[0] + packet.size(), ::BHAco::__U__PackageDetail());
    }
    if(curWarnings.size() == 0)
    {
        __os->writeSize(0);
    }
    else
    {
        __os->write(&curWarnings[0], &curWarnings[0] + curWarnings.size());
    }
    __os->write(nRetCode);
}

void
BHAco::SDataPacket::__read(::IceInternal::BasicStream* __is)
{
    msgHeader.__read(__is);
    ::BHAco::__read(__is, packet, ::BHAco::__U__PackageDetail());
    __is->read(curWarnings);
    __is->read(nRetCode);
}

void
BHAco::__addObject(const RequestPtr& p, ::IceInternal::GCCountMap& c)
{
    p->__addObject(c);
}

bool
BHAco::__usesClasses(const RequestPtr& p)
{
    return p->__usesClasses();
}

void
BHAco::__decRefUnsafe(const RequestPtr& p)
{
    p->__decRefUnsafe();
}

void
BHAco::__clearHandleUnsafe(RequestPtr& p)
{
    p.__clearHandleUnsafe();
}

::BHAco::SDataPacket
IceProxy::BHAco::Request::RequestData(const ::BHAco::SDataPacket& dp, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __checkTwowayOnly(__BHAco__Request__RequestData_name);
            __delBase = __getDelegate();
            ::IceDelegate::BHAco::Request* __del = dynamic_cast< ::IceDelegate::BHAco::Request*>(__delBase.get());
            return __del->RequestData(dp, __ctx);
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, __cnt);
        }
    }
}

void
IceProxy::BHAco::Request::sayHello(::Ice::Int delay, const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __delBase = __getDelegate();
            ::IceDelegate::BHAco::Request* __del = dynamic_cast< ::IceDelegate::BHAco::Request*>(__delBase.get());
            __del->sayHello(delay, __ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapperRelaxed(__delBase, __ex, __cnt);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, __cnt);
        }
    }
}

void
IceProxy::BHAco::Request::shutdown(const ::Ice::Context* __ctx)
{
    int __cnt = 0;
    while(true)
    {
        ::IceInternal::Handle< ::IceDelegate::Ice::Object> __delBase;
        try
        {
            __delBase = __getDelegate();
            ::IceDelegate::BHAco::Request* __del = dynamic_cast< ::IceDelegate::BHAco::Request*>(__delBase.get());
            __del->shutdown(__ctx);
            return;
        }
        catch(const ::IceInternal::LocalExceptionWrapper& __ex)
        {
            __handleExceptionWrapper(__delBase, __ex);
        }
        catch(const ::Ice::LocalException& __ex)
        {
            __handleException(__delBase, __ex, __cnt);
        }
    }
}

const ::std::string&
IceProxy::BHAco::Request::ice_staticId()
{
    return ::BHAco::Request::ice_staticId();
}

::IceInternal::Handle< ::IceDelegateM::Ice::Object>
IceProxy::BHAco::Request::__createDelegateM()
{
    return ::IceInternal::Handle< ::IceDelegateM::Ice::Object>(new ::IceDelegateM::BHAco::Request);
}

::IceInternal::Handle< ::IceDelegateD::Ice::Object>
IceProxy::BHAco::Request::__createDelegateD()
{
    return ::IceInternal::Handle< ::IceDelegateD::Ice::Object>(new ::IceDelegateD::BHAco::Request);
}

bool
IceProxy::BHAco::operator==(const ::IceProxy::BHAco::Request& l, const ::IceProxy::BHAco::Request& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) == static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::BHAco::operator!=(const ::IceProxy::BHAco::Request& l, const ::IceProxy::BHAco::Request& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) != static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::BHAco::operator<(const ::IceProxy::BHAco::Request& l, const ::IceProxy::BHAco::Request& r)
{
    return static_cast<const ::IceProxy::Ice::Object&>(l) < static_cast<const ::IceProxy::Ice::Object&>(r);
}

bool
IceProxy::BHAco::operator<=(const ::IceProxy::BHAco::Request& l, const ::IceProxy::BHAco::Request& r)
{
    return l < r || l == r;
}

bool
IceProxy::BHAco::operator>(const ::IceProxy::BHAco::Request& l, const ::IceProxy::BHAco::Request& r)
{
    return !(l < r) && !(l == r);
}

bool
IceProxy::BHAco::operator>=(const ::IceProxy::BHAco::Request& l, const ::IceProxy::BHAco::Request& r)
{
    return !(l < r);
}

::BHAco::SDataPacket
IceDelegateM::BHAco::Request::RequestData(const ::BHAco::SDataPacket& dp, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __BHAco__Request__RequestData_name, ::Ice::Normal, __context, __compress);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        dp.__write(__os);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        ::IceInternal::BasicStream* __is = __og.is();
        if(!__ok)
        {
            try
            {
                __is->throwException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
            }
        }
        ::BHAco::SDataPacket __ret;
        __ret.__read(__is);
        return __ret;
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::BHAco::Request::sayHello(::Ice::Int delay, const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __BHAco__Request__sayHello_name, ::Ice::Idempotent, __context, __compress);
    try
    {
        ::IceInternal::BasicStream* __os = __og.os();
        __os->write(delay);
    }
    catch(const ::Ice::LocalException& __ex)
    {
        __og.abort(__ex);
    }
    bool __ok = __og.invoke();
    try
    {
        ::IceInternal::BasicStream* __is = __og.is();
        if(!__ok)
        {
            try
            {
                __is->throwException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
            }
        }
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

void
IceDelegateM::BHAco::Request::shutdown(const ::Ice::Context* __context)
{
    ::IceInternal::Outgoing __og(__connection.get(), __reference.get(), __BHAco__Request__shutdown_name, ::Ice::Normal, __context, __compress);
    bool __ok = __og.invoke();
    try
    {
        ::IceInternal::BasicStream* __is = __og.is();
        if(!__ok)
        {
            try
            {
                __is->throwException();
            }
            catch(const ::Ice::UserException& __ex)
            {
                throw ::Ice::UnknownUserException(__FILE__, __LINE__, __ex.ice_name());
            }
        }
    }
    catch(const ::Ice::LocalException& __ex)
    {
        throw ::IceInternal::LocalExceptionWrapper(__ex, false);
    }
}

::BHAco::SDataPacket
IceDelegateD::BHAco::Request::RequestData(const ::BHAco::SDataPacket& dp, const ::Ice::Context* __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __BHAco__Request__RequestData_name, ::Ice::Normal, __context);
    while(true)
    {
        ::IceInternal::Direct __direct(__current);
        ::BHAco::SDataPacket __ret;
        try
        {
            ::BHAco::Request* __servant = dynamic_cast< ::BHAco::Request*>(__direct.servant().get());
            if(!__servant)
            {
                ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
                __opEx.id = __current.id;
                __opEx.facet = __current.facet;
                __opEx.operation = __current.operation;
                throw __opEx;
            }
            try
            {
                __ret = __servant->RequestData(dp, __current);
            }
            catch(const ::Ice::LocalException& __ex)
            {
                throw ::IceInternal::LocalExceptionWrapper(__ex, false);
            }
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
        return __ret;
    }
}

void
IceDelegateD::BHAco::Request::sayHello(::Ice::Int delay, const ::Ice::Context* __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __BHAco__Request__sayHello_name, ::Ice::Idempotent, __context);
    while(true)
    {
        ::IceInternal::Direct __direct(__current);
        try
        {
            ::BHAco::Request* __servant = dynamic_cast< ::BHAco::Request*>(__direct.servant().get());
            if(!__servant)
            {
                ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
                __opEx.id = __current.id;
                __opEx.facet = __current.facet;
                __opEx.operation = __current.operation;
                throw __opEx;
            }
            try
            {
                __servant->sayHello(delay, __current);
            }
            catch(const ::Ice::LocalException& __ex)
            {
                throw ::IceInternal::LocalExceptionWrapper(__ex, false);
            }
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
        return;
    }
}

void
IceDelegateD::BHAco::Request::shutdown(const ::Ice::Context* __context)
{
    ::Ice::Current __current;
    __initCurrent(__current, __BHAco__Request__shutdown_name, ::Ice::Normal, __context);
    while(true)
    {
        ::IceInternal::Direct __direct(__current);
        try
        {
            ::BHAco::Request* __servant = dynamic_cast< ::BHAco::Request*>(__direct.servant().get());
            if(!__servant)
            {
                ::Ice::OperationNotExistException __opEx(__FILE__, __LINE__);
                __opEx.id = __current.id;
                __opEx.facet = __current.facet;
                __opEx.operation = __current.operation;
                throw __opEx;
            }
            try
            {
                __servant->shutdown(__current);
            }
            catch(const ::Ice::LocalException& __ex)
            {
                throw ::IceInternal::LocalExceptionWrapper(__ex, false);
            }
        }
        catch(...)
        {
            __direct.destroy();
            throw;
        }
        __direct.destroy();
        return;
    }
}

::Ice::ObjectPtr
BHAco::Request::ice_clone() const
{
    throw ::Ice::CloneNotImplementedException(__FILE__, __LINE__);
    return 0; // to avoid a warning with some compilers
}

static const ::std::string __BHAco__Request_ids[2] =
{
    "::BHAco::Request",
    "::Ice::Object"
};

bool
BHAco::Request::ice_isA(const ::std::string& _s, const ::Ice::Current&) const
{
    return ::std::binary_search(__BHAco__Request_ids, __BHAco__Request_ids + 2, _s);
}

::std::vector< ::std::string>
BHAco::Request::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&__BHAco__Request_ids[0], &__BHAco__Request_ids[2]);
}

const ::std::string&
BHAco::Request::ice_id(const ::Ice::Current&) const
{
    return __BHAco__Request_ids[0];
}

const ::std::string&
BHAco::Request::ice_staticId()
{
    return __BHAco__Request_ids[0];
}

::IceInternal::DispatchStatus
BHAco::Request::___RequestData(::IceInternal::Incoming&__inS, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::IceInternal::BasicStream* __os = __inS.os();
    ::BHAco::SDataPacket dp;
    dp.__read(__is);
    ::BHAco::SDataPacket __ret = RequestData(dp, __current);
    __ret.__write(__os);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
BHAco::Request::___sayHello(::IceInternal::Incoming&__inS, const ::Ice::Current& __current) const
{
    __checkMode(::Ice::Idempotent, __current.mode);
    ::IceInternal::BasicStream* __is = __inS.is();
    ::Ice::Int delay;
    __is->read(delay);
    sayHello(delay, __current);
    return ::IceInternal::DispatchOK;
}

::IceInternal::DispatchStatus
BHAco::Request::___shutdown(::IceInternal::Incoming&, const ::Ice::Current& __current)
{
    __checkMode(::Ice::Normal, __current.mode);
    shutdown(__current);
    return ::IceInternal::DispatchOK;
}

static ::std::string __BHAco__Request_all[] =
{
    "RequestData",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "sayHello",
    "shutdown"
};

::IceInternal::DispatchStatus
BHAco::Request::__dispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair< ::std::string*, ::std::string*> r = ::std::equal_range(__BHAco__Request_all, __BHAco__Request_all + 7, current.operation);
    if(r.first == r.second)
    {
        return ::IceInternal::DispatchOperationNotExist;
    }

    switch(r.first - __BHAco__Request_all)
    {
        case 0:
        {
            return ___RequestData(in, current);
        }
        case 1:
        {
            return ___ice_id(in, current);
        }
        case 2:
        {
            return ___ice_ids(in, current);
        }
        case 3:
        {
            return ___ice_isA(in, current);
        }
        case 4:
        {
            return ___ice_ping(in, current);
        }
        case 5:
        {
            return ___sayHello(in, current);
        }
        case 6:
        {
            return ___shutdown(in, current);
        }
    }

    assert(false);
    return ::IceInternal::DispatchOperationNotExist;
}

void
BHAco::Request::__write(::IceInternal::BasicStream* __os) const
{
    __os->writeTypeId(ice_staticId());
    __os->startWriteSlice();
    __os->endWriteSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__write(__os);
#else
    ::Ice::Object::__write(__os);
#endif
}

void
BHAco::Request::__read(::IceInternal::BasicStream* __is, bool __rid)
{
    if(__rid)
    {
        ::std::string myId;
        __is->readTypeId(myId);
    }
    __is->startReadSlice();
    __is->endReadSlice();
#if defined(_MSC_VER) && (_MSC_VER < 1300) // VC++ 6 compiler bug
    Object::__read(__is, true);
#else
    ::Ice::Object::__read(__is, true);
#endif
}

void
BHAco::Request::__write(const ::Ice::OutputStreamPtr&) const
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type BHAco::Request was not generated with stream support";
    throw ex;
}

void
BHAco::Request::__read(const ::Ice::InputStreamPtr&, bool)
{
    Ice::MarshalException ex(__FILE__, __LINE__);
    ex.reason = "type BHAco::Request was not generated with stream support";
    throw ex;
}

void 
BHAco::__patch__RequestPtr(void* __addr, ::Ice::ObjectPtr& v)
{
    ::BHAco::RequestPtr* p = static_cast< ::BHAco::RequestPtr*>(__addr);
    assert(p);
    *p = ::BHAco::RequestPtr::dynamicCast(v);
    if(v && !*p)
    {
        ::Ice::UnexpectedObjectException e(__FILE__, __LINE__);
        e.type = v->ice_id();
        e.expectedType = ::BHAco::Request::ice_staticId();
        throw e;
    }
}

bool
BHAco::operator==(const ::BHAco::Request& l, const ::BHAco::Request& r)
{
    return static_cast<const ::Ice::Object&>(l) == static_cast<const ::Ice::Object&>(r);
}

bool
BHAco::operator!=(const ::BHAco::Request& l, const ::BHAco::Request& r)
{
    return static_cast<const ::Ice::Object&>(l) != static_cast<const ::Ice::Object&>(r);
}

bool
BHAco::operator<(const ::BHAco::Request& l, const ::BHAco::Request& r)
{
    return static_cast<const ::Ice::Object&>(l) < static_cast<const ::Ice::Object&>(r);
}

bool
BHAco::operator<=(const ::BHAco::Request& l, const ::BHAco::Request& r)
{
    return l < r || l == r;
}

bool
BHAco::operator>(const ::BHAco::Request& l, const ::BHAco::Request& r)
{
    return !(l < r) && !(l == r);
}

bool
BHAco::operator>=(const ::BHAco::Request& l, const ::BHAco::Request& r)
{
    return !(l < r);
}
