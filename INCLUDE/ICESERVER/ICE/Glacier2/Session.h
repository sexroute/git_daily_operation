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

#ifndef __Glacier2_Session_h__
#define __Glacier2_Session_h__

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
#include <Ice/BuiltinSequences.h>
#include <Ice/Identity.h>
#include <Glacier2/SSLInfo.h>
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

class Session;
GLACIER2_API bool operator==(const Session&, const Session&);
GLACIER2_API bool operator!=(const Session&, const Session&);
GLACIER2_API bool operator<(const Session&, const Session&);
GLACIER2_API bool operator<=(const Session&, const Session&);
GLACIER2_API bool operator>(const Session&, const Session&);
GLACIER2_API bool operator>=(const Session&, const Session&);

class StringSet;
GLACIER2_API bool operator==(const StringSet&, const StringSet&);
GLACIER2_API bool operator!=(const StringSet&, const StringSet&);
GLACIER2_API bool operator<(const StringSet&, const StringSet&);
GLACIER2_API bool operator<=(const StringSet&, const StringSet&);
GLACIER2_API bool operator>(const StringSet&, const StringSet&);
GLACIER2_API bool operator>=(const StringSet&, const StringSet&);

class IdentitySet;
GLACIER2_API bool operator==(const IdentitySet&, const IdentitySet&);
GLACIER2_API bool operator!=(const IdentitySet&, const IdentitySet&);
GLACIER2_API bool operator<(const IdentitySet&, const IdentitySet&);
GLACIER2_API bool operator<=(const IdentitySet&, const IdentitySet&);
GLACIER2_API bool operator>(const IdentitySet&, const IdentitySet&);
GLACIER2_API bool operator>=(const IdentitySet&, const IdentitySet&);

class SessionControl;
GLACIER2_API bool operator==(const SessionControl&, const SessionControl&);
GLACIER2_API bool operator!=(const SessionControl&, const SessionControl&);
GLACIER2_API bool operator<(const SessionControl&, const SessionControl&);
GLACIER2_API bool operator<=(const SessionControl&, const SessionControl&);
GLACIER2_API bool operator>(const SessionControl&, const SessionControl&);
GLACIER2_API bool operator>=(const SessionControl&, const SessionControl&);

class SessionManager;
GLACIER2_API bool operator==(const SessionManager&, const SessionManager&);
GLACIER2_API bool operator!=(const SessionManager&, const SessionManager&);
GLACIER2_API bool operator<(const SessionManager&, const SessionManager&);
GLACIER2_API bool operator<=(const SessionManager&, const SessionManager&);
GLACIER2_API bool operator>(const SessionManager&, const SessionManager&);
GLACIER2_API bool operator>=(const SessionManager&, const SessionManager&);

class SSLSessionManager;
GLACIER2_API bool operator==(const SSLSessionManager&, const SSLSessionManager&);
GLACIER2_API bool operator!=(const SSLSessionManager&, const SSLSessionManager&);
GLACIER2_API bool operator<(const SSLSessionManager&, const SSLSessionManager&);
GLACIER2_API bool operator<=(const SSLSessionManager&, const SSLSessionManager&);
GLACIER2_API bool operator>(const SSLSessionManager&, const SSLSessionManager&);
GLACIER2_API bool operator>=(const SSLSessionManager&, const SSLSessionManager&);

}

}

