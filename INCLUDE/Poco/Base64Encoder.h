//
// Base64Encoder.h
//
// $Id: //poco/1.3/Foundation/include/Poco/Base64Encoder.h#2 $
//
// Library: Foundation
// Package: Streams
// Module:  Base64
//
// Definition of class Base64Encoder.
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


#ifndef Foundation_Base64Encoder_INCLUDED
#define Foundation_Base64Encoder_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/UnbufferedStreamBuf.h"
#include <ostream>


namespace Poco {


class Foundation_API Base64EncoderBuf: public UnbufferedStreamBuf
	/// This streambuf base64-encodes all data written
	/// to it and forwards it to a connected
	/// ostream.
{
public:
	Base64EncoderBuf(std::ostream& ostr);
	~Base64EncoderBuf();
	
	int close();
		/// Closes the stream buffer.

	void setLineLength(int lineLength);
		/// Specify the line length.
		///
		/// After the given number of characters have been written, 
		/// a newline character will be written.
		///
		/// Specify 0 for an unlimited line length.

	int getLineLength() const;
		/// Returns the currently set line length.
	
private:
	int writeToDevice(char c);

	unsigned char _group[3];
	int           _groupLength;
	int           _pos;
	int           _lineLength;
	std::ostream& _ostr;
	
	static const unsigned char OUT_ENCODING[64];
	
	friend class Base64DecoderBuf;
};


class Foundation_API Base64EncoderIOS: public virtual std::ios
	/// The base class for Base64Encoder.
	///
	/// This class is needed to ensure the correct initialization
	/// order of the stream buffer and base classes.
{
public:
	Base64EncoderIOS(std::ostream& ostr);
	~Base64EncoderIOS();
	int close();
	Base64EncoderBuf* rdbuf();

protected:
	Base64EncoderBuf _buf;
};


class Foundation_API Base64Encoder: public Base64EncoderIOS, public std::ostream
	/// This ostream base64-encodes all data
	/// written to it and forwards it to
	/// a connected ostream.
	/// Always call close() when done
	/// writing data, to ensure proper
	/// completion of the encoding operation.
{
public:
	Base64Encoder(std::ostream& ostr);
	~Base64Encoder();
};


} // namespace Poco


#endif // Foundation_Base64Encoder_INCLUDED
