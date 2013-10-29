--TEST--
Check for simple_encrypt presence
--SKIPIF--
<?php if (!extension_loaded("simple_encrypt")) print "skip"; ?>
--FILE--
<?php 
echo "simple_encrypt extension is available";
/*
	you can add regression tests for your extension here

  the output of your test code has to be equal to the
  text in the --EXPECT-- section below for the tests
  to pass, differences between the output and the
  expected text are interpreted as failure

	see php5/README.TESTING for further information on
  writing regression tests
*/
?>
--EXPECT--
simple_encrypt extension is available
