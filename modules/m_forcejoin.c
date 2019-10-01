/*
 * SIRCd: the ircd for those who like unreal prefixes.
 * m_forcejoin.c: Force joins a user
 *
 * Copyright (C) 2010 Elizabeth Jennifer Myers. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 * 4. You agree to use this for good and not evil. If you whine about this
 *    clause in any way, your licence to use this software is revoked.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */


#include "stdinc.h"
#include "client.h"
#include "hash.h"		/* for find_client() */
#include "numeric.h"
#include "logger.h"
#include "s_serv.h"
#include "s_conf.h"
#include "modules.h"
#include "s_newconf.h"

static int me_forcejoin(struct Client *, struct Client *, int, const char **);
static int mo_forcejoin(struct Client *, struct Client *, int, const char **);

static void user_join_override(struct Client *, struct Client *, struct Client *, const char *);

extern int h_channel_join; /* In channel.c */

struct Message forcejoin_msgtab = {
    "FORCEJOIN", 0, 0, 0, MFLG_SLOW,
    {mg_unreg, mg_not_oper, mg_ignore, mg_ignore, {me_forcejoin, 2}, {mo_forcejoin, 2}}
};

mapi_clist_av1 forcejoin_clist[] = { &forcejoin_msgtab, NULL };

DECLARE_MODULE_AV1(forcejoin, NULL, NULL, forcejoin_clist, NULL, NULL, "$Revision: 1 $");

/*
** mo_forcejoin
**      parv[1] = forcejoin victim
**      parv[2] = forcejoin channel list
*/
static int
mo_forcejoin(struct Client *client_p, struct Client *source_p, int parc, const char *parv[])
{
    struct Client *target_p;
    const char *user, *chanlist;
    int chasing = 0;

    user = parv[1];

    if(!IsOper(source_p)) {
        sendto_one(source_p, form_str(ERR_NOPRIVS), me.name, source_p->name);
        return 0;
    }

    if(EmptyString(parv[2])) {
        sendto_one(source_p, form_str(ERR_NEEDMOREPARAMS), me.name, source_p->name, "FORCEJOIN");
        return 0;
    } else
        chanlist = parv[2];

    if((target_p = find_chasing(source_p, user, &chasing)) == NULL)
        return 0;

    if(!MyClient(target_p) && !IsOperAdmin(source_p)) {
        sendto_one_notice(source_p, ":Nick %s is not on your server and you are not admin",
                          target_p->name);
        return 0;
    }

    sendto_realops_snomask(SNO_GENERAL, L_NETWIDE,
                           "Received FORCEJOIN message for %s!%s@%s. From %s (Channels: %s)",
                           target_p->name, target_p->username, target_p->orighost,
                           source_p->name, chanlist);

    ilog(L_MAIN, "FORCEJOIN called for [%s] by %s!%s@%s",
         target_p->name, source_p->name, source_p->username, source_p->host);

    sendto_one_notice(target_p, ":You have been forced to join %s",
                      chanlist);

    if(!MyClient(target_p)) {
        struct Client *cptr = target_p->servptr;
        sendto_one(cptr, ":%s ENCAP %s FORCEJOIN %s :%s",
                   get_id(source_p, cptr), cptr->name, get_id(target_p, cptr), chanlist);
        return 0;
    }

    user_join_override(client_p, source_p, target_p, chanlist);

    return 0;
}

/*
 * me_forcejoin
 *      parv[1] = forcejoin victim
 *      parv[2] = forcejoin channel list
 */
static int
me_forcejoin(struct Client *client_p, struct Client *source_p, int parc, const char *parv[])
{
    struct Client *target_p;
    const char *user, *chanlist;
    int chasing = 0;

    user = parv[1];

    if(EmptyString(parv[2]))
        return 0;
    else
        chanlist = parv[2];

    /* Find the user */
    if((target_p = find_chasing(source_p, user, &chasing)) == NULL)
        return 0;

    if(IsServer(target_p) || IsMe(target_p))
        return 0;

    ilog(L_MAIN, "FORCEJOIN called for [%s] by %s!%s@%s",
         target_p->name, source_p->name, source_p->username, source_p->host);

    if(!MyClient(target_p)) {
        struct Client *cptr = target_p->servptr;
        sendto_one(cptr, ":%s ENCAP %s FORCEJOIN %s :%s",
                   get_id(source_p, cptr), cptr->name, get_id(target_p, cptr), chanlist);
        return 0;
    }

    user_join_override(client_p, source_p, target_p, chanlist);

    return 0;
}

/* Join a channel, ignoring forwards, +ib, etc. It notifes source_p of any errors joining
 * NB: this assumes a local user.
 */
