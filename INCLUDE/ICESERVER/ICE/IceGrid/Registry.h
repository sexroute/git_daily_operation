// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.2.1
// Generated from file `Registry.ice'

#ifndef __IceGrid_Registry_h__
#define __IceGrid_Registry_h__

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
#include <IceGrid/Exception.h>
#include <IceGrid/Session.h>
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

class Registry;
ICE_GRID_API bool operator==(const Registry&, const Registry&);
ICE_GRID_API bool operator!=(const Registry&, const Registry&);
ICE_GRID_API bool operator<(const Registry&, const Registry&);
ICE_GRID_API bool operator<=(const Registry&, const Registry&);
ICE_GRID_API bool operator>(const Registry&, const Registry&);
ICE_GRID_API bool operator>=(const Registry&, const Registry&);

}

}

namespace IceGrid
{

class Registry;
ICE_GRID_API bool operator==(const Registry&, const Registry&);
ICE_GRID_API bool operator!=(const Registry&, const Registry&);
ICE_GRID_API bool operator<(const Registry&, const Registry&);
ICE_GRID_API bool operator<=(const Registry&, const Registry&);
ICE_GRID_API bool operator>(const Registry&, const Registry&);
ICE_GRID_API bool operator>=(const Registry&, const Registry&);

}

namespace IceInternal
{

ICE_GRID_API void incRef(::IceGrid::Registry*);
ICE_GRID_API void decRef(::IceGrid::Registry*);

ICE_GRID_API void incRef(::IceProxy::IceGrid::Registry*);
ICE_GRID_API void decRef(::IceProxy::IceGrid::Registry*);

}

namespace IceGrid
{

typedef ::IceInternal::Handle< ::IceGrid::Registry> RegistryPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::IceGrid::Registry> RegistryPrx;

ICE_GRID_API void __write(::IceInternal::BasicStream*, const RegistryPrx&);
ICE_GRID_API void __read(::IceInternal::BasicStream*, RegistryPrx&);
ICE_GRID_API void __write(::IceInternal::BasicStream*, const RegistryPtr&);
ICE_GRID_API void __patch__RegistryPtr(void*, ::Ice::ObjectPtr&);

ICE_GRID_API void __addObject(const RegistryPtr&, ::IceInternal::GCCountMap&);
ICE_GRID_API bool __usesClasses(const RegistryPtr&);
ICE_GRID_API void __decRefUnsafe(const RegistryPtr&);
ICE_GRID_API void __clearHandleUnsafe(RegistryPtr&);

}

namespace IceGrid
{

}

namespace IceAsync
{

}

namespace IceProxy
{

namespace IceGrid
{

class ICE_GRID_API Registry : virtual public ::IceProxy::Ice::Object
{
public:

    ::IceGrid::SessionPrx createSession(const ::std::string& userId, const ::std::string& password)
    {
        return createSession(userId, password, 0);
    }
    ::IceGrid::SessionPrx createSession(const ::std::string& userId, const ::std::string& password, const ::Ice::Context& __ctx)
    {
        return createSession(userId, password, &__ctx);
    }
    
private:

    ::IceGrid::SessionPrx createSession(const ::std::string&, const ::std::string&, const ::Ice::Context*);
    
public:

    ::IceGrid::AdminSessionPrx createAdminSession(const ::std::string& userId, const ::std::string& password)
    {
        return createAdminSession(userId, password, 0);
    }
    ::IceGrid::AdminSessionPrx createAdminSession(const ::std::string& userId, const ::std::string& password, const ::Ice::Context& __ctx)
    {
        return createAdminSession(userId, password, &__ctx);
    }
    
private:

    ::IceGrid::AdminSessionPrx createAdminSession(const ::std::string&, const ::std::string&, const ::Ice::Context*);
    
public:

    ::IceGrid::SessionPrx createSessionFromSecureConnection()
    {
        return createSessionFromSecureConnection(0);
    }
    ::IceGrid::SessionPrx createSessionFromSecureConnection(const ::Ice::Context& __ctx)
    {
        return createSessionFromSecureConnection(&__ctx);
    }
    
private:

    ::IceGrid::SessionPrx createSessionFromSecureConnection(const ::Ice::Context*);
    
public:

    ::IceGrid::AdminSessionPrx createAdminSessionFromSecureConnection()
    {
        return createAdminSessionFromSecureConnection(0);
    }
    ::IceGrid::AdminSessionPrx createAdminSessionFromSecureConnection(const ::Ice::Context& __ctx)
    {
        return createAdminSessionFromSecureConnection(&__ctx);
    }
    
private:

    ::IceGrid::AdminSessionPrx createAdminSessionFromSecureConnection(const ::Ice::Context*);
    
public:

    ::Ice::Int getSessionTimeout()
    {
        return getSessionTimeout(0);
    }
    ::Ice::Int getSessionTimeout(const ::Ice::Context& __ctx)
    {
        return getSessionTimeout(&__ctx);
    }
    
private:

    ::Ice::Int getSessionTimeout(const ::Ice::Context*);
    
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

class ICE_GRID_API Registry : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual ::IceGrid::SessionPrx createSession(const ::std::string&, const ::std::string&, const ::Ice::Context*) = 0;

    virtual ::IceGrid::AdminSessionPrx createAdminSession(const ::std::string&, const ::std::string&, const ::Ice::Context*) = 0;

    virtual ::IceGrid::SessionPrx createSessionFromSecureConnection(const ::Ice::Context*) = 0;

    virtual ::IceGrid::AdminSessionPrx createAdminSessionFromSecureConnection(const ::Ice::Context*) = 0;

    virtual ::Ice::Int getSessionTimeout(const ::Ice::Context*) = 0;
};

}

}

namespace IceDelegateM
{

namespace IceGrid
{

class ICE_GRID_API Registry : virtual public ::IceDelegate::IceGrid::Registry,
                              virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual ::IceGrid::SessionPrx createSession(const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual ::IceGrid::AdminSessionPrx createAdminSession(const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual ::IceGrid::SessionPrx createSessionFromSecureConnection(const ::Ice::Context*);

    virtual ::IceGrid::AdminSessionPrx createAdminSessionFromSecureConnection(const ::Ice::Context*);

    virtual ::Ice::Int getSessionTimeout(const ::Ice::Context*);
};

}

}

namespace IceDelegateD
{

namespace IceGrid
{

class ICE_GRID_API Registry : virtual public ::IceDelegate::IceGrid::Registry,
                              virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual ::IceGrid::SessionPrx createSession(const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual ::IceGrid::AdminSessionPrx createAdminSession(const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual ::IceGrid::SessionPrx createSessionFromSecureConnection(const ::Ice::Context*);

    virtual ::IceGrid::AdminSessionPrx createAdminSessionFromSecureConnection(const ::Ice::Context*);

    virtual ::Ice::Int getSessionTimeout(const ::Ice::Context*);
};

}

}

namespace IceGrid
{

class ICE_GRID_API Registry : virtual public ::Ice::Object
{
public:

    typedef RegistryPrx ProxyType;
    typedef RegistryPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::IceGrid::SessionPrx createSession(const ::std::string&, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___createSession(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::IceGrid::AdminSessionPrx createAdminSession(const ::std::string&, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___createAdminSession(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::IceGrid::SessionPrx createSessionFromSecureConnection(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___createSessionFromSecureConnection(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::IceGrid::AdminSessionPrx createAdminSessionFromSecureConnection(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___createAdminSessionFromSecureConnection(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::Int getSessionTimeout(const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___getSessionTimeout(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void ICE_GRID_API __patch__RegistryPtr(void*, ::Ice::ObjectPtr&);

}

#endif
