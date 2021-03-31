#!/usr/bin/sh
echo .RC   = $1

if [ ! -e $1 ] ; then
  echo 错误: 请指定资源文件\(*.rc\)
fi

if [ -e $1 ] ; then

# WCHEAD 
#echo getting HEAD version
#WCHEAD=`svn log -r HEAD ^/ | grep -E '^r[0-9]{1,}' | sed 's/^r\([0-9]\{1,\}\)/\1/' | awk '/^([0-9]+)/{print $1}'`
#echo HEAD  = $WCHEAD

#echo FILE_VERSION = $2.$3.$4.$WCHEAD
echo PRODUCT_VERSION = $5.$6.$7.$8

sed -e "s/\(FILEVERSION[^0-9]\+\)\([0-9]\{1,\}[,\.]\)\{3\}[0-9]\{1,\}/\1$5,$6,$7,\$WCREV\$/" \
    -e "s/\(FileVersion[^0-9]\+\)\([0-9]\{1,\}[,\.]\)\{3\}[0-9]\{1,\}/\1$5.$6.$7.\$WCREV\$/" \
    -e "s/\(PRODUCTVERSION[^0-9]\+\)\([0-9]\{1,\}[,\.]\x20\?\)\{3\}[0-9]\{1,\}/\1$5,$6,$7,$8/" \
    -e "s/\(ProductVersion[^0-9]\+\)\([0-9]\{1,\}[,\.]\x20\?\)\{3\}[0-9]\{1,\}/\1$5.$6.$7.$8/" \
    -e "s/\(\xB0\xE6\xB1\xBE\x20\+\)\([0-9]\{1,\}[,\.]\)\{3\}[0-9]\{1,\}/\1$5.$6.$7.$8/" \
 $1 > $1.tmpl
 
SubWCRev.exe `(dirname $1)` $1.tmpl $1

rm -f $1.tmpl
fi
