// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.2.1
// Generated from file `Descriptor.ice'

#ifndef __IceGrid_Descriptor_h__
#define __IceGrid_Descriptor_h__

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
#include <Ice/FactoryTable.h>
#include <Ice/StreamF.h>
#include <Ice/Identity.h>
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

class CommunicatorDescriptor;
ICE_GRID_API bool operator==(const CommunicatorDescriptor&, const CommunicatorDescriptor&);
ICE_GRID_API bool operator!=(const CommunicatorDescriptor&, const CommunicatorDescriptor&);
ICE_GRID_API bool operator<(const CommunicatorDescriptor&, const CommunicatorDescriptor&);
ICE_GRID_API bool operator<=(const CommunicatorDescriptor&, const CommunicatorDescriptor&);
ICE_GRID_API bool operator>(const CommunicatorDescriptor&, const CommunicatorDescriptor&);
ICE_GRID_API bool operator>=(const CommunicatorDescriptor&, const CommunicatorDescriptor&);

class ServerDescriptor;
ICE_GRID_API bool operator==(const ServerDescriptor&, const ServerDescriptor&);
ICE_GRID_API bool operator!=(const ServerDescriptor&, const ServerDescriptor&);
ICE_GRID_API bool operator<(const ServerDescriptor&, const ServerDescriptor&);
ICE_GRID_API bool operator<=(const ServerDescriptor&, const ServerDescriptor&);
ICE_GRID_API bool operator>(const ServerDescriptor&, const ServerDescriptor&);
ICE_GRID_API bool operator>=(const ServerDescriptor&, const ServerDescriptor&);

class ServiceDescriptor;
ICE_GRID_API bool operator==(const ServiceDescriptor&, const ServiceDescriptor&);
ICE_GRID_API bool operator!=(const ServiceDescriptor&, const ServiceDescriptor&);
ICE_GRID_API bool operator<(const ServiceDescriptor&, const ServiceDescriptor&);
ICE_GRID_API bool operator<=(const ServiceDescriptor&, const ServiceDescriptor&);
ICE_GRID_API bool operator>(const ServiceDescriptor&, const ServiceDescriptor&);
ICE_GRID_API bool operator>=(const ServiceDescriptor&, const ServiceDescriptor&);

class IceBoxDescriptor;
ICE_GRID_API bool operator==(const IceBoxDescriptor&, const IceBoxDescriptor&);
ICE_GRID_API bool operator!=(const IceBoxDescriptor&, const IceBoxDescriptor&);
ICE_GRID_API bool operator<(const IceBoxDescriptor&, const IceBoxDescriptor&);
ICE_GRID_API bool operator<=(const IceBoxDescriptor&, const IceBoxDescriptor&);
ICE_GRID_API bool operator>(const IceBoxDescriptor&, const IceBoxDescriptor&);
ICE_GRID_API bool operator>=(const IceBoxDescriptor&, const IceBoxDescriptor&);

class LoadBalancingPolicy;
ICE_GRID_API bool operator==(const LoadBalancingPolicy&, const LoadBalancingPolicy&);
ICE_GRID_API bool operator!=(const LoadBalancingPolicy&, const LoadBalancingPolicy&);
ICE_GRID_API bool operator<(const LoadBalancingPolicy&, const LoadBalancingPolicy&);
ICE_GRID_API bool operator<=(const LoadBalancingPolicy&, const LoadBalancingPolicy&);
ICE_GRID_API bool operator>(const LoadBalancingPolicy&, const LoadBalancingPolicy&);
ICE_GRID_API bool operator>=(const LoadBalancingPolicy&, const LoadBalancingPolicy&);

class RandomLoadBalancingPolicy;
ICE_GRID_API bool operator==(const RandomLoadBalancingPolicy&, const RandomLoadBalancingPolicy&);
ICE_GRID_API bool operator!=(const RandomLoadBalancingPolicy&, const RandomLoadBalancingPolicy&);
ICE_GRID_API bool operator<(const RandomLoadBalancingPolicy&, const RandomLoadBalancingPolicy&);
ICE_GRID_API bool operator<=(const RandomLoadBalancingPolicy&, const RandomLoadBalancingPolicy&);
ICE_GRID_API bool operator>(const RandomLoadBalancingPolicy&, const RandomLoadBalancingPolicy&);
ICE_GRID_API bool operator>=(const RandomLoadBalancingPolicy&, const RandomLoadBalancingPolicy&);

class OrderedLoadBalancingPolicy;
ICE_GRID_API bool operator==(const OrderedLoadBalancingPolicy&, const OrderedLoadBalancingPolicy&);
ICE_GRID_API bool operator!=(const OrderedLoadBalancingPolicy&, const OrderedLoadBalancingPolicy&);
ICE_GRID_API bool operator<(const OrderedLoadBalancingPolicy&, const OrderedLoadBalancingPolicy&);
ICE_GRID_API bool operator<=(const OrderedLoadBalancingPolicy&, const OrderedLoadBalancingPolicy&);
ICE_GRID_API bool operator>(const OrderedLoadBalancingPolicy&, const OrderedLoadBalancingPolicy&);
ICE_GRID_API bool operator>=(const OrderedLoadBalancingPolicy&, const OrderedLoadBalancingPolicy&);

class RoundRobinLoadBalancingPolicy;
ICE_GRID_API bool operator==(const RoundRobinLoadBalancingPolicy&, const RoundRobinLoadBalancingPolicy&);
ICE_GRID_API bool operator!=(const RoundRobinLoadBalancingPolicy&, const RoundRobinLoadBalancingPolicy&);
ICE_GRID_API bool operator<(const RoundRobinLoadBalancingPolicy&, const RoundRobinLoadBalancingPolicy&);
ICE_GRID_API bool operator<=(const RoundRobinLoadBalancingPolicy&, const RoundRobinLoadBalancingPolicy&);
ICE_GRID_API bool operator>(const RoundRobinLoadBalancingPolicy&, const RoundRobinLoadBalancingPolicy&);
ICE_GRID_API bool operator>=(const RoundRobinLoadBalancingPolicy&, const RoundRobinLoadBalancingPolicy&);

class AdaptiveLoadBalancingPolicy;
ICE_GRID_API bool operator==(const AdaptiveLoadBalancingPolicy&, const AdaptiveLoadBalancingPolicy&);
ICE_GRID_API bool operator!=(const AdaptiveLoadBalancingPolicy&, const AdaptiveLoadBalancingPolicy&);
ICE_GRID_API bool operator<(const AdaptiveLoadBalancingPolicy&, const AdaptiveLoadBalancingPolicy&);
ICE_GRID_API bool operator<=(const AdaptiveLoadBalancingPolicy&, const AdaptiveLoadBalancingPolicy&);
ICE_GRID_API bool operator>(const AdaptiveLoadBalancingPolicy&, const AdaptiveLoadBalancingPolicy&);
ICE_GRID_API bool operator>=(const AdaptiveLoadBalancingPolicy&, const AdaptiveLoadBalancingPolicy&);

class BoxedString;
ICE_GRID_API bool operator==(const BoxedString&, const BoxedString&);
ICE_GRID_API bool operator!=(const BoxedString&, const BoxedString&);
ICE_GRID_API bool operator<(const BoxedString&, const BoxedString&);
ICE_GRID_API bool operator<=(const BoxedString&, const BoxedString&);
ICE_GRID_API bool operator>(const BoxedString&, const BoxedString&);
ICE_GRID_API bool operator>=(const BoxedString&, const BoxedString&);

class BoxedDistributionDescriptor;
ICE_GRID_API bool operator==(const BoxedDistributionDescriptor&, const BoxedDistributionDescriptor&);
ICE_GRID_API bool operator!=(const BoxedDistributionDescriptor&, const BoxedDistributionDescriptor&);
ICE_GRID_API bool operator<(const BoxedDistributionDescriptor&, const BoxedDistributionDescriptor&);
ICE_GRID_API bool operator<=(const BoxedDistributionDescriptor&, const BoxedDistributionDescriptor&);
ICE_GRID_API bool operator>(const BoxedDistributionDescriptor&, const BoxedDistributionDescriptor&);
ICE_GRID_API bool operator>=(const BoxedDistributionDescriptor&, const BoxedDistributionDescriptor&);

}

}

