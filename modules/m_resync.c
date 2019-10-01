/* ircd-chatd - the one with SCTP
 * m_resync - resynchronise yourself to a channel
 * client only, takes 1 argument.
 * not visible to anyone else. usually used in anonchannels if you get opped.
 * available under the same license as the rest of chatd.
 */

/* List of ircd includes from ../include/ */
#include "stdinc.h"
#include "modules.h"
#include "hook.h"
#include "client.h"
#include "ircd.h"
#include "send.h"
#include "channel.h"

static int modinit(void)
{
return 0;
}

static void moduninit(void) {}

static int mc_resync (struct Client *client, struct Client *source, int parc, const char *parv[]) {
	/* Get the obvious stuff out the way first.
	 */
	struct Channel *chptr = find_channel(parv[1]); // Holds pointer to resync.
	struct membership *msptr;
	if (chptr==NULL) {
		// Nerf. Pretend it exists and user isn't in it.
		sendto_one_numeric(source, ERR_NOTONCHANNEL, form_str(ERR_NOTONCHANNEL), chptr->chname);
	} {
		msptr = find_channel_membership(chptr, source);
		if (msptr = NULL) {
			// Nerf.
			sendto_one_numeric(source, ERR_NOTONCHANNEL, form_str(ERR_NOTONCHANNEL), chptr->chname);
		} else {
			sendto_one(source, ":%s!%s@%s PART %s :fake part to aid resync", source->name, source->username, source->host, chptr->chname);
			if (IsCapable(source, CLICAP_EXTENDED_JOIN))
				sendto_one(source, ":%s!%s@%s JOIN %s %s :%s", source->name, source->username, source->host, chptr->chname,
                                             EmptyString(source->user->suser) ? "*" : source->user->suser,
                                             source->info);
			else
				sendto_one(source, ":%s!%s@%s JOIN %s", source->name, source->username, source->host, chptr->chname);

		}
	}
}

struct Message resync_tab = {
"RESYNC", 0,0,MFLG_SLOW, mg_unreg, {mc_resync,1}, mg_ignore, mg_ignore, {mc_resync,1}
};

mapi_clist_av1 resync_clist[] = {&resync_tab,NULL};

DECLARE_MODULE_AV1(m_resync, modinit, moduninit, resync_clist, NULL, NULL, "v0.2.8");
