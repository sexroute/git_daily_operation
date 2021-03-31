// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.2.1
// Generated from file `Locator.ice'

#ifndef __Ice_Locator_h__
#define __Ice_Locator_h__

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
#include <Ice/Identity.h>
#include <Ice/ProcessF.h>
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

class LocatorRegistry;
ICE_API bool operator==(const LocatorRegistry&, const LocatorRegistry&);
ICE_API bool operator!=(const LocatorRegistry&, const LocatorRegistry&);
ICE_API bool operator<(const LocatorRegistry&, const LocatorRegistry&);
ICE_API bool operator<=(const LocatorRegistry&, const LocatorRegistry&);
ICE_API bool operator>(const LocatorRegistry&, const LocatorRegistry&);
ICE_API bool operator>=(const LocatorRegistry&, const LocatorRegistry&);

class Locator;
ICE_API bool operator==(const Locator&, const Locator&);
ICE_API bool operator!=(const Locator&, const Locator&);
ICE_API bool operator<(const Locator&, const Locator&);
ICE_API bool operator<=(const Locator&, const Locator&);
ICE_API bool operator>(const Locator&, const Locator&);
ICE_API bool operator>=(const Locator&, const Locator&);

}

}

namespace Ice
{

class LocatorRegistry;
ICE_API bool operator==(const LocatorRegistry&, const LocatorRegistry&);
ICE_API bool operator!=(const LocatorRegistry&, const LocatorRegistry&);
ICE_API bool operator<(const LocatorRegistry&, const LocatorRegistry&);
ICE_API bool operator<=(const LocatorRegistry&, const LocatorRegistry&);
ICE_API bool operator>(const LocatorRegistry&, const LocatorRegistry&);
ICE_API bool operator>=(const LocatorRegistry&, const LocatorRegistry&);

class Locator;
ICE_API bool operator==(const Locator&, const Locator&);
ICE_API bool operator!=(const Locator&, const Locator&);
ICE_API bool operator<(const Locator&, const Locator&);
ICE_API bool operator<=(const Locator&, const Locator&);
ICE_API bool operator>(const Locator&, const Locator&);
ICE_API bool operator>=(const Locator&, const Locator&);

}

namespace IceInternal
{

ICE_API void incRef(::Ice::LocatorRegistry*);
ICE_API void decRef(::Ice::LocatorRegistry*);

ICE_API void incRef(::IceProxy::Ice::LocatorRegistry*);
ICE_API void decRef(::IceProxy::Ice::LocatorRegistry*);

ICE_API void incRef(::Ice::Locator*);
ICE_API void decRef(::Ice::Locator*);

ICE_API void incRef(::IceProxy::Ice::Locator*);
ICE_API void decRef(::IceProxy::Ice::Locator*);

}

namespace Ice
{

typedef ::IceInternal::Handle< ::Ice::LocatorRegistry> LocatorRegistryPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::Ice::LocatorRegistry> LocatorRegistryPrx;

ICE_API void __write(::IceInternal::BasicStream*, const LocatorRegistryPrx&);
ICE_API void __read(::IceInternal::BasicStream*, LocatorRegistryPrx&);
ICE_API void __write(::IceInternal::BasicStream*, const LocatorRegistryPtr&);
ICE_API void __patch__LocatorRegistryPtr(void*, ::Ice::ObjectPtr&);

ICE_API void __addObject(const LocatorRegistryPtr&, ::IceInternal::GCCountMap&);
ICE_API bool __usesClasses(const LocatorRegistryPtr&);
ICE_API void __decRefUnsafe(const LocatorRegistryPtr&);
ICE_API void __clearHandleUnsafe(LocatorRegistryPtr&);

typedef ::IceInternal::Handle< ::Ice::Locator> LocatorPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::Ice::Locator> LocatorPrx;

ICE_API void __write(::IceInternal::BasicStream*, const LocatorPrx&);
ICE_API void __read(::IceInternal::BasicStream*, LocatorPrx&);
ICE_API void __write(::IceInternal::BasicStream*, const LocatorPtr&);
ICE_API void __patch__LocatorPtr(void*, ::Ice::ObjectPtr&);

ICE_API void __addObject(const LocatorPtr&, ::IceInternal::GCCountMap&);
ICE_API bool __usesClasses(const LocatorPtr&);
ICE_API void __decRefUnsafe(const LocatorPtr&);
ICE_API void __clearHandleUnsafe(LocatorPtr&);

}

