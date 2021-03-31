// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.2.1
// Generated from file `Process.ice'

#ifndef __Ice_Process_h__
#define __Ice_Process_h__

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

class Process;
ICE_API bool operator==(const Process&, const Process&);
ICE_API bool operator!=(const Process&, const Process&);
ICE_API bool operator<(const Process&, const Process&);
ICE_API bool operator<=(const Process&, const Process&);
ICE_API bool operator>(const Process&, const Process&);
ICE_API bool operator>=(const Process&, const Process&);

}

}

namespace Ice
{

class Process;
ICE_API bool operator==(const Process&, const Process&);
ICE_API bool operator!=(const Process&, const Process&);
ICE_API bool operator<(const Process&, const Process&);
ICE_API bool operator<=(const Process&, const Process&);
ICE_API bool operator>(const Process&, const Process&);
ICE_API bool operator>=(const Process&, const Process&);

}

namespace IceInternal
{

ICE_API void incRef(::Ice::Process*);
ICE_API void decRef(::Ice::Process*);

ICE_API void incRef(::IceProxy::Ice::Process*);
ICE_API void decRef(::IceProxy::Ice::Process*);

}

namespace Ice
{

typedef ::IceInternal::Handle< ::Ice::Process> ProcessPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::Ice::Process> ProcessPrx;

ICE_API void __write(::IceInternal::BasicStream*, const ProcessPrx&);
ICE_API void __read(::IceInternal::BasicStream*, ProcessPrx&);
ICE_API void __write(::IceInternal::BasicStream*, const ProcessPtr&);
ICE_API void __patch__ProcessPtr(void*, ::Ice::ObjectPtr&);

ICE_API void __addObject(const ProcessPtr&, ::IceInternal::GCCountMap&);
ICE_API bool __usesClasses(const ProcessPtr&);
ICE_API void __decRefUnsafe(const ProcessPtr&);
ICE_API void __clearHandleUnsafe(ProcessPtr&);

}

namespace Ice
{

}

namespace Ice
{

class ICE_API AMI_Process_shutdown : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response() = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::Ice::ProcessPrx&, const ::Ice::Context*);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::Ice::AMI_Process_shutdown> AMI_Process_shutdownPtr;

}

namespace IceProxy
{

namespace Ice
{

class ICE_API Process : virtual public ::IceProxy::Ice::Object
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
    void shutdown_async(const ::Ice::AMI_Process_shutdownPtr&);
    void shutdown_async(const ::Ice::AMI_Process_shutdownPtr&, const ::Ice::Context&);

    void writeMessage(const ::std::string& message, ::Ice::Int fd)
    {
        writeMessage(message, fd, 0);
    }
    void writeMessage(const ::std::string& message, ::Ice::Int fd, const ::Ice::Context& __ctx)
    {
        writeMessage(message, fd, &__ctx);
    }
    
private:

    void writeMessage(const ::std::string&, ::Ice::Int, const ::Ice::Context*);
    
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

class ICE_API Process : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual void shutdown(const ::Ice::Context*) = 0;

    virtual void writeMessage(const ::std::string&, ::Ice::Int, const ::Ice::Context*) = 0;
};

}

}

namespace IceDelegateM
{

namespace Ice
{

class ICE_API Process : virtual public ::IceDelegate::Ice::Process,
                        virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual void shutdown(const ::Ice::Context*);

    virtual void writeMessage(const ::std::string&, ::Ice::Int, const ::Ice::Context*);
};

}

}

namespace IceDelegateD
{

namespace Ice
{

class ICE_API Process : virtual public ::IceDelegate::Ice::Process,
                        virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual void shutdown(const ::Ice::Context*);

    virtual void writeMessage(const ::std::string&, ::Ice::Int, const ::Ice::Context*);
};

}

}

namespace Ice
{

class ICE_API Process : virtual public ::Ice::Object
{
public:

    typedef ProcessPrx ProxyType;
    typedef ProcessPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual void shutdown(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___shutdown(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void writeMessage(const ::std::string&, ::Ice::Int, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___writeMessage(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void ICE_API __patch__ProcessPtr(void*, ::Ice::ObjectPtr&);

}

#endif
