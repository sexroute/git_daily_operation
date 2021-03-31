//
// Base64Decoder.h
//
// $Id: //poco/1.3/Foundation/include/Poco/Base64Decoder.h#1 $
//
// Library: Foundation
// Package: Streams
// Module:  Base64
//
// Definition of class Base64Decoder.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef Foundation_Base64Decoder_INCLUDED
#define Foundation_Base64Decoder_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/UnbufferedStreamBuf.h"
#include <istream>


namespace Poco {


class Foundation_API Base64DecoderBuf: public UnbufferedStreamBuf
	/// This streambuf base64-decodes all data read
	/// from the istream connected to it.
{
public:
	Base64DecoderBuf(std::istream& istr);
	~Base64DecoderBuf();
	
private:
	int readFromDevice();
	int readOne();

	unsigned char _group[3];
	int           _groupLength;
	int           _groupIndex;
	std::istream& _istr;
	
	static unsigned char IN_ENCODING[256];
	static bool          IN_ENCODING_INIT;
};


class Foundation_API Base64DecoderIOS: public virtual std::ios
	/// The base class for Base64Decoder.
	///
	/// This class is needed to ensure the correct initialization
	/// order of the stream buffer and base classes.
{
public:
	Base64DecoderIOS(std::istream& istr);
	~Base64DecoderIOS();
	Base64DecoderBuf* rdbuf();

protected:
	Base64DecoderBuf _buf;
};


class Foundation_API Base64Decoder: public Base64DecoderIOS, public std::istream
	/// This istream base64-decodes all data
	/// read from the istream connected to it.
{
public:
	Base64Decoder(std::istream& istr);
	~Base64Decoder();
};


} // namespace Poco


#endif // Foundation_Base64Decoder_INCLUDED