namespace Ice
{

class ICE_API AdapterNotFoundException : public ::Ice::UserException
{
public:

    AdapterNotFoundException() {}
    virtual ~AdapterNotFoundException() throw();

    virtual ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);

    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

static AdapterNotFoundException __AdapterNotFoundException_init;

class ICE_API InvalidReplicaGroupIdException : public ::Ice::UserException
{
public:

    InvalidReplicaGroupIdException() {}
    virtual ~InvalidReplicaGroupIdException() throw();

    virtual ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);

    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

class ICE_API AdapterAlreadyActiveException : public ::Ice::UserException
{
public:

    AdapterAlreadyActiveException() {}
    virtual ~AdapterAlreadyActiveException() throw();

    virtual ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);

    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

class ICE_API ObjectNotFoundException : public ::Ice::UserException
{
public:

    ObjectNotFoundException() {}
    virtual ~ObjectNotFoundException() throw();

    virtual ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);

    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

class ICE_API ServerNotFoundException : public ::Ice::UserException
{
public:

    ServerNotFoundException() {}
    virtual ~ServerNotFoundException() throw();

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

namespace Ice
{

class ICE_API AMD_Locator_findObjectById : virtual public ::IceUtil::Shared
{
public:

    virtual void ice_response(const ::Ice::ObjectPrx&) = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;
    virtual void ice_exception(const ::std::exception&) = 0;
    virtual void ice_exception() = 0;
};

typedef ::IceUtil::Handle< ::Ice::AMD_Locator_findObjectById> AMD_Locator_findObjectByIdPtr;

class ICE_API AMD_Locator_findAdapterById : virtual public ::IceUtil::Shared
{
public:

    virtual void ice_response(const ::Ice::ObjectPrx&) = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;
    virtual void ice_exception(const ::std::exception&) = 0;
    virtual void ice_exception() = 0;
};

typedef ::IceUtil::Handle< ::Ice::AMD_Locator_findAdapterById> AMD_Locator_findAdapterByIdPtr;

class ICE_API AMI_LocatorRegistry_setAdapterDirectProxy : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response() = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::Ice::LocatorRegistryPrx&, const ::std::string&, const ::Ice::ObjectPrx&, const ::Ice::Context*);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::Ice::AMI_LocatorRegistry_setAdapterDirectProxy> AMI_LocatorRegistry_setAdapterDirectProxyPtr;

class ICE_API AMD_LocatorRegistry_setAdapterDirectProxy : virtual public ::IceUtil::Shared
{
public:

    virtual void ice_response() = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;
    virtual void ice_exception(const ::std::exception&) = 0;
    virtual void ice_exception() = 0;
};

typedef ::IceUtil::Handle< ::Ice::AMD_LocatorRegistry_setAdapterDirectProxy> AMD_LocatorRegistry_setAdapterDirectProxyPtr;

class ICE_API AMI_LocatorRegistry_setReplicatedAdapterDirectProxy : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response() = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::Ice::LocatorRegistryPrx&, const ::std::string&, const ::std::string&, const ::Ice::ObjectPrx&, const ::Ice::Context*);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::Ice::AMI_LocatorRegistry_setReplicatedAdapterDirectProxy> AMI_LocatorRegistry_setReplicatedAdapterDirectProxyPtr;

class ICE_API AMD_LocatorRegistry_setReplicatedAdapterDirectProxy : virtual public ::IceUtil::Shared
{
public:

