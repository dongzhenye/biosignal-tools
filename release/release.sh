#!/bin/bash 
# make a new release of the biosig4c++ toolbox
#   
#  usage:
#    ./release.sh VERSION_NAME
#
#  for example:
#    ./release.sh 1.6.1
#
# Copyright (C) 2004,2008,2011,2012,2013,2014 Alois Schloegl 



#####################################
# SOURCE RELEASE
#####################################

CWD=`pwd`
BIOSIG4M_DIR=$CWD/biosig4matlab
BIOSIG4C_DIR=$CWD/biosig4c++-$1

echo "== prepare space (delete previous attempts) =="
rm -rf biosig-code $BIOSIG4M_DIR $BIOSIG4C_DIR $BIOSIG4M_DIR/../NaN $BIOSIG4M_DIR/../tsa
rm  biosig-$1*.gz biosig-$1*.zip biosig4c++[-_]$1*.gz  biosig4c++-$1*.zip $B4OM

SRCDIR=$HOME"/src"
SRCDIR=`pwd`"/../.."

echo "== clone biosig =="
git clone $SRCDIR/biosig-code biosig-code 
mv biosig-code/biosig4matlab $BIOSIG4M_DIR
mv biosig-code/biosig4c++    $BIOSIG4C_DIR

echo "==  make biosig4c++-$1.src.tar.gz =="
VERSIONFILE=$BIOSIG4C_DIR/VERSION
echo '# BioSig http://biosig.sf.net/' > $VERSIONFILE
echo -e '# Version:\tbiosig4c++/libbiosig\t' $1 >> $VERSIONFILE
echo -n -e '# Date:\t' >> $VERSIONFILE
date +%Y-%m-%d >> $VERSIONFILE


DESCRIPTION=$BIOSIG4C_DIR"/DESCRIPTION"
echo 'Name: BioSig' > $DESCRIPTION
echo 'Version:' $1 >> $DESCRIPTION
echo 'Date:' $(date +%Y-%m-%d) >> $DESCRIPTION
echo 'Author: Alois Schloegl <alois.schloegl@ist.ac.at>' >> $DESCRIPTION
echo 'Maintainer: Alois Schloegl' >> $DESCRIPTION
echo 'Title: BioSig' >> $DESCRIPTION
echo 'Description: A software library for biomedical signal processing.' >> $DESCRIPTION
echo 'Depends: octave (> 2.9.0),' >> $DESCRIPTION
echo 'License: GPL version 3 or later' >> $DESCRIPTION
echo 'Url: http://biosig.sf.net' >> $DESCRIPTION

rm $BIOSIG4C_DIR/todo.txt
(cd $CWD/biosig4c++-$1 && \
	gawk -f eventcodes.awk "extern/eventcodes.txt" && \
	awk -f annotatedECG.awk "extern/11073-10102-AnnexB.txt" > "11073-10102-AnnexB.i" && \
	awk -f units.awk "extern/units.csv" > "units.i" ) 

# 'tar h ...' needed for symbolic link extern/units.csv, etc. 
(cd $CWD; tar acvfh biosig4c++-$1.src.tar.gz biosig4c++-$1 > /dev/null; cd $SRCDIR) 
ln $CWD/biosig4c++-$1.src.tar.gz $CWD/biosig4c++_$1.orig.tar.gz 

#####################################
# DEBIAN PACKAGING
#####################################

rm -rf $BIOSIG4C_DIR
tar xvf $CWD/biosig4c++_$1.orig.tar.gz
cd $BIOSIG4C_DIR
cp -r ../debian debian
debuild


