Simple Encrypt Extension For PHP
================================

Functions:
----------
(1) string se_encrypt(string $data, string $key);

* $data: encrypt data.
* $key: encrypt key.

(2) string se_decrypt(string $data, string $key);

* $data: decrypt data.
* $key: decrypt key.

install:
--------
``` shell
$ git clone https://github.com/liexusong/simple_encrypt
$ cd simple_encrypt
$ phpize
$ configure --with-php-config=path-to/php-config
$ make
$ make install
```

modifed php.ini and add "extension = simple_encrypt.so" entry

example:
--------
```php
<?php

  $key = "#xls^&@)(~@!8;3x";

  $enc_str = se_encrypt("127.0.0.1|1920123|liexusong|13610137508", $key);

  echo $enc_str, "\n"; // encrypt string
  echo base64_decode($enc_str), "\n"; // base64_decode() string;

  $org_str = se_decrypt($enc_str, $key); // decrypt string
  echo $org_str, "\n";

?>
```
