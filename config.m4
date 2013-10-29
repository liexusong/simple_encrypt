dnl $Id$
dnl config.m4 for extension simple_encrypt

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(simple_encrypt, for simple_encrypt support,
dnl Make sure that the comment is aligned:
dnl [  --with-simple_encrypt             Include simple_encrypt support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(simple_encrypt, whether to enable simple_encrypt support,
dnl Make sure that the comment is aligned:
[  --enable-simple_encrypt           Enable simple_encrypt support])

if test "$PHP_SIMPLE_ENCRYPT" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-simple_encrypt -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/simple_encrypt.h"  # you most likely want to change this
  dnl if test -r $PHP_SIMPLE_ENCRYPT/$SEARCH_FOR; then # path given as parameter
  dnl   SIMPLE_ENCRYPT_DIR=$PHP_SIMPLE_ENCRYPT
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for simple_encrypt files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       SIMPLE_ENCRYPT_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$SIMPLE_ENCRYPT_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the simple_encrypt distribution])
  dnl fi

  dnl # --with-simple_encrypt -> add include path
  dnl PHP_ADD_INCLUDE($SIMPLE_ENCRYPT_DIR/include)

  dnl # --with-simple_encrypt -> check for lib and symbol presence
  dnl LIBNAME=simple_encrypt # you may want to change this
  dnl LIBSYMBOL=simple_encrypt # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $SIMPLE_ENCRYPT_DIR/lib, SIMPLE_ENCRYPT_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_SIMPLE_ENCRYPTLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong simple_encrypt lib version or lib not found])
  dnl ],[
  dnl   -L$SIMPLE_ENCRYPT_DIR/lib -lm -ldl
  dnl ])
  dnl
  dnl PHP_SUBST(SIMPLE_ENCRYPT_SHARED_LIBADD)

  PHP_NEW_EXTENSION(simple_encrypt, simple_encrypt.c encrypt.c bit.c, $ext_shared)
fi
