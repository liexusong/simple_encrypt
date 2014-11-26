/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2007 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Liexusong <280259971@qq.com>                                 |
  +----------------------------------------------------------------------+
*/

/* $Id: header,v 1.16.2.1.2.1 2007/01/01 19:32:09 iliaa Exp $ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "ext/standard/md5.h"
#include "php_simple_encrypt.h"
#include "encrypt.h"

/* If you declare any globals in php_simple_encrypt.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(simple_encrypt)
*/

/* True global resources - no need for thread safety here */
static int le_simple_encrypt;

/* {{{ simple_encrypt_functions[]
 *
 * Every user visible function must have an entry in simple_encrypt_functions[].
 */
zend_function_entry simple_encrypt_functions[] = {
    PHP_FE(se_encrypt,   NULL)
    PHP_FE(se_decrypt,   NULL)
    {NULL, NULL, NULL}
};
/* }}} */

/* {{{ simple_encrypt_module_entry
 */
zend_module_entry simple_encrypt_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    "simple_encrypt",
    simple_encrypt_functions,
    PHP_MINIT(simple_encrypt),
    PHP_MSHUTDOWN(simple_encrypt),
    PHP_RINIT(simple_encrypt),        /* Replace with NULL if there's nothing to do at request start */
    PHP_RSHUTDOWN(simple_encrypt),    /* Replace with NULL if there's nothing to do at request end */
    PHP_MINFO(simple_encrypt),
#if ZEND_MODULE_API_NO >= 20010901
    "0.1", /* Replace with version number for your extension */
#endif
    STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_SIMPLE_ENCRYPT
ZEND_GET_MODULE(simple_encrypt)
#endif

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(simple_encrypt)
{
    /* If you have INI entries, uncomment these lines 
    REGISTER_INI_ENTRIES();
    */
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(simple_encrypt)
{
    /* uncomment this line if you have INI entries
    UNREGISTER_INI_ENTRIES();
    */
    return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(simple_encrypt)
{
    return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(simple_encrypt)
{
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(simple_encrypt)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "simple_encrypt support", "enabled");
    php_info_print_table_end();
}
/* }}} */


static char hexconvtab[] = "0123456789abcdef";

/* {{{ se_bin2hex
 */
static char *se_bin2hex(const unsigned char *old, const size_t oldlen, size_t *newlen)
{
    register unsigned char *result = NULL;
    size_t i, j;

    result = (unsigned char *) safe_emalloc(oldlen, 2 * sizeof(char), 1);

    for (i = j = 0; i < oldlen; i++) {
        result[j++] = hexconvtab[old[i] >> 4];
        result[j++] = hexconvtab[old[i] & 15];
    }
    result[j] = '\0';

    if (newlen)
        *newlen = oldlen * 2 * sizeof(char);
    return (char *)result;
}
/* }}} */

/* {{{ se_hex2bin
 */
static char *se_hex2bin(const unsigned char *old, const size_t oldlen, size_t *newlen)
{
    register unsigned char *str;
    size_t target_length = oldlen >> 1;
    size_t i, j;

    str = (unsigned char *)safe_emalloc(target_length, sizeof(char), 1);

    for (i = j = 0; i < target_length; i++) {
        char c = old[j++];
        if (c >= '0' && c <= '9') {
            str[i] = (c - '0') << 4;
        } else if (c >= 'a' && c <= 'f') {
            str[i] = (c - 'a' + 10) << 4;
        } else if (c >= 'A' && c <= 'F') {
            str[i] = (c - 'A' + 10) << 4;
        } else {
            efree(str);
            return NULL;
        }
        c = old[j++];
        if (c >= '0' && c <= '9') {
            str[i] |= c - '0';
        } else if (c >= 'a' && c <= 'f') {
            str[i] |= c - 'a' + 10;
        } else if (c >= 'A' && c <= 'F') {
            str[i] |= c - 'A' + 10;
        } else {
            efree(str);
            return NULL;
        }
    }

    str[target_length] = '\0';

    if (newlen)
        *newlen = target_length;
    return (char *)str;
}


/* {{{ proto string gf_encrypt(string arg)
   Return a encrypt string */
PHP_FUNCTION(se_encrypt)
{
    char *str, *key;
    int len, klen;
    char inbuf[8], *binstr, *hexstr;
    int count, i, start;
    int binlen, hexlen, clen;

    // md5 context
    PHP_MD5_CTX context;
    unsigned char digest[16];
    char raw_key[8];

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &str, &len, &key,
                                                            &klen) == FAILURE) {
        RETURN_FALSE;
    }

    PHP_MD5Init(&context);
    PHP_MD5Update(&context, key, klen);
    PHP_MD5Final(digest, &context);

    for (i = 0; i < 8; i++) {
        raw_key[i] = (char)(digest[i * 2] ^ digest[i * 2 + 1]);
    }

    /* How many blocks for 8 bytes */
    if (len % 8 == 0) {
        count = len / 8;
    } else {
        count = len / 8 + 1;
    }

    binlen = count * 8;
    binstr = (char *)emalloc(binlen); /* Get memory for save binary string */
    if (!binstr) {
        RETURN_FALSE;
    }

    for (i = 0; i < count; i++) {
        start = i * 8; /* start copy position */
        clen = start + 8 > len ? len - start : 8; /* copy length */

        memcpy(inbuf, str + start, clen); /* copy string */

        if (clen < 8) { /* if not enough 8 bytes, fill up '\0' */
            int j;
            for (j = clen; j < 8; j++) {
                inbuf[j] = 0;
            }
        }

        DES_encipher(inbuf, binstr + start, raw_key);
    }

    /* Change binary data to hex data */
    hexstr = se_bin2hex(binstr, binlen, &hexlen);

    efree(binstr); /* Free binary string */

    RETURN_STRINGL(hexstr, hexlen, 0);
}
/* }}} */


/* {{{ */
static char *se_hexstr_decrypt(char *str, int len, int *newlen,
    char *key, int klen)
{
    int i, count, start;
    char *binstr, *retstr;
    int binlen;

    // md5 context
    PHP_MD5_CTX context;
    unsigned char digest[16];
    char raw_key[8];

    binstr = se_hex2bin(str, len, &binlen); /* hex string to bin string */

    retstr = (char *)emalloc(binlen); /* Get memory for save return string */
    if (!retstr) {
        efree(binstr);
        return NULL;
    }

    PHP_MD5Init(&context);
    PHP_MD5Update(&context, key, klen);
    PHP_MD5Final(digest, &context);

    for (i = 0; i < 8; i++) {
        raw_key[i] = (char)(digest[i * 2] ^ digest[i * 2 + 1]);
    }

    count = binlen / 8; /* How many blocks with 8 bytes */
    for (i = 0; i < count; i++) {
        start = i * 8;
        DES_decipher((const char *)(binstr+start), retstr+start, raw_key);
    }

    while (retstr[binlen-1] == '\0') /* Discard '\0' character */
        binlen--;
    *newlen = binlen;

    efree(binstr);

    return retstr;
}
/* }}} */


/* {{{ */
PHP_FUNCTION(se_decrypt)
{
    char *str, *key, *retstr;
    int len, klen, retlen;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss", &str, &len, &key,
                                                            &klen) == FAILURE) {
        RETURN_FALSE;
    }

    retstr = se_hexstr_decrypt(str, len, &retlen, key, klen);
    if (retstr == NULL) {
        RETURN_FALSE;
    }

    RETURN_STRINGL(retstr, retlen, 0);
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