namespace Glacier2
{

class Session;
GLACIER2_API bool operator==(const Session&, const Session&);
GLACIER2_API bool operator!=(const Session&, const Session&);
GLACIER2_API bool operator<(const Session&, const Session&);
GLACIER2_API bool operator<=(const Session&, const Session&);
GLACIER2_API bool operator>(const Session&, const Session&);
GLACIER2_API bool operator>=(const Session&, const Session&);

class StringSet;
GLACIER2_API bool operator==(const StringSet&, const StringSet&);
GLACIER2_API bool operator!=(const StringSet&, const StringSet&);
GLACIER2_API bool operator<(const StringSet&, const StringSet&);
GLACIER2_API bool operator<=(const StringSet&, const StringSet&);
GLACIER2_API bool operator>(const StringSet&, const StringSet&);
GLACIER2_API bool operator>=(const StringSet&, const StringSet&);

class IdentitySet;
GLACIER2_API bool operator==(const IdentitySet&, const IdentitySet&);
GLACIER2_API bool operator!=(const IdentitySet&, const IdentitySet&);
GLACIER2_API bool operator<(const IdentitySet&, const IdentitySet&);
GLACIER2_API bool operator<=(const IdentitySet&, const IdentitySet&);
GLACIER2_API bool operator>(const IdentitySet&, const IdentitySet&);
GLACIER2_API bool operator>=(const IdentitySet&, const IdentitySet&);

class SessionControl;
GLACIER2_API bool operator==(const SessionControl&, const SessionControl&);
GLACIER2_API bool operator!=(const SessionControl&, const SessionControl&);
GLACIER2_API bool operator<(const SessionControl&, const SessionControl&);
GLACIER2_API bool operator<=(const SessionControl&, const SessionControl&);
GLACIER2_API bool operator>(const SessionControl&, const SessionControl&);
GLACIER2_API bool operator>=(const SessionControl&, const SessionControl&);

class SessionManager;
GLACIER2_API bool operator==(const SessionManager&, const SessionManager&);
GLACIER2_API bool operator!=(const SessionManager&, const SessionManager&);
GLACIER2_API bool operator<(const SessionManager&, const SessionManager&);
GLACIER2_API bool operator<=(const SessionManager&, const SessionManager&);
GLACIER2_API bool operator>(const SessionManager&, const SessionManager&);
GLACIER2_API bool operator>=(const SessionManager&, const SessionManager&);

class SSLSessionManager;
GLACIER2_API bool operator==(const SSLSessionManager&, const SSLSessionManager&);
GLACIER2_API bool operator!=(const SSLSessionManager&, const SSLSessionManager&);
GLACIER2_API bool operator<(const SSLSessionManager&, const SSLSessionManager&);
GLACIER2_API bool operator<=(const SSLSessionManager&, const SSLSessionManager&);
GLACIER2_API bool operator>(const SSLSessionManager&, const SSLSessionManager&);
GLACIER2_API bool operator>=(const SSLSessionManager&, const SSLSessionManager&);

}

namespace IceInternal
{

GLACIER2_API void incRef(::Glacier2::Session*);
GLACIER2_API void decRef(::Glacier2::Session*);

GLACIER2_API void incRef(::IceProxy::Glacier2::Session*);
GLACIER2_API void decRef(::IceProxy::Glacier2::Session*);

GLACIER2_API void incRef(::Glacier2::StringSet*);
GLACIER2_API void decRef(::Glacier2::StringSet*);

GLACIER2_API void incRef(::IceProxy::Glacier2::StringSet*);
GLACIER2_API void decRef(::IceProxy::Glacier2::StringSet*);

GLACIER2_API void incRef(::Glacier2::IdentitySet*);
GLACIER2_API void decRef(::Glacier2::IdentitySet*);

GLACIER2_API void incRef(::IceProxy::Glacier2::IdentitySet*);
GLACIER2_API void decRef(::IceProxy::Glacier2::IdentitySet*);

GLACIER2_API void incRef(::Glacier2::SessionControl*);
GLACIER2_API void decRef(::Glacier2::SessionControl*);

GLACIER2_API void incRef(::IceProxy::Glacier2::SessionControl*);
GLACIER2_API void decRef(::IceProxy::Glacier2::SessionControl*);

GLACIER2_API void incRef(::Glacier2::SessionManager*);
GLACIER2_API void decRef(::Glacier2::SessionManager*);

GLACIER2_API void incRef(::IceProxy::Glacier2::SessionManager*);
GLACIER2_API void decRef(::IceProxy::Glacier2::SessionManager*);

GLACIER2_API void incRef(::Glacier2::SSLSessionManager*);
GLACIER2_API void decRef(::Glacier2::SSLSessionManager*);

GLACIER2_API void incRef(::IceProxy::Glacier2::SSLSessionManager*);
GLACIER2_API void decRef(::IceProxy::Glacier2::SSLSessionManager*);

}

