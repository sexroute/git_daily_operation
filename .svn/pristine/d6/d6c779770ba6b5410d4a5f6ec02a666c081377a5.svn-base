// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.2.1
// Generated from file `Router.ice'

#ifndef __Glacier2_Router_h__
#define __Glacier2_Router_h__

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
#include <Ice/Direct.h>
#include <Ice/UserExceptionFactory.h>
#include <Ice/FactoryTable.h>
#include <Ice/StreamF.h>
#include <Ice/Router.h>
#include <Glacier2/Session.h>
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

class Router;
GLACIER2_API bool operator==(const Router&, const Router&);
GLACIER2_API bool operator!=(const Router&, const Router&);
GLACIER2_API bool operator<(const Router&, const Router&);
GLACIER2_API bool operator<=(const Router&, const Router&);
GLACIER2_API bool operator>(const Router&, const Router&);
GLACIER2_API bool operator>=(const Router&, const Router&);

class Admin;
GLACIER2_API bool operator==(const Admin&, const Admin&);
GLACIER2_API bool operator!=(const Admin&, const Admin&);
GLACIER2_API bool operator<(const Admin&, const Admin&);
GLACIER2_API bool operator<=(const Admin&, const Admin&);
GLACIER2_API bool operator>(const Admin&, const Admin&);
GLACIER2_API bool operator>=(const Admin&, const Admin&);

}

}

namespace Glacier2
{

class Router;
GLACIER2_API bool operator==(const Router&, const Router&);
GLACIER2_API bool operator!=(const Router&, const Router&);
GLACIER2_API bool operator<(const Router&, const Router&);
GLACIER2_API bool operator<=(const Router&, const Router&);
GLACIER2_API bool operator>(const Router&, const Router&);
GLACIER2_API bool operator>=(const Router&, const Router&);

class Admin;
GLACIER2_API bool operator==(const Admin&, const Admin&);
GLACIER2_API bool operator!=(const Admin&, const Admin&);
GLACIER2_API bool operator<(const Admin&, const Admin&);
GLACIER2_API bool operator<=(const Admin&, const Admin&);
GLACIER2_API bool operator>(const Admin&, const Admin&);
GLACIER2_API bool operator>=(const Admin&, const Admin&);

}

namespace IceInternal
{

GLACIER2_API void incRef(::Glacier2::Router*);
GLACIER2_API void decRef(::Glacier2::Router*);

GLACIER2_API void incRef(::IceProxy::Glacier2::Router*);
GLACIER2_API void decRef(::IceProxy::Glacier2::Router*);

GLACIER2_API void incRef(::Glacier2::Admin*);
GLACIER2_API void decRef(::Glacier2::Admin*);

GLACIER2_API void incRef(::IceProxy::Glacier2::Admin*);
GLACIER2_API void decRef(::IceProxy::Glacier2::Admin*);

}

namespace Glacier2
{

typedef ::IceInternal::Handle< ::Glacier2::Router> RouterPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::Glacier2::Router> RouterPrx;

GLACIER2_API void __write(::IceInternal::BasicStream*, const RouterPrx&);
GLACIER2_API void __read(::IceInternal::BasicStream*, RouterPrx&);
GLACIER2_API void __write(::IceInternal::BasicStream*, const RouterPtr&);
GLACIER2_API void __patch__RouterPtr(void*, ::Ice::ObjectPtr&);

GLACIER2_API void __addObject(const RouterPtr&, ::IceInternal::GCCountMap&);
GLACIER2_API bool __usesClasses(const RouterPtr&);
GLACIER2_API void __decRefUnsafe(const RouterPtr&);
GLACIER2_API void __clearHandleUnsafe(RouterPtr&);

typedef ::IceInternal::Handle< ::Glacier2::Admin> AdminPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::Glacier2::Admin> AdminPrx;

GLACIER2_API void __write(::IceInternal::BasicStream*, const AdminPrx&);
GLACIER2_API void __read(::IceInternal::BasicStream*, AdminPrx&);
GLACIER2_API void __write(::IceInternal::BasicStream*, const AdminPtr&);
GLACIER2_API void __patch__AdminPtr(void*, ::Ice::ObjectPtr&);

GLACIER2_API void __addObject(const AdminPtr&, ::IceInternal::GCCountMap&);
GLACIER2_API bool __usesClasses(const AdminPtr&);
GLACIER2_API void __decRefUnsafe(const AdminPtr&);
GLACIER2_API void __clearHandleUnsafe(AdminPtr&);

}

namespace Glacier2
{

class GLACIER2_API PermissionDeniedException : public ::Ice::UserException
{
public:

    PermissionDeniedException() {}
    explicit PermissionDeniedException(const ::std::string&);
    virtual ~PermissionDeniedException() throw();

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

static PermissionDeniedException __PermissionDeniedException_init;

class GLACIER2_API SessionNotExistException : public ::Ice::UserException
{
public:

    SessionNotExistException() {}
    virtual ~SessionNotExistException() throw();

    virtual ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);

    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

}

namespace IceProxy
{

namespace Glacier2
{

class GLACIER2_API Router : virtual public ::IceProxy::Ice::Router
{
public:

