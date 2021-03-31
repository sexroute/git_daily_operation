/**
 * @file Handler.h
 * @brief ACOConfig SAX2 Handler
 * @author Xiaoguang Sun
 *
 * $LastChangedRevision: 106 $
 *
 * $HeadURL: file:///home/solaris/svn/repositories/heracles/trunk/aco/src/Handler.cpp $
 *
 * $LastChangedBy: solaris $
 *
 * Created:     Jul. 23, 2004
 *
 * $LastChangedDate: 2008-02-12 23:25:36 +0800 (Tue, 12 Feb 2008) $
 */

#include "aco/ACOConfig.h"
#include "aco/Handler.h"

#ifdef WIN32
#define strncasecmp _strnicmp
#endif

bool unfinished[]={true,true,true,true,true,
                   true,true,true,true,true,
                   true,true,true,true,true};

enum {IGNOREDNODE,SERVERNODE,LICENSENODE,DOMAINNODE,CERTIFICATENODE,
      COMPRESSNODE,COMPRESSIONALGORITHMNODE,SSLNODE,PRIVATEKEYNODE,
      COMPRESSIONLEVELNODE,SERVICEVERSIONNODE,HEADERBLOCKINITSIZENODE,
      BODYBLOCKINITSIZENODE,TIMEOUTNODE,CANODE}currentNode=IGNOREDNODE;

void ACOConfigSAX2Handler::startElement(const XMLCh* const uri,const XMLCh* const localname,
                                        const XMLCh* const qname,const xercesc::Attributes& attrs){
  char* message = xercesc::XMLString::transcode(qname);

  currentNode=IGNOREDNODE;
  if(strncasecmp(message,"Server",sizeof("Server"))==0)
    currentNode=SERVERNODE;
  if(strncasecmp(message,"License",sizeof("License"))==0)
    currentNode=LICENSENODE;
  if(strncasecmp(message,"Domain",sizeof("Domain"))==0)
    currentNode=DOMAINNODE;
  if(strncasecmp(message,"CompressEnabled",sizeof("ChecksumEnabled"))==0)
    currentNode=COMPRESSNODE;
  if(strncasecmp(message,"CompressionAlgorithm",sizeof("CompressionAlgorithm"))==0)
    currentNode=COMPRESSIONALGORITHMNODE;
  if(strncasecmp(message,"CompressionLevel",sizeof("CompressionLevel"))==0)
    currentNode=COMPRESSIONLEVELNODE;
  if(strncasecmp(message,"ServiceVersion",sizeof("ServiceVersion"))==0)
    currentNode=SERVICEVERSIONNODE;
  if(strncasecmp(message,"HeaderBlockInitSize",sizeof("HeaderBlockInitSize"))==0)
    currentNode=HEADERBLOCKINITSIZENODE;
  if(strncasecmp(message,"BodyBlockInitSize",sizeof("BodyBlockInitSize"))==0)
    currentNode=BODYBLOCKINITSIZENODE;
  if(strncasecmp(message,"Timeout",sizeof("Timeout"))==0)
    currentNode=TIMEOUTNODE;
  if(strncasecmp(message,"SSL",sizeof("SSL"))==0)
    currentNode=SSLNODE;
  if(strncasecmp(message,"CA",sizeof("CA"))==0)
    currentNode=CANODE;
  if(strncasecmp(message,"Certificate",sizeof("Certificate"))==0)
    currentNode=CERTIFICATENODE;
  if(strncasecmp(message,"PrivateKey",sizeof("PrivateKey"))==0)
    currentNode=PRIVATEKEYNODE;

  xercesc::XMLString::release(&message);
}