namespace Glacier2
{

typedef ::IceInternal::Handle< ::Glacier2::Session> SessionPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::Glacier2::Session> SessionPrx;

GLACIER2_API void __write(::IceInternal::BasicStream*, const SessionPrx&);
GLACIER2_API void __read(::IceInternal::BasicStream*, SessionPrx&);
GLACIER2_API void __write(::IceInternal::BasicStream*, const SessionPtr&);
GLACIER2_API void __patch__SessionPtr(void*, ::Ice::ObjectPtr&);

GLACIER2_API void __addObject(const SessionPtr&, ::IceInternal::GCCountMap&);
GLACIER2_API bool __usesClasses(const SessionPtr&);
GLACIER2_API void __decRefUnsafe(const SessionPtr&);
GLACIER2_API void __clearHandleUnsafe(SessionPtr&);

typedef ::IceInternal::Handle< ::Glacier2::StringSet> StringSetPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::Glacier2::StringSet> StringSetPrx;

GLACIER2_API void __write(::IceInternal::BasicStream*, const StringSetPrx&);
GLACIER2_API void __read(::IceInternal::BasicStream*, StringSetPrx&);
GLACIER2_API void __write(::IceInternal::BasicStream*, const StringSetPtr&);
GLACIER2_API void __patch__StringSetPtr(void*, ::Ice::ObjectPtr&);

GLACIER2_API void __addObject(const StringSetPtr&, ::IceInternal::GCCountMap&);
GLACIER2_API bool __usesClasses(const StringSetPtr&);
GLACIER2_API void __decRefUnsafe(const StringSetPtr&);
GLACIER2_API void __clearHandleUnsafe(StringSetPtr&);

typedef ::IceInternal::Handle< ::Glacier2::IdentitySet> IdentitySetPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::Glacier2::IdentitySet> IdentitySetPrx;

GLACIER2_API void __write(::IceInternal::BasicStream*, const IdentitySetPrx&);
GLACIER2_API void __read(::IceInternal::BasicStream*, IdentitySetPrx&);
GLACIER2_API void __write(::IceInternal::BasicStream*, const IdentitySetPtr&);
GLACIER2_API void __patch__IdentitySetPtr(void*, ::Ice::ObjectPtr&);

GLACIER2_API void __addObject(const IdentitySetPtr&, ::IceInternal::GCCountMap&);
GLACIER2_API bool __usesClasses(const IdentitySetPtr&);
GLACIER2_API void __decRefUnsafe(const IdentitySetPtr&);
GLACIER2_API void __clearHandleUnsafe(IdentitySetPtr&);

typedef ::IceInternal::Handle< ::Glacier2::SessionControl> SessionControlPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::Glacier2::SessionControl> SessionControlPrx;

GLACIER2_API void __write(::IceInternal::BasicStream*, const SessionControlPrx&);
GLACIER2_API void __read(::IceInternal::BasicStream*, SessionControlPrx&);
GLACIER2_API void __write(::IceInternal::BasicStream*, const SessionControlPtr&);
GLACIER2_API void __patch__SessionControlPtr(void*, ::Ice::ObjectPtr&);

GLACIER2_API void __addObject(const SessionControlPtr&, ::IceInternal::GCCountMap&);
GLACIER2_API bool __usesClasses(const SessionControlPtr&);
GLACIER2_API void __decRefUnsafe(const SessionControlPtr&);
GLACIER2_API void __clearHandleUnsafe(SessionControlPtr&);

typedef ::IceInternal::Handle< ::Glacier2::SessionManager> SessionManagerPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::Glacier2::SessionManager> SessionManagerPrx;

GLACIER2_API void __write(::IceInternal::BasicStream*, const SessionManagerPrx&);
GLACIER2_API void __read(::IceInternal::BasicStream*, SessionManagerPrx&);
GLACIER2_API void __write(::IceInternal::BasicStream*, const SessionManagerPtr&);
GLACIER2_API void __patch__SessionManagerPtr(void*, ::Ice::ObjectPtr&);

GLACIER2_API void __addObject(const SessionManagerPtr&, ::IceInternal::GCCountMap&);
GLACIER2_API bool __usesClasses(const SessionManagerPtr&);
GLACIER2_API void __decRefUnsafe(const SessionManagerPtr&);
GLACIER2_API void __clearHandleUnsafe(SessionManagerPtr&);

typedef ::IceInternal::Handle< ::Glacier2::SSLSessionManager> SSLSessionManagerPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::Glacier2::SSLSessionManager> SSLSessionManagerPrx;

GLACIER2_API void __write(::IceInternal::BasicStream*, const SSLSessionManagerPrx&);
GLACIER2_API void __read(::IceInternal::BasicStream*, SSLSessionManagerPrx&);
GLACIER2_API void __write(::IceInternal::BasicStream*, const SSLSessionManagerPtr&);
GLACIER2_API void __patch__SSLSessionManagerPtr(void*, ::Ice::ObjectPtr&);

GLACIER2_API void __addObject(const SSLSessionManagerPtr&, ::IceInternal::GCCountMap&);
GLACIER2_API bool __usesClasses(const SSLSessionManagerPtr&);
GLACIER2_API void __decRefUnsafe(const SSLSessionManagerPtr&);
GLACIER2_API void __clearHandleUnsafe(SSLSessionManagerPtr&);

}