namespace IceGrid
{

class CommunicatorDescriptor;
ICE_GRID_API bool operator==(const CommunicatorDescriptor&, const CommunicatorDescriptor&);
ICE_GRID_API bool operator!=(const CommunicatorDescriptor&, const CommunicatorDescriptor&);
ICE_GRID_API bool operator<(const CommunicatorDescriptor&, const CommunicatorDescriptor&);
ICE_GRID_API bool operator<=(const CommunicatorDescriptor&, const CommunicatorDescriptor&);
ICE_GRID_API bool operator>(const CommunicatorDescriptor&, const CommunicatorDescriptor&);
ICE_GRID_API bool operator>=(const CommunicatorDescriptor&, const CommunicatorDescriptor&);

class ServerDescriptor;
ICE_GRID_API bool operator==(const ServerDescriptor&, const ServerDescriptor&);
ICE_GRID_API bool operator!=(const ServerDescriptor&, const ServerDescriptor&);
ICE_GRID_API bool operator<(const ServerDescriptor&, const ServerDescriptor&);
ICE_GRID_API bool operator<=(const ServerDescriptor&, const ServerDescriptor&);
ICE_GRID_API bool operator>(const ServerDescriptor&, const ServerDescriptor&);
ICE_GRID_API bool operator>=(const ServerDescriptor&, const ServerDescriptor&);

class ServiceDescriptor;
ICE_GRID_API bool operator==(const ServiceDescriptor&, const ServiceDescriptor&);
ICE_GRID_API bool operator!=(const ServiceDescriptor&, const ServiceDescriptor&);
ICE_GRID_API bool operator<(const ServiceDescriptor&, const ServiceDescriptor&);
ICE_GRID_API bool operator<=(const ServiceDescriptor&, const ServiceDescriptor&);
ICE_GRID_API bool operator>(const ServiceDescriptor&, const ServiceDescriptor&);
ICE_GRID_API bool operator>=(const ServiceDescriptor&, const ServiceDescriptor&);

class IceBoxDescriptor;
ICE_GRID_API bool operator==(const IceBoxDescriptor&, const IceBoxDescriptor&);
ICE_GRID_API bool operator!=(const IceBoxDescriptor&, const IceBoxDescriptor&);
ICE_GRID_API bool operator<(const IceBoxDescriptor&, const IceBoxDescriptor&);
ICE_GRID_API bool operator<=(const IceBoxDescriptor&, const IceBoxDescriptor&);
ICE_GRID_API bool operator>(const IceBoxDescriptor&, const IceBoxDescriptor&);
ICE_GRID_API bool operator>=(const IceBoxDescriptor&, const IceBoxDescriptor&);

class LoadBalancingPolicy;
ICE_GRID_API bool operator==(const LoadBalancingPolicy&, const LoadBalancingPolicy&);
ICE_GRID_API bool operator!=(const LoadBalancingPolicy&, const LoadBalancingPolicy&);
ICE_GRID_API bool operator<(const LoadBalancingPolicy&, const LoadBalancingPolicy&);
ICE_GRID_API bool operator<=(const LoadBalancingPolicy&, const LoadBalancingPolicy&);
ICE_GRID_API bool operator>(const LoadBalancingPolicy&, const LoadBalancingPolicy&);
ICE_GRID_API bool operator>=(const LoadBalancingPolicy&, const LoadBalancingPolicy&);

class RandomLoadBalancingPolicy;
ICE_GRID_API bool operator==(const RandomLoadBalancingPolicy&, const RandomLoadBalancingPolicy&);
ICE_GRID_API bool operator!=(const RandomLoadBalancingPolicy&, const RandomLoadBalancingPolicy&);
ICE_GRID_API bool operator<(const RandomLoadBalancingPolicy&, const RandomLoadBalancingPolicy&);
ICE_GRID_API bool operator<=(const RandomLoadBalancingPolicy&, const RandomLoadBalancingPolicy&);
ICE_GRID_API bool operator>(const RandomLoadBalancingPolicy&, const RandomLoadBalancingPolicy&);
ICE_GRID_API bool operator>=(const RandomLoadBalancingPolicy&, const RandomLoadBalancingPolicy&);

class OrderedLoadBalancingPolicy;
ICE_GRID_API bool operator==(const OrderedLoadBalancingPolicy&, const OrderedLoadBalancingPolicy&);
ICE_GRID_API bool operator!=(const OrderedLoadBalancingPolicy&, const OrderedLoadBalancingPolicy&);
ICE_GRID_API bool operator<(const OrderedLoadBalancingPolicy&, const OrderedLoadBalancingPolicy&);
ICE_GRID_API bool operator<=(const OrderedLoadBalancingPolicy&, const OrderedLoadBalancingPolicy&);
ICE_GRID_API bool operator>(const OrderedLoadBalancingPolicy&, const OrderedLoadBalancingPolicy&);
ICE_GRID_API bool operator>=(const OrderedLoadBalancingPolicy&, const OrderedLoadBalancingPolicy&);

class RoundRobinLoadBalancingPolicy;
ICE_GRID_API bool operator==(const RoundRobinLoadBalancingPolicy&, const RoundRobinLoadBalancingPolicy&);
ICE_GRID_API bool operator!=(const RoundRobinLoadBalancingPolicy&, const RoundRobinLoadBalancingPolicy&);
ICE_GRID_API bool operator<(const RoundRobinLoadBalancingPolicy&, const RoundRobinLoadBalancingPolicy&);
ICE_GRID_API bool operator<=(const RoundRobinLoadBalancingPolicy&, const RoundRobinLoadBalancingPolicy&);
ICE_GRID_API bool operator>(const RoundRobinLoadBalancingPolicy&, const RoundRobinLoadBalancingPolicy&);
ICE_GRID_API bool operator>=(const RoundRobinLoadBalancingPolicy&, const RoundRobinLoadBalancingPolicy&);

class AdaptiveLoadBalancingPolicy;
ICE_GRID_API bool operator==(const AdaptiveLoadBalancingPolicy&, const AdaptiveLoadBalancingPolicy&);
ICE_GRID_API bool operator!=(const AdaptiveLoadBalancingPolicy&, const AdaptiveLoadBalancingPolicy&);
ICE_GRID_API bool operator<(const AdaptiveLoadBalancingPolicy&, const AdaptiveLoadBalancingPolicy&);
ICE_GRID_API bool operator<=(const AdaptiveLoadBalancingPolicy&, const AdaptiveLoadBalancingPolicy&);
ICE_GRID_API bool operator>(const AdaptiveLoadBalancingPolicy&, const AdaptiveLoadBalancingPolicy&);
ICE_GRID_API bool operator>=(const AdaptiveLoadBalancingPolicy&, const AdaptiveLoadBalancingPolicy&);

class BoxedString;
ICE_GRID_API bool operator==(const BoxedString&, const BoxedString&);
ICE_GRID_API bool operator!=(const BoxedString&, const BoxedString&);
ICE_GRID_API bool operator<(const BoxedString&, const BoxedString&);
ICE_GRID_API bool operator<=(const BoxedString&, const BoxedString&);
ICE_GRID_API bool operator>(const BoxedString&, const BoxedString&);
ICE_GRID_API bool operator>=(const BoxedString&, const BoxedString&);

class BoxedDistributionDescriptor;
ICE_GRID_API bool operator==(const BoxedDistributionDescriptor&, const BoxedDistributionDescriptor&);
ICE_GRID_API bool operator!=(const BoxedDistributionDescriptor&, const BoxedDistributionDescriptor&);
ICE_GRID_API bool operator<(const BoxedDistributionDescriptor&, const BoxedDistributionDescriptor&);
ICE_GRID_API bool operator<=(const BoxedDistributionDescriptor&, const BoxedDistributionDescriptor&);
ICE_GRID_API bool operator>(const BoxedDistributionDescriptor&, const BoxedDistributionDescriptor&);
ICE_GRID_API bool operator>=(const BoxedDistributionDescriptor&, const BoxedDistributionDescriptor&);

}

