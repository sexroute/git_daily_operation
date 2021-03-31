// **********************************************************************
//
// Copyright (c) 2003-2007 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************
#include <string>
#include <ice/Handle.h>
#include <ice/Logger.h>

#ifndef LOG_I_H
#define LOG_I_H

class LogI : public Ice::Logger
{
public:

    LogI();

    virtual void print(const std::string&);
    virtual void trace(const std::string&, const std::string&);
    virtual void warning(const std::string&);
    virtual void error(const std::string&);

    void message(const std::string&, const int& iLogInd = 0);
    //void setHandle(HWND);
private:

    void post(const std::string&, const int& iLogInd = 0);

    std::string _buffer;
    HWND _hwnd;
};

typedef IceUtil::Handle<LogI> LogIPtr;

#endif
