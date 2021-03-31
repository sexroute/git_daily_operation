/**
 * @file Handler.h
 * @brief ACOConfig SAX2 Handler
 * @author Xiaoguang Sun
 *
 * $LastChangedRevision: 106 $
 *
 * $HeadURL: file:///home/solaris/svn/repositories/heracles/trunk/aco/include/aco/Handler.h $
 *
 * $LastChangedBy: solaris $
 *
 * Created:     Jul. 23, 2004
 *
 * $LastChangedDate: 2008-02-12 23:25:36 +0800 (Tue, 12 Feb 2008) $
 */

#include <xercesc/sax2/DefaultHandler.hpp>

/**
 * forward declaration
 */
class ACOConfig;

/**
 * @class ACOConfigSAX2Handler
 * @brief ACOConfig SAX2 Handler
 */
class ACOConfigSAX2Handler:public xercesc::DefaultHandler
{
private:
  ACOConfig* __config;
public:
  ACOConfigSAX2Handler(){
    __config=ACOConfig::Instance();
  }

  virtual void startElement(const XMLCh* const uri,
          const XMLCh* const localname,
          const XMLCh* const qname,
          const xercesc::Attributes& attrs);

  virtual void characters(const XMLCh *const chars,const unsigned int length);

  virtual void fatalError(const xercesc::SAXParseException&);
};
