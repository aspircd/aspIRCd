/*
 * hide certfp for ircd-chatd.
 * heavily based on override.
 * requires a +C-aware WHOIS.
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

mapi_hfn_list_av1 hidecert_hfnlist[] = {
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
	if (!((data->oldumodes ^ source_p->umodes) & user_modes['F']))
		return;

	if (source_p->certfp == NULL)
	{
		sendto_one_notice(source_p, ":*** You need to have a certificate in the first instance to be able to hide it... silly goose!");
		source_p->umodes &= ~user_modes['F'];
		return;
	}
}

static int
_modinit(void)
{
	/* add the usermode to the available slot */
	user_modes['F'] = find_umode_slot();
	user_mode_names['F'] = "hidecert";
	construct_umodebuf();

	return 0;
}

static void
_moddeinit(void)
{
	/* disable the umode and remove it from the available list */
	user_modes['F'] = 0;
	user_mode_names['F'] = NULL;
	construct_umodebuf();
}

DECLARE_MODULE_AV1(hidecert, _modinit, _moddeinit, NULL, NULL,
			hidecert_hfnlist, "$Revision: 3526 $");
