// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.2.1
// Generated from file `UserAccountMapper.ice'

#ifndef __IceGrid_UserAccountMapper_h__
#define __IceGrid_UserAccountMapper_h__

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

#ifndef ICE_GRID_API
#   ifdef ICE_GRID_API_EXPORTS
#       define ICE_GRID_API ICE_DECLSPEC_EXPORT
#   else
#       define ICE_GRID_API ICE_DECLSPEC_IMPORT
#   endif
#endif

namespace IceProxy
{

namespace IceGrid
{

class UserAccountMapper;
ICE_GRID_API bool operator==(const UserAccountMapper&, const UserAccountMapper&);
ICE_GRID_API bool operator!=(const UserAccountMapper&, const UserAccountMapper&);
ICE_GRID_API bool operator<(const UserAccountMapper&, const UserAccountMapper&);
ICE_GRID_API bool operator<=(const UserAccountMapper&, const UserAccountMapper&);
ICE_GRID_API bool operator>(const UserAccountMapper&, const UserAccountMapper&);
ICE_GRID_API bool operator>=(const UserAccountMapper&, const UserAccountMapper&);

}

}

namespace IceGrid
{

class UserAccountMapper;
ICE_GRID_API bool operator==(const UserAccountMapper&, const UserAccountMapper&);
ICE_GRID_API bool operator!=(const UserAccountMapper&, const UserAccountMapper&);
ICE_GRID_API bool operator<(const UserAccountMapper&, const UserAccountMapper&);
ICE_GRID_API bool operator<=(const UserAccountMapper&, const UserAccountMapper&);
ICE_GRID_API bool operator>(const UserAccountMapper&, const UserAccountMapper&);
ICE_GRID_API bool operator>=(const UserAccountMapper&, const UserAccountMapper&);

}

namespace IceInternal
{

ICE_GRID_API void incRef(::IceGrid::UserAccountMapper*);
ICE_GRID_API void decRef(::IceGrid::UserAccountMapper*);

ICE_GRID_API void incRef(::IceProxy::IceGrid::UserAccountMapper*);
ICE_GRID_API void decRef(::IceProxy::IceGrid::UserAccountMapper*);

}

namespace IceGrid
{

typedef ::IceInternal::Handle< ::IceGrid::UserAccountMapper> UserAccountMapperPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::IceGrid::UserAccountMapper> UserAccountMapperPrx;

ICE_GRID_API void __write(::IceInternal::BasicStream*, const UserAccountMapperPrx&);
ICE_GRID_API void __read(::IceInternal::BasicStream*, UserAccountMapperPrx&);
ICE_GRID_API void __write(::IceInternal::BasicStream*, const UserAccountMapperPtr&);
ICE_GRID_API void __patch__UserAccountMapperPtr(void*, ::Ice::ObjectPtr&);

ICE_GRID_API void __addObject(const UserAccountMapperPtr&, ::IceInternal::GCCountMap&);
ICE_GRID_API bool __usesClasses(const UserAccountMapperPtr&);
ICE_GRID_API void __decRefUnsafe(const UserAccountMapperPtr&);
ICE_GRID_API void __clearHandleUnsafe(UserAccountMapperPtr&);

}

namespace IceGrid
{

class ICE_GRID_API UserAccountNotFoundException : public ::Ice::UserException
{
public:

    UserAccountNotFoundException() {}
    virtual ~UserAccountNotFoundException() throw();

    virtual ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);

    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

static UserAccountNotFoundException __UserAccountNotFoundException_init;

}

namespace IceProxy
{

namespace IceGrid
{

class ICE_GRID_API UserAccountMapper : virtual public ::IceProxy::Ice::Object
{
public:

    ::std::string getUserAccount(const ::std::string& user)
    {
        return getUserAccount(user, 0);
    }
    ::std::string getUserAccount(const ::std::string& user, const ::Ice::Context& __ctx)
    {
        return getUserAccount(user, &__ctx);
    }
    
private:

    ::std::string getUserAccount(const ::std::string&, const ::Ice::Context*);
    
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

class ICE_GRID_API UserAccountMapper : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual ::std::string getUserAccount(const ::std::string&, const ::Ice::Context*) = 0;
};

}

}

namespace IceDelegateM
{

namespace IceGrid
{

class ICE_GRID_API UserAccountMapper : virtual public ::IceDelegate::IceGrid::UserAccountMapper,
                                       virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual ::std::string getUserAccount(const ::std::string&, const ::Ice::Context*);
};

}

}

namespace IceDelegateD
{

namespace IceGrid
{

class ICE_GRID_API UserAccountMapper : virtual public ::IceDelegate::IceGrid::UserAccountMapper,
                                       virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual ::std::string getUserAccount(const ::std::string&, const ::Ice::Context*);
};

}

}

namespace IceGrid
{

class ICE_GRID_API UserAccountMapper : virtual public ::Ice::Object
{
public:

    typedef UserAccountMapperPrx ProxyType;
    typedef UserAccountMapperPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::std::string getUserAccount(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___getUserAccount(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void ICE_GRID_API __patch__UserAccountMapperPtr(void*, ::Ice::ObjectPtr&);

}

#endif