namespace Glacier2
{

class GLACIER2_API CannotCreateSessionException : public ::Ice::UserException
{
public:

    CannotCreateSessionException() {}
    explicit CannotCreateSessionException(const ::std::string&);
    virtual ~CannotCreateSessionException() throw();

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

static CannotCreateSessionException __CannotCreateSessionException_init;

}

namespace Glacier2
{

class GLACIER2_API AMI_SessionControl_destroy : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response() = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::Glacier2::SessionControlPrx&, const ::Ice::Context*);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::Glacier2::AMI_SessionControl_destroy> AMI_SessionControl_destroyPtr;

}

namespace IceProxy
{

namespace Glacier2
{

class GLACIER2_API Session : virtual public ::IceProxy::Ice::Object
{
public:

    void destroy()
    {
        destroy(0);
    }
    void destroy(const ::Ice::Context& __ctx)
    {
        destroy(&__ctx);
    }
    
private:

    void destroy(const ::Ice::Context*);
    
public:
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

class GLACIER2_API StringSet : virtual public ::IceProxy::Ice::Object
{
public:

    void add(const ::Ice::StringSeq& additions)
    {
        add(additions, 0);
    }
    void add(const ::Ice::StringSeq& additions, const ::Ice::Context& __ctx)
    {
        add(additions, &__ctx);
    }
    
private:

    void add(const ::Ice::StringSeq&, const ::Ice::Context*);
    
public:

    void remove(const ::Ice::StringSeq& deletions)
    {
        remove(deletions, 0);
    }
    void remove(const ::Ice::StringSeq& deletions, const ::Ice::Context& __ctx)
    {
        remove(deletions, &__ctx);
    }
    
private:

    void remove(const ::Ice::StringSeq&, const ::Ice::Context*);
    
public:

    ::Ice::StringSeq get()
    {
        return get(0);
    }
    ::Ice::StringSeq get(const ::Ice::Context& __ctx)
    {
        return get(&__ctx);
    }
    
private:

    ::Ice::StringSeq get(const ::Ice::Context*);
    
public:
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

class GLACIER2_API IdentitySet : virtual public ::IceProxy::Ice::Object
{
public:

    void add(const ::Ice::IdentitySeq& additions)
    {
        add(additions, 0);
    }
    void add(const ::Ice::IdentitySeq& additions, const ::Ice::Context& __ctx)
    {
        add(additions, &__ctx);
    }
    
private:

    void add(const ::Ice::IdentitySeq&, const ::Ice::Context*);
    
public:

    void remove(const ::Ice::IdentitySeq& deletions)
    {
        remove(deletions, 0);
    }
    void remove(const ::Ice::IdentitySeq& deletions, const ::Ice::Context& __ctx)
    {
        remove(deletions, &__ctx);
    }
    
private:

    void remove(const ::Ice::IdentitySeq&, const ::Ice::Context*);
    
public:

    ::Ice::IdentitySeq get()
    {
        return get(0);
    }
    ::Ice::IdentitySeq get(const ::Ice::Context& __ctx)
    {
        return get(&__ctx);
    }
    
private:

    ::Ice::IdentitySeq get(const ::Ice::Context*);
    
public:
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

class GLACIER2_API SessionControl : virtual public ::IceProxy::Ice::Object
{
public:

    ::Glacier2::StringSetPrx categories()
    {
        return categories(0);
    }
    ::Glacier2::StringSetPrx categories(const ::Ice::Context& __ctx)
    {
        return categories(&__ctx);
    }
    
private:

    ::Glacier2::StringSetPrx categories(const ::Ice::Context*);
    
public:

    ::Glacier2::StringSetPrx adapterIds()
    {
        return adapterIds(0);
    }
    ::Glacier2::StringSetPrx adapterIds(const ::Ice::Context& __ctx)
    {
        return adapterIds(&__ctx);
    }
    
private:

    ::Glacier2::StringSetPrx adapterIds(const ::Ice::Context*);
    
public:

    ::Glacier2::IdentitySetPrx identities()
    {
        return identities(0);
    }
    ::Glacier2::IdentitySetPrx identities(const ::Ice::Context& __ctx)
    {
        return identities(&__ctx);
    }
    
private:

    ::Glacier2::IdentitySetPrx identities(const ::Ice::Context*);
    
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

    void destroy()
    {
        destroy(0);
    }
    void destroy(const ::Ice::Context& __ctx)
    {
        destroy(&__ctx);
    }
    
private:

    void destroy(const ::Ice::Context*);
    
public:
    void destroy_async(const ::Glacier2::AMI_SessionControl_destroyPtr&);
    void destroy_async(const ::Glacier2::AMI_SessionControl_destroyPtr&, const ::Ice::Context&);
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

class GLACIER2_API SessionManager : virtual public ::IceProxy::Ice::Object
{
public:

    ::Glacier2::SessionPrx create(const ::std::string& userId, const ::Glacier2::SessionControlPrx& control)
    {
        return create(userId, control, 0);
    }
    ::Glacier2::SessionPrx create(const ::std::string& userId, const ::Glacier2::SessionControlPrx& control, const ::Ice::Context& __ctx)
    {
        return create(userId, control, &__ctx);
    }
    
private:

    ::Glacier2::SessionPrx create(const ::std::string&, const ::Glacier2::SessionControlPrx&, const ::Ice::Context*);
    
public:
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

class GLACIER2_API SSLSessionManager : virtual public ::IceProxy::Ice::Object
{
public:

    ::Glacier2::SessionPrx create(const ::Glacier2::SSLInfo& info, const ::Glacier2::SessionControlPrx& control)
    {
        return create(info, control, 0);
    }
    ::Glacier2::SessionPrx create(const ::Glacier2::SSLInfo& info, const ::Glacier2::SessionControlPrx& control, const ::Ice::Context& __ctx)
    {
        return create(info, control, &__ctx);
    }
    
private:

    ::Glacier2::SessionPrx create(const ::Glacier2::SSLInfo&, const ::Glacier2::SessionControlPrx&, const ::Ice::Context*);
    
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

class GLACIER2_API Session : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual void destroy(const ::Ice::Context*) = 0;
};

class GLACIER2_API StringSet : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual void add(const ::Ice::StringSeq&, const ::Ice::Context*) = 0;

    virtual void remove(const ::Ice::StringSeq&, const ::Ice::Context*) = 0;

