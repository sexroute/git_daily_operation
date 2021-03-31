// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.2.1
// Generated from file `Observer.ice'

#ifndef __IceGrid_Observer_h__
#define __IceGrid_Observer_h__

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
#include <Glacier2/Session.h>
#include <IceGrid/Exception.h>
#include <IceGrid/Descriptor.h>
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

class NodeObserver;
ICE_GRID_API bool operator==(const NodeObserver&, const NodeObserver&);
ICE_GRID_API bool operator!=(const NodeObserver&, const NodeObserver&);
ICE_GRID_API bool operator<(const NodeObserver&, const NodeObserver&);
ICE_GRID_API bool operator<=(const NodeObserver&, const NodeObserver&);
ICE_GRID_API bool operator>(const NodeObserver&, const NodeObserver&);
ICE_GRID_API bool operator>=(const NodeObserver&, const NodeObserver&);

class ApplicationObserver;
ICE_GRID_API bool operator==(const ApplicationObserver&, const ApplicationObserver&);
ICE_GRID_API bool operator!=(const ApplicationObserver&, const ApplicationObserver&);
ICE_GRID_API bool operator<(const ApplicationObserver&, const ApplicationObserver&);
ICE_GRID_API bool operator<=(const ApplicationObserver&, const ApplicationObserver&);
ICE_GRID_API bool operator>(const ApplicationObserver&, const ApplicationObserver&);
ICE_GRID_API bool operator>=(const ApplicationObserver&, const ApplicationObserver&);

class AdapterObserver;
ICE_GRID_API bool operator==(const AdapterObserver&, const AdapterObserver&);
ICE_GRID_API bool operator!=(const AdapterObserver&, const AdapterObserver&);
ICE_GRID_API bool operator<(const AdapterObserver&, const AdapterObserver&);
ICE_GRID_API bool operator<=(const AdapterObserver&, const AdapterObserver&);
ICE_GRID_API bool operator>(const AdapterObserver&, const AdapterObserver&);
ICE_GRID_API bool operator>=(const AdapterObserver&, const AdapterObserver&);

class ObjectObserver;
ICE_GRID_API bool operator==(const ObjectObserver&, const ObjectObserver&);
ICE_GRID_API bool operator!=(const ObjectObserver&, const ObjectObserver&);
ICE_GRID_API bool operator<(const ObjectObserver&, const ObjectObserver&);
ICE_GRID_API bool operator<=(const ObjectObserver&, const ObjectObserver&);
ICE_GRID_API bool operator>(const ObjectObserver&, const ObjectObserver&);
ICE_GRID_API bool operator>=(const ObjectObserver&, const ObjectObserver&);

class RegistryObserver;
ICE_GRID_API bool operator==(const RegistryObserver&, const RegistryObserver&);
ICE_GRID_API bool operator!=(const RegistryObserver&, const RegistryObserver&);
ICE_GRID_API bool operator<(const RegistryObserver&, const RegistryObserver&);
ICE_GRID_API bool operator<=(const RegistryObserver&, const RegistryObserver&);
ICE_GRID_API bool operator>(const RegistryObserver&, const RegistryObserver&);
ICE_GRID_API bool operator>=(const RegistryObserver&, const RegistryObserver&);

}

}

namespace IceGrid
{

class NodeObserver;
ICE_GRID_API bool operator==(const NodeObserver&, const NodeObserver&);
ICE_GRID_API bool operator!=(const NodeObserver&, const NodeObserver&);
ICE_GRID_API bool operator<(const NodeObserver&, const NodeObserver&);
ICE_GRID_API bool operator<=(const NodeObserver&, const NodeObserver&);
ICE_GRID_API bool operator>(const NodeObserver&, const NodeObserver&);
ICE_GRID_API bool operator>=(const NodeObserver&, const NodeObserver&);

class ApplicationObserver;
ICE_GRID_API bool operator==(const ApplicationObserver&, const ApplicationObserver&);
ICE_GRID_API bool operator!=(const ApplicationObserver&, const ApplicationObserver&);
ICE_GRID_API bool operator<(const ApplicationObserver&, const ApplicationObserver&);
ICE_GRID_API bool operator<=(const ApplicationObserver&, const ApplicationObserver&);
ICE_GRID_API bool operator>(const ApplicationObserver&, const ApplicationObserver&);
ICE_GRID_API bool operator>=(const ApplicationObserver&, const ApplicationObserver&);

class AdapterObserver;
ICE_GRID_API bool operator==(const AdapterObserver&, const AdapterObserver&);
ICE_GRID_API bool operator!=(const AdapterObserver&, const AdapterObserver&);
ICE_GRID_API bool operator<(const AdapterObserver&, const AdapterObserver&);
ICE_GRID_API bool operator<=(const AdapterObserver&, const AdapterObserver&);
ICE_GRID_API bool operator>(const AdapterObserver&, const AdapterObserver&);
ICE_GRID_API bool operator>=(const AdapterObserver&, const AdapterObserver&);

class ObjectObserver;
ICE_GRID_API bool operator==(const ObjectObserver&, const ObjectObserver&);
ICE_GRID_API bool operator!=(const ObjectObserver&, const ObjectObserver&);
ICE_GRID_API bool operator<(const ObjectObserver&, const ObjectObserver&);
ICE_GRID_API bool operator<=(const ObjectObserver&, const ObjectObserver&);
ICE_GRID_API bool operator>(const ObjectObserver&, const ObjectObserver&);
ICE_GRID_API bool operator>=(const ObjectObserver&, const ObjectObserver&);

class RegistryObserver;
ICE_GRID_API bool operator==(const RegistryObserver&, const RegistryObserver&);
ICE_GRID_API bool operator!=(const RegistryObserver&, const RegistryObserver&);
ICE_GRID_API bool operator<(const RegistryObserver&, const RegistryObserver&);
ICE_GRID_API bool operator<=(const RegistryObserver&, const RegistryObserver&);
ICE_GRID_API bool operator>(const RegistryObserver&, const RegistryObserver&);
ICE_GRID_API bool operator>=(const RegistryObserver&, const RegistryObserver&);

}

