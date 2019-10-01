/*
 * how many of these have i made
 * requires a +MP aware whois.
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
char *hidechannels_privilege = "oper:hidechannels";
char *hidechannels_override_privilege = "oper:spy";
unsigned int hidechannels_isoperonly = 0;
unsigned int hidechannels_override_isoperonly = 0;

static void
conf_set_hidechannels_hide_privilege(void *data)
{
	hidechannels_privilege = rb_strdup(data);
}

static void
conf_set_hidechannels_override_privilege(void *data)
{
	hidechannels_override_privilege = rb_strdup(data);
}

static void
conf_set_hidechannels_hide_operonly(void *data)
{
	hidechannels_isoperonly = * (unsigned int *) data;
}

static void
conf_set_hidechannels_override_operonly(void *data)
{
	hidechannels_override_isoperonly = * (unsigned int *) data;
}

mapi_hfn_list_av1 hidechannels_hfnlist[] = {
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
	if (!((data->oldumodes ^ source_p->umodes) & user_modes['P']) && !((data->oldumodes ^ source_p->umodes) & user_modes['M']))
		return;

	if (hidechannels_isoperonly != 0 && !((data->oldumodes ^ source_p->umodes) & user_modes['M']) && !HasPrivilege(source_p, hidechannels_privilege))
	{
		sendto_one_notice(source_p, ":*** You need the privilege %s to be able to hide your channel list.", hidechannels_privilege);
		source_p->umodes &= ~user_modes['M'];
	}

	if (hidechannels_override_isoperonly != 0 && !((data->oldumodes ^ source_p->umodes) & user_modes['P']) && !HasPrivilege(source_p, hidechannels_override_privilege))
	{
		sendto_one_notice(source_p, ":*** You need the privilege %s to be able to see the channel lists of +M users.", hidechannels_override_privilege);
		source_p->umodes &= ~user_modes['P'];
	}
	return;
}

static int
_modinit(void)
{
	/* add the usermode to the available slot */
	add_top_conf("channelhiding", NULL, NULL, NULL);
	add_conf_item("channelhiding", "hide_operonly", CF_YESNO, conf_set_hidechannels_hide_operonly);
	add_conf_item("channelhiding", "hide_privilege", CF_QSTRING, conf_set_hidechannels_hide_privilege);
	add_conf_item("channelhiding", "override_operonly", CF_YESNO, conf_set_hidechannels_override_operonly);
	add_conf_item("channelhiding", "override_privilege", CF_QSTRING, conf_set_hidechannels_override_privilege);
	user_modes['M'] = find_umode_slot();
	user_mode_names['M'] = "chanhide";
	user_modes['P'] = find_umode_slot();
	user_mode_names['P'] = "override_chanhide";
	construct_umodebuf();

	return 0;
}

static void
_moddeinit(void)
{
	/* disable the umode and remove it from the available list */
	user_modes['M'] = 0;
	user_modes['P'] = 0;
	user_mode_names['M'] = 0;
	user_mode_names['P'] = 0;
	remove_conf_item("channelhiding", "hide_operonly");
	remove_conf_item("channelhiding", "hide_privilege");
	remove_conf_item("channelhiding", "override_operonly");
	remove_conf_item("channelhiding", "override_privilege");
	remove_top_conf("channelhiding");
	construct_umodebuf();
}

DECLARE_MODULE_AV1(hidechannels, _modinit, _moddeinit, NULL, NULL,
			hidechannels_hfnlist, "$Revision: 3526 $");