    ::std::string getCategoryForClient()
    {
        return getCategoryForClient(0);
    }
    ::std::string getCategoryForClient(const ::Ice::Context& __ctx)
    {
        return getCategoryForClient(&__ctx);
    }
    
private:

    ::std::string getCategoryForClient(const ::Ice::Context*);
    
public:

    ::Glacier2::SessionPrx createSession(const ::std::string& userId, const ::std::string& password)
    {
        return createSession(userId, password, 0);
    }
    ::Glacier2::SessionPrx createSession(const ::std::string& userId, const ::std::string& password, const ::Ice::Context& __ctx)
    {
        return createSession(userId, password, &__ctx);
    }
    
private:

    ::Glacier2::SessionPrx createSession(const ::std::string&, const ::std::string&, const ::Ice::Context*);
    
public:

    ::Glacier2::SessionPrx createSessionFromSecureConnection()
    {
        return createSessionFromSecureConnection(0);
    }
    ::Glacier2::SessionPrx createSessionFromSecureConnection(const ::Ice::Context& __ctx)
    {
        return createSessionFromSecureConnection(&__ctx);
    }
    
private:

    ::Glacier2::SessionPrx createSessionFromSecureConnection(const ::Ice::Context*);
    
public:

    void destroySession()
    {
        destroySession(0);
    }
    void destroySession(const ::Ice::Context& __ctx)
    {
        destroySession(&__ctx);
    }
    
private:

    void destroySession(const ::Ice::Context*);
    
public:

    ::Ice::Long getSessionTimeout()
    {
        return getSessionTimeout(0);
    }
    ::Ice::Long getSessionTimeout(const ::Ice::Context& __ctx)
    {
        return getSessionTimeout(&__ctx);
    }
    
private:

    ::Ice::Long getSessionTimeout(const ::Ice::Context*);
    
public:
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

class GLACIER2_API Admin : virtual public ::IceProxy::Ice::Object
{
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

namespace Glacier2
{

class GLACIER2_API Router : virtual public ::IceDelegate::Ice::Router
{
public:

    virtual ::std::string getCategoryForClient(const ::Ice::Context*) = 0;

    virtual ::Glacier2::SessionPrx createSession(const ::std::string&, const ::std::string&, const ::Ice::Context*) = 0;

    virtual ::Glacier2::SessionPrx createSessionFromSecureConnection(const ::Ice::Context*) = 0;

    virtual void destroySession(const ::Ice::Context*) = 0;

    virtual ::Ice::Long getSessionTimeout(const ::Ice::Context*) = 0;
};

class GLACIER2_API Admin : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual void shutdown(const ::Ice::Context*) = 0;
};

}

}

namespace IceDelegateM
{

namespace Glacier2
{

class GLACIER2_API Router : virtual public ::IceDelegate::Glacier2::Router,
                            virtual public ::IceDelegateM::Ice::Router
{
public:

    virtual ::std::string getCategoryForClient(const ::Ice::Context*);

    virtual ::Glacier2::SessionPrx createSession(const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual ::Glacier2::SessionPrx createSessionFromSecureConnection(const ::Ice::Context*);

    virtual void destroySession(const ::Ice::Context*);

    virtual ::Ice::Long getSessionTimeout(const ::Ice::Context*);
};

class GLACIER2_API Admin : virtual public ::IceDelegate::Glacier2::Admin,
                           virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual void shutdown(const ::Ice::Context*);
};

}

}

namespace IceDelegateD
{

namespace Glacier2
{

class GLACIER2_API Router : virtual public ::IceDelegate::Glacier2::Router,
                            virtual public ::IceDelegateD::Ice::Router
{
public:

    virtual ::std::string getCategoryForClient(const ::Ice::Context*);

    virtual ::Glacier2::SessionPrx createSession(const ::std::string&, const ::std::string&, const ::Ice::Context*);

    virtual ::Glacier2::SessionPrx createSessionFromSecureConnection(const ::Ice::Context*);

    virtual void destroySession(const ::Ice::Context*);

    virtual ::Ice::Long getSessionTimeout(const ::Ice::Context*);
};

class GLACIER2_API Admin : virtual public ::IceDelegate::Glacier2::Admin,
                           virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual void shutdown(const ::Ice::Context*);
};

}

}

namespace Glacier2
{

class GLACIER2_API Router : virtual public ::Ice::Router
{
public:

    typedef RouterPrx ProxyType;
    typedef RouterPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::std::string getCategoryForClient(const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___getCategoryForClient(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual ::Glacier2::SessionPrx createSession(const ::std::string&, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___createSession(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Glacier2::SessionPrx createSessionFromSecureConnection(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___createSessionFromSecureConnection(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void destroySession(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___destroySession(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::Long getSessionTimeout(const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___getSessionTimeout(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void GLACIER2_API __patch__RouterPtr(void*, ::Ice::ObjectPtr&);

class GLACIER2_API Admin : virtual public ::Ice::Object
{
public:

    typedef AdminPrx ProxyType;
    typedef AdminPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual void shutdown(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___shutdown(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void GLACIER2_API __patch__AdminPtr(void*, ::Ice::ObjectPtr&);

}

#endif