    virtual void ice_response() = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;
    virtual void ice_exception(const ::std::exception&) = 0;
    virtual void ice_exception() = 0;
};

typedef ::IceUtil::Handle< ::Ice::AMD_LocatorRegistry_setReplicatedAdapterDirectProxy> AMD_LocatorRegistry_setReplicatedAdapterDirectProxyPtr;

class ICE_API AMD_LocatorRegistry_setServerProcessProxy : virtual public ::IceUtil::Shared
{
public:

    virtual void ice_response() = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;
    virtual void ice_exception(const ::std::exception&) = 0;
    virtual void ice_exception() = 0;
};

typedef ::IceUtil::Handle< ::Ice::AMD_LocatorRegistry_setServerProcessProxy> AMD_LocatorRegistry_setServerProcessProxyPtr;

}

namespace IceAsync
{

namespace Ice
{

class ICE_API AMD_Locator_findObjectById : public ::Ice::AMD_Locator_findObjectById, public ::IceInternal::IncomingAsync
{
public:

    AMD_Locator_findObjectById(::IceInternal::Incoming&);

    virtual void ice_response(const ::Ice::ObjectPrx&);
    virtual void ice_exception(const ::Ice::Exception&);
    virtual void ice_exception(const ::std::exception&);
    virtual void ice_exception();
};

class ICE_API AMD_Locator_findAdapterById : public ::Ice::AMD_Locator_findAdapterById, public ::IceInternal::IncomingAsync
{
public:

    AMD_Locator_findAdapterById(::IceInternal::Incoming&);

    virtual void ice_response(const ::Ice::ObjectPrx&);
    virtual void ice_exception(const ::Ice::Exception&);
    virtual void ice_exception(const ::std::exception&);
    virtual void ice_exception();
};

class ICE_API AMD_LocatorRegistry_setAdapterDirectProxy : public ::Ice::AMD_LocatorRegistry_setAdapterDirectProxy, public ::IceInternal::IncomingAsync
{
public:

    AMD_LocatorRegistry_setAdapterDirectProxy(::IceInternal::Incoming&);

    virtual void ice_response();
    virtual void ice_exception(const ::Ice::Exception&);
    virtual void ice_exception(const ::std::exception&);
    virtual void ice_exception();
};

class ICE_API AMD_LocatorRegistry_setReplicatedAdapterDirectProxy : public ::Ice::AMD_LocatorRegistry_setReplicatedAdapterDirectProxy, public ::IceInternal::IncomingAsync
{
public:

    AMD_LocatorRegistry_setReplicatedAdapterDirectProxy(::IceInternal::Incoming&);

    virtual void ice_response();
    virtual void ice_exception(const ::Ice::Exception&);
    virtual void ice_exception(const ::std::exception&);
    virtual void ice_exception();
};

class ICE_API AMD_LocatorRegistry_setServerProcessProxy : public ::Ice::AMD_LocatorRegistry_setServerProcessProxy, public ::IceInternal::IncomingAsync
{
public:

    AMD_LocatorRegistry_setServerProcessProxy(::IceInternal::Incoming&);

    virtual void ice_response();
    virtual void ice_exception(const ::Ice::Exception&);
    virtual void ice_exception(const ::std::exception&);
    virtual void ice_exception();
};

}

}

namespace IceProxy
{

namespace Ice
{

class ICE_API Locator : virtual public ::IceProxy::Ice::Object
{
public:

    ::Ice::ObjectPrx findObjectById(const ::Ice::Identity& id)
    {
        return findObjectById(id, 0);
    }
    ::Ice::ObjectPrx findObjectById(const ::Ice::Identity& id, const ::Ice::Context& __ctx)
    {
        return findObjectById(id, &__ctx);
    }
    
private:

    ::Ice::ObjectPrx findObjectById(const ::Ice::Identity&, const ::Ice::Context*);
    
public:

    ::Ice::ObjectPrx findAdapterById(const ::std::string& id)
    {
        return findAdapterById(id, 0);
    }
    ::Ice::ObjectPrx findAdapterById(const ::std::string& id, const ::Ice::Context& __ctx)
    {
        return findAdapterById(id, &__ctx);
    }
    
private:

    ::Ice::ObjectPrx findAdapterById(const ::std::string&, const ::Ice::Context*);
    
public:

    ::Ice::LocatorRegistryPrx getRegistry()
    {
        return getRegistry(0);
    }
    ::Ice::LocatorRegistryPrx getRegistry(const ::Ice::Context& __ctx)
    {
        return getRegistry(&__ctx);
    }
    
private:

    ::Ice::LocatorRegistryPrx getRegistry(const ::Ice::Context*);
    
public:
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

class ICE_API LocatorRegistry : virtual public ::IceProxy::Ice::Object
{
public:

    void setAdapterDirectProxy(const ::std::string& id, const ::Ice::ObjectPrx& proxy)
    {
        setAdapterDirectProxy(id, proxy, 0);
    }
    void setAdapterDirectProxy(const ::std::string& id, const ::Ice::ObjectPrx& proxy, const ::Ice::Context& __ctx)
    {
        setAdapterDirectProxy(id, proxy, &__ctx);
    }
    
private:

    void setAdapterDirectProxy(const ::std::string&, const ::Ice::ObjectPrx&, const ::Ice::Context*);
    
public:
    void setAdapterDirectProxy_async(const ::Ice::AMI_LocatorRegistry_setAdapterDirectProxyPtr&, const ::std::string&, const ::Ice::ObjectPrx&);
    void setAdapterDirectProxy_async(const ::Ice::AMI_LocatorRegistry_setAdapterDirectProxyPtr&, const ::std::string&, const ::Ice::ObjectPrx&, const ::Ice::Context&);

    void setReplicatedAdapterDirectProxy(const ::std::string& adapterId, const ::std::string& replicaGroupId, const ::Ice::ObjectPrx& p)
    {
        setReplicatedAdapterDirectProxy(adapterId, replicaGroupId, p, 0);
    }
    void setReplicatedAdapterDirectProxy(const ::std::string& adapterId, const ::std::string& replicaGroupId, const ::Ice::ObjectPrx& p, const ::Ice::Context& __ctx)
    {
        setReplicatedAdapterDirectProxy(adapterId, replicaGroupId, p, &__ctx);
    }
    
private:

    void setReplicatedAdapterDirectProxy(const ::std::string&, const ::std::string&, const ::Ice::ObjectPrx&, const ::Ice::Context*);
    
public:
    void setReplicatedAdapterDirectProxy_async(const ::Ice::AMI_LocatorRegistry_setReplicatedAdapterDirectProxyPtr&, const ::std::string&, const ::std::string&, const ::Ice::ObjectPrx&);
    void setReplicatedAdapterDirectProxy_async(const ::Ice::AMI_LocatorRegistry_setReplicatedAdapterDirectProxyPtr&, const ::std::string&, const ::std::string&, const ::Ice::ObjectPrx&, const ::Ice::Context&);

    void setServerProcessProxy(const ::std::string& id, const ::Ice::ProcessPrx& proxy)
    {
        setServerProcessProxy(id, proxy, 0);
    }
    void setServerProcessProxy(const ::std::string& id, const ::Ice::ProcessPrx& proxy, const ::Ice::Context& __ctx)
    {
        setServerProcessProxy(id, proxy, &__ctx);
    }
    
private:

    void setServerProcessProxy(const ::std::string&, const ::Ice::ProcessPrx&, const ::Ice::Context*);
    
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

class ICE_API Locator : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual ::Ice::ObjectPrx findObjectById(const ::Ice::Identity&, const ::Ice::Context*) = 0;

    virtual ::Ice::ObjectPrx findAdapterById(const ::std::string&, const ::Ice::Context*) = 0;

    virtual ::Ice::LocatorRegistryPrx getRegistry(const ::Ice::Context*) = 0;
};

class ICE_API LocatorRegistry : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual void setAdapterDirectProxy(const ::std::string&, const ::Ice::ObjectPrx&, const ::Ice::Context*) = 0;

