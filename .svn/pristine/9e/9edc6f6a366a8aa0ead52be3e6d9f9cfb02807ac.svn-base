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

#ifndef __Ice_Router_h__
#define __Ice_Router_h__

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
#include <Ice/StreamF.h>
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

namespace IceProxy
{

namespace Ice
{

class Router;
ICE_API bool operator==(const Router&, const Router&);
ICE_API bool operator!=(const Router&, const Router&);
ICE_API bool operator<(const Router&, const Router&);
ICE_API bool operator<=(const Router&, const Router&);
ICE_API bool operator>(const Router&, const Router&);
ICE_API bool operator>=(const Router&, const Router&);

}

}

namespace Ice
{

class Router;
ICE_API bool operator==(const Router&, const Router&);
ICE_API bool operator!=(const Router&, const Router&);
ICE_API bool operator<(const Router&, const Router&);
ICE_API bool operator<=(const Router&, const Router&);
ICE_API bool operator>(const Router&, const Router&);
ICE_API bool operator>=(const Router&, const Router&);

}

namespace IceInternal
{

ICE_API void incRef(::Ice::Router*);
ICE_API void decRef(::Ice::Router*);

ICE_API void incRef(::IceProxy::Ice::Router*);
ICE_API void decRef(::IceProxy::Ice::Router*);

}

namespace Ice
{

typedef ::IceInternal::Handle< ::Ice::Router> RouterPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::Ice::Router> RouterPrx;

ICE_API void __write(::IceInternal::BasicStream*, const RouterPrx&);
ICE_API void __read(::IceInternal::BasicStream*, RouterPrx&);
ICE_API void __write(::IceInternal::BasicStream*, const RouterPtr&);
ICE_API void __patch__RouterPtr(void*, ::Ice::ObjectPtr&);

ICE_API void __addObject(const RouterPtr&, ::IceInternal::GCCountMap&);
ICE_API bool __usesClasses(const RouterPtr&);
ICE_API void __decRefUnsafe(const RouterPtr&);
ICE_API void __clearHandleUnsafe(RouterPtr&);

}

namespace Ice
{

}

namespace IceProxy
{

namespace Ice
{

class ICE_API Router : virtual public ::IceProxy::Ice::Object
{
public:

    ::Ice::ObjectPrx getClientProxy()
    {
        return getClientProxy(0);
    }
    ::Ice::ObjectPrx getClientProxy(const ::Ice::Context& __ctx)
    {
        return getClientProxy(&__ctx);
    }
    
private:

    ::Ice::ObjectPrx getClientProxy(const ::Ice::Context*);
    
public:

    ::Ice::ObjectPrx getServerProxy()
    {
        return getServerProxy(0);
    }
    ::Ice::ObjectPrx getServerProxy(const ::Ice::Context& __ctx)
    {
        return getServerProxy(&__ctx);
    }
    
private:

    ::Ice::ObjectPrx getServerProxy(const ::Ice::Context*);
    
public:

    ICE_DEPRECATED_API void addProxy(const ::Ice::ObjectPrx& proxy)
    {
        addProxy(proxy, 0);
    }
    ICE_DEPRECATED_API void addProxy(const ::Ice::ObjectPrx& proxy, const ::Ice::Context& __ctx)
    {
        addProxy(proxy, &__ctx);
    }
    
private:

    void addProxy(const ::Ice::ObjectPrx&, const ::Ice::Context*);
    
public:

    ::Ice::ObjectProxySeq addProxies(const ::Ice::ObjectProxySeq& proxies)
    {
        return addProxies(proxies, 0);
    }
    ::Ice::ObjectProxySeq addProxies(const ::Ice::ObjectProxySeq& proxies, const ::Ice::Context& __ctx)
    {
        return addProxies(proxies, &__ctx);
    }
    
private:

    ::Ice::ObjectProxySeq addProxies(const ::Ice::ObjectProxySeq&, const ::Ice::Context*);
    
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

namespace Ice
{

class ICE_API Router : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual ::Ice::ObjectPrx getClientProxy(const ::Ice::Context*) = 0;

    virtual ::Ice::ObjectPrx getServerProxy(const ::Ice::Context*) = 0;

    virtual void addProxy(const ::Ice::ObjectPrx&, const ::Ice::Context*) = 0;

    virtual ::Ice::ObjectProxySeq addProxies(const ::Ice::ObjectProxySeq&, const ::Ice::Context*) = 0;
};

}

}

namespace IceDelegateM
{

namespace Ice
{

class ICE_API Router : virtual public ::IceDelegate::Ice::Router,
                       virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual ::Ice::ObjectPrx getClientProxy(const ::Ice::Context*);

    virtual ::Ice::ObjectPrx getServerProxy(const ::Ice::Context*);

    virtual void addProxy(const ::Ice::ObjectPrx&, const ::Ice::Context*);

    virtual ::Ice::ObjectProxySeq addProxies(const ::Ice::ObjectProxySeq&, const ::Ice::Context*);
};

}

}

namespace IceDelegateD
{

namespace Ice
{

class ICE_API Router : virtual public ::IceDelegate::Ice::Router,
                       virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual ::Ice::ObjectPrx getClientProxy(const ::Ice::Context*);

    virtual ::Ice::ObjectPrx getServerProxy(const ::Ice::Context*);

    virtual void addProxy(const ::Ice::ObjectPrx&, const ::Ice::Context*);

    virtual ::Ice::ObjectProxySeq addProxies(const ::Ice::ObjectProxySeq&, const ::Ice::Context*);
};

}

}

namespace Ice
{

class ICE_API Router : virtual public ::Ice::Object
{
public:

    typedef RouterPrx ProxyType;
    typedef RouterPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::Ice::ObjectPrx getClientProxy(const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___getClientProxy(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual ::Ice::ObjectPrx getServerProxy(const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___getServerProxy(::IceInternal::Incoming&, const ::Ice::Current&) const;

    ICE_DEPRECATED_API virtual void addProxy(const ::Ice::ObjectPrx&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___addProxy(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::ObjectProxySeq addProxies(const ::Ice::ObjectProxySeq&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___addProxies(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void ICE_API __patch__RouterPtr(void*, ::Ice::ObjectPtr&);

}

#endif
