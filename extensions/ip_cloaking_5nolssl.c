/*
 * Charybdis: an advanced ircd
 * ip_cloaking_5nolssl.c: provide user hostname cloaking
 *
 * Written originally by nenolod, altered to use FNV by Elizabeth in 2008
 * altered some more by groente
 *
 * Further modified, after initial modification by ellenor to use sha256-hmac,
 * to use bundled sha256 and thus not depend on openssl/libressl (hence
 * "nolssl")
 */

#include "stdinc.h"
#include "modules.h"
#include "hook.h"
#include "client.h"
#include "ircd.h"
#include "send.h"
#include "hash.h"
#include "s_conf.h"
#include "s_user.h"
#include "s_serv.h"
#include "numeric.h"
#include "newconf.h"

#include "modules/sha256.h"

char *secretsalt = "32qwnqoWI@DpMd&w";
char *cloakprefix = "net/";

static void
conf_set_secretsalt(void *data)
{
    secretsalt = rb_strdup(data);
}

static void
conf_set_cloakprefix(void *data)
{
    cloakprefix = rb_strdup(data);
}

static int
_modinit(void)
{
    /* add the usermode to the available slot */
    user_modes['x'] = find_umode_slot();
    user_mode_names['x'] = "cloaked";
    construct_umodebuf();

    add_top_conf("cloaking", NULL, NULL, NULL);
    add_conf_item("cloaking", "secretsalt", CF_QSTRING, conf_set_secretsalt);
    add_conf_item("cloaking", "prefix", CF_QSTRING, conf_set_cloakprefix);

    return 0;
}

static void
_moddeinit(void)
{
    /* disable the umode and remove it from the available list */
    user_modes['x'] = 0;
    user_mode_names['x'] = NULL;
    construct_umodebuf();

    remove_top_conf("cloaking");
    remove_conf_item("cloaking", "secretsalt");
    remove_conf_item("cloaking", "prefix");
}

static void check_umode_change(void *data);
static void check_new_user(void *data);
mapi_hfn_list_av1 ip_cloaking_hfnlist[] = {
    { "umode_changed", (hookfn) check_umode_change },
    { "new_local_user", (hookfn) check_new_user },
    { NULL, NULL }
};

DECLARE_MODULE_AV1(ip_cloaking, _modinit, _moddeinit, NULL, NULL,
                   ip_cloaking_hfnlist, "$Revision: 3526 $");

static char *
do_ip_cloak_part(const char *part)
{
    unsigned char hash[33] = "";
    char *inbuf = rb_strdup(part);
    char buf[33] = "";
    int i;
    for (i = strlen(part)+1;i>0;i--) {
        inbuf[i-1] = part[i-1] ^ secretsalt[(i-1)%strlen(secretsalt)];
    }
    // part on secretsalt
    sha256_hash(inbuf, &hash, 32);
    rb_snprintf(buf, sizeof(buf), "%.2X%.2X%.2X%.2X", hash[2], hash[4], hash[6], hash[8]);
    return rb_strdup(buf);
}

static void
do_ip_cloak(const char *inbuf, char *outbuf)
{
    unsigned int a, b, c, d;
    struct in_addr in_addr;
    char buf[512], *alpha, *beta, *gamma;
    alpha = rb_malloc(512);
    beta = rb_malloc(512);
    gamma = rb_malloc(512);
    rb_inet_pton(AF_INET, inbuf, &in_addr);
    a = (in_addr.s_addr & 0xff000000) >> 24;
    b = (in_addr.s_addr & 0x00ff0000) >> 16;
    c = (in_addr.s_addr & 0x0000ff00) >> 8;
    d = in_addr.s_addr & 0x000000ff;
    rb_sprintf(alpha, "%s", inbuf);
    rb_sprintf(beta, "%u.%u.%u", a, b, c);
    rb_sprintf(gamma, "%u.%u", a, b);
    rb_sprintf(outbuf, "%s.%s.%s.i4msk", do_ip_cloak_part(alpha), do_ip_cloak_part(beta), do_ip_cloak_part(gamma));
}

static void
do_host_cloak_ipv6(const char *inbuf, char *outbuf)
{
    unsigned char *a, *b, *c, *d;
    char buf[512], *alpha, *beta, *gamma;
    struct in6_addr in_addr;
    a = rb_malloc(512);
    b = rb_malloc(512);
    c = rb_malloc(512);
    alpha = rb_malloc(512);
    beta = rb_malloc(512);
    gamma = rb_malloc(512);
    rb_inet_pton(AF_INET6, inbuf, &in_addr);
    rb_sprintf(c, "%2x%2x.%2x%2x.%2x%2x.%2x%2x.%2x%2x.%2x%2x",
		in_addr.s6_addr[0],
		in_addr.s6_addr[1],
		in_addr.s6_addr[2],
		in_addr.s6_addr[3],
		in_addr.s6_addr[4],
		in_addr.s6_addr[5],
		in_addr.s6_addr[6],
		in_addr.s6_addr[7],
		in_addr.s6_addr[8],
		in_addr.s6_addr[9],
		in_addr.s6_addr[10],
		in_addr.s6_addr[11]
	);
    rb_sprintf(b, "%2x%2x.%2x%2x.%2x%2x.%2x%2x",
		in_addr.s6_addr[0],
		in_addr.s6_addr[1],
		in_addr.s6_addr[2],
		in_addr.s6_addr[3],
		in_addr.s6_addr[4],
		in_addr.s6_addr[5],
		in_addr.s6_addr[6],
		in_addr.s6_addr[7]
	);
    rb_sprintf(a, "%2x%2x.%2x%2x",
		in_addr.s6_addr[0],
		in_addr.s6_addr[1],
		in_addr.s6_addr[2],
		in_addr.s6_addr[3]
	);
    rb_sprintf(alpha, "%s", inbuf);
    rb_sprintf(beta, "%s.%s.%s", a, b, c);
    rb_sprintf(gamma, "%s.%s", a, b);
    rb_sprintf(outbuf, "%s:%s:%s:i6msk", do_ip_cloak_part(alpha), do_ip_cloak_part(beta), do_ip_cloak_part(gamma));
}

