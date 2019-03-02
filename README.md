## aspIRCd

**aspIRCd** is a high performance, lightweight, and scalable
IRC daemon. It is a fork of the now-defunct ircd-seven and seeks to continue in
the direction ircd-seven was headed.

**ChatIRCd**
some of the coding from ChatIRCd has been used on aspIRCd. Credit to Ben (www.chatlounge.net)

**sIRCd**
Coding from sIRCd has been used on aspIRCd. Credit to Craigory (www.baconrashers.net)

## Supported Platforms

All modern \*NIX systems should work. You need the equivalent of the following
Debian packages:

 - `libssl-dev`
 - `flex`
 - `bison`
 - `build-essential`
 - `libsqlite3-dev`
 - `libtool`
 - `autoconf`
 
 You however have a choice of choosing any of your favourite SSL supporting applications. One of them, we primarily support is one of that of OpenSSL.
 To make sure SSL ports works, issue a certificate to your IRC Network by going in the `main directory@/bin` and running:
 `./genssl.sh`
 
 ## Installation
 
 This is a quick setup guide. In order to install, fork this repository : `git clone https://github.com/baconcoders/aspIRCd.git`
 
Then `cd aspIRCd`
then to build the configuration file: `./autogen.sh`
then use `libtoolize --ltdl` to link all the modules

then to configure the IRCd run `./configure`
to build the IRCd run `make`
followed by `make install`

your IRCd will be installed in `/HOME DIRECTORY/ircd` by default.

## Debian/ubuntu Users

If you have a newly installed OS, you should primarily run `apt-get update` followed by `apt-get install libssl-dev flex bison build-essential libsqlite3-dev pkg-config autoconf openssl libtool`

Read the included documentation for detailed compilation and install
directions.

## Support
Interested in meeting the developers?
The official channel for aspIRCd is `#aspIRCd` on
`irc.aspircd.com Port 6667/+6697(SSL)`

Finding services that fully supports this IRCd?
https://github.com/baconcoders/Ashtheme
the protocol for this IRCd is **aspircd.c** provided with services

Please use ***GitHub issue tracker*** for any issues