    virtual ::Ice::StringSeq get(const ::Ice::Context*) = 0;
};

class GLACIER2_API IdentitySet : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual void add(const ::Ice::IdentitySeq&, const ::Ice::Context*) = 0;

    virtual void remove(const ::Ice::IdentitySeq&, const ::Ice::Context*) = 0;

    virtual ::Ice::IdentitySeq get(const ::Ice::Context*) = 0;
};

class GLACIER2_API SessionControl : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual ::Glacier2::StringSetPrx categories(const ::Ice::Context*) = 0;

    virtual ::Glacier2::StringSetPrx adapterIds(const ::Ice::Context*) = 0;

    virtual ::Glacier2::IdentitySetPrx identities(const ::Ice::Context*) = 0;

    virtual ::Ice::Int getSessionTimeout(const ::Ice::Context*) = 0;

    virtual void destroy(const ::Ice::Context*) = 0;
};

class GLACIER2_API SessionManager : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual ::Glacier2::SessionPrx create(const ::std::string&, const ::Glacier2::SessionControlPrx&, const ::Ice::Context*) = 0;
};

class GLACIER2_API SSLSessionManager : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual ::Glacier2::SessionPrx create(const ::Glacier2::SSLInfo&, const ::Glacier2::SessionControlPrx&, const ::Ice::Context*) = 0;
};

}

}

namespace IceDelegateM
{

namespace Glacier2
{

class GLACIER2_API Session : virtual public ::IceDelegate::Glacier2::Session,
                             virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual void destroy(const ::Ice::Context*);
};

class GLACIER2_API StringSet : virtual public ::IceDelegate::Glacier2::StringSet,
                               virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual void add(const ::Ice::StringSeq&, const ::Ice::Context*);

    virtual void remove(const ::Ice::StringSeq&, const ::Ice::Context*);

    virtual ::Ice::StringSeq get(const ::Ice::Context*);
};

class GLACIER2_API IdentitySet : virtual public ::IceDelegate::Glacier2::IdentitySet,
                                 virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual void add(const ::Ice::IdentitySeq&, const ::Ice::Context*);

    virtual void remove(const ::Ice::IdentitySeq&, const ::Ice::Context*);

    virtual ::Ice::IdentitySeq get(const ::Ice::Context*);
};

class GLACIER2_API SessionControl : virtual public ::IceDelegate::Glacier2::SessionControl,
                                    virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual ::Glacier2::StringSetPrx categories(const ::Ice::Context*);

    virtual ::Glacier2::StringSetPrx adapterIds(const ::Ice::Context*);

    virtual ::Glacier2::IdentitySetPrx identities(const ::Ice::Context*);

    virtual ::Ice::Int getSessionTimeout(const ::Ice::Context*);

    virtual void destroy(const ::Ice::Context*);
};

class GLACIER2_API SessionManager : virtual public ::IceDelegate::Glacier2::SessionManager,
                                    virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual ::Glacier2::SessionPrx create(const ::std::string&, const ::Glacier2::SessionControlPrx&, const ::Ice::Context*);
};

class GLACIER2_API SSLSessionManager : virtual public ::IceDelegate::Glacier2::SSLSessionManager,
                                       virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual ::Glacier2::SessionPrx create(const ::Glacier2::SSLInfo&, const ::Glacier2::SessionControlPrx&, const ::Ice::Context*);
};

}

}

namespace IceDelegateD
{

namespace Glacier2
{

class GLACIER2_API Session : virtual public ::IceDelegate::Glacier2::Session,
                             virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual void destroy(const ::Ice::Context*);
};

class GLACIER2_API StringSet : virtual public ::IceDelegate::Glacier2::StringSet,
                               virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual void add(const ::Ice::StringSeq&, const ::Ice::Context*);

    virtual void remove(const ::Ice::StringSeq&, const ::Ice::Context*);

    virtual ::Ice::StringSeq get(const ::Ice::Context*);
};

