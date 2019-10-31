## aspIRCd

**aspIRCd** is a high performance, IRCv3.2 capable, and scalable
IRC daemon. It is a fork of the now-defunct ircd-seven and seeks to continue in
the direction ircd-seven was headed.

**charybdis**
some of the coding has been taken from charybdis 3.5.7 , credits to
charybdis maintainers.

## General Information
- This IRC daemon (aspIRCd) is relatively different community than Umbrellixians’ asterIRC project
- AspIRCd is not associated or in cooperative relation with irca
- However we have some similarities with the ideology of the project and its base coding
- to reserve our content , we manipulated the statement to inform people that AspIRCd’s former ircd (ircd-nine) is the code holder for 3.5:charybdis
while , it ran on 3.4:charybdis
- enjoy running our software!

## Main differences between this release and depreciated Charybdis 4:
- It has +q (owner mode) , +a (admin mode) and the general +Ohv modes which Charybdis already contains. Due to this we had to shift the quiet mode to +y
- It is headed in the direction of UnrealIRCd and hence a lot of features on the Charybdis platform.
- It has one more major feature +h which IRC Operators set to declare themselves as helpops and show an swhois ‘is available for help.’ in their /whois
- It has in built support for mbedTLS which is automatically selected on ARM based servers
- It has support for Stream Control Transmission Protocol (SCTP) for connection and linking servers
- It has support for starttls and TLS v1.3 (most secured TLS base in the IRC industry)

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
 
* Then `cd aspIRCd`
* then to build the configuration file: `./autogen.sh`
* then use `libtoolize --ltdl` to link all the modules

* then to run the configure script, this will ask you simple questions important for your IRC setup, run `./Config`
* to build the IRCd run `make`
* followed by `make install`

**your IRCd will be installed in `/HOME DIRECTORY/ircd` by default.**

## Debian/ubuntu Users

If you have a newly installed OS, you should primarily run `apt-get update` followed by `apt-get install libssl-dev flex bison build-essential libsqlite3-dev pkg-config autoconf openssl libtool`

Read the included documentation for detailed compilation and install
directions.

## Support
Interested in meeting the developers?
The official channel for aspIRCd is `#aspIRCd` on
`irc.aspircd.com Port 6667/+6697(SSL)`

Finding services that fully supports this IRCd?
https://github.com/aspircd/shale-services
the protocol for this IRCd is **aspircd.c** provided with services
for further references of how to load the protocol, refer to shale.example.conf

Please use ***GitHub issue tracker*** for any issues
