/*
 * ssl only msg.
 * heavily based on override.
 * requires a +t-aware m_message.
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
#include "privilege.h"
#include "s_newconf.h"

static void check_umode_change(void *data);

mapi_hfn_list_av1 sslonlymsg_hfnlist[] = {
	{ "umode_changed", (hookfn) check_umode_change },
	{ NULL, NULL }
};

static void
check_umode_change(void *vdata)
{
	hook_data_umode_changed *data = (hook_data_umode_changed *)vdata;
	struct Client *source_p = data->client;

	if (!MyClient(source_p))
		return;

	/* didn't change +C umode, we don't need to do anything */
	if (!((data->oldumodes ^ source_p->umodes) & user_modes['t']))
		return;

	if (!IsSSLClient(source_p))
	{
		sendto_one_notice(source_p, ":(Mode Error)"
		"What's the point of only accepting SSL messages"
		" if you yourself do not use SSL? Silly goose.");
		source_p->umodes &= ~user_modes['t'];
		return;
	}
}

static int
_modinit(void)
{
	/* add the usermode to the next available slot */
	user_modes['t'] = find_umode_slot();
	user_mode_names['t'] = "sslonlymsg";
	construct_umodebuf();

	return 0;
}

static void
_moddeinit(void)
{
	/* disable the umode and remove it from the available list */
	user_modes['t'] = 0;
	user_mode_names['t'] = NULL;
	construct_umodebuf();
}

DECLARE_MODULE_AV1(sslonlymsg, _modinit, _moddeinit, NULL, NULL,
			sslonlymsg_hfnlist, "$Revision: 3526 $");
