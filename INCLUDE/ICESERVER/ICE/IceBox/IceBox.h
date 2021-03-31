// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.2.1
// Generated from file `IceBox.ice'

#ifndef __IceBox_IceBox_h__
#define __IceBox_IceBox_h__

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
#include <Ice/BuiltinSequences.h>
#include <Ice/CommunicatorF.h>
#include <Ice/PropertiesF.h>
#include <Ice/SliceChecksumDict.h>
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

#ifndef ICE_BOX_API
#   ifdef ICE_BOX_API_EXPORTS
#       define ICE_BOX_API ICE_DECLSPEC_EXPORT
#   else
#       define ICE_BOX_API ICE_DECLSPEC_IMPORT
#   endif
#endif

namespace IceProxy
{

namespace IceBox
{

class ServiceManager;
ICE_BOX_API bool operator==(const ServiceManager&, const ServiceManager&);
ICE_BOX_API bool operator!=(const ServiceManager&, const ServiceManager&);
ICE_BOX_API bool operator<(const ServiceManager&, const ServiceManager&);
ICE_BOX_API bool operator<=(const ServiceManager&, const ServiceManager&);
ICE_BOX_API bool operator>(const ServiceManager&, const ServiceManager&);
ICE_BOX_API bool operator>=(const ServiceManager&, const ServiceManager&);

}

}

namespace IceBox
{

class Service;
ICE_BOX_API bool operator==(const Service&, const Service&);
ICE_BOX_API bool operator!=(const Service&, const Service&);
ICE_BOX_API bool operator<(const Service&, const Service&);
ICE_BOX_API bool operator<=(const Service&, const Service&);
ICE_BOX_API bool operator>(const Service&, const Service&);
ICE_BOX_API bool operator>=(const Service&, const Service&);

class ServiceManager;
ICE_BOX_API bool operator==(const ServiceManager&, const ServiceManager&);
ICE_BOX_API bool operator!=(const ServiceManager&, const ServiceManager&);
ICE_BOX_API bool operator<(const ServiceManager&, const ServiceManager&);
ICE_BOX_API bool operator<=(const ServiceManager&, const ServiceManager&);
ICE_BOX_API bool operator>(const ServiceManager&, const ServiceManager&);
ICE_BOX_API bool operator>=(const ServiceManager&, const ServiceManager&);

}

namespace IceInternal
{

ICE_BOX_API void incRef(::IceBox::Service*);
ICE_BOX_API void decRef(::IceBox::Service*);

ICE_BOX_API void incRef(::IceBox::ServiceManager*);
ICE_BOX_API void decRef(::IceBox::ServiceManager*);

ICE_BOX_API void incRef(::IceProxy::IceBox::ServiceManager*);
ICE_BOX_API void decRef(::IceProxy::IceBox::ServiceManager*);

}

namespace IceBox
{

typedef ::IceInternal::Handle< ::IceBox::Service> ServicePtr;

typedef ::IceInternal::Handle< ::IceBox::ServiceManager> ServiceManagerPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::IceBox::ServiceManager> ServiceManagerPrx;

ICE_BOX_API void __write(::IceInternal::BasicStream*, const ServiceManagerPrx&);
ICE_BOX_API void __read(::IceInternal::BasicStream*, ServiceManagerPrx&);
ICE_BOX_API void __write(::IceInternal::BasicStream*, const ServiceManagerPtr&);
ICE_BOX_API void __patch__ServiceManagerPtr(void*, ::Ice::ObjectPtr&);

ICE_BOX_API void __addObject(const ServiceManagerPtr&, ::IceInternal::GCCountMap&);
ICE_BOX_API bool __usesClasses(const ServiceManagerPtr&);
ICE_BOX_API void __decRefUnsafe(const ServiceManagerPtr&);
ICE_BOX_API void __clearHandleUnsafe(ServiceManagerPtr&);

}

namespace IceBox
{

class ICE_BOX_API FailureException : public ::Ice::LocalException
{
public:

    FailureException(const char*, int);
    FailureException(const char*, int, const ::std::string&);
    virtual ~FailureException() throw();