namespace IceInternal
{

ICE_GRID_API void incRef(::IceGrid::CommunicatorDescriptor*);
ICE_GRID_API void decRef(::IceGrid::CommunicatorDescriptor*);

ICE_GRID_API void incRef(::IceProxy::IceGrid::CommunicatorDescriptor*);
ICE_GRID_API void decRef(::IceProxy::IceGrid::CommunicatorDescriptor*);

ICE_GRID_API void incRef(::IceGrid::ServerDescriptor*);
ICE_GRID_API void decRef(::IceGrid::ServerDescriptor*);

ICE_GRID_API void incRef(::IceProxy::IceGrid::ServerDescriptor*);
ICE_GRID_API void decRef(::IceProxy::IceGrid::ServerDescriptor*);

ICE_GRID_API void incRef(::IceGrid::ServiceDescriptor*);
ICE_GRID_API void decRef(::IceGrid::ServiceDescriptor*);

ICE_GRID_API void incRef(::IceProxy::IceGrid::ServiceDescriptor*);
ICE_GRID_API void decRef(::IceProxy::IceGrid::ServiceDescriptor*);

ICE_GRID_API void incRef(::IceGrid::IceBoxDescriptor*);
ICE_GRID_API void decRef(::IceGrid::IceBoxDescriptor*);

ICE_GRID_API void incRef(::IceProxy::IceGrid::IceBoxDescriptor*);
ICE_GRID_API void decRef(::IceProxy::IceGrid::IceBoxDescriptor*);

ICE_GRID_API void incRef(::IceGrid::LoadBalancingPolicy*);
ICE_GRID_API void decRef(::IceGrid::LoadBalancingPolicy*);

ICE_GRID_API void incRef(::IceProxy::IceGrid::LoadBalancingPolicy*);
ICE_GRID_API void decRef(::IceProxy::IceGrid::LoadBalancingPolicy*);

ICE_GRID_API void incRef(::IceGrid::RandomLoadBalancingPolicy*);
ICE_GRID_API void decRef(::IceGrid::RandomLoadBalancingPolicy*);

ICE_GRID_API void incRef(::IceProxy::IceGrid::RandomLoadBalancingPolicy*);
ICE_GRID_API void decRef(::IceProxy::IceGrid::RandomLoadBalancingPolicy*);

ICE_GRID_API void incRef(::IceGrid::OrderedLoadBalancingPolicy*);
ICE_GRID_API void decRef(::IceGrid::OrderedLoadBalancingPolicy*);

ICE_GRID_API void incRef(::IceProxy::IceGrid::OrderedLoadBalancingPolicy*);
ICE_GRID_API void decRef(::IceProxy::IceGrid::OrderedLoadBalancingPolicy*);

ICE_GRID_API void incRef(::IceGrid::RoundRobinLoadBalancingPolicy*);
ICE_GRID_API void decRef(::IceGrid::RoundRobinLoadBalancingPolicy*);

ICE_GRID_API void incRef(::IceProxy::IceGrid::RoundRobinLoadBalancingPolicy*);
ICE_GRID_API void decRef(::IceProxy::IceGrid::RoundRobinLoadBalancingPolicy*);

ICE_GRID_API void incRef(::IceGrid::AdaptiveLoadBalancingPolicy*);
ICE_GRID_API void decRef(::IceGrid::AdaptiveLoadBalancingPolicy*);

ICE_GRID_API void incRef(::IceProxy::IceGrid::AdaptiveLoadBalancingPolicy*);
ICE_GRID_API void decRef(::IceProxy::IceGrid::AdaptiveLoadBalancingPolicy*);

ICE_GRID_API void incRef(::IceGrid::BoxedString*);
ICE_GRID_API void decRef(::IceGrid::BoxedString*);

ICE_GRID_API void incRef(::IceProxy::IceGrid::BoxedString*);
ICE_GRID_API void decRef(::IceProxy::IceGrid::BoxedString*);

ICE_GRID_API void incRef(::IceGrid::BoxedDistributionDescriptor*);
ICE_GRID_API void decRef(::IceGrid::BoxedDistributionDescriptor*);

ICE_GRID_API void incRef(::IceProxy::IceGrid::BoxedDistributionDescriptor*);
ICE_GRID_API void decRef(::IceProxy::IceGrid::BoxedDistributionDescriptor*);

}

namespace IceGrid
{

typedef ::IceInternal::Handle< ::IceGrid::CommunicatorDescriptor> CommunicatorDescriptorPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::IceGrid::CommunicatorDescriptor> CommunicatorDescriptorPrx;

ICE_GRID_API void __write(::IceInternal::BasicStream*, const CommunicatorDescriptorPrx&);
ICE_GRID_API void __read(::IceInternal::BasicStream*, CommunicatorDescriptorPrx&);
ICE_GRID_API void __write(::IceInternal::BasicStream*, const CommunicatorDescriptorPtr&);
ICE_GRID_API void __patch__CommunicatorDescriptorPtr(void*, ::Ice::ObjectPtr&);

ICE_GRID_API void __addObject(const CommunicatorDescriptorPtr&, ::IceInternal::GCCountMap&);
ICE_GRID_API bool __usesClasses(const CommunicatorDescriptorPtr&);
ICE_GRID_API void __decRefUnsafe(const CommunicatorDescriptorPtr&);
ICE_GRID_API void __clearHandleUnsafe(CommunicatorDescriptorPtr&);

typedef ::IceInternal::Handle< ::IceGrid::ServerDescriptor> ServerDescriptorPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::IceGrid::ServerDescriptor> ServerDescriptorPrx;

ICE_GRID_API void __write(::IceInternal::BasicStream*, const ServerDescriptorPrx&);
ICE_GRID_API void __read(::IceInternal::BasicStream*, ServerDescriptorPrx&);
ICE_GRID_API void __write(::IceInternal::BasicStream*, const ServerDescriptorPtr&);
ICE_GRID_API void __patch__ServerDescriptorPtr(void*, ::Ice::ObjectPtr&);

ICE_GRID_API void __addObject(const ServerDescriptorPtr&, ::IceInternal::GCCountMap&);
ICE_GRID_API bool __usesClasses(const ServerDescriptorPtr&);
ICE_GRID_API void __decRefUnsafe(const ServerDescriptorPtr&);
ICE_GRID_API void __clearHandleUnsafe(ServerDescriptorPtr&);

typedef ::IceInternal::Handle< ::IceGrid::ServiceDescriptor> ServiceDescriptorPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::IceGrid::ServiceDescriptor> ServiceDescriptorPrx;

ICE_GRID_API void __write(::IceInternal::BasicStream*, const ServiceDescriptorPrx&);
ICE_GRID_API void __read(::IceInternal::BasicStream*, ServiceDescriptorPrx&);
ICE_GRID_API void __write(::IceInternal::BasicStream*, const ServiceDescriptorPtr&);
ICE_GRID_API void __patch__ServiceDescriptorPtr(void*, ::Ice::ObjectPtr&);

ICE_GRID_API void __addObject(const ServiceDescriptorPtr&, ::IceInternal::GCCountMap&);
ICE_GRID_API bool __usesClasses(const ServiceDescriptorPtr&);
ICE_GRID_API void __decRefUnsafe(const ServiceDescriptorPtr&);
ICE_GRID_API void __clearHandleUnsafe(ServiceDescriptorPtr&);

typedef ::IceInternal::Handle< ::IceGrid::IceBoxDescriptor> IceBoxDescriptorPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::IceGrid::IceBoxDescriptor> IceBoxDescriptorPrx;

ICE_GRID_API void __write(::IceInternal::BasicStream*, const IceBoxDescriptorPrx&);
ICE_GRID_API void __read(::IceInternal::BasicStream*, IceBoxDescriptorPrx&);
ICE_GRID_API void __write(::IceInternal::BasicStream*, const IceBoxDescriptorPtr&);
ICE_GRID_API void __patch__IceBoxDescriptorPtr(void*, ::Ice::ObjectPtr&);

ICE_GRID_API void __addObject(const IceBoxDescriptorPtr&, ::IceInternal::GCCountMap&);
ICE_GRID_API bool __usesClasses(const IceBoxDescriptorPtr&);
ICE_GRID_API void __decRefUnsafe(const IceBoxDescriptorPtr&);
ICE_GRID_API void __clearHandleUnsafe(IceBoxDescriptorPtr&);

typedef ::IceInternal::Handle< ::IceGrid::LoadBalancingPolicy> LoadBalancingPolicyPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::IceGrid::LoadBalancingPolicy> LoadBalancingPolicyPrx;

ICE_GRID_API void __write(::IceInternal::BasicStream*, const LoadBalancingPolicyPrx&);
ICE_GRID_API void __read(::IceInternal::BasicStream*, LoadBalancingPolicyPrx&);
ICE_GRID_API void __write(::IceInternal::BasicStream*, const LoadBalancingPolicyPtr&);
ICE_GRID_API void __patch__LoadBalancingPolicyPtr(void*, ::Ice::ObjectPtr&);

ICE_GRID_API void __addObject(const LoadBalancingPolicyPtr&, ::IceInternal::GCCountMap&);
ICE_GRID_API bool __usesClasses(const LoadBalancingPolicyPtr&);
ICE_GRID_API void __decRefUnsafe(const LoadBalancingPolicyPtr&);
ICE_GRID_API void __clearHandleUnsafe(LoadBalancingPolicyPtr&);

typedef ::IceInternal::Handle< ::IceGrid::RandomLoadBalancingPolicy> RandomLoadBalancingPolicyPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::IceGrid::RandomLoadBalancingPolicy> RandomLoadBalancingPolicyPrx;

ICE_GRID_API void __write(::IceInternal::BasicStream*, const RandomLoadBalancingPolicyPrx&);
ICE_GRID_API void __read(::IceInternal::BasicStream*, RandomLoadBalancingPolicyPrx&);
ICE_GRID_API void __write(::IceInternal::BasicStream*, const RandomLoadBalancingPolicyPtr&);
ICE_GRID_API void __patch__RandomLoadBalancingPolicyPtr(void*, ::Ice::ObjectPtr&);

ICE_GRID_API void __addObject(const RandomLoadBalancingPolicyPtr&, ::IceInternal::GCCountMap&);
ICE_GRID_API bool __usesClasses(const RandomLoadBalancingPolicyPtr&);
ICE_GRID_API void __decRefUnsafe(const RandomLoadBalancingPolicyPtr&);
ICE_GRID_API void __clearHandleUnsafe(RandomLoadBalancingPolicyPtr&);

typedef ::IceInternal::Handle< ::IceGrid::OrderedLoadBalancingPolicy> OrderedLoadBalancingPolicyPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::IceGrid::OrderedLoadBalancingPolicy> OrderedLoadBalancingPolicyPrx;

ICE_GRID_API void __write(::IceInternal::BasicStream*, const OrderedLoadBalancingPolicyPrx&);
ICE_GRID_API void __read(::IceInternal::BasicStream*, OrderedLoadBalancingPolicyPrx&);
ICE_GRID_API void __write(::IceInternal::BasicStream*, const OrderedLoadBalancingPolicyPtr&);
ICE_GRID_API void __patch__OrderedLoadBalancingPolicyPtr(void*, ::Ice::ObjectPtr&);

ICE_GRID_API void __addObject(const OrderedLoadBalancingPolicyPtr&, ::IceInternal::GCCountMap&);
ICE_GRID_API bool __usesClasses(const OrderedLoadBalancingPolicyPtr&);
ICE_GRID_API void __decRefUnsafe(const OrderedLoadBalancingPolicyPtr&);
ICE_GRID_API void __clearHandleUnsafe(OrderedLoadBalancingPolicyPtr&);

typedef ::IceInternal::Handle< ::IceGrid::RoundRobinLoadBalancingPolicy> RoundRobinLoadBalancingPolicyPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::IceGrid::RoundRobinLoadBalancingPolicy> RoundRobinLoadBalancingPolicyPrx;

ICE_GRID_API void __write(::IceInternal::BasicStream*, const RoundRobinLoadBalancingPolicyPrx&);
ICE_GRID_API void __read(::IceInternal::BasicStream*, RoundRobinLoadBalancingPolicyPrx&);
ICE_GRID_API void __write(::IceInternal::BasicStream*, const RoundRobinLoadBalancingPolicyPtr&);
ICE_GRID_API void __patch__RoundRobinLoadBalancingPolicyPtr(void*, ::Ice::ObjectPtr&);

ICE_GRID_API void __addObject(const RoundRobinLoadBalancingPolicyPtr&, ::IceInternal::GCCountMap&);
ICE_GRID_API bool __usesClasses(const RoundRobinLoadBalancingPolicyPtr&);
ICE_GRID_API void __decRefUnsafe(const RoundRobinLoadBalancingPolicyPtr&);
ICE_GRID_API void __clearHandleUnsafe(RoundRobinLoadBalancingPolicyPtr&);

typedef ::IceInternal::Handle< ::IceGrid::AdaptiveLoadBalancingPolicy> AdaptiveLoadBalancingPolicyPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::IceGrid::AdaptiveLoadBalancingPolicy> AdaptiveLoadBalancingPolicyPrx;

ICE_GRID_API void __write(::IceInternal::BasicStream*, const AdaptiveLoadBalancingPolicyPrx&);
ICE_GRID_API void __read(::IceInternal::BasicStream*, AdaptiveLoadBalancingPolicyPrx&);
ICE_GRID_API void __write(::IceInternal::BasicStream*, const AdaptiveLoadBalancingPolicyPtr&);
ICE_GRID_API void __patch__AdaptiveLoadBalancingPolicyPtr(void*, ::Ice::ObjectPtr&);

ICE_GRID_API void __addObject(const AdaptiveLoadBalancingPolicyPtr&, ::IceInternal::GCCountMap&);
ICE_GRID_API bool __usesClasses(const AdaptiveLoadBalancingPolicyPtr&);
ICE_GRID_API void __decRefUnsafe(const AdaptiveLoadBalancingPolicyPtr&);
ICE_GRID_API void __clearHandleUnsafe(AdaptiveLoadBalancingPolicyPtr&);

typedef ::IceInternal::Handle< ::IceGrid::BoxedString> BoxedStringPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::IceGrid::BoxedString> BoxedStringPrx;

ICE_GRID_API void __write(::IceInternal::BasicStream*, const BoxedStringPrx&);
ICE_GRID_API void __read(::IceInternal::BasicStream*, BoxedStringPrx&);
ICE_GRID_API void __write(::IceInternal::BasicStream*, const BoxedStringPtr&);
ICE_GRID_API void __patch__BoxedStringPtr(void*, ::Ice::ObjectPtr&);

ICE_GRID_API void __addObject(const BoxedStringPtr&, ::IceInternal::GCCountMap&);
ICE_GRID_API bool __usesClasses(const BoxedStringPtr&);
ICE_GRID_API void __decRefUnsafe(const BoxedStringPtr&);
ICE_GRID_API void __clearHandleUnsafe(BoxedStringPtr&);

typedef ::IceInternal::Handle< ::IceGrid::BoxedDistributionDescriptor> BoxedDistributionDescriptorPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::IceGrid::BoxedDistributionDescriptor> BoxedDistributionDescriptorPrx;

ICE_GRID_API void __write(::IceInternal::BasicStream*, const BoxedDistributionDescriptorPrx&);
ICE_GRID_API void __read(::IceInternal::BasicStream*, BoxedDistributionDescriptorPrx&);
ICE_GRID_API void __write(::IceInternal::BasicStream*, const BoxedDistributionDescriptorPtr&);
ICE_GRID_API void __patch__BoxedDistributionDescriptorPtr(void*, ::Ice::ObjectPtr&);

ICE_GRID_API void __addObject(const BoxedDistributionDescriptorPtr&, ::IceInternal::GCCountMap&);
ICE_GRID_API bool __usesClasses(const BoxedDistributionDescriptorPtr&);
ICE_GRID_API void __decRefUnsafe(const BoxedDistributionDescriptorPtr&);
ICE_GRID_API void __clearHandleUnsafe(BoxedDistributionDescriptorPtr&);

}

