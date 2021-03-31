// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

// Ice version 3.2.1
// Generated from file `FileServer.ice'

#ifndef __IcePatch2_FileServer_h__
#define __IcePatch2_FileServer_h__

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
#include <IcePatch2/FileInfo.h>
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

#ifndef ICE_PATCH2_API
#   ifdef ICE_PATCH2_API_EXPORTS
#       define ICE_PATCH2_API ICE_DECLSPEC_EXPORT
#   else
#       define ICE_PATCH2_API ICE_DECLSPEC_IMPORT
#   endif
#endif

namespace IceProxy
{

namespace IcePatch2
{

class FileServer;
ICE_PATCH2_API bool operator==(const FileServer&, const FileServer&);
ICE_PATCH2_API bool operator!=(const FileServer&, const FileServer&);
ICE_PATCH2_API bool operator<(const FileServer&, const FileServer&);
ICE_PATCH2_API bool operator<=(const FileServer&, const FileServer&);
ICE_PATCH2_API bool operator>(const FileServer&, const FileServer&);
ICE_PATCH2_API bool operator>=(const FileServer&, const FileServer&);

class Admin;
ICE_PATCH2_API bool operator==(const Admin&, const Admin&);
ICE_PATCH2_API bool operator!=(const Admin&, const Admin&);
ICE_PATCH2_API bool operator<(const Admin&, const Admin&);
ICE_PATCH2_API bool operator<=(const Admin&, const Admin&);
ICE_PATCH2_API bool operator>(const Admin&, const Admin&);
ICE_PATCH2_API bool operator>=(const Admin&, const Admin&);

}

}

namespace IcePatch2
{

class FileServer;
ICE_PATCH2_API bool operator==(const FileServer&, const FileServer&);
ICE_PATCH2_API bool operator!=(const FileServer&, const FileServer&);
ICE_PATCH2_API bool operator<(const FileServer&, const FileServer&);
ICE_PATCH2_API bool operator<=(const FileServer&, const FileServer&);
ICE_PATCH2_API bool operator>(const FileServer&, const FileServer&);
ICE_PATCH2_API bool operator>=(const FileServer&, const FileServer&);

class Admin;
ICE_PATCH2_API bool operator==(const Admin&, const Admin&);
ICE_PATCH2_API bool operator!=(const Admin&, const Admin&);
ICE_PATCH2_API bool operator<(const Admin&, const Admin&);
ICE_PATCH2_API bool operator<=(const Admin&, const Admin&);
ICE_PATCH2_API bool operator>(const Admin&, const Admin&);
ICE_PATCH2_API bool operator>=(const Admin&, const Admin&);

}

namespace IceInternal
{

ICE_PATCH2_API void incRef(::IcePatch2::FileServer*);
ICE_PATCH2_API void decRef(::IcePatch2::FileServer*);

ICE_PATCH2_API void incRef(::IceProxy::IcePatch2::FileServer*);
ICE_PATCH2_API void decRef(::IceProxy::IcePatch2::FileServer*);

ICE_PATCH2_API void incRef(::IcePatch2::Admin*);
ICE_PATCH2_API void decRef(::IcePatch2::Admin*);

ICE_PATCH2_API void incRef(::IceProxy::IcePatch2::Admin*);
ICE_PATCH2_API void decRef(::IceProxy::IcePatch2::Admin*);

}

