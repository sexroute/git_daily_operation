// BlobStream.h: interface for the CBlobStream class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLOBSTREAM_H__3BFB518C_1173_42D4_86EE_C6CB383B830E__INCLUDED_)
#define AFX_BLOBSTREAM_H__3BFB518C_1173_42D4_86EE_C6CB383B830E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <CustomDB.h>
#include <atldbcli.h>

// CBlobStream
//
//		This class is adapted from Microsoft's AOTBLOB sample's CISSHelper
//
class CBlobStream : public ISequentialStream  
{
public:
	CBlobStream();
	virtual ~CBlobStream();

	virtual void Clear();

	void *GetBuffer() {return (void*)m_pBuffer;}

	//
	// ISequentialStream interface implementation.
	//
    virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
    
    virtual ULONG STDMETHODCALLTYPE AddRef( void);
    
    virtual ULONG STDMETHODCALLTYPE Release( void);
 
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE Read( 
        /* [length_is][size_is][out] */ void __RPC_FAR *pv,
        /* [in] */ ULONG cb,
        /* [out] */ ULONG __RPC_FAR *pcbRead);
    
    virtual /* [local] */ HRESULT STDMETHODCALLTYPE Write( 
        /* [size_is][in] */ const void __RPC_FAR *pv,
        /* [in] */ ULONG cb,
        /* [out] */ ULONG __RPC_FAR *pcbWritten);

	HRESULT WriteFromStream(IStream *pStream, ULONG cb, ULONG *pcbWritten);

protected:
	BYTE*		m_pBuffer;
	ULONG		m_nLength,
				m_iPosRead,
				m_iPosWrite,
				m_nRef;
	DBSTATUS	m_dbStatus;
};

#endif // !defined(AFX_BLOBSTREAM_H__3BFB518C_1173_42D4_86EE_C6CB383B830E__INCLUDED_)