namespace IceGrid
{

typedef ::std::map< ::std::string, ::std::string> StringStringDict;

class __U__StringStringDict { };
ICE_GRID_API void __write(::IceInternal::BasicStream*, const StringStringDict&, __U__StringStringDict);
ICE_GRID_API void __read(::IceInternal::BasicStream*, StringStringDict&, __U__StringStringDict);

struct PropertyDescriptor
{
    ::std::string name;
    ::std::string value;

    ICE_GRID_API bool operator==(const PropertyDescriptor&) const;
    ICE_GRID_API bool operator!=(const PropertyDescriptor&) const;
    ICE_GRID_API bool operator<(const PropertyDescriptor&) const;
    ICE_GRID_API bool operator<=(const PropertyDescriptor& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    ICE_GRID_API bool operator>(const PropertyDescriptor& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    ICE_GRID_API bool operator>=(const PropertyDescriptor& __rhs) const
    {
        return !operator<(__rhs);
    }

    ICE_GRID_API void __write(::IceInternal::BasicStream*) const;
    ICE_GRID_API void __read(::IceInternal::BasicStream*);
};

typedef ::std::vector< ::IceGrid::PropertyDescriptor> PropertyDescriptorSeq;

class __U__PropertyDescriptorSeq { };
ICE_GRID_API void __write(::IceInternal::BasicStream*, const ::IceGrid::PropertyDescriptor*, const ::IceGrid::PropertyDescriptor*, __U__PropertyDescriptorSeq);
ICE_GRID_API void __read(::IceInternal::BasicStream*, PropertyDescriptorSeq&, __U__PropertyDescriptorSeq);

struct PropertySetDescriptor
{
    ::Ice::StringSeq references;
    ::IceGrid::PropertyDescriptorSeq properties;

    ICE_GRID_API bool operator==(const PropertySetDescriptor&) const;
    ICE_GRID_API bool operator!=(const PropertySetDescriptor&) const;
    ICE_GRID_API bool operator<(const PropertySetDescriptor&) const;
    ICE_GRID_API bool operator<=(const PropertySetDescriptor& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    ICE_GRID_API bool operator>(const PropertySetDescriptor& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    ICE_GRID_API bool operator>=(const PropertySetDescriptor& __rhs) const
    {
        return !operator<(__rhs);
    }

    ICE_GRID_API void __write(::IceInternal::BasicStream*) const;
    ICE_GRID_API void __read(::IceInternal::BasicStream*);
};

typedef ::std::map< ::std::string, ::IceGrid::PropertySetDescriptor> PropertySetDescriptorDict;

class __U__PropertySetDescriptorDict { };
ICE_GRID_API void __write(::IceInternal::BasicStream*, const PropertySetDescriptorDict&, __U__PropertySetDescriptorDict);
ICE_GRID_API void __read(::IceInternal::BasicStream*, PropertySetDescriptorDict&, __U__PropertySetDescriptorDict);

struct ObjectDescriptor
{
    ::Ice::Identity id;
    ::std::string type;

    ICE_GRID_API bool operator==(const ObjectDescriptor&) const;
    ICE_GRID_API bool operator!=(const ObjectDescriptor&) const;
    ICE_GRID_API bool operator<(const ObjectDescriptor&) const;
    ICE_GRID_API bool operator<=(const ObjectDescriptor& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    ICE_GRID_API bool operator>(const ObjectDescriptor& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    ICE_GRID_API bool operator>=(const ObjectDescriptor& __rhs) const
    {
        return !operator<(__rhs);
    }

    ICE_GRID_API void __write(::IceInternal::BasicStream*) const;
    ICE_GRID_API void __read(::IceInternal::BasicStream*);
};

typedef ::std::vector< ::IceGrid::ObjectDescriptor> ObjectDescriptorSeq;

class __U__ObjectDescriptorSeq { };
ICE_GRID_API void __write(::IceInternal::BasicStream*, const ::IceGrid::ObjectDescriptor*, const ::IceGrid::ObjectDescriptor*, __U__ObjectDescriptorSeq);
ICE_GRID_API void __read(::IceInternal::BasicStream*, ObjectDescriptorSeq&, __U__ObjectDescriptorSeq);

struct AdapterDescriptor
{
    ::std::string name;
    ::std::string description;
    ::std::string id;
    ::std::string replicaGroupId;
    ::std::string priority;
    bool registerProcess;
    bool serverLifetime;
    ::IceGrid::ObjectDescriptorSeq objects;
    ::IceGrid::ObjectDescriptorSeq allocatables;

    ICE_GRID_API bool operator==(const AdapterDescriptor&) const;
    ICE_GRID_API bool operator!=(const AdapterDescriptor&) const;
    ICE_GRID_API bool operator<(const AdapterDescriptor&) const;
    ICE_GRID_API bool operator<=(const AdapterDescriptor& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    ICE_GRID_API bool operator>(const AdapterDescriptor& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    ICE_GRID_API bool operator>=(const AdapterDescriptor& __rhs) const
    {
        return !operator<(__rhs);
    }

    ICE_GRID_API void __write(::IceInternal::BasicStream*) const;
    ICE_GRID_API void __read(::IceInternal::BasicStream*);
};

typedef ::std::vector< ::IceGrid::AdapterDescriptor> AdapterDescriptorSeq;

class __U__AdapterDescriptorSeq { };
ICE_GRID_API void __write(::IceInternal::BasicStream*, const ::IceGrid::AdapterDescriptor*, const ::IceGrid::AdapterDescriptor*, __U__AdapterDescriptorSeq);
ICE_GRID_API void __read(::IceInternal::BasicStream*, AdapterDescriptorSeq&, __U__AdapterDescriptorSeq);

struct DbEnvDescriptor
{
    ::std::string name;
    ::std::string description;
    ::std::string dbHome;
    ::IceGrid::PropertyDescriptorSeq properties;

    ICE_GRID_API bool operator==(const DbEnvDescriptor&) const;
    ICE_GRID_API bool operator!=(const DbEnvDescriptor&) const;
    ICE_GRID_API bool operator<(const DbEnvDescriptor&) const;
    ICE_GRID_API bool operator<=(const DbEnvDescriptor& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    ICE_GRID_API bool operator>(const DbEnvDescriptor& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    ICE_GRID_API bool operator>=(const DbEnvDescriptor& __rhs) const
    {
        return !operator<(__rhs);
    }

    ICE_GRID_API void __write(::IceInternal::BasicStream*) const;
    ICE_GRID_API void __read(::IceInternal::BasicStream*);
};

typedef ::std::vector< ::IceGrid::DbEnvDescriptor> DbEnvDescriptorSeq;

class __U__DbEnvDescriptorSeq { };
ICE_GRID_API void __write(::IceInternal::BasicStream*, const ::IceGrid::DbEnvDescriptor*, const ::IceGrid::DbEnvDescriptor*, __U__DbEnvDescriptorSeq);
ICE_GRID_API void __read(::IceInternal::BasicStream*, DbEnvDescriptorSeq&, __U__DbEnvDescriptorSeq);

struct DistributionDescriptor
{
    ::std::string icepatch;
    ::Ice::StringSeq directories;

    ICE_GRID_API bool operator==(const DistributionDescriptor&) const;
    ICE_GRID_API bool operator!=(const DistributionDescriptor&) const;
    ICE_GRID_API bool operator<(const DistributionDescriptor&) const;
    ICE_GRID_API bool operator<=(const DistributionDescriptor& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    ICE_GRID_API bool operator>(const DistributionDescriptor& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    ICE_GRID_API bool operator>=(const DistributionDescriptor& __rhs) const
    {
        return !operator<(__rhs);
    }

    ICE_GRID_API void __write(::IceInternal::BasicStream*) const;
    ICE_GRID_API void __read(::IceInternal::BasicStream*);
};

typedef ::std::vector< ::IceGrid::ServerDescriptorPtr> ServerDescriptorSeq;

class __U__ServerDescriptorSeq { };
ICE_GRID_API void __write(::IceInternal::BasicStream*, const ::IceGrid::ServerDescriptorPtr*, const ::IceGrid::ServerDescriptorPtr*, __U__ServerDescriptorSeq);
ICE_GRID_API void __read(::IceInternal::BasicStream*, ServerDescriptorSeq&, __U__ServerDescriptorSeq);

typedef ::std::vector< ::IceGrid::ServiceDescriptorPtr> ServiceDescriptorSeq;

class __U__ServiceDescriptorSeq { };
ICE_GRID_API void __write(::IceInternal::BasicStream*, const ::IceGrid::ServiceDescriptorPtr*, const ::IceGrid::ServiceDescriptorPtr*, __U__ServiceDescriptorSeq);
ICE_GRID_API void __read(::IceInternal::BasicStream*, ServiceDescriptorSeq&, __U__ServiceDescriptorSeq);

struct ServerInstanceDescriptor
{
    ::std::string _cpp_template;
    ::IceGrid::StringStringDict parameterValues;
    ::IceGrid::PropertySetDescriptor propertySet;
    ::IceGrid::PropertySetDescriptorDict servicePropertySets;

    ICE_GRID_API bool operator==(const ServerInstanceDescriptor&) const;
    ICE_GRID_API bool operator!=(const ServerInstanceDescriptor&) const;
    ICE_GRID_API bool operator<(const ServerInstanceDescriptor&) const;
    ICE_GRID_API bool operator<=(const ServerInstanceDescriptor& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    ICE_GRID_API bool operator>(const ServerInstanceDescriptor& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    ICE_GRID_API bool operator>=(const ServerInstanceDescriptor& __rhs) const
    {
        return !operator<(__rhs);
    }

    ICE_GRID_API void __write(::IceInternal::BasicStream*) const;
    ICE_GRID_API void __read(::IceInternal::BasicStream*);
};

typedef ::std::vector< ::IceGrid::ServerInstanceDescriptor> ServerInstanceDescriptorSeq;

class __U__ServerInstanceDescriptorSeq { };
ICE_GRID_API void __write(::IceInternal::BasicStream*, const ::IceGrid::ServerInstanceDescriptor*, const ::IceGrid::ServerInstanceDescriptor*, __U__ServerInstanceDescriptorSeq);
ICE_GRID_API void __read(::IceInternal::BasicStream*, ServerInstanceDescriptorSeq&, __U__ServerInstanceDescriptorSeq);

struct TemplateDescriptor
{
    ::IceGrid::CommunicatorDescriptorPtr descriptor;
    ::Ice::StringSeq parameters;
    ::IceGrid::StringStringDict parameterDefaults;

    ICE_GRID_API bool operator==(const TemplateDescriptor&) const;
    ICE_GRID_API bool operator!=(const TemplateDescriptor&) const;
    ICE_GRID_API bool operator<(const TemplateDescriptor&) const;
    ICE_GRID_API bool operator<=(const TemplateDescriptor& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    ICE_GRID_API bool operator>(const TemplateDescriptor& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    ICE_GRID_API bool operator>=(const TemplateDescriptor& __rhs) const
    {
        return !operator<(__rhs);
    }

    ICE_GRID_API void __write(::IceInternal::BasicStream*) const;
    ICE_GRID_API void __read(::IceInternal::BasicStream*);
};

typedef ::std::map< ::std::string, ::IceGrid::TemplateDescriptor> TemplateDescriptorDict;

class __U__TemplateDescriptorDict { };
ICE_GRID_API void __write(::IceInternal::BasicStream*, const TemplateDescriptorDict&, __U__TemplateDescriptorDict);
ICE_GRID_API void __read(::IceInternal::BasicStream*, TemplateDescriptorDict&, __U__TemplateDescriptorDict);

struct ServiceInstanceDescriptor
{
    ::std::string _cpp_template;
    ::IceGrid::StringStringDict parameterValues;
    ::IceGrid::ServiceDescriptorPtr descriptor;
    ::IceGrid::PropertySetDescriptor propertySet;

    ICE_GRID_API bool operator==(const ServiceInstanceDescriptor&) const;
    ICE_GRID_API bool operator!=(const ServiceInstanceDescriptor&) const;
    ICE_GRID_API bool operator<(const ServiceInstanceDescriptor&) const;
    ICE_GRID_API bool operator<=(const ServiceInstanceDescriptor& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    ICE_GRID_API bool operator>(const ServiceInstanceDescriptor& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    ICE_GRID_API bool operator>=(const ServiceInstanceDescriptor& __rhs) const
    {
        return !operator<(__rhs);
    }

    ICE_GRID_API void __write(::IceInternal::BasicStream*) const;
    ICE_GRID_API void __read(::IceInternal::BasicStream*);
};

typedef ::std::vector< ::IceGrid::ServiceInstanceDescriptor> ServiceInstanceDescriptorSeq;

class __U__ServiceInstanceDescriptorSeq { };
ICE_GRID_API void __write(::IceInternal::BasicStream*, const ::IceGrid::ServiceInstanceDescriptor*, const ::IceGrid::ServiceInstanceDescriptor*, __U__ServiceInstanceDescriptorSeq);
ICE_GRID_API void __read(::IceInternal::BasicStream*, ServiceInstanceDescriptorSeq&, __U__ServiceInstanceDescriptorSeq);

struct NodeDescriptor
{
    ::IceGrid::StringStringDict variables;
    ::IceGrid::ServerInstanceDescriptorSeq serverInstances;
    ::IceGrid::ServerDescriptorSeq servers;
    ::std::string loadFactor;
    ::std::string description;
    ::IceGrid::PropertySetDescriptorDict propertySets;

    ICE_GRID_API bool operator==(const NodeDescriptor&) const;
    ICE_GRID_API bool operator!=(const NodeDescriptor&) const;
    ICE_GRID_API bool operator<(const NodeDescriptor&) const;
    ICE_GRID_API bool operator<=(const NodeDescriptor& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    ICE_GRID_API bool operator>(const NodeDescriptor& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    ICE_GRID_API bool operator>=(const NodeDescriptor& __rhs) const
    {
        return !operator<(__rhs);
    }

    ICE_GRID_API void __write(::IceInternal::BasicStream*) const;
    ICE_GRID_API void __read(::IceInternal::BasicStream*);
};

typedef ::std::map< ::std::string, ::IceGrid::NodeDescriptor> NodeDescriptorDict;

class __U__NodeDescriptorDict { };
ICE_GRID_API void __write(::IceInternal::BasicStream*, const NodeDescriptorDict&, __U__NodeDescriptorDict);
ICE_GRID_API void __read(::IceInternal::BasicStream*, NodeDescriptorDict&, __U__NodeDescriptorDict);

struct ReplicaGroupDescriptor
{
    ::std::string id;
    ::IceGrid::LoadBalancingPolicyPtr loadBalancing;
    ::IceGrid::ObjectDescriptorSeq objects;
    ::std::string description;

    ICE_GRID_API bool operator==(const ReplicaGroupDescriptor&) const;
    ICE_GRID_API bool operator!=(const ReplicaGroupDescriptor&) const;
    ICE_GRID_API bool operator<(const ReplicaGroupDescriptor&) const;
    ICE_GRID_API bool operator<=(const ReplicaGroupDescriptor& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    ICE_GRID_API bool operator>(const ReplicaGroupDescriptor& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    ICE_GRID_API bool operator>=(const ReplicaGroupDescriptor& __rhs) const
    {
        return !operator<(__rhs);
    }

    ICE_GRID_API void __write(::IceInternal::BasicStream*) const;
    ICE_GRID_API void __read(::IceInternal::BasicStream*);
};

typedef ::std::vector< ::IceGrid::ReplicaGroupDescriptor> ReplicaGroupDescriptorSeq;

class __U__ReplicaGroupDescriptorSeq { };
ICE_GRID_API void __write(::IceInternal::BasicStream*, const ::IceGrid::ReplicaGroupDescriptor*, const ::IceGrid::ReplicaGroupDescriptor*, __U__ReplicaGroupDescriptorSeq);
ICE_GRID_API void __read(::IceInternal::BasicStream*, ReplicaGroupDescriptorSeq&, __U__ReplicaGroupDescriptorSeq);

struct ApplicationDescriptor
{
    ::std::string name;
    ::IceGrid::StringStringDict variables;
    ::IceGrid::ReplicaGroupDescriptorSeq replicaGroups;
    ::IceGrid::TemplateDescriptorDict serverTemplates;
    ::IceGrid::TemplateDescriptorDict serviceTemplates;
    ::IceGrid::NodeDescriptorDict nodes;
    ::IceGrid::DistributionDescriptor distrib;
    ::std::string description;
    ::IceGrid::PropertySetDescriptorDict propertySets;

    ICE_GRID_API bool operator==(const ApplicationDescriptor&) const;
    ICE_GRID_API bool operator!=(const ApplicationDescriptor&) const;
    ICE_GRID_API bool operator<(const ApplicationDescriptor&) const;
    ICE_GRID_API bool operator<=(const ApplicationDescriptor& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    ICE_GRID_API bool operator>(const ApplicationDescriptor& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    ICE_GRID_API bool operator>=(const ApplicationDescriptor& __rhs) const
    {
        return !operator<(__rhs);
    }

    ICE_GRID_API void __write(::IceInternal::BasicStream*) const;
    ICE_GRID_API void __read(::IceInternal::BasicStream*);
};

typedef ::std::vector< ::IceGrid::ApplicationDescriptor> ApplicationDescriptorSeq;

class __U__ApplicationDescriptorSeq { };
ICE_GRID_API void __write(::IceInternal::BasicStream*, const ::IceGrid::ApplicationDescriptor*, const ::IceGrid::ApplicationDescriptor*, __U__ApplicationDescriptorSeq);
ICE_GRID_API void __read(::IceInternal::BasicStream*, ApplicationDescriptorSeq&, __U__ApplicationDescriptorSeq);

struct NodeUpdateDescriptor
{
    ::std::string name;
    ::IceGrid::BoxedStringPtr description;
    ::IceGrid::StringStringDict variables;
    ::Ice::StringSeq removeVariables;
    ::IceGrid::PropertySetDescriptorDict propertySets;
    ::Ice::StringSeq removePropertySets;
    ::IceGrid::ServerInstanceDescriptorSeq serverInstances;
    ::IceGrid::ServerDescriptorSeq servers;
    ::Ice::StringSeq removeServers;
    ::IceGrid::BoxedStringPtr loadFactor;

    ICE_GRID_API bool operator==(const NodeUpdateDescriptor&) const;
    ICE_GRID_API bool operator!=(const NodeUpdateDescriptor&) const;
    ICE_GRID_API bool operator<(const NodeUpdateDescriptor&) const;
    ICE_GRID_API bool operator<=(const NodeUpdateDescriptor& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    ICE_GRID_API bool operator>(const NodeUpdateDescriptor& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    ICE_GRID_API bool operator>=(const NodeUpdateDescriptor& __rhs) const
    {
        return !operator<(__rhs);
    }

    ICE_GRID_API void __write(::IceInternal::BasicStream*) const;
    ICE_GRID_API void __read(::IceInternal::BasicStream*);
};

typedef ::std::vector< ::IceGrid::NodeUpdateDescriptor> NodeUpdateDescriptorSeq;

class __U__NodeUpdateDescriptorSeq { };
ICE_GRID_API void __write(::IceInternal::BasicStream*, const ::IceGrid::NodeUpdateDescriptor*, const ::IceGrid::NodeUpdateDescriptor*, __U__NodeUpdateDescriptorSeq);
ICE_GRID_API void __read(::IceInternal::BasicStream*, NodeUpdateDescriptorSeq&, __U__NodeUpdateDescriptorSeq);

struct ApplicationUpdateDescriptor
{
    ::std::string name;
    ::IceGrid::BoxedStringPtr description;
    ::IceGrid::BoxedDistributionDescriptorPtr distrib;
    ::IceGrid::StringStringDict variables;
    ::Ice::StringSeq removeVariables;
    ::IceGrid::PropertySetDescriptorDict propertySets;
    ::Ice::StringSeq removePropertySets;
    ::IceGrid::ReplicaGroupDescriptorSeq replicaGroups;
    ::Ice::StringSeq removeReplicaGroups;
    ::IceGrid::TemplateDescriptorDict serverTemplates;
    ::Ice::StringSeq removeServerTemplates;
    ::IceGrid::TemplateDescriptorDict serviceTemplates;
    ::Ice::StringSeq removeServiceTemplates;
    ::IceGrid::NodeUpdateDescriptorSeq nodes;
    ::Ice::StringSeq removeNodes;

    ICE_GRID_API bool operator==(const ApplicationUpdateDescriptor&) const;
    ICE_GRID_API bool operator!=(const ApplicationUpdateDescriptor&) const;
    ICE_GRID_API bool operator<(const ApplicationUpdateDescriptor&) const;
    ICE_GRID_API bool operator<=(const ApplicationUpdateDescriptor& __rhs) const
    {
        return operator<(__rhs) || operator==(__rhs);
    }
    ICE_GRID_API bool operator>(const ApplicationUpdateDescriptor& __rhs) const
    {
        return !operator<(__rhs) && !operator==(__rhs);
    }
    ICE_GRID_API bool operator>=(const ApplicationUpdateDescriptor& __rhs) const
    {
        return !operator<(__rhs);
    }

    ICE_GRID_API void __write(::IceInternal::BasicStream*) const;
    ICE_GRID_API void __read(::IceInternal::BasicStream*);
};

}

namespace IceProxy
{

namespace IceGrid
{

class ICE_GRID_API CommunicatorDescriptor : virtual public ::IceProxy::Ice::Object
{
public:
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

class ICE_GRID_API ServerDescriptor : virtual public ::IceProxy::IceGrid::CommunicatorDescriptor
{
public:
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

class ICE_GRID_API ServiceDescriptor : virtual public ::IceProxy::IceGrid::CommunicatorDescriptor
{
public:
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

class ICE_GRID_API IceBoxDescriptor : virtual public ::IceProxy::IceGrid::ServerDescriptor
{
public:
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

class ICE_GRID_API LoadBalancingPolicy : virtual public ::IceProxy::Ice::Object
{
public:
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

class ICE_GRID_API RandomLoadBalancingPolicy : virtual public ::IceProxy::IceGrid::LoadBalancingPolicy
{
public:
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

class ICE_GRID_API OrderedLoadBalancingPolicy : virtual public ::IceProxy::IceGrid::LoadBalancingPolicy
{
public:
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

class ICE_GRID_API RoundRobinLoadBalancingPolicy : virtual public ::IceProxy::IceGrid::LoadBalancingPolicy
{
public:
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

class ICE_GRID_API AdaptiveLoadBalancingPolicy : virtual public ::IceProxy::IceGrid::LoadBalancingPolicy
{
public:
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

class ICE_GRID_API BoxedString : virtual public ::IceProxy::Ice::Object
{
public:
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

class ICE_GRID_API BoxedDistributionDescriptor : virtual public ::IceProxy::Ice::Object
{
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

class ICE_GRID_API CommunicatorDescriptor : virtual public ::IceDelegate::Ice::Object
{
public:
};

class ICE_GRID_API ServerDescriptor : virtual public ::IceDelegate::IceGrid::CommunicatorDescriptor
{
public:
};

class ICE_GRID_API ServiceDescriptor : virtual public ::IceDelegate::IceGrid::CommunicatorDescriptor
{
public:
};

class ICE_GRID_API IceBoxDescriptor : virtual public ::IceDelegate::IceGrid::ServerDescriptor
{
public:
};

class ICE_GRID_API LoadBalancingPolicy : virtual public ::IceDelegate::Ice::Object
{
public:
};

class ICE_GRID_API RandomLoadBalancingPolicy : virtual public ::IceDelegate::IceGrid::LoadBalancingPolicy
{
public:
};

class ICE_GRID_API OrderedLoadBalancingPolicy : virtual public ::IceDelegate::IceGrid::LoadBalancingPolicy
{
public:
};

class ICE_GRID_API RoundRobinLoadBalancingPolicy : virtual public ::IceDelegate::IceGrid::LoadBalancingPolicy
{
public:
};

class ICE_GRID_API AdaptiveLoadBalancingPolicy : virtual public ::IceDelegate::IceGrid::LoadBalancingPolicy
{
public:
};

class ICE_GRID_API BoxedString : virtual public ::IceDelegate::Ice::Object
{
public:
};

class ICE_GRID_API BoxedDistributionDescriptor : virtual public ::IceDelegate::Ice::Object
{
public:
};

}

}

namespace IceDelegateM
{

namespace IceGrid
{

class ICE_GRID_API CommunicatorDescriptor : virtual public ::IceDelegate::IceGrid::CommunicatorDescriptor,
                                            virtual public ::IceDelegateM::Ice::Object
{
public:
};

class ICE_GRID_API ServerDescriptor : virtual public ::IceDelegate::IceGrid::ServerDescriptor,
                                      virtual public ::IceDelegateM::IceGrid::CommunicatorDescriptor
{
public:
};

class ICE_GRID_API ServiceDescriptor : virtual public ::IceDelegate::IceGrid::ServiceDescriptor,
                                       virtual public ::IceDelegateM::IceGrid::CommunicatorDescriptor
{
public:
};

class ICE_GRID_API IceBoxDescriptor : virtual public ::IceDelegate::IceGrid::IceBoxDescriptor,
                                      virtual public ::IceDelegateM::IceGrid::ServerDescriptor
{
public:
};

class ICE_GRID_API LoadBalancingPolicy : virtual public ::IceDelegate::IceGrid::LoadBalancingPolicy,
                                         virtual public ::IceDelegateM::Ice::Object
{
public:
};

class ICE_GRID_API RandomLoadBalancingPolicy : virtual public ::IceDelegate::IceGrid::RandomLoadBalancingPolicy,
                                               virtual public ::IceDelegateM::IceGrid::LoadBalancingPolicy
{
public:
};

class ICE_GRID_API OrderedLoadBalancingPolicy : virtual public ::IceDelegate::IceGrid::OrderedLoadBalancingPolicy,
                                                virtual public ::IceDelegateM::IceGrid::LoadBalancingPolicy
{
public:
};

class ICE_GRID_API RoundRobinLoadBalancingPolicy : virtual public ::IceDelegate::IceGrid::RoundRobinLoadBalancingPolicy,
                                                   virtual public ::IceDelegateM::IceGrid::LoadBalancingPolicy
{
public:
};

class ICE_GRID_API AdaptiveLoadBalancingPolicy : virtual public ::IceDelegate::IceGrid::AdaptiveLoadBalancingPolicy,
                                                 virtual public ::IceDelegateM::IceGrid::LoadBalancingPolicy
{
public:
};

class ICE_GRID_API BoxedString : virtual public ::IceDelegate::IceGrid::BoxedString,
                                 virtual public ::IceDelegateM::Ice::Object
{
public:
};

class ICE_GRID_API BoxedDistributionDescriptor : virtual public ::IceDelegate::IceGrid::BoxedDistributionDescriptor,
                                                 virtual public ::IceDelegateM::Ice::Object
{
public:
};

}

}

namespace IceDelegateD
{

namespace IceGrid
{

class ICE_GRID_API CommunicatorDescriptor : virtual public ::IceDelegate::IceGrid::CommunicatorDescriptor,
                                            virtual public ::IceDelegateD::Ice::Object
{
public:
};

class ICE_GRID_API ServerDescriptor : virtual public ::IceDelegate::IceGrid::ServerDescriptor,
                                      virtual public ::IceDelegateD::IceGrid::CommunicatorDescriptor
{
public:
};

class ICE_GRID_API ServiceDescriptor : virtual public ::IceDelegate::IceGrid::ServiceDescriptor,
                                       virtual public ::IceDelegateD::IceGrid::CommunicatorDescriptor
{
public:
};

class ICE_GRID_API IceBoxDescriptor : virtual public ::IceDelegate::IceGrid::IceBoxDescriptor,
                                      virtual public ::IceDelegateD::IceGrid::ServerDescriptor
{
public:
};

class ICE_GRID_API LoadBalancingPolicy : virtual public ::IceDelegate::IceGrid::LoadBalancingPolicy,
                                         virtual public ::IceDelegateD::Ice::Object
{
public:
};

class ICE_GRID_API RandomLoadBalancingPolicy : virtual public ::IceDelegate::IceGrid::RandomLoadBalancingPolicy,
                                               virtual public ::IceDelegateD::IceGrid::LoadBalancingPolicy
{
public:
};

class ICE_GRID_API OrderedLoadBalancingPolicy : virtual public ::IceDelegate::IceGrid::OrderedLoadBalancingPolicy,
                                                virtual public ::IceDelegateD::IceGrid::LoadBalancingPolicy
{
public:
};

class ICE_GRID_API RoundRobinLoadBalancingPolicy : virtual public ::IceDelegate::IceGrid::RoundRobinLoadBalancingPolicy,
                                                   virtual public ::IceDelegateD::IceGrid::LoadBalancingPolicy
{
public:
};

class ICE_GRID_API AdaptiveLoadBalancingPolicy : virtual public ::IceDelegate::IceGrid::AdaptiveLoadBalancingPolicy,
                                                 virtual public ::IceDelegateD::IceGrid::LoadBalancingPolicy
{
public:
};

class ICE_GRID_API BoxedString : virtual public ::IceDelegate::IceGrid::BoxedString,
                                 virtual public ::IceDelegateD::Ice::Object
{
public:
};

class ICE_GRID_API BoxedDistributionDescriptor : virtual public ::IceDelegate::IceGrid::BoxedDistributionDescriptor,
                                                 virtual public ::IceDelegateD::Ice::Object
{
public:
};

}

}

namespace IceGrid
{

class ICE_GRID_API CommunicatorDescriptor : virtual public ::Ice::Object
{
public:

    typedef CommunicatorDescriptorPrx ProxyType;
    typedef CommunicatorDescriptorPtr PointerType;
    
    CommunicatorDescriptor() {}
    CommunicatorDescriptor(const ::IceGrid::AdapterDescriptorSeq&, const ::IceGrid::PropertySetDescriptor&, const ::IceGrid::DbEnvDescriptorSeq&, const ::Ice::StringSeq&, const ::std::string&);
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();


    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);

    static const ::Ice::ObjectFactoryPtr& ice_factory();

protected:

    virtual ~CommunicatorDescriptor() {}

    friend class CommunicatorDescriptor__staticInit;

public:

    ::IceGrid::AdapterDescriptorSeq adapters;

    ::IceGrid::PropertySetDescriptor propertySet;

    ::IceGrid::DbEnvDescriptorSeq dbEnvs;

    ::Ice::StringSeq logs;

    ::std::string description;
};

class CommunicatorDescriptor__staticInit
{
public:

    ::IceGrid::CommunicatorDescriptor _init;
};

static ::IceGrid::CommunicatorDescriptor__staticInit _CommunicatorDescriptor_init;

void ICE_GRID_API __patch__CommunicatorDescriptorPtr(void*, ::Ice::ObjectPtr&);

class ICE_GRID_API ServerDescriptor : virtual public ::IceGrid::CommunicatorDescriptor
{
public:

    typedef ServerDescriptorPrx ProxyType;
    typedef ServerDescriptorPtr PointerType;
    
    ServerDescriptor() {}
    ServerDescriptor(const ::IceGrid::AdapterDescriptorSeq&, const ::IceGrid::PropertySetDescriptor&, const ::IceGrid::DbEnvDescriptorSeq&, const ::Ice::StringSeq&, const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::StringSeq&, const ::Ice::StringSeq&, const ::std::string&, const ::std::string&, const ::std::string&, bool, const ::IceGrid::DistributionDescriptor&, bool, const ::std::string&);
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();


    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);

    static const ::Ice::ObjectFactoryPtr& ice_factory();

protected:

    virtual ~ServerDescriptor() {}

public:

    ::std::string id;

    ::std::string exe;

    ::std::string pwd;

    ::Ice::StringSeq options;

    ::Ice::StringSeq envs;

    ::std::string activation;

    ::std::string activationTimeout;

    ::std::string deactivationTimeout;

    bool applicationDistrib;

    ::IceGrid::DistributionDescriptor distrib;

    bool allocatable;

    ::std::string user;
};

void ICE_GRID_API __patch__ServerDescriptorPtr(void*, ::Ice::ObjectPtr&);

class ICE_GRID_API ServiceDescriptor : virtual public ::IceGrid::CommunicatorDescriptor
{
public:

    typedef ServiceDescriptorPrx ProxyType;
    typedef ServiceDescriptorPtr PointerType;
    
    ServiceDescriptor() {}
    ServiceDescriptor(const ::IceGrid::AdapterDescriptorSeq&, const ::IceGrid::PropertySetDescriptor&, const ::IceGrid::DbEnvDescriptorSeq&, const ::Ice::StringSeq&, const ::std::string&, const ::std::string&, const ::std::string&);
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();


    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);

    static const ::Ice::ObjectFactoryPtr& ice_factory();

protected:

    virtual ~ServiceDescriptor() {}

public:

    ::std::string name;

    ::std::string entry;
};

void ICE_GRID_API __patch__ServiceDescriptorPtr(void*, ::Ice::ObjectPtr&);

class ICE_GRID_API IceBoxDescriptor : virtual public ::IceGrid::ServerDescriptor
{
public:

    typedef IceBoxDescriptorPrx ProxyType;
    typedef IceBoxDescriptorPtr PointerType;
    
    IceBoxDescriptor() {}
    IceBoxDescriptor(const ::IceGrid::AdapterDescriptorSeq&, const ::IceGrid::PropertySetDescriptor&, const ::IceGrid::DbEnvDescriptorSeq&, const ::Ice::StringSeq&, const ::std::string&, const ::std::string&, const ::std::string&, const ::std::string&, const ::Ice::StringSeq&, const ::Ice::StringSeq&, const ::std::string&, const ::std::string&, const ::std::string&, bool, const ::IceGrid::DistributionDescriptor&, bool, const ::std::string&, const ::IceGrid::ServiceInstanceDescriptorSeq&);
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual void __incRef();
    virtual void __decRef();
    virtual void __addObject(::IceInternal::GCCountMap&);
    virtual bool __usesClasses();
    virtual void __gcReachable(::IceInternal::GCCountMap&) const;
    virtual void __gcClear();

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);

    static const ::Ice::ObjectFactoryPtr& ice_factory();

protected:

    virtual ~IceBoxDescriptor() {}

public:

    ::IceGrid::ServiceInstanceDescriptorSeq services;
};

void ICE_GRID_API __patch__IceBoxDescriptorPtr(void*, ::Ice::ObjectPtr&);

class ICE_GRID_API LoadBalancingPolicy : virtual public ::Ice::Object
{
public:

    typedef LoadBalancingPolicyPrx ProxyType;
    typedef LoadBalancingPolicyPtr PointerType;
    
    LoadBalancingPolicy() {}
    explicit LoadBalancingPolicy(const ::std::string&);
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();


    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);

    static const ::Ice::ObjectFactoryPtr& ice_factory();

protected:

    virtual ~LoadBalancingPolicy() {}

public:

    ::std::string nReplicas;
};

void ICE_GRID_API __patch__LoadBalancingPolicyPtr(void*, ::Ice::ObjectPtr&);

class ICE_GRID_API RandomLoadBalancingPolicy : virtual public ::IceGrid::LoadBalancingPolicy
{
public:

    typedef RandomLoadBalancingPolicyPrx ProxyType;
    typedef RandomLoadBalancingPolicyPtr PointerType;
    
    RandomLoadBalancingPolicy() {}
    explicit RandomLoadBalancingPolicy(const ::std::string&);
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);

    static const ::Ice::ObjectFactoryPtr& ice_factory();

protected:

    virtual ~RandomLoadBalancingPolicy() {}
};

void ICE_GRID_API __patch__RandomLoadBalancingPolicyPtr(void*, ::Ice::ObjectPtr&);

class ICE_GRID_API OrderedLoadBalancingPolicy : virtual public ::IceGrid::LoadBalancingPolicy
{
public:

    typedef OrderedLoadBalancingPolicyPrx ProxyType;
    typedef OrderedLoadBalancingPolicyPtr PointerType;
    
    OrderedLoadBalancingPolicy() {}
    explicit OrderedLoadBalancingPolicy(const ::std::string&);
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);

    static const ::Ice::ObjectFactoryPtr& ice_factory();

protected:

    virtual ~OrderedLoadBalancingPolicy() {}
};

void ICE_GRID_API __patch__OrderedLoadBalancingPolicyPtr(void*, ::Ice::ObjectPtr&);

class ICE_GRID_API RoundRobinLoadBalancingPolicy : virtual public ::IceGrid::LoadBalancingPolicy
{
public:

    typedef RoundRobinLoadBalancingPolicyPrx ProxyType;
    typedef RoundRobinLoadBalancingPolicyPtr PointerType;
    
    RoundRobinLoadBalancingPolicy() {}
    explicit RoundRobinLoadBalancingPolicy(const ::std::string&);
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);

    static const ::Ice::ObjectFactoryPtr& ice_factory();

protected:

    virtual ~RoundRobinLoadBalancingPolicy() {}
};

void ICE_GRID_API __patch__RoundRobinLoadBalancingPolicyPtr(void*, ::Ice::ObjectPtr&);

class ICE_GRID_API AdaptiveLoadBalancingPolicy : virtual public ::IceGrid::LoadBalancingPolicy
{
public:

    typedef AdaptiveLoadBalancingPolicyPrx ProxyType;
    typedef AdaptiveLoadBalancingPolicyPtr PointerType;
    
    AdaptiveLoadBalancingPolicy() {}
    AdaptiveLoadBalancingPolicy(const ::std::string&, const ::std::string&);
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();


    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);

    static const ::Ice::ObjectFactoryPtr& ice_factory();

protected:

    virtual ~AdaptiveLoadBalancingPolicy() {}

public:

    ::std::string loadSample;
};

void ICE_GRID_API __patch__AdaptiveLoadBalancingPolicyPtr(void*, ::Ice::ObjectPtr&);

class ICE_GRID_API BoxedString : virtual public ::Ice::Object
{
public:

    typedef BoxedStringPrx ProxyType;
    typedef BoxedStringPtr PointerType;
    
    BoxedString() {}
    explicit BoxedString(const ::std::string&);
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();


    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);

    static const ::Ice::ObjectFactoryPtr& ice_factory();

protected:

    virtual ~BoxedString() {}

public:

    ::std::string value;
};

void ICE_GRID_API __patch__BoxedStringPtr(void*, ::Ice::ObjectPtr&);

class ICE_GRID_API BoxedDistributionDescriptor : virtual public ::Ice::Object
{
public:

    typedef BoxedDistributionDescriptorPrx ProxyType;
    typedef BoxedDistributionDescriptorPtr PointerType;
    
    BoxedDistributionDescriptor() {}
    explicit BoxedDistributionDescriptor(const ::IceGrid::DistributionDescriptor&);
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();


    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);

    static const ::Ice::ObjectFactoryPtr& ice_factory();

protected:

    virtual ~BoxedDistributionDescriptor() {}

public:

    ::IceGrid::DistributionDescriptor value;
};

void ICE_GRID_API __patch__BoxedDistributionDescriptorPtr(void*, ::Ice::ObjectPtr&);

}

#endif