    virtual void setReplicatedAdapterDirectProxy(const ::std::string&, const ::std::string&, const ::Ice::ObjectPrx&, const ::Ice::Context*) = 0;

    virtual void setServerProcessProxy(const ::std::string&, const ::Ice::ProcessPrx&, const ::Ice::Context*) = 0;
};

}

}

namespace IceDelegateM
{

namespace Ice
{

class ICE_API Locator : virtual public ::IceDelegate::Ice::Locator,
                        virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual ::Ice::ObjectPrx findObjectById(const ::Ice::Identity&, const ::Ice::Context*);

    virtual ::Ice::ObjectPrx findAdapterById(const ::std::string&, const ::Ice::Context*);

    virtual ::Ice::LocatorRegistryPrx getRegistry(const ::Ice::Context*);
};

class ICE_API LocatorRegistry : virtual public ::IceDelegate::Ice::LocatorRegistry,
                                virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual void setAdapterDirectProxy(const ::std::string&, const ::Ice::ObjectPrx&, const ::Ice::Context*);

    virtual void setReplicatedAdapterDirectProxy(const ::std::string&, const ::std::string&, const ::Ice::ObjectPrx&, const ::Ice::Context*);

    virtual void setServerProcessProxy(const ::std::string&, const ::Ice::ProcessPrx&, const ::Ice::Context*);
};

}

}

namespace IceDelegateD
{

namespace Ice
{

class ICE_API Locator : virtual public ::IceDelegate::Ice::Locator,
                        virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual ::Ice::ObjectPrx findObjectById(const ::Ice::Identity&, const ::Ice::Context*);

    virtual ::Ice::ObjectPrx findAdapterById(const ::std::string&, const ::Ice::Context*);

    virtual ::Ice::LocatorRegistryPrx getRegistry(const ::Ice::Context*);
};

class ICE_API LocatorRegistry : virtual public ::IceDelegate::Ice::LocatorRegistry,
                                virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual void setAdapterDirectProxy(const ::std::string&, const ::Ice::ObjectPrx&, const ::Ice::Context*);

    virtual void setReplicatedAdapterDirectProxy(const ::std::string&, const ::std::string&, const ::Ice::ObjectPrx&, const ::Ice::Context*);

    virtual void setServerProcessProxy(const ::std::string&, const ::Ice::ProcessPrx&, const ::Ice::Context*);
};

}

}

namespace Ice
{

class ICE_API Locator : virtual public ::Ice::Object
{
public:

    typedef LocatorPrx ProxyType;
    typedef LocatorPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual void findObjectById_async(const ::Ice::AMD_Locator_findObjectByIdPtr&, const ::Ice::Identity&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___findObjectById(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual void findAdapterById_async(const ::Ice::AMD_Locator_findAdapterByIdPtr&, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___findAdapterById(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual ::Ice::LocatorRegistryPrx getRegistry(const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___getRegistry(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void ICE_API __patch__LocatorPtr(void*, ::Ice::ObjectPtr&);

class ICE_API LocatorRegistry : virtual public ::Ice::Object
{
public:

    typedef LocatorRegistryPrx ProxyType;
    typedef LocatorRegistryPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual void setAdapterDirectProxy_async(const ::Ice::AMD_LocatorRegistry_setAdapterDirectProxyPtr&, const ::std::string&, const ::Ice::ObjectPrx&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___setAdapterDirectProxy(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void setReplicatedAdapterDirectProxy_async(const ::Ice::AMD_LocatorRegistry_setReplicatedAdapterDirectProxyPtr&, const ::std::string&, const ::std::string&, const ::Ice::ObjectPrx&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___setReplicatedAdapterDirectProxy(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void setServerProcessProxy_async(const ::Ice::AMD_LocatorRegistry_setServerProcessProxyPtr&, const ::std::string&, const ::Ice::ProcessPrx&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___setServerProcessProxy(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void ICE_API __patch__LocatorRegistryPtr(void*, ::Ice::ObjectPtr&);

}

#endif
