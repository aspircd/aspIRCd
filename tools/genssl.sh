#!/bin/sh
echo "Generating self-signed certificate .. "
openssl req -x509 -nodes -newkey rsa:4096 -keyout ../etc/ssl.key -out ../etc/ssl.cert

echo "Generating Diffie-Hellman file for secure SSL/TLS negotiation .. "
openssl dhparam -out ../etc/dh.pem 3072

echo "
The included reference.conf uses these file locations by default. For others,
please ensure that the following lines in the serverinfo block are thus:

    ssl_private_key = \"etc/ssl.key\";
    ssl_cert = \"etc/ssl.cert\";
    ssl_dh_params = \"etc/dh.pem\";

Enjoy using SSL/TLS.
"