namespace IcePatch2
{

typedef ::IceInternal::Handle< ::IcePatch2::FileServer> FileServerPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::IcePatch2::FileServer> FileServerPrx;

ICE_PATCH2_API void __write(::IceInternal::BasicStream*, const FileServerPrx&);
ICE_PATCH2_API void __read(::IceInternal::BasicStream*, FileServerPrx&);
ICE_PATCH2_API void __write(::IceInternal::BasicStream*, const FileServerPtr&);
ICE_PATCH2_API void __patch__FileServerPtr(void*, ::Ice::ObjectPtr&);

ICE_PATCH2_API void __addObject(const FileServerPtr&, ::IceInternal::GCCountMap&);
ICE_PATCH2_API bool __usesClasses(const FileServerPtr&);
ICE_PATCH2_API void __decRefUnsafe(const FileServerPtr&);
ICE_PATCH2_API void __clearHandleUnsafe(FileServerPtr&);

typedef ::IceInternal::Handle< ::IcePatch2::Admin> AdminPtr;
typedef ::IceInternal::ProxyHandle< ::IceProxy::IcePatch2::Admin> AdminPrx;

ICE_PATCH2_API void __write(::IceInternal::BasicStream*, const AdminPrx&);
ICE_PATCH2_API void __read(::IceInternal::BasicStream*, AdminPrx&);
ICE_PATCH2_API void __write(::IceInternal::BasicStream*, const AdminPtr&);
ICE_PATCH2_API void __patch__AdminPtr(void*, ::Ice::ObjectPtr&);

ICE_PATCH2_API void __addObject(const AdminPtr&, ::IceInternal::GCCountMap&);
ICE_PATCH2_API bool __usesClasses(const AdminPtr&);
ICE_PATCH2_API void __decRefUnsafe(const AdminPtr&);
ICE_PATCH2_API void __clearHandleUnsafe(AdminPtr&);

}

namespace IcePatch2
{

typedef ::std::vector< ::Ice::ByteSeq> ByteSeqSeq;

class __U__ByteSeqSeq { };
ICE_PATCH2_API void __write(::IceInternal::BasicStream*, const ::Ice::ByteSeq*, const ::Ice::ByteSeq*, __U__ByteSeqSeq);
ICE_PATCH2_API void __read(::IceInternal::BasicStream*, ByteSeqSeq&, __U__ByteSeqSeq);

class ICE_PATCH2_API PartitionOutOfRangeException : public ::Ice::UserException
{
public:

    PartitionOutOfRangeException() {}
    virtual ~PartitionOutOfRangeException() throw();

    virtual ::std::string ice_name() const;
    virtual ::Ice::Exception* ice_clone() const;
    virtual void ice_throw() const;

    static const ::IceInternal::UserExceptionFactoryPtr& ice_factory();

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);

    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

static PartitionOutOfRangeException __PartitionOutOfRangeException_init;

class ICE_PATCH2_API FileAccessException : public ::Ice::UserException
{
public:

    FileAccessException() {}
    explicit FileAccessException(const ::std::string&);
    virtual ~FileAccessException() throw();

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

}

namespace IcePatch2
{

class ICE_PATCH2_API AMI_FileServer_getFileInfoSeq : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response(const ::IcePatch2::FileInfoSeq&) = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::IcePatch2::FileServerPrx&, ::Ice::Int, const ::Ice::Context*);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::IcePatch2::AMI_FileServer_getFileInfoSeq> AMI_FileServer_getFileInfoSeqPtr;

class ICE_PATCH2_API AMI_FileServer_getFileCompressed : public ::IceInternal::OutgoingAsync
{
public:

    virtual void ice_response(const ::std::pair<const ::Ice::Byte*, const ::Ice::Byte*>&) = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;

    void __invoke(const ::IcePatch2::FileServerPrx&, const ::std::string&, ::Ice::Int, ::Ice::Int, const ::Ice::Context*);

protected:

    virtual void __response(bool);
};

typedef ::IceUtil::Handle< ::IcePatch2::AMI_FileServer_getFileCompressed> AMI_FileServer_getFileCompressedPtr;

class ICE_PATCH2_API AMD_FileServer_getFileCompressed : virtual public ::IceUtil::Shared
{
public:

    virtual void ice_response(const ::std::pair<const ::Ice::Byte*, const ::Ice::Byte*>&) = 0;
    virtual void ice_exception(const ::Ice::Exception&) = 0;
    virtual void ice_exception(const ::std::exception&) = 0;
    virtual void ice_exception() = 0;
};

typedef ::IceUtil::Handle< ::IcePatch2::AMD_FileServer_getFileCompressed> AMD_FileServer_getFileCompressedPtr;

}