namespace IceInternal
{

ICE_GRID_API void incRef(::IceGrid::NodeObserver*);
ICE_GRID_API void decRef(::IceGrid::NodeObserver*);

ICE_GRID_API void incRef(::IceProxy::IceGrid::NodeObserver*);
ICE_GRID_API void decRef(::IceProxy::IceGrid::NodeObserver*);

ICE_GRID_API void incRef(::IceGrid::ApplicationObserver*);
ICE_GRID_API void decRef(::IceGrid::ApplicationObserver*);

ICE_GRID_API void incRef(::IceProxy::IceGrid::ApplicationObserver*);
ICE_GRID_API void decRef(::IceProxy::IceGrid::ApplicationObserver*);

ICE_GRID_API void incRef(::IceGrid::AdapterObserver*);
ICE_GRID_API void decRef(::IceGrid::AdapterObserver*);

ICE_GRID_API void incRef(::IceProxy::IceGrid::AdapterObserver*);
ICE_GRID_API void decRef(::IceProxy::IceGrid::AdapterObserver*);

ICE_GRID_API void incRef(::IceGrid::ObjectObserver*);
ICE_GRID_API void decRef(::IceGrid::ObjectObserver*);

ICE_GRID_API void incRef(::IceProxy::IceGrid::ObjectObserver*);
ICE_GRID_API void decRef(::IceProxy::IceGrid::ObjectObserver*);

ICE_GRID_API void incRef(::IceGrid::RegistryObserver*);
ICE_GRID_API void decRef(::IceGrid::RegistryObserver*);

ICE_GRID_API void incRef(::IceProxy::IceGrid::RegistryObserver*);
ICE_GRID_API void decRef(::IceProxy::IceGrid::RegistryObserver*);

}

namespace IceGrid
{

typedef ::IceInternal::Handle< ::IceGrid::NodeObserver> NodeObserverPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::IceGrid::NodeObserver> NodeObserverPrx;

ICE_GRID_API void __write(::IceInternal::BasicStream*, const NodeObserverPrx&);
ICE_GRID_API void __read(::IceInternal::BasicStream*, NodeObserverPrx&);
ICE_GRID_API void __write(::IceInternal::BasicStream*, const NodeObserverPtr&);
ICE_GRID_API void __patch__NodeObserverPtr(void*, ::Ice::ObjectPtr&);

ICE_GRID_API void __addObject(const NodeObserverPtr&, ::IceInternal::GCCountMap&);
ICE_GRID_API bool __usesClasses(const NodeObserverPtr&);
ICE_GRID_API void __decRefUnsafe(const NodeObserverPtr&);
ICE_GRID_API void __clearHandleUnsafe(NodeObserverPtr&);

typedef ::IceInternal::Handle< ::IceGrid::ApplicationObserver> ApplicationObserverPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::IceGrid::ApplicationObserver> ApplicationObserverPrx;

ICE_GRID_API void __write(::IceInternal::BasicStream*, const ApplicationObserverPrx&);
ICE_GRID_API void __read(::IceInternal::BasicStream*, ApplicationObserverPrx&);
ICE_GRID_API void __write(::IceInternal::BasicStream*, const ApplicationObserverPtr&);
ICE_GRID_API void __patch__ApplicationObserverPtr(void*, ::Ice::ObjectPtr&);

ICE_GRID_API void __addObject(const ApplicationObserverPtr&, ::IceInternal::GCCountMap&);
ICE_GRID_API bool __usesClasses(const ApplicationObserverPtr&);
ICE_GRID_API void __decRefUnsafe(const ApplicationObserverPtr&);
ICE_GRID_API void __clearHandleUnsafe(ApplicationObserverPtr&);

typedef ::IceInternal::Handle< ::IceGrid::AdapterObserver> AdapterObserverPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::IceGrid::AdapterObserver> AdapterObserverPrx;

ICE_GRID_API void __write(::IceInternal::BasicStream*, const AdapterObserverPrx&);
ICE_GRID_API void __read(::IceInternal::BasicStream*, AdapterObserverPrx&);
ICE_GRID_API void __write(::IceInternal::BasicStream*, const AdapterObserverPtr&);
ICE_GRID_API void __patch__AdapterObserverPtr(void*, ::Ice::ObjectPtr&);

ICE_GRID_API void __addObject(const AdapterObserverPtr&, ::IceInternal::GCCountMap&);
ICE_GRID_API bool __usesClasses(const AdapterObserverPtr&);
ICE_GRID_API void __decRefUnsafe(const AdapterObserverPtr&);
ICE_GRID_API void __clearHandleUnsafe(AdapterObserverPtr&);

typedef ::IceInternal::Handle< ::IceGrid::ObjectObserver> ObjectObserverPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::IceGrid::ObjectObserver> ObjectObserverPrx;

ICE_GRID_API void __write(::IceInternal::BasicStream*, const ObjectObserverPrx&);
ICE_GRID_API void __read(::IceInternal::BasicStream*, ObjectObserverPrx&);
ICE_GRID_API void __write(::IceInternal::BasicStream*, const ObjectObserverPtr&);
ICE_GRID_API void __patch__ObjectObserverPtr(void*, ::Ice::ObjectPtr&);

ICE_GRID_API void __addObject(const ObjectObserverPtr&, ::IceInternal::GCCountMap&);
ICE_GRID_API bool __usesClasses(const ObjectObserverPtr&);
ICE_GRID_API void __decRefUnsafe(const ObjectObserverPtr&);
ICE_GRID_API void __clearHandleUnsafe(ObjectObserverPtr&);

typedef ::IceInternal::Handle< ::IceGrid::RegistryObserver> RegistryObserverPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::IceGrid::RegistryObserver> RegistryObserverPrx;

ICE_GRID_API void __write(::IceInternal::BasicStream*, const RegistryObserverPrx&);
ICE_GRID_API void __read(::IceInternal::BasicStream*, RegistryObserverPrx&);
ICE_GRID_API void __write(::IceInternal::BasicStream*, const RegistryObserverPtr&);
ICE_GRID_API void __patch__RegistryObserverPtr(void*, ::Ice::ObjectPtr&);

ICE_GRID_API void __addObject(const RegistryObserverPtr&, ::IceInternal::GCCountMap&);
ICE_GRID_API bool __usesClasses(const RegistryObserverPtr&);
ICE_GRID_API void __decRefUnsafe(const RegistryObserverPtr&);
ICE_GRID_API void __clearHandleUnsafe(RegistryObserverPtr&);

}