void ACOConfigSAX2Handler::characters(const XMLCh *const chars,const unsigned int length){
  char* content = xercesc::XMLString::transcode(chars);

  switch(currentNode){
  case SERVERNODE:
    if(unfinished[SERVERNODE])
      __config->__server=content;
    unfinished[SERVERNODE]=false;
    break;
  case LICENSENODE:
    if(unfinished[LICENSENODE])
      __config->__license=content;
    unfinished[LICENSENODE]=false;
    break;
  case DOMAINNODE:
    if(unfinished[DOMAINNODE])
      __config->__domain=content;
    unfinished[DOMAINNODE]=false;
    break;
  case CANODE:
    if(unfinished[CANODE])
      __config->__ca=content;
    unfinished[CANODE]=false;
    break;
  case CERTIFICATENODE:
    if(unfinished[CERTIFICATENODE])
      __config->__certificate=content;
    unfinished[CERTIFICATENODE]=false;
    break;
  case PRIVATEKEYNODE:
    if(unfinished[PRIVATEKEYNODE])
      __config->__privkey=content;
    unfinished[PRIVATEKEYNODE]=false;
    break;
  case COMPRESSNODE:
    if(unfinished[COMPRESSNODE])
      __config->__compress=strncasecmp(content,"true",sizeof("true"))==0;
    unfinished[COMPRESSNODE]=false;
    break;
  case COMPRESSIONALGORITHMNODE:
    if(unfinished[COMPRESSIONALGORITHMNODE])
      __config->__compressionalgorithm=atoi(content);
    unfinished[COMPRESSIONALGORITHMNODE]=false;
    break;
  case COMPRESSIONLEVELNODE:
    if(unfinished[COMPRESSIONLEVELNODE])
      __config->__compressionlevel=atoi(content);
    unfinished[COMPRESSIONLEVELNODE]=false;
    break;
  case SERVICEVERSIONNODE:
    if(unfinished[SERVICEVERSIONNODE])
      __config->__serviceversion=atoi(content);
    unfinished[SERVICEVERSIONNODE]=false;
    break;
  case HEADERBLOCKINITSIZENODE:
    if(unfinished[HEADERBLOCKINITSIZENODE]){
      unsigned int len=strlen(content)-1;
      unsigned char unit=content[len];
      content[len]=0;
      size_t size=atoi(content);
      switch(unit){
      case 'K':
      case 'k':
        size*=1024;
        break;
      case 'm':
      case 'M':
        size*=1024*1024;
        break;
      case 'g':
      case 'G':
        size*=1024*1024*1024;
        break;
      }
      __config->__header_block_init_size=size;
    }
    unfinished[HEADERBLOCKINITSIZENODE]=false;
    break;
  case BODYBLOCKINITSIZENODE:
    if(unfinished[BODYBLOCKINITSIZENODE]){
      unsigned int len=strlen(content)-1;
      unsigned char unit=content[len];
      content[len]=0;
      size_t size=atoi(content);
      switch(unit){
      case 'K':
      case 'k':
        size*=1024;
        break;
      case 'm':
      case 'M':
        size*=1024*1024;
        break;
      case 'g':
      case 'G':
        size*=1024*1024*1024;
        break;
      }
      __config->__body_block_init_size=size;
    }
    unfinished[BODYBLOCKINITSIZENODE]=false;
    break;
  case TIMEOUTNODE:
    if(unfinished[TIMEOUTNODE]){
      unsigned int len=strlen(content)-1;
      unsigned char unit=content[len];
      content[len]=0;
      unsigned int timeout=atoi(content);
      switch(unit){
      case 'M':
      case 'm':
        timeout*=60;
        break;
      case 'H':
      case 'h':
        timeout*=3600;
        break;
      default:
        break;
      }
      __config->__timeout_seconds=timeout;
    }
    unfinished[TIMEOUTNODE]=false;
    break;
  case SSLNODE:
    if(unfinished[SSLNODE]){
      __config->__ssl=strncasecmp(content,"true",sizeof("true"))==0;
    }
    unfinished[SSLNODE]=false;
    break;
  }

  xercesc::XMLString::release(&content);
}

void ACOConfigSAX2Handler::fatalError(const xercesc::SAXParseException& ex){
  char* message = xercesc::XMLString::transcode(ex.getMessage());

  __config->__error="Fatal Error: ";
  __config->__error+=message;
  __config->__error+=" at line: ";
  __config->__error+=ex.getLineNumber();
  
 // std::printf("%s\n",__config->__error.c_str());

  xercesc::XMLString::release(&message);
}
