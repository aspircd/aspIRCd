/*
 * hide idle time for ircd-chatd.
 * heavily based on override.
 * requires a +I-aware WHOIS.
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
#include "newconf.h"

static void check_umode_change(void *data);
char *hideidle_privilege = "oper:hideidle";
unsigned int hideidle_isoperonly = 0;

static void
conf_set_hideidle_privilege(void *data)
{
	hideidle_privilege = rb_strdup(data);
}

static void
conf_set_hideidle_operonly(void *data)
{
	hideidle_isoperonly = * (unsigned int *) data;
}

mapi_hfn_list_av1 hideidle_hfnlist[] = {
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
	if (!((data->oldumodes ^ source_p->umodes) & user_modes['I']))
		return;

	if (hideidle_isoperonly != 0 && !HasPrivilege(source_p, hideidle_privilege))
	{
		sendto_one_notice(source_p, ":*** You need the privilege %s to be able to hide your idle time.", hideidle_privilege);
		source_p->umodes &= ~user_modes['I'];
		return;
	}
}

static int
_modinit(void)
{
	/* add the usermode to the available slot */
	add_conf_item("general", "hideidle_requires_oper", CF_YESNO, conf_set_hideidle_operonly);
	add_conf_item("general", "hideidle_requires_privilege", CF_QSTRING, conf_set_hideidle_privilege);
	user_modes['I'] = find_umode_slot();
	user_mode_names['I'] = "hideidle";
	construct_umodebuf();

	return 0;
}

static void
_moddeinit(void)
{
	/* disable the umode and remove it from the available list */
	user_modes['I'] = 0;
	user_mode_names['I'] = 0;
	remove_conf_item("general", "hideidle_requires_oper");
	remove_conf_item("general", "hideidle_requires_privilege");
	construct_umodebuf();
}

DECLARE_MODULE_AV1(hideidle, _modinit, _moddeinit, NULL, NULL,
			hideidle_hfnlist, "$Revision: 3526 $");