namespace IceGrid
{

struct ServerDynamicInfo
{
    ::std::string id;
    ::IceGrid::ServerState state;
    ::Ice::Int pid;
    bool enabled;

    ICE_GRID_API bool operator==(const ServerDynamicInfo&) const;
    ICE_GRID_API bool operator!=(const ServerDynamicInfo&) const;
    ICE_GRID_API bool operator<(const ServerDynamicInfo&) const;
    ICE_GRID_API bool operator<=(const ServerDynamicInfo& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    ICE_GRID_API bool operator>(const ServerDynamicInfo& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    ICE_GRID_API bool operator>=(const ServerDynamicInfo& __rhs) const
    {
        return !operator<(__rhs);
    }

    ICE_GRID_API void __write(::IceInternal::BasicStream*) const;
    ICE_GRID_API void __read(::IceInternal::BasicStream*);
};

typedef ::std::vector< ::IceGrid::ServerDynamicInfo> ServerDynamicInfoSeq;

class __U__ServerDynamicInfoSeq { };
ICE_GRID_API void __write(::IceInternal::BasicStream*, const ::IceGrid::ServerDynamicInfo*, const ::IceGrid::ServerDynamicInfo*, __U__ServerDynamicInfoSeq);
ICE_GRID_API void __read(::IceInternal::BasicStream*, ServerDynamicInfoSeq&, __U__ServerDynamicInfoSeq);

struct AdapterDynamicInfo
{
    ::std::string id;
    ::Ice::ObjectPrx proxy;

    ICE_GRID_API bool operator==(const AdapterDynamicInfo&) const;
    ICE_GRID_API bool operator!=(const AdapterDynamicInfo&) const;
    ICE_GRID_API bool operator<(const AdapterDynamicInfo&) const;
    ICE_GRID_API bool operator<=(const AdapterDynamicInfo& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    ICE_GRID_API bool operator>(const AdapterDynamicInfo& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    ICE_GRID_API bool operator>=(const AdapterDynamicInfo& __rhs) const
    {
        return !operator<(__rhs);
    }

    ICE_GRID_API void __write(::IceInternal::BasicStream*) const;
    ICE_GRID_API void __read(::IceInternal::BasicStream*);
};

typedef ::std::vector< ::IceGrid::AdapterDynamicInfo> AdapterDynamicInfoSeq;

class __U__AdapterDynamicInfoSeq { };
ICE_GRID_API void __write(::IceInternal::BasicStream*, const ::IceGrid::AdapterDynamicInfo*, const ::IceGrid::AdapterDynamicInfo*, __U__AdapterDynamicInfoSeq);
ICE_GRID_API void __read(::IceInternal::BasicStream*, AdapterDynamicInfoSeq&, __U__AdapterDynamicInfoSeq);

struct NodeDynamicInfo
{
    ::IceGrid::NodeInfo info;
    ::IceGrid::ServerDynamicInfoSeq servers;
    ::IceGrid::AdapterDynamicInfoSeq adapters;

    ICE_GRID_API bool operator==(const NodeDynamicInfo&) const;
    ICE_GRID_API bool operator!=(const NodeDynamicInfo&) const;
    ICE_GRID_API bool operator<(const NodeDynamicInfo&) const;
    ICE_GRID_API bool operator<=(const NodeDynamicInfo& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    ICE_GRID_API bool operator>(const NodeDynamicInfo& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    ICE_GRID_API bool operator>=(const NodeDynamicInfo& __rhs) const
    {
        return !operator<(__rhs);
    }

    ICE_GRID_API void __write(::IceInternal::BasicStream*) const;
    ICE_GRID_API void __read(::IceInternal::BasicStream*);
};

typedef ::std::vector< ::IceGrid::NodeDynamicInfo> NodeDynamicInfoSeq;

class __U__NodeDynamicInfoSeq { };
ICE_GRID_API void __write(::IceInternal::BasicStream*, const ::IceGrid::NodeDynamicInfo*, const ::IceGrid::NodeDynamicInfo*, __U__NodeDynamicInfoSeq);
ICE_GRID_API void __read(::IceInternal::BasicStream*, NodeDynamicInfoSeq&, __U__NodeDynamicInfoSeq);

}

namespace IceGrid
{

class ICE_GRID_API AMI_NodeObserver_nodeInit : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response() = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::IceGrid::NodeObserverPrx&, const ::IceGrid::NodeDynamicInfoSeq&, const ::Ice::Context*);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::IceGrid::AMI_NodeObserver_nodeInit> AMI_NodeObserver_nodeInitPtr;

class ICE_GRID_API AMI_ApplicationObserver_applicationInit : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response() = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::IceGrid::ApplicationObserverPrx&, ::Ice::Int, const ::IceGrid::ApplicationInfoSeq&, const ::Ice::Context*);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::IceGrid::AMI_ApplicationObserver_applicationInit> AMI_ApplicationObserver_applicationInitPtr;

class ICE_GRID_API AMI_AdapterObserver_adapterInit : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response() = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::IceGrid::AdapterObserverPrx&, const ::IceGrid::AdapterInfoSeq&, const ::Ice::Context*);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::IceGrid::AMI_AdapterObserver_adapterInit> AMI_AdapterObserver_adapterInitPtr;

class ICE_GRID_API AMI_ObjectObserver_objectInit : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response() = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::IceGrid::ObjectObserverPrx&, const ::IceGrid::ObjectInfoSeq&, const ::Ice::Context*);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::IceGrid::AMI_ObjectObserver_objectInit> AMI_ObjectObserver_objectInitPtr;

class ICE_GRID_API AMI_RegistryObserver_registryInit : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response() = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::IceGrid::RegistryObserverPrx&, const ::IceGrid::RegistryInfoSeq&, const ::Ice::Context*);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::IceGrid::AMI_RegistryObserver_registryInit> AMI_RegistryObserver_registryInitPtr;

}

namespace IceAsync
{

}

namespace IceProxy
{

namespace IceGrid
{

class ICE_GRID_API NodeObserver : virtual public ::IceProxy::Ice::Object
{
public:

    void nodeInit(const ::IceGrid::NodeDynamicInfoSeq& nodes)
    {
        nodeInit(nodes, 0);
    }
    void nodeInit(const ::IceGrid::NodeDynamicInfoSeq& nodes, const ::Ice::Context& __ctx)
    {
        nodeInit(nodes, &__ctx);
    }
    
private:

    void nodeInit(const ::IceGrid::NodeDynamicInfoSeq&, const ::Ice::Context*);
    
public:
    void nodeInit_async(const ::IceGrid::AMI_NodeObserver_nodeInitPtr&, const ::IceGrid::NodeDynamicInfoSeq&);
    void nodeInit_async(const ::IceGrid::AMI_NodeObserver_nodeInitPtr&, const ::IceGrid::NodeDynamicInfoSeq&, const ::Ice::Context&);

    void nodeUp(const ::IceGrid::NodeDynamicInfo& node)
    {
        nodeUp(node, 0);
    }
    void nodeUp(const ::IceGrid::NodeDynamicInfo& node, const ::Ice::Context& __ctx)
    {
        nodeUp(node, &__ctx);
    }
    
private:

    void nodeUp(const ::IceGrid::NodeDynamicInfo&, const ::Ice::Context*);
    
public:

    void nodeDown(const ::std::string& name)
    {
        nodeDown(name, 0);
    }
    void nodeDown(const ::std::string& name, const ::Ice::Context& __ctx)
    {
        nodeDown(name, &__ctx);
    }
    
private:

    void nodeDown(const ::std::string&, const ::Ice::Context*);
    
public:

    void updateServer(const ::std::string& node, const ::IceGrid::ServerDynamicInfo& updatedInfo)
    {
        updateServer(node, updatedInfo, 0);
    }
    void updateServer(const ::std::string& node, const ::IceGrid::ServerDynamicInfo& updatedInfo, const ::Ice::Context& __ctx)
    {
        updateServer(node, updatedInfo, &__ctx);
    }
    
private:

    void updateServer(const ::std::string&, const ::IceGrid::ServerDynamicInfo&, const ::Ice::Context*);
    
public:

    void updateAdapter(const ::std::string& node, const ::IceGrid::AdapterDynamicInfo& updatedInfo)
    {
        updateAdapter(node, updatedInfo, 0);
    }
    void updateAdapter(const ::std::string& node, const ::IceGrid::AdapterDynamicInfo& updatedInfo, const ::Ice::Context& __ctx)
    {
        updateAdapter(node, updatedInfo, &__ctx);
    }
    
private:

    void updateAdapter(const ::std::string&, const ::IceGrid::AdapterDynamicInfo&, const ::Ice::Context*);
    
public:
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

class ICE_GRID_API ApplicationObserver : virtual public ::IceProxy::Ice::Object
{
public:

    void applicationInit(::Ice::Int serial, const ::IceGrid::ApplicationInfoSeq& applications)
    {
        applicationInit(serial, applications, 0);
    }
    void applicationInit(::Ice::Int serial, const ::IceGrid::ApplicationInfoSeq& applications, const ::Ice::Context& __ctx)
    {
        applicationInit(serial, applications, &__ctx);
    }
    
private:

    void applicationInit(::Ice::Int, const ::IceGrid::ApplicationInfoSeq&, const ::Ice::Context*);
    
public:
    void applicationInit_async(const ::IceGrid::AMI_ApplicationObserver_applicationInitPtr&, ::Ice::Int, const ::IceGrid::ApplicationInfoSeq&);
    void applicationInit_async(const ::IceGrid::AMI_ApplicationObserver_applicationInitPtr&, ::Ice::Int, const ::IceGrid::ApplicationInfoSeq&, const ::Ice::Context&);

    void applicationAdded(::Ice::Int serial, const ::IceGrid::ApplicationInfo& desc)
    {
        applicationAdded(serial, desc, 0);
    }
    void applicationAdded(::Ice::Int serial, const ::IceGrid::ApplicationInfo& desc, const ::Ice::Context& __ctx)
    {
        applicationAdded(serial, desc, &__ctx);
    }
    
private:

    void applicationAdded(::Ice::Int, const ::IceGrid::ApplicationInfo&, const ::Ice::Context*);
    
public:

    void applicationRemoved(::Ice::Int serial, const ::std::string& name)
    {
        applicationRemoved(serial, name, 0);
    }
    void applicationRemoved(::Ice::Int serial, const ::std::string& name, const ::Ice::Context& __ctx)
    {
        applicationRemoved(serial, name, &__ctx);
    }
    
private:

    void applicationRemoved(::Ice::Int, const ::std::string&, const ::Ice::Context*);
    
public:

    void applicationUpdated(::Ice::Int serial, const ::IceGrid::ApplicationUpdateInfo& desc)
    {
        applicationUpdated(serial, desc, 0);
    }
    void applicationUpdated(::Ice::Int serial, const ::IceGrid::ApplicationUpdateInfo& desc, const ::Ice::Context& __ctx)
    {
        applicationUpdated(serial, desc, &__ctx);
    }
    
private:

    void applicationUpdated(::Ice::Int, const ::IceGrid::ApplicationUpdateInfo&, const ::Ice::Context*);
    
public:
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

class ICE_GRID_API AdapterObserver : virtual public ::IceProxy::Ice::Object
{
public:

    void adapterInit(const ::IceGrid::AdapterInfoSeq& adpts)
    {
        adapterInit(adpts, 0);
    }
    void adapterInit(const ::IceGrid::AdapterInfoSeq& adpts, const ::Ice::Context& __ctx)
    {
        adapterInit(adpts, &__ctx);
    }
    
private:

    void adapterInit(const ::IceGrid::AdapterInfoSeq&, const ::Ice::Context*);
    
public:
    void adapterInit_async(const ::IceGrid::AMI_AdapterObserver_adapterInitPtr&, const ::IceGrid::AdapterInfoSeq&);
    void adapterInit_async(const ::IceGrid::AMI_AdapterObserver_adapterInitPtr&, const ::IceGrid::AdapterInfoSeq&, const ::Ice::Context&);

    void adapterAdded(const ::IceGrid::AdapterInfo& info)
    {
        adapterAdded(info, 0);
    }
    void adapterAdded(const ::IceGrid::AdapterInfo& info, const ::Ice::Context& __ctx)
    {
        adapterAdded(info, &__ctx);
    }
    
private:

    void adapterAdded(const ::IceGrid::AdapterInfo&, const ::Ice::Context*);
    
public:

    void adapterUpdated(const ::IceGrid::AdapterInfo& info)
    {
        adapterUpdated(info, 0);
    }
    void adapterUpdated(const ::IceGrid::AdapterInfo& info, const ::Ice::Context& __ctx)
    {
        adapterUpdated(info, &__ctx);
    }
    
private:

    void adapterUpdated(const ::IceGrid::AdapterInfo&, const ::Ice::Context*);
    
public:

    void adapterRemoved(const ::std::string& id)
    {
        adapterRemoved(id, 0);
    }
    void adapterRemoved(const ::std::string& id, const ::Ice::Context& __ctx)
    {
        adapterRemoved(id, &__ctx);
    }
    
private:

    void adapterRemoved(const ::std::string&, const ::Ice::Context*);
    
public:
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

class ICE_GRID_API ObjectObserver : virtual public ::IceProxy::Ice::Object
{
public:

    void objectInit(const ::IceGrid::ObjectInfoSeq& objects)
    {
        objectInit(objects, 0);
    }
    void objectInit(const ::IceGrid::ObjectInfoSeq& objects, const ::Ice::Context& __ctx)
    {
        objectInit(objects, &__ctx);
    }
    
private:

    void objectInit(const ::IceGrid::ObjectInfoSeq&, const ::Ice::Context*);
    
public:
    void objectInit_async(const ::IceGrid::AMI_ObjectObserver_objectInitPtr&, const ::IceGrid::ObjectInfoSeq&);
    void objectInit_async(const ::IceGrid::AMI_ObjectObserver_objectInitPtr&, const ::IceGrid::ObjectInfoSeq&, const ::Ice::Context&);

    void objectAdded(const ::IceGrid::ObjectInfo& info)
    {
        objectAdded(info, 0);
    }
    void objectAdded(const ::IceGrid::ObjectInfo& info, const ::Ice::Context& __ctx)
    {
        objectAdded(info, &__ctx);
    }
    
private:

    void objectAdded(const ::IceGrid::ObjectInfo&, const ::Ice::Context*);
    
public:

    void objectUpdated(const ::IceGrid::ObjectInfo& info)
    {
        objectUpdated(info, 0);
    }
    void objectUpdated(const ::IceGrid::ObjectInfo& info, const ::Ice::Context& __ctx)
    {
        objectUpdated(info, &__ctx);
    }
    
private:

    void objectUpdated(const ::IceGrid::ObjectInfo&, const ::Ice::Context*);
    
public:

    void objectRemoved(const ::Ice::Identity& id)
    {
        objectRemoved(id, 0);
    }
    void objectRemoved(const ::Ice::Identity& id, const ::Ice::Context& __ctx)
    {
        objectRemoved(id, &__ctx);
    }
    
private:

    void objectRemoved(const ::Ice::Identity&, const ::Ice::Context*);
    
public:
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

class ICE_GRID_API RegistryObserver : virtual public ::IceProxy::Ice::Object
{
public:

    void registryInit(const ::IceGrid::RegistryInfoSeq& registries)
    {
        registryInit(registries, 0);
    }
    void registryInit(const ::IceGrid::RegistryInfoSeq& registries, const ::Ice::Context& __ctx)
    {
        registryInit(registries, &__ctx);
    }
    
private:

    void registryInit(const ::IceGrid::RegistryInfoSeq&, const ::Ice::Context*);
    
public:
    void registryInit_async(const ::IceGrid::AMI_RegistryObserver_registryInitPtr&, const ::IceGrid::RegistryInfoSeq&);
    void registryInit_async(const ::IceGrid::AMI_RegistryObserver_registryInitPtr&, const ::IceGrid::RegistryInfoSeq&, const ::Ice::Context&);

    void registryUp(const ::IceGrid::RegistryInfo& node)
    {
        registryUp(node, 0);
    }
    void registryUp(const ::IceGrid::RegistryInfo& node, const ::Ice::Context& __ctx)
    {
        registryUp(node, &__ctx);
    }
    
private:

    void registryUp(const ::IceGrid::RegistryInfo&, const ::Ice::Context*);
    
public:

    void registryDown(const ::std::string& name)
    {
        registryDown(name, 0);
    }
    void registryDown(const ::std::string& name, const ::Ice::Context& __ctx)
    {
        registryDown(name, &__ctx);
    }
    
private:

    void registryDown(const ::std::string&, const ::Ice::Context*);
    
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

class ICE_GRID_API NodeObserver : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual void nodeInit(const ::IceGrid::NodeDynamicInfoSeq&, const ::Ice::Context*) = 0;

    virtual void nodeUp(const ::IceGrid::NodeDynamicInfo&, const ::Ice::Context*) = 0;

    virtual void nodeDown(const ::std::string&, const ::Ice::Context*) = 0;

    virtual void updateServer(const ::std::string&, const ::IceGrid::ServerDynamicInfo&, const ::Ice::Context*) = 0;

    virtual void updateAdapter(const ::std::string&, const ::IceGrid::AdapterDynamicInfo&, const ::Ice::Context*) = 0;
};

class ICE_GRID_API ApplicationObserver : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual void applicationInit(::Ice::Int, const ::IceGrid::ApplicationInfoSeq&, const ::Ice::Context*) = 0;

    virtual void applicationAdded(::Ice::Int, const ::IceGrid::ApplicationInfo&, const ::Ice::Context*) = 0;

    virtual void applicationRemoved(::Ice::Int, const ::std::string&, const ::Ice::Context*) = 0;

    virtual void applicationUpdated(::Ice::Int, const ::IceGrid::ApplicationUpdateInfo&, const ::Ice::Context*) = 0;
};

class ICE_GRID_API AdapterObserver : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual void adapterInit(const ::IceGrid::AdapterInfoSeq&, const ::Ice::Context*) = 0;