void user_join_override(struct Client * client_p, struct Client * source_p, struct Client * target_p, const char * channels)
{
    static char jbuf[BUFSIZE];
    struct ConfItem *aconf;
    struct Channel *chptr = NULL;
    char *name;
    const char *modes;
    char *p = NULL;
    int flags;
    char *chanlist;

    jbuf[0] = '\0';

    if(channels == NULL)
        return;

    /* rebuild the list of channels theyre supposed to be joining. */
    chanlist = LOCAL_COPY(channels);
    for(name = rb_strtok_r(chanlist, ",", &p); name; name = rb_strtok_r(NULL, ",", &p)) {
        /* check the length and name of channel is ok */
        if(!check_channel_name_loc(target_p, name) || (strlen(name) > LOC_CHANNELLEN)) {
            sendto_one_numeric(source_p, ERR_BADCHANNAME,
                               form_str(ERR_BADCHANNAME), (unsigned char *) name);
            continue;
        }

        /* join 0 parts all channels */
        if(*name == '0' && (name[1] == ',' || name[1] == '\0') && name == chanlist) {
            (void) strcpy(jbuf, "0");
            continue;
        }

        /* check it begins with # */
        else if(!IsChannelName(name)) {
            sendto_one_numeric(source_p, ERR_NOSUCHCHANNEL,
                               form_str(ERR_NOSUCHCHANNEL), name);
            continue;
        }

        /* see if its resv'd */
        if(!IsExemptResv(target_p) && (aconf = hash_find_resv(name))) {
            sendto_one_numeric(source_p, ERR_BADCHANNAME,
                               form_str(ERR_BADCHANNAME), name);

            /* dont update tracking for jupe exempt users, these
             * are likely to be spamtrap leaves
             */
            if(IsExemptJupe(source_p))
                aconf->port--;

            continue;
        }

        if(splitmode && !IsOper(target_p) && ConfigChannel.no_join_on_split) {
            sendto_one(source_p, form_str(ERR_UNAVAILRESOURCE),
                       me.name, source_p->name, name);
            continue;
        }

        if(*jbuf)
            (void) strcat(jbuf, ",");
        (void) rb_strlcat(jbuf, name, sizeof(jbuf));
    }

    for(name = rb_strtok_r(jbuf, ",", &p); name; name = rb_strtok_r(NULL, ",", &p)) {
        /* JOIN 0 simply parts all channels the user is in */
        if(*name == '0' && !atoi(name)) {
            if(target_p->user->channel.head == NULL)
                continue;

            do_join_0(&me, target_p);
            continue;
        }

        if((chptr = find_channel(name)) != NULL) {
            if(IsMember(target_p, chptr)) {
                /* debugging is fun... */
                sendto_one_notice(source_p, ":*** Notice -- %s is already in %s",
                                  target_p->name, chptr->chname);
                return;
            }

            add_user_to_channel(chptr, target_p, CHFL_PEON);
            if (chptr->mode.join_num && rb_current_time() - chptr->join_delta >= chptr->mode.join_time) {
                chptr->join_count = 0;
                chptr->join_delta = rb_current_time();
            }
            chptr->join_count++;

            sendto_channel_local(ALL_MEMBERS, chptr, ":%s!%s@%s JOIN :%s",
                                 target_p->name, target_p->username,
                                 target_p->host, chptr->chname);

            sendto_server(target_p, chptr, CAP_TS6, NOCAPS,
                          ":%s JOIN %ld %s +",
                          get_id(target_p, client_p), (long) chptr->channelts,
                          chptr->chname);

            del_invite(chptr, target_p);

            if(chptr->topic != NULL) {
                sendto_one(target_p, form_str(RPL_TOPIC), me.name,
                           target_p->name, chptr->chname, chptr->topic);
                sendto_one(target_p, form_str(RPL_TOPICWHOTIME),
                           me.name, source_p->name, chptr->chname,
                           chptr->topic_info, chptr->topic_time);
            }

            channel_member_names(chptr, target_p, 1);
        } else {
            char statusmodes[5] = "";

            if(!check_channel_name(name)) {
                sendto_one(source_p, form_str(ERR_BADCHANNAME), me.name,
                           source_p->name, (unsigned char *) name);
                return;
            }

            /* channel name must begin with & or # */
            if(!IsChannelName(name)) {
                sendto_one(source_p, form_str(ERR_BADCHANNAME), me.name,
                           source_p->name, (unsigned char *) name);
                return;
            }

            /* name can't be longer than CHANNELLEN */
            if(strlen(name) > CHANNELLEN) {
                sendto_one_notice(source_p, ":Channel name is too long");
                return;
            }

            chptr = get_or_create_channel(target_p, name, NULL);

            flags = CHFL_CHANOP;

            if(ConfigChannel.admin_on_channel_create && ConfigChannel.use_admin)
                flags |= CHFL_SOP;

            add_user_to_channel(chptr, target_p, flags);
            if (chptr->mode.join_num &&
                rb_current_time() - chptr->join_delta >= chptr->mode.join_time) {
                chptr->join_count = 0;
                chptr->join_delta = rb_current_time();
            }
            chptr->join_count++;

            sendto_channel_local(ALL_MEMBERS, chptr, ":%s!%s@%s JOIN :%s",
                                 target_p->name, target_p->username,
                                 target_p->host, chptr->chname);

            /* autochanmodes stuff */
            if(ConfigChannel.autochanmodes) {
                char * ch;
                for(ch = ConfigChannel.autochanmodes; *ch != '\0'; ch++) {
                    chptr->mode.mode |= chmode_table[(unsigned int)*ch].mode_type;
                }
            } else {
                chptr->mode.mode |= MODE_TOPICLIMIT;
                chptr->mode.mode |= MODE_NOPRIVMSGS;
            }

            modes = channel_modes(chptr, &me);
            sendto_channel_local(CHFL_CHANOP, chptr, ":%s MODE %s %s",
                                 me.name, chptr->chname, modes);

            if (flags & CHFL_QOP)
                strcat(statusmodes, "~");

            if (flags & CHFL_SOP)
                strcat(statusmodes, "&");

            strcat(statusmodes, "@");

            sendto_server(target_p, chptr, CAP_TS6, NOCAPS,
                          ":%s SJOIN %ld %s %s :%s%s",
                          me.id, (long) chptr->channelts,
                          chptr->chname, modes, statusmodes,
                          get_id(target_p, client_p));

            target_p->localClient->last_join_time = rb_current_time();
            channel_member_names(chptr, target_p, 1);

            /* Call channel join hooks */

            /* we do this to let the oper know that a channel was created, this will be
             * seen from the server handling the command instead of the server that
             * the oper is on.
             */
            sendto_one_notice(source_p, ":*** Notice -- Creating channel %s", chptr->chname);
        }
    }


    return;
}
