/*
 * ircd-chatd - a dumb ircd
 * m_metadata.c - Metadata
 * Please see LICENSE in the root of the project.
 * Taken from pitIRCd. originally ShadowIRCd, I think.
 */

#include "stdinc.h"

#include "send.h"
#include "channel.h"
#include "client.h"
#include "common.h"
#include "config.h"
#include "ircd.h"
#include "numeric.h"
#include "s_conf.h"
#include "s_newconf.h"
#include "s_serv.h"
#include "hash.h"
#include "msg.h"
#include "parse.h"
#include "modules.h"
#include "whowas.h"
#include "monitor.h"

static int me_metadata(struct Client *, struct Client *, int, const char **);
static int m_metadata(struct Client *, struct Client *, int, const char **);

struct Message metadata_msgtab = {
	"METADATA", 0, 0, 0, MFLG_SLOW,
	{mg_ignore, {m_metadata, 3}, mg_ignore, mg_ignore, {me_metadata, 3}, mg_ignore}
};

mapi_clist_av1 metadata_clist[] = {
	&metadata_msgtab, NULL
};

DECLARE_MODULE_AV1(metadata, NULL, NULL, metadata_clist, NULL, NULL, "$Revision$");

static int
me_metadata(struct Client *client_p, struct Client *source_p, int parc, const char *parv[])
{
	if(IsChannelName(parv[2]))
	{
		struct Channel *chptr;

		if((chptr = find_channel(parv[2])) == NULL)
			return 0;

		if(!irccmp(parv[1], "ADD") && parv[4] != NULL)
			channel_metadata_add(chptr, parv[3], parv[4], 0);
		if(!irccmp(parv[1], "DELETE") && parv[3] != NULL)
			channel_metadata_delete(chptr, parv[3], 0);
	}

	else
	{
		struct Client *target_p;

		if((target_p = find_id(parv[2])) == NULL)
			return 0;

		if(!target_p->user)
			return 0;

		if(!irccmp(parv[1], "ADD") && parv[4] != NULL)
			user_metadata_add(target_p, parv[3], parv[4], 0);
		if(!irccmp(parv[1], "DELETE") && parv[3] != NULL)
			user_metadata_delete(target_p, parv[3], 0);
	}
	return 0;
}


/*
** m_metadata - User set channel metadata
** I seriously fucking did this -- janicez
*/
static int
m_metadata(struct Client *client_p, struct Client *source_p, int parc, const char *parv[])
{
	if(!check_channel_name(parv[2]))
	{
		sendto_one_numeric(source_p, ERR_BADCHANNAME, form_str(ERR_BADCHANNAME), parv[3]);
		return 0;
	}

	struct Channel *chptr;
	struct membership *msptr;
	if((chptr = find_channel(parv[2])) == NULL)
	{
		sendto_one_numeric(source_p, ERR_NOSUCHCHANNEL,
				   form_str(ERR_NOSUCHCHANNEL), parv[2]);
		return 0;
	}
	struct Metadata *md;
	if(!irccmp(parv[1], "FIND") && parv[3] != NULL) {
		md = channel_metadata_find(chptr, parv[3]);
		if (md != NULL)
		{
			sendto_one(source_p, ":%s 802 %s %s %s :%s", me.name, source_p->name, parv[2], md->name, md->value);
			return 0;
		} else {
			sendto_one(source_p, ":%s 803 %s %s %s :Metadatum nonexistant", me.name, source_p->name, parv[2], parv[3]);
			return 0;
		}
	}

	// Uhhh.... rizight. I don't think this is gonna work (mumble mumble) -- janicez
	if ((msptr = find_channel_membership(chptr, source_p)) == NULL)
	{
		sendto_one_numeric(source_p, ERR_NOTONCHANNEL,
				   form_str(ERR_NOTONCHANNEL), parv[2]);
		return 0;
	}

	// Yeah. No. --janicez
	if (!is_any_op(msptr) && !IsOverride(source_p))
	{
		sendto_one(source_p, form_str(ERR_CHANOPRIVSNEEDED),
				me.name, source_p->name, parv[2]);
		return 0;
	}

	if (!irccmp(parv[3], "NOREPEAT"))
	{
		sendto_one(source_p, ":%s 801 %s %s %s :NOREPEAT modification prohibited -- used by IRCd", me.name, source_p->name, parv[2], parv[3]);
		return 0;
	}

	if (parv[3][0] == '@')
	{
		sendto_one(source_p, ":%s 801 %s %s %s :Reserved space modification prohibited -- Reserved for use by IRCd.", me.name, source_p->name, parv[2], parv[3]);
		return 0;
	}

	if (!strcmp(parv[3], "FOUNDER"))
	{
		sendto_one(source_p, ":%s 801 %s %s %s :FOUNDER modification prohibited -- Reserved for future use by IRCd.", me.name, source_p->name, parv[2], parv[3]);
		return 0;
	}

	// And here we are. If the user isn't querying, but adding or deleting, set, and if the channel is global (#), propagate.
	if(!irccmp(parv[1], "ADD") && parv[4] != NULL)
		channel_metadata_add(chptr, parv[3], parv[4], 1);
	if(!irccmp(parv[1], "DELETE") && parv[3] != NULL)
		channel_metadata_delete(chptr, parv[3], 1);
	return 0;
}