    virtual void adapterAdded(const ::IceGrid::AdapterInfo&, const ::Ice::Context*) = 0;

    virtual void adapterUpdated(const ::IceGrid::AdapterInfo&, const ::Ice::Context*) = 0;

    virtual void adapterRemoved(const ::std::string&, const ::Ice::Context*) = 0;
};

class ICE_GRID_API ObjectObserver : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual void objectInit(const ::IceGrid::ObjectInfoSeq&, const ::Ice::Context*) = 0;

    virtual void objectAdded(const ::IceGrid::ObjectInfo&, const ::Ice::Context*) = 0;

    virtual void objectUpdated(const ::IceGrid::ObjectInfo&, const ::Ice::Context*) = 0;

    virtual void objectRemoved(const ::Ice::Identity&, const ::Ice::Context*) = 0;
};

class ICE_GRID_API RegistryObserver : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual void registryInit(const ::IceGrid::RegistryInfoSeq&, const ::Ice::Context*) = 0;

    virtual void registryUp(const ::IceGrid::RegistryInfo&, const ::Ice::Context*) = 0;

    virtual void registryDown(const ::std::string&, const ::Ice::Context*) = 0;
};

}

}

namespace IceDelegateM
{

namespace IceGrid
{

class ICE_GRID_API NodeObserver : virtual public ::IceDelegate::IceGrid::NodeObserver,
                                  virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual void nodeInit(const ::IceGrid::NodeDynamicInfoSeq&, const ::Ice::Context*);

    virtual void nodeUp(const ::IceGrid::NodeDynamicInfo&, const ::Ice::Context*);

    virtual void nodeDown(const ::std::string&, const ::Ice::Context*);

    virtual void updateServer(const ::std::string&, const ::IceGrid::ServerDynamicInfo&, const ::Ice::Context*);

    virtual void updateAdapter(const ::std::string&, const ::IceGrid::AdapterDynamicInfo&, const ::Ice::Context*);
};

class ICE_GRID_API ApplicationObserver : virtual public ::IceDelegate::IceGrid::ApplicationObserver,
                                         virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual void applicationInit(::Ice::Int, const ::IceGrid::ApplicationInfoSeq&, const ::Ice::Context*);

    virtual void applicationAdded(::Ice::Int, const ::IceGrid::ApplicationInfo&, const ::Ice::Context*);

    virtual void applicationRemoved(::Ice::Int, const ::std::string&, const ::Ice::Context*);

    virtual void applicationUpdated(::Ice::Int, const ::IceGrid::ApplicationUpdateInfo&, const ::Ice::Context*);
};

class ICE_GRID_API AdapterObserver : virtual public ::IceDelegate::IceGrid::AdapterObserver,
                                     virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual void adapterInit(const ::IceGrid::AdapterInfoSeq&, const ::Ice::Context*);

    virtual void adapterAdded(const ::IceGrid::AdapterInfo&, const ::Ice::Context*);

    virtual void adapterUpdated(const ::IceGrid::AdapterInfo&, const ::Ice::Context*);

    virtual void adapterRemoved(const ::std::string&, const ::Ice::Context*);
};

class ICE_GRID_API ObjectObserver : virtual public ::IceDelegate::IceGrid::ObjectObserver,
                                    virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual void objectInit(const ::IceGrid::ObjectInfoSeq&, const ::Ice::Context*);

    virtual void objectAdded(const ::IceGrid::ObjectInfo&, const ::Ice::Context*);

    virtual void objectUpdated(const ::IceGrid::ObjectInfo&, const ::Ice::Context*);

    virtual void objectRemoved(const ::Ice::Identity&, const ::Ice::Context*);
};

class ICE_GRID_API RegistryObserver : virtual public ::IceDelegate::IceGrid::RegistryObserver,
                                      virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual void registryInit(const ::IceGrid::RegistryInfoSeq&, const ::Ice::Context*);

    virtual void registryUp(const ::IceGrid::RegistryInfo&, const ::Ice::Context*);

    virtual void registryDown(const ::std::string&, const ::Ice::Context*);
};

}

}

namespace IceDelegateD
{

namespace IceGrid
{

class ICE_GRID_API NodeObserver : virtual public ::IceDelegate::IceGrid::NodeObserver,
                                  virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual void nodeInit(const ::IceGrid::NodeDynamicInfoSeq&, const ::Ice::Context*);

    virtual void nodeUp(const ::IceGrid::NodeDynamicInfo&, const ::Ice::Context*);

    virtual void nodeDown(const ::std::string&, const ::Ice::Context*);

    virtual void updateServer(const ::std::string&, const ::IceGrid::ServerDynamicInfo&, const ::Ice::Context*);

    virtual void updateAdapter(const ::std::string&, const ::IceGrid::AdapterDynamicInfo&, const ::Ice::Context*);
};

class ICE_GRID_API ApplicationObserver : virtual public ::IceDelegate::IceGrid::ApplicationObserver,
                                         virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual void applicationInit(::Ice::Int, const ::IceGrid::ApplicationInfoSeq&, const ::Ice::Context*);