    virtual ::std::string ice_name() const;
    virtual void ice_print(::std::ostream&) const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    ::std::string reason;
};

class ICE_BOX_API AlreadyStartedException : public ::Ice::UserException
{
public:

    AlreadyStartedException() {}
    virtual ~AlreadyStartedException() throw();

    virtual ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);

    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

static AlreadyStartedException __AlreadyStartedException_init;

class ICE_BOX_API AlreadyStoppedException : public ::Ice::UserException
{
public:

    AlreadyStoppedException() {}
    virtual ~AlreadyStoppedException() throw();

    virtual ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);

    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

class ICE_BOX_API NoSuchServiceException : public ::Ice::UserException
{
public:

    NoSuchServiceException() {}
    virtual ~NoSuchServiceException() throw();

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

namespace IceBox
{

class ICE_BOX_API ServiceManager : virtual public ::IceProxy::Ice::Object
{
public:

    ::Ice::SliceChecksumDict getSliceChecksums()
    {
        return getSliceChecksums(0);
    }
    ::Ice::SliceChecksumDict getSliceChecksums(const ::Ice::Context& __ctx)
    {
        return getSliceChecksums(&__ctx);
    }
    
private:

    ::Ice::SliceChecksumDict getSliceChecksums(const ::Ice::Context*);
    
public:

    void startService(const ::std::string& service)
    {
        startService(service, 0);
    }
    void startService(const ::std::string& service, const ::Ice::Context& __ctx)
    {
        startService(service, &__ctx);
    }
    
private:

    void startService(const ::std::string&, const ::Ice::Context*);
    
public:

    void stopService(const ::std::string& service)
    {
        stopService(service, 0);
    }
    void stopService(const ::std::string& service, const ::Ice::Context& __ctx)
    {
        stopService(service, &__ctx);
    }
    
private:

    void stopService(const ::std::string&, const ::Ice::Context*);
    
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

namespace IceBox
{

class ICE_BOX_API ServiceManager : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual ::Ice::SliceChecksumDict getSliceChecksums(const ::Ice::Context*) = 0;

    virtual void startService(const ::std::string&, const ::Ice::Context*) = 0;

    virtual void stopService(const ::std::string&, const ::Ice::Context*) = 0;

    virtual void shutdown(const ::Ice::Context*) = 0;
};

}

}

namespace IceDelegateM
{

namespace IceBox
{

class ICE_BOX_API ServiceManager : virtual public ::IceDelegate::IceBox::ServiceManager,
                                   virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual ::Ice::SliceChecksumDict getSliceChecksums(const ::Ice::Context*);

    virtual void startService(const ::std::string&, const ::Ice::Context*);

    virtual void stopService(const ::std::string&, const ::Ice::Context*);

    virtual void shutdown(const ::Ice::Context*);
};

}

}

namespace IceDelegateD
{

namespace IceBox
{

class ICE_BOX_API ServiceManager : virtual public ::IceDelegate::IceBox::ServiceManager,
                                   virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual ::Ice::SliceChecksumDict getSliceChecksums(const ::Ice::Context*);

    virtual void startService(const ::std::string&, const ::Ice::Context*);

    virtual void stopService(const ::std::string&, const ::Ice::Context*);

    virtual void shutdown(const ::Ice::Context*);
};

}

}

namespace IceBox
{

class ICE_BOX_API Service : virtual public ::Ice::LocalObject
{
public:

    typedef ServicePtr PointerType;
    

    virtual void start(const ::std::string&, const ::Ice::CommunicatorPtr&, const ::Ice::StringSeq&) = 0;

    virtual void stop() = 0;
};

class ICE_BOX_API ServiceManager : virtual public ::Ice::Object
{
public:

    typedef ServiceManagerPrx ProxyType;
    typedef ServiceManagerPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::Ice::SliceChecksumDict getSliceChecksums(const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___getSliceChecksums(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual void startService(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___startService(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void stopService(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___stopService(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void shutdown(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___shutdown(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void ICE_BOX_API __patch__ServiceManagerPtr(void*, ::Ice::ObjectPtr&);

}

#endif
