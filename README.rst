######
gmailc
######

Minimalist gmail checker written in C based on curl

Configuration
=============

To configure, match :code:`~/.config/gmailc.conf` to:

.. code:: bash

  username USER
  password PASS

Two-factor authentication
-------------------------

To handle Google's Two-factor authentication, set up an `application specific password <https://support.google.com/accounts/answer/185833?hl=en>`_ and use that instead of your account password in the config file.

Usage
=====

.. code:: bash

  Usage: gmailc [options] 
  Options:
    -h, --help  Print this message and exit.
    -p, --password  Specify a password.
    -u, --username  Specify a username.
    -v, --verbose Enable verbose mode.
    --version Display version.

TODO
====

* Write manpage