    virtual void applicationAdded(::Ice::Int, const ::IceGrid::ApplicationInfo&, const ::Ice::Context*);

    virtual void applicationRemoved(::Ice::Int, const ::std::string&, const ::Ice::Context*);

    virtual void applicationUpdated(::Ice::Int, const ::IceGrid::ApplicationUpdateInfo&, const ::Ice::Context*);
};

class ICE_GRID_API AdapterObserver : virtual public ::IceDelegate::IceGrid::AdapterObserver,
                                     virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual void adapterInit(const ::IceGrid::AdapterInfoSeq&, const ::Ice::Context*);

    virtual void adapterAdded(const ::IceGrid::AdapterInfo&, const ::Ice::Context*);

    virtual void adapterUpdated(const ::IceGrid::AdapterInfo&, const ::Ice::Context*);

    virtual void adapterRemoved(const ::std::string&, const ::Ice::Context*);
};

class ICE_GRID_API ObjectObserver : virtual public ::IceDelegate::IceGrid::ObjectObserver,
                                    virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual void objectInit(const ::IceGrid::ObjectInfoSeq&, const ::Ice::Context*);

    virtual void objectAdded(const ::IceGrid::ObjectInfo&, const ::Ice::Context*);

    virtual void objectUpdated(const ::IceGrid::ObjectInfo&, const ::Ice::Context*);

    virtual void objectRemoved(const ::Ice::Identity&, const ::Ice::Context*);
};

class ICE_GRID_API RegistryObserver : virtual public ::IceDelegate::IceGrid::RegistryObserver,
                                      virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual void registryInit(const ::IceGrid::RegistryInfoSeq&, const ::Ice::Context*);

    virtual void registryUp(const ::IceGrid::RegistryInfo&, const ::Ice::Context*);

    virtual void registryDown(const ::std::string&, const ::Ice::Context*);
};

}

}

namespace IceGrid
{

class ICE_GRID_API NodeObserver : virtual public ::Ice::Object
{
public:

    typedef NodeObserverPrx ProxyType;
    typedef NodeObserverPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual void nodeInit(const ::IceGrid::NodeDynamicInfoSeq&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___nodeInit(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void nodeUp(const ::IceGrid::NodeDynamicInfo&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___nodeUp(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void nodeDown(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___nodeDown(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void updateServer(const ::std::string&, const ::IceGrid::ServerDynamicInfo&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___updateServer(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void updateAdapter(const ::std::string&, const ::IceGrid::AdapterDynamicInfo&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___updateAdapter(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void ICE_GRID_API __patch__NodeObserverPtr(void*, ::Ice::ObjectPtr&);

class ICE_GRID_API ApplicationObserver : virtual public ::Ice::Object
{
public:

    typedef ApplicationObserverPrx ProxyType;
    typedef ApplicationObserverPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual void applicationInit(::Ice::Int, const ::IceGrid::ApplicationInfoSeq&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___applicationInit(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void applicationAdded(::Ice::Int, const ::IceGrid::ApplicationInfo&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___applicationAdded(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void applicationRemoved(::Ice::Int, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___applicationRemoved(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void applicationUpdated(::Ice::Int, const ::IceGrid::ApplicationUpdateInfo&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___applicationUpdated(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void ICE_GRID_API __patch__ApplicationObserverPtr(void*, ::Ice::ObjectPtr&);

class ICE_GRID_API AdapterObserver : virtual public ::Ice::Object
{
public:

    typedef AdapterObserverPrx ProxyType;
    typedef AdapterObserverPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual void adapterInit(const ::IceGrid::AdapterInfoSeq&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___adapterInit(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void adapterAdded(const ::IceGrid::AdapterInfo&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___adapterAdded(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void adapterUpdated(const ::IceGrid::AdapterInfo&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___adapterUpdated(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void adapterRemoved(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___adapterRemoved(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void ICE_GRID_API __patch__AdapterObserverPtr(void*, ::Ice::ObjectPtr&);

class ICE_GRID_API ObjectObserver : virtual public ::Ice::Object
{
public:

    typedef ObjectObserverPrx ProxyType;
    typedef ObjectObserverPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual void objectInit(const ::IceGrid::ObjectInfoSeq&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___objectInit(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void objectAdded(const ::IceGrid::ObjectInfo&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___objectAdded(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void objectUpdated(const ::IceGrid::ObjectInfo&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___objectUpdated(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void objectRemoved(const ::Ice::Identity&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___objectRemoved(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void ICE_GRID_API __patch__ObjectObserverPtr(void*, ::Ice::ObjectPtr&);

class ICE_GRID_API RegistryObserver : virtual public ::Ice::Object
{
public:

    typedef RegistryObserverPrx ProxyType;
    typedef RegistryObserverPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual void registryInit(const ::IceGrid::RegistryInfoSeq&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___registryInit(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void registryUp(const ::IceGrid::RegistryInfo&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___registryUp(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void registryDown(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___registryDown(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void ICE_GRID_API __patch__RegistryObserverPtr(void*, ::Ice::ObjectPtr&);

}

#endif
