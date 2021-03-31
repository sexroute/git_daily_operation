// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.2.1
// Generated from file `Session.ice'

#ifndef __IceGrid_Session_h__
#define __IceGrid_Session_h__

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

class Session;
ICE_GRID_API bool operator==(const Session&, const Session&);
ICE_GRID_API bool operator!=(const Session&, const Session&);
ICE_GRID_API bool operator<(const Session&, const Session&);
ICE_GRID_API bool operator<=(const Session&, const Session&);
ICE_GRID_API bool operator>(const Session&, const Session&);
ICE_GRID_API bool operator>=(const Session&, const Session&);

}

}

namespace IceGrid
{

class Session;
ICE_GRID_API bool operator==(const Session&, const Session&);
ICE_GRID_API bool operator!=(const Session&, const Session&);
ICE_GRID_API bool operator<(const Session&, const Session&);
ICE_GRID_API bool operator<=(const Session&, const Session&);
ICE_GRID_API bool operator>(const Session&, const Session&);
ICE_GRID_API bool operator>=(const Session&, const Session&);

}

namespace IceInternal
{

ICE_GRID_API void incRef(::IceGrid::Session*);
ICE_GRID_API void decRef(::IceGrid::Session*);

ICE_GRID_API void incRef(::IceProxy::IceGrid::Session*);
ICE_GRID_API void decRef(::IceProxy::IceGrid::Session*);

}

namespace IceGrid
{

typedef ::IceInternal::Handle< ::IceGrid::Session> SessionPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::IceGrid::Session> SessionPrx;

ICE_GRID_API void __write(::IceInternal::BasicStream*, const SessionPrx&);
ICE_GRID_API void __read(::IceInternal::BasicStream*, SessionPrx&);
ICE_GRID_API void __write(::IceInternal::BasicStream*, const SessionPtr&);
ICE_GRID_API void __patch__SessionPtr(void*, ::Ice::ObjectPtr&);

ICE_GRID_API void __addObject(const SessionPtr&, ::IceInternal::GCCountMap&);
ICE_GRID_API bool __usesClasses(const SessionPtr&);
ICE_GRID_API void __decRefUnsafe(const SessionPtr&);
ICE_GRID_API void __clearHandleUnsafe(SessionPtr&);

}

namespace IceGrid
{

}

namespace IceGrid
{

class ICE_GRID_API AMI_Session_allocateObjectById : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response(const ::Ice::ObjectPrx&) = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::IceGrid::SessionPrx&, const ::Ice::Identity&, const ::Ice::Context*);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::IceGrid::AMI_Session_allocateObjectById> AMI_Session_allocateObjectByIdPtr;

class ICE_GRID_API AMD_Session_allocateObjectById : virtual public ::IceUtil::Shared
{
public:

    virtual void ice_response(const ::Ice::ObjectPrx&) = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;
    virtual void ice_exception(const ::std::exception&) = 0;
    virtual void ice_exception() = 0;
};

typedef ::IceUtil::Handle< ::IceGrid::AMD_Session_allocateObjectById> AMD_Session_allocateObjectByIdPtr;

class ICE_GRID_API AMI_Session_allocateObjectByType : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response(const ::Ice::ObjectPrx&) = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::IceGrid::SessionPrx&, const ::std::string&, const ::Ice::Context*);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::IceGrid::AMI_Session_allocateObjectByType> AMI_Session_allocateObjectByTypePtr;

class ICE_GRID_API AMD_Session_allocateObjectByType : virtual public ::IceUtil::Shared
{
public:

    virtual void ice_response(const ::Ice::ObjectPrx&) = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;
    virtual void ice_exception(const ::std::exception&) = 0;
    virtual void ice_exception() = 0;
};

typedef ::IceUtil::Handle< ::IceGrid::AMD_Session_allocateObjectByType> AMD_Session_allocateObjectByTypePtr;

}

namespace IceAsync
{

namespace IceGrid
{

class ICE_GRID_API AMD_Session_allocateObjectById : public ::IceGrid::AMD_Session_allocateObjectById, public ::IceInternal::IncomingAsync
{
public:

    AMD_Session_allocateObjectById(::IceInternal::Incoming&);

    virtual void ice_response(const ::Ice::ObjectPrx&);
    virtual void ice_exception(const ::Ice::Exception&);
    virtual void ice_exception(const ::std::exception&);
    virtual void ice_exception();
};

class ICE_GRID_API AMD_Session_allocateObjectByType : public ::IceGrid::AMD_Session_allocateObjectByType, public ::IceInternal::IncomingAsync
{
public:

    AMD_Session_allocateObjectByType(::IceInternal::Incoming&);

    virtual void ice_response(const ::Ice::ObjectPrx&);
    virtual void ice_exception(const ::Ice::Exception&);
    virtual void ice_exception(const ::std::exception&);
    virtual void ice_exception();
};

}

}

namespace IceProxy
{

namespace IceGrid
{

class ICE_GRID_API Session : virtual public ::IceProxy::Glacier2::Session
{
public:

    void keepAlive()
    {
        keepAlive(0);
    }
    void keepAlive(const ::Ice::Context& __ctx)
    {
        keepAlive(&__ctx);
    }
    
private:

    void keepAlive(const ::Ice::Context*);
    
public:

    ::Ice::ObjectPrx allocateObjectById(const ::Ice::Identity& id)
    {
        return allocateObjectById(id, 0);
    }
    ::Ice::ObjectPrx allocateObjectById(const ::Ice::Identity& id, const ::Ice::Context& __ctx)
    {
        return allocateObjectById(id, &__ctx);
    }
    
private:

    ::Ice::ObjectPrx allocateObjectById(const ::Ice::Identity&, const ::Ice::Context*);
    
public:
    void allocateObjectById_async(const ::IceGrid::AMI_Session_allocateObjectByIdPtr&, const ::Ice::Identity&);
    void allocateObjectById_async(const ::IceGrid::AMI_Session_allocateObjectByIdPtr&, const ::Ice::Identity&, const ::Ice::Context&);

    ::Ice::ObjectPrx allocateObjectByType(const ::std::string& type)
    {
        return allocateObjectByType(type, 0);
    }
    ::Ice::ObjectPrx allocateObjectByType(const ::std::string& type, const ::Ice::Context& __ctx)
    {
        return allocateObjectByType(type, &__ctx);
    }
    
private:

    ::Ice::ObjectPrx allocateObjectByType(const ::std::string&, const ::Ice::Context*);
    
public:
    void allocateObjectByType_async(const ::IceGrid::AMI_Session_allocateObjectByTypePtr&, const ::std::string&);
    void allocateObjectByType_async(const ::IceGrid::AMI_Session_allocateObjectByTypePtr&, const ::std::string&, const ::Ice::Context&);

    void releaseObject(const ::Ice::Identity& id)
    {
        releaseObject(id, 0);
    }
    void releaseObject(const ::Ice::Identity& id, const ::Ice::Context& __ctx)
    {
        releaseObject(id, &__ctx);
    }
    
private:

    void releaseObject(const ::Ice::Identity&, const ::Ice::Context*);
    
public:

    void setAllocationTimeout(::Ice::Int timeout)
    {
        setAllocationTimeout(timeout, 0);
    }
    void setAllocationTimeout(::Ice::Int timeout, const ::Ice::Context& __ctx)
    {
        setAllocationTimeout(timeout, &__ctx);
    }
    
private:

    void setAllocationTimeout(::Ice::Int, const ::Ice::Context*);
    
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

class ICE_GRID_API Session : virtual public ::IceDelegate::Glacier2::Session
{
public:

    virtual void keepAlive(const ::Ice::Context*) = 0;

    virtual ::Ice::ObjectPrx allocateObjectById(const ::Ice::Identity&, const ::Ice::Context*) = 0;

    virtual ::Ice::ObjectPrx allocateObjectByType(const ::std::string&, const ::Ice::Context*) = 0;

    virtual void releaseObject(const ::Ice::Identity&, const ::Ice::Context*) = 0;

    virtual void setAllocationTimeout(::Ice::Int, const ::Ice::Context*) = 0;
};

}

}

namespace IceDelegateM
{

namespace IceGrid
{

class ICE_GRID_API Session : virtual public ::IceDelegate::IceGrid::Session,
                             virtual public ::IceDelegateM::Glacier2::Session
{
public:

    virtual void keepAlive(const ::Ice::Context*);

    virtual ::Ice::ObjectPrx allocateObjectById(const ::Ice::Identity&, const ::Ice::Context*);

    virtual ::Ice::ObjectPrx allocateObjectByType(const ::std::string&, const ::Ice::Context*);

    virtual void releaseObject(const ::Ice::Identity&, const ::Ice::Context*);

    virtual void setAllocationTimeout(::Ice::Int, const ::Ice::Context*);
};

}

}

namespace IceDelegateD
{

namespace IceGrid
{

class ICE_GRID_API Session : virtual public ::IceDelegate::IceGrid::Session,
                             virtual public ::IceDelegateD::Glacier2::Session
{
public:

    virtual void keepAlive(const ::Ice::Context*);

    virtual ::Ice::ObjectPrx allocateObjectById(const ::Ice::Identity&, const ::Ice::Context*);

    virtual ::Ice::ObjectPrx allocateObjectByType(const ::std::string&, const ::Ice::Context*);

    virtual void releaseObject(const ::Ice::Identity&, const ::Ice::Context*);

    virtual void setAllocationTimeout(::Ice::Int, const ::Ice::Context*);
};

}

}

namespace IceGrid
{

class ICE_GRID_API Session : virtual public ::Glacier2::Session
{
public:

    typedef SessionPrx ProxyType;
    typedef SessionPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual void keepAlive(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___keepAlive(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void allocateObjectById_async(const ::IceGrid::AMD_Session_allocateObjectByIdPtr&, const ::Ice::Identity&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___allocateObjectById(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void allocateObjectByType_async(const ::IceGrid::AMD_Session_allocateObjectByTypePtr&, const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___allocateObjectByType(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void releaseObject(const ::Ice::Identity&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___releaseObject(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void setAllocationTimeout(::Ice::Int, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___setAllocationTimeout(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void ICE_GRID_API __patch__SessionPtr(void*, ::Ice::ObjectPtr&);

}

#endif
