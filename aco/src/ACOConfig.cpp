/**
 * @file ACOConfig.cpp
 * @brief ACO configuration provider
 * @author Xiaoguang Sun
 *
 * $LastChangedRevision: 106 $
 *
 * $HeadURL: file:///home/solaris/svn/repositories/heracles/trunk/aco/src/ACOConfig.cpp $
 *
 * $LastChangedBy: solaris $
 *
 * Created:     Jul. 23, 2004
 *
 * $LastChangedDate: 2008-02-12 23:25:36 +0800 (Tue, 12 Feb 2008) $
 */

#include "aco/ACOConfig.h"
#include "aco/Handler.h"
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/util/XMLString.hpp>
#include <iostream>

#ifndef ACOCONF_XML
#define ACOCONF_XML    "acoconf.xml"
#endif

/**
 * @brief ACOConfig::__error definition
 */
std::string ACOConfig::__error = "";

/**
 * @brief ACOConfig::__server definition
 */
std::string ACOConfig::__server = "127.0.0.1:7001";

/**
 * @brief ACOConfig::__license definition
 */
std::string ACOConfig::__license = "";

/**
 * @brief ACOConfig::__domain definition
 */
std::string ACOConfig::__domain = "";

/**
 * @brief ACOConfig::__compress definition
 */
bool ACOConfig::__compress=true;

/**
 * @brief use ssl
 */
bool ACOConfig::__ssl=false;

std::string ACOConfig::__certificate = "cli.crt";

std::string ACOConfig::__ca = "ca.crt";

std::string ACOConfig::__privkey = "cli.key";

/**
 * @brief ACOConfig::__compressionalgorithm definition
 */
unsigned int ACOConfig::__compressionalgorithm=0;

/**
 * @brief ACOConfig::__compressionlevel definition
 */
unsigned int ACOConfig::__compressionlevel=9;

/**
 * @brief ACOConfig::__serviceversion definition
 */
unsigned int ACOConfig::__serviceversion = 1;

/**
 * @brief ACOConfig::__header_block_init_size definition
 */
size_t ACOConfig::__header_block_init_size = 64*1024;

/**
 * @brief ACOConfig::__body_block_init_size definition
 */
size_t ACOConfig::__body_block_init_size = 1024*1024;

/**
 * @brief ACOConfig::__timeout_seconds definition
 */
unsigned int ACOConfig::__timeout_seconds = 10;

/**
 * @brief ACOConfig::__instance definition
 */
ACOConfig ACOConfig::__instance;

ACOConfig* ACOConfig::Instance(){
  return &__instance;
}

ACOConfig::ACOConfig()
{
  try{
    xercesc::XMLPlatformUtils::Initialize();
  }
  catch(const xercesc::XMLException& ex){
    char * errmsg=xercesc::XMLString::transcode(ex.getMessage());
    __error=errmsg;
    xercesc::XMLString::release(&errmsg);
    return;
  }

  const char xmlfile[]=ACOCONF_XML;
  xercesc::SAX2XMLReader* parser = xercesc::XMLReaderFactory::createXMLReader();

  // set xml parser's feature
  parser->setFeature(xercesc::XMLUni::fgSAX2CoreNameSpaces,true);
  parser->setFeature(xercesc::XMLUni::fgSAX2CoreValidation,false);

  ACOConfigSAX2Handler* acohandler = new ACOConfigSAX2Handler();
  parser->setContentHandler(acohandler);
  parser->setErrorHandler(acohandler);
  
  try{
    parser->parse(xmlfile);
  }
  catch (const xercesc::XMLException& ex) {
    char * errmsg=xercesc::XMLString::transcode(ex.getMessage());
    __error=errmsg;
    xercesc::XMLString::release(&errmsg);
    return;
  }
  catch (const xercesc::SAXParseException& ex){
    char * errmsg=xercesc::XMLString::transcode(ex.getMessage());
    __error=errmsg;
    xercesc::XMLString::release(&errmsg);
    return;
  }
  catch (...) {
    __error="Unexpected Exception";
    return;
  }

  delete parser;
  delete acohandler;

  try{
    xercesc::XMLPlatformUtils::Terminate();
  }
  catch(const xercesc::XMLException& ex){
    char * errmsg=xercesc::XMLString::transcode(ex.getMessage());
    __error=errmsg;
    xercesc::XMLString::release(&errmsg);
    return;
  }
  return; 
}