namespace IceAsync
{

namespace IcePatch2
{

class ICE_PATCH2_API AMD_FileServer_getFileCompressed : public ::IcePatch2::AMD_FileServer_getFileCompressed, public ::IceInternal::IncomingAsync
{
public:

    AMD_FileServer_getFileCompressed(::IceInternal::Incoming&);

    virtual void ice_response(const ::std::pair<const ::Ice::Byte*, const ::Ice::Byte*>&);
    virtual void ice_exception(const ::Ice::Exception&);
    virtual void ice_exception(const ::std::exception&);
    virtual void ice_exception();
};

}

}

namespace IceProxy
{

namespace IcePatch2
{

class ICE_PATCH2_API FileServer : virtual public ::IceProxy::Ice::Object
{
public:

    ::IcePatch2::FileInfoSeq getFileInfoSeq(::Ice::Int partition)
    {
        return getFileInfoSeq(partition, 0);
    }
    ::IcePatch2::FileInfoSeq getFileInfoSeq(::Ice::Int partition, const ::Ice::Context& __ctx)
    {
        return getFileInfoSeq(partition, &__ctx);
    }
    
private:

    ::IcePatch2::FileInfoSeq getFileInfoSeq(::Ice::Int, const ::Ice::Context*);
    
public:
    void getFileInfoSeq_async(const ::IcePatch2::AMI_FileServer_getFileInfoSeqPtr&, ::Ice::Int);
    void getFileInfoSeq_async(const ::IcePatch2::AMI_FileServer_getFileInfoSeqPtr&, ::Ice::Int, const ::Ice::Context&);

    ::IcePatch2::ByteSeqSeq getChecksumSeq()
    {
        return getChecksumSeq(0);
    }
    ::IcePatch2::ByteSeqSeq getChecksumSeq(const ::Ice::Context& __ctx)
    {
        return getChecksumSeq(&__ctx);
    }
    
private:

    ::IcePatch2::ByteSeqSeq getChecksumSeq(const ::Ice::Context*);
    
public:

    ::Ice::ByteSeq getChecksum()
    {
        return getChecksum(0);
    }
    ::Ice::ByteSeq getChecksum(const ::Ice::Context& __ctx)
    {
        return getChecksum(&__ctx);
    }
    
private:

    ::Ice::ByteSeq getChecksum(const ::Ice::Context*);
    
public:

    ::Ice::ByteSeq getFileCompressed(const ::std::string& path, ::Ice::Int pos, ::Ice::Int num)
    {
        return getFileCompressed(path, pos, num, 0);
    }
    ::Ice::ByteSeq getFileCompressed(const ::std::string& path, ::Ice::Int pos, ::Ice::Int num, const ::Ice::Context& __ctx)
    {
        return getFileCompressed(path, pos, num, &__ctx);
    }
    
private:

    ::Ice::ByteSeq getFileCompressed(const ::std::string&, ::Ice::Int, ::Ice::Int, const ::Ice::Context*);
    
public:
    void getFileCompressed_async(const ::IcePatch2::AMI_FileServer_getFileCompressedPtr&, const ::std::string&, ::Ice::Int, ::Ice::Int);
    void getFileCompressed_async(const ::IcePatch2::AMI_FileServer_getFileCompressedPtr&, const ::std::string&, ::Ice::Int, ::Ice::Int, const ::Ice::Context&);
    
    static const ::std::string& ice_staticId();

private: 

    virtual ::IceInternal::Handle< ::IceDelegateM::Ice::Object> __createDelegateM();
    virtual ::IceInternal::Handle< ::IceDelegateD::Ice::Object> __createDelegateD();
};

class ICE_PATCH2_API Admin : virtual public ::IceProxy::Ice::Object
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

namespace IcePatch2
{

class ICE_PATCH2_API FileServer : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual ::IcePatch2::FileInfoSeq getFileInfoSeq(::Ice::Int, const ::Ice::Context*) = 0;