class GLACIER2_API IdentitySet : virtual public ::IceDelegate::Glacier2::IdentitySet,
                                 virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual void add(const ::Ice::IdentitySeq&, const ::Ice::Context*);

    virtual void remove(const ::Ice::IdentitySeq&, const ::Ice::Context*);

    virtual ::Ice::IdentitySeq get(const ::Ice::Context*);
};

class GLACIER2_API SessionControl : virtual public ::IceDelegate::Glacier2::SessionControl,
                                    virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual ::Glacier2::StringSetPrx categories(const ::Ice::Context*);

    virtual ::Glacier2::StringSetPrx adapterIds(const ::Ice::Context*);

    virtual ::Glacier2::IdentitySetPrx identities(const ::Ice::Context*);

    virtual ::Ice::Int getSessionTimeout(const ::Ice::Context*);

    virtual void destroy(const ::Ice::Context*);
};

class GLACIER2_API SessionManager : virtual public ::IceDelegate::Glacier2::SessionManager,
                                    virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual ::Glacier2::SessionPrx create(const ::std::string&, const ::Glacier2::SessionControlPrx&, const ::Ice::Context*);
};

class GLACIER2_API SSLSessionManager : virtual public ::IceDelegate::Glacier2::SSLSessionManager,
                                       virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual ::Glacier2::SessionPrx create(const ::Glacier2::SSLInfo&, const ::Glacier2::SessionControlPrx&, const ::Ice::Context*);
};

}

}

namespace Glacier2
{

class GLACIER2_API Session : virtual public ::Ice::Object
{
public:

    typedef SessionPrx ProxyType;
    typedef SessionPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual void destroy(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___destroy(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void GLACIER2_API __patch__SessionPtr(void*, ::Ice::ObjectPtr&);

class GLACIER2_API StringSet : virtual public ::Ice::Object
{
public:

    typedef StringSetPrx ProxyType;
    typedef StringSetPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual void add(const ::Ice::StringSeq&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___add(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void remove(const ::Ice::StringSeq&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___remove(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::StringSeq get(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___get(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void GLACIER2_API __patch__StringSetPtr(void*, ::Ice::ObjectPtr&);

class GLACIER2_API IdentitySet : virtual public ::Ice::Object
{
public:

    typedef IdentitySetPrx ProxyType;
    typedef IdentitySetPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual void add(const ::Ice::IdentitySeq&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___add(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void remove(const ::Ice::IdentitySeq&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___remove(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::IdentitySeq get(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___get(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void GLACIER2_API __patch__IdentitySetPtr(void*, ::Ice::ObjectPtr&);

class GLACIER2_API SessionControl : virtual public ::Ice::Object
{
public:

    typedef SessionControlPrx ProxyType;
    typedef SessionControlPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::Glacier2::StringSetPrx categories(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___categories(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Glacier2::StringSetPrx adapterIds(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___adapterIds(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Glacier2::IdentitySetPrx identities(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___identities(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::Ice::Int getSessionTimeout(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___getSessionTimeout(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void destroy(const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___destroy(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void GLACIER2_API __patch__SessionControlPtr(void*, ::Ice::ObjectPtr&);

class GLACIER2_API SessionManager : virtual public ::Ice::Object
{
public:

    typedef SessionManagerPrx ProxyType;
    typedef SessionManagerPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::Glacier2::SessionPrx create(const ::std::string&, const ::Glacier2::SessionControlPrx&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___create(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void GLACIER2_API __patch__SessionManagerPtr(void*, ::Ice::ObjectPtr&);

class GLACIER2_API SSLSessionManager : virtual public ::Ice::Object
{
public:

    typedef SSLSessionManagerPrx ProxyType;
    typedef SSLSessionManagerPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::Glacier2::SessionPrx create(const ::Glacier2::SSLInfo&, const ::Glacier2::SessionControlPrx&, const ::Ice::Current& = ::Ice::Current()) = 0;
    ::IceInternal::DispatchStatus ___create(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void GLACIER2_API __patch__SSLSessionManagerPtr(void*, ::Ice::ObjectPtr&);

}

#endif
