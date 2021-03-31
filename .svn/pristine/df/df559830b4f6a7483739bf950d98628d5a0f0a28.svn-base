#!/usr/bin/sh

usage() 
{
	echo USAGE : `basename $0` src 
}

if test $# -gt 2; then
	usage
	exit -1
fi

FILE_NAME=$1
DIR_NAME=`dirname $1`

if test ! -e $FILE_NAME ; then
	echo ERROR : There is no file $FILE_NAME. 	
	usage 	
	exit -1 
fi


VERSION=`strings -e l $FILE_NAME | grep -A 1  ProductVersion | grep '[0-9]*\(\.[0-9]*\)\{3\}'` 

NAME=`basename $FILE_NAME | sed -e 's/\..*//g'`
 	
cp $FILE_NAME $DIR_NAME/$NAME-$VERSION.exe

exit 0