    virtual ::IcePatch2::ByteSeqSeq getChecksumSeq(const ::Ice::Context*) = 0;

    virtual ::Ice::ByteSeq getChecksum(const ::Ice::Context*) = 0;

    virtual ::Ice::ByteSeq getFileCompressed(const ::std::string&, ::Ice::Int, ::Ice::Int, const ::Ice::Context*) = 0;
};

class ICE_PATCH2_API Admin : virtual public ::IceDelegate::Ice::Object
{
public:

    virtual void shutdown(const ::Ice::Context*) = 0;
};

}

}

namespace IceDelegateM
{

namespace IcePatch2
{

class ICE_PATCH2_API FileServer : virtual public ::IceDelegate::IcePatch2::FileServer,
                                  virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual ::IcePatch2::FileInfoSeq getFileInfoSeq(::Ice::Int, const ::Ice::Context*);

    virtual ::IcePatch2::ByteSeqSeq getChecksumSeq(const ::Ice::Context*);

    virtual ::Ice::ByteSeq getChecksum(const ::Ice::Context*);

    virtual ::Ice::ByteSeq getFileCompressed(const ::std::string&, ::Ice::Int, ::Ice::Int, const ::Ice::Context*);
};

class ICE_PATCH2_API Admin : virtual public ::IceDelegate::IcePatch2::Admin,
                             virtual public ::IceDelegateM::Ice::Object
{
public:

    virtual void shutdown(const ::Ice::Context*);
};

}

}

namespace IceDelegateD
{

namespace IcePatch2
{

class ICE_PATCH2_API FileServer : virtual public ::IceDelegate::IcePatch2::FileServer,
                                  virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual ::IcePatch2::FileInfoSeq getFileInfoSeq(::Ice::Int, const ::Ice::Context*);

    virtual ::IcePatch2::ByteSeqSeq getChecksumSeq(const ::Ice::Context*);

    virtual ::Ice::ByteSeq getChecksum(const ::Ice::Context*);

    virtual ::Ice::ByteSeq getFileCompressed(const ::std::string&, ::Ice::Int, ::Ice::Int, const ::Ice::Context*);
};

class ICE_PATCH2_API Admin : virtual public ::IceDelegate::IcePatch2::Admin,
                             virtual public ::IceDelegateD::Ice::Object
{
public:

    virtual void shutdown(const ::Ice::Context*);
};

}

}

namespace IcePatch2
{

class ICE_PATCH2_API FileServer : virtual public ::Ice::Object
{
public:

    typedef FileServerPrx ProxyType;
    typedef FileServerPtr PointerType;
    
    virtual ::Ice::ObjectPtr ice_clone() const;

    virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
    virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
    virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
    static const ::std::string& ice_staticId();

    virtual ::IcePatch2::FileInfoSeq getFileInfoSeq(::Ice::Int, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___getFileInfoSeq(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual ::IcePatch2::ByteSeqSeq getChecksumSeq(const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___getChecksumSeq(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual ::Ice::ByteSeq getChecksum(const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___getChecksum(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual void getFileCompressed_async(const ::IcePatch2::AMD_FileServer_getFileCompressedPtr&, const ::std::string&, ::Ice::Int, ::Ice::Int, const ::Ice::Current& = ::Ice::Current()) const = 0;
    ::IceInternal::DispatchStatus ___getFileCompressed(::IceInternal::Incoming&, const ::Ice::Current&) const;

    virtual ::IceInternal::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);

    virtual void __write(::IceInternal::BasicStream*) const;
    virtual void __read(::IceInternal::BasicStream*, bool);
    virtual void __write(const ::Ice::OutputStreamPtr&) const;
    virtual void __read(const ::Ice::InputStreamPtr&, bool);
};

void ICE_PATCH2_API __patch__FileServerPtr(void*, ::Ice::ObjectPtr&);

class ICE_PATCH2_API Admin : virtual public ::Ice::Object
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

void ICE_PATCH2_API __patch__AdminPtr(void*, ::Ice::ObjectPtr&);

}

#endif