static void
distribute_hostchange(struct Client *client_p, char *newhost)
{
    if (newhost != client_p->orighost)
        sendto_one_numeric(client_p, RPL_HOSTHIDDEN, "%s :is now your hidden host",
                           newhost);
    else
        sendto_one_numeric(client_p, RPL_HOSTHIDDEN, "%s :hostname reset",
                           newhost);

    sendto_server(NULL, NULL,
                  CAP_EUID | CAP_TS6, NOCAPS, ":%s CHGHOST %s :%s",
                  use_id(&me), use_id(client_p), newhost);
    sendto_server(NULL, NULL,
                  CAP_TS6, CAP_EUID, ":%s ENCAP * CHGHOST %s :%s",
                  use_id(&me), use_id(client_p), newhost);

    change_nick_user_host(client_p, client_p->name, client_p->username, newhost, 0, "Changing host");

    if (newhost != client_p->orighost)
        SetDynSpoof(client_p);
    else
        ClearDynSpoof(client_p);
}

static void
do_host_cloak_host(const char *inbuf, char *outbuf)
{
    unsigned char *hash;
    char buf[3];
    char output[HOSTLEN+1];
    int i, j;

    hash = do_ip_cloak_part(inbuf);

    output[0]=0;

    char *oldhost;
    j = 0;
    oldhost = rb_strdup(inbuf);
    int hostlen = 0;

    for (i = 0; i < strlen(oldhost); i++) {
        oldhost++;
        hostlen++;
        if (*oldhost == '.') {
            break;
        }
    }

    for (i = 0; i < 61; i = i + 2) {
        if (i >= hostlen && i >= 8) break;
        sprintf(buf, "%.2X", hash[i]);
        strcat(output,buf);
    }

    rb_strlcpy(outbuf,cloakprefix,HOSTLEN+1);
    rb_strlcat(outbuf,output,HOSTLEN+1);
    rb_strlcat(outbuf,oldhost,HOSTLEN+1);
}

static void
do_host_cloak_ip(const char *inbuf, char *outbuf)
{
    /* None of the characters in this table can be valid in an IP */
    char chartable[] = "ghijklmnopqrstuvwxyz";
    char *tptr;
    int sepcount = 0;
    int totalcount = 0;
    int ipv6 = 0;

    if (strchr(inbuf, ':')) {
        ipv6 = 1;

        /* Damn you IPv6...
         * We count the number of colons so we can calculate how much
         * of the host to cloak. This is because some hostmasks may not
         * have as many octets as we'd like.
         *
         * We have to do this ahead of time because doing this during
         * the actual cloaking would get ugly
         */
        for (tptr = inbuf; *tptr != '\0'; tptr++)
            if (*tptr == ':')
                totalcount++;
    } else if (!strchr(inbuf, '.'))
        return;
    if (ipv6)
       do_host_cloak_ipv6(inbuf, outbuf);
    else
       do_ip_cloak(inbuf, outbuf);
}

static void
check_umode_change(void *vdata)
{
    hook_data_umode_changed *data = (hook_data_umode_changed *)vdata;
    struct Client *source_p = data->client;

    if (!MyClient(source_p))
        return;

    /* didn't change +h umode, we don't need to do anything */
    if (!((data->oldumodes ^ source_p->umodes) & user_modes['x']))
        return;

    if (source_p->umodes & user_modes['x']) {
        if (IsIPSpoof(source_p) || source_p->localClient->mangledhost == NULL || (IsDynSpoof(source_p) && strcmp(source_p->host, source_p->localClient->mangledhost))) {
            source_p->umodes &= ~user_modes['x'];
            return;
        }
        if (strcmp(source_p->host, source_p->localClient->mangledhost)) {
            distribute_hostchange(source_p, source_p->localClient->mangledhost);
        } else /* not really nice, but we need to send this numeric here */
            sendto_one_numeric(source_p, RPL_HOSTHIDDEN, "%s :is now your hidden host",
                               source_p->host);
    } else if (!(source_p->umodes & user_modes['x'])) {
        if (source_p->localClient->mangledhost != NULL &&
            !strcmp(source_p->host, source_p->localClient->mangledhost)) {
            distribute_hostchange(source_p, source_p->orighost);
        }
    }
}

static void
check_new_user(void *vdata)
{
    struct Client *source_p = (void *)vdata;

    if (IsIPSpoof(source_p)) {
        source_p->umodes &= ~user_modes['x'];
        return;
    }
    source_p->localClient->mangledhost = rb_malloc(HOSTLEN + 1);
    if (!irccmp(source_p->orighost, source_p->sockhost))
        do_host_cloak_ip(source_p->orighost, source_p->localClient->mangledhost);
    else
        do_host_cloak_host(source_p->orighost, source_p->localClient->mangledhost);
    if (IsDynSpoof(source_p))
        source_p->umodes &= ~user_modes['x'];
    if (source_p->umodes & user_modes['x']) {
        rb_strlcpy(source_p->host, source_p->localClient->mangledhost, sizeof(source_p->host));
        if (irccmp(source_p->host, source_p->orighost))
            SetDynSpoof(source_p);
    }
}
