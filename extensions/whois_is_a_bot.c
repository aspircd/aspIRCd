/*
 * "is a bot on this IRC server." for ircd-chatd.
 * modular for obvious reasons. (not worth coring it)
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


static void h_isabot_whois(hook_data_client *);
static void h_isabot_high_whois(hook_data_client *);
mapi_hfn_list_av1 whois_isabot_hfnlist[] = {
	{ "doing_whois",	(hookfn) h_isabot_whois },
	{ "doing_whois_global",	(hookfn) h_isabot_whois },
	{ "doing_whois_top",	(hookfn) h_isabot_high_whois },
	{ "doing_whois_top_global",	(hookfn) h_isabot_high_whois },
	{ NULL, NULL }
};

static void check_umode_change(void *data);
char *isabotstring = "";
char isabotloc = 1;

#define IsUnrealStyle()	(isabotloc != 0)
#define IsChatdStyle()	(isabotloc == 0)

static void
conf_set_isabotstring(void *data)
{
	isabotstring = rb_strdup(data);
}

static void
conf_set_isabotloc(void *data)
{
	isabotloc = *(unsigned int *)data;
}

static void
isabot_whois(hook_data_client *data)
{
	if(!EmptyString(isabotstring) && ((data->target->umodes & user_modes['B']) != 0x0))
	{
		sendto_one_numeric(data->client, RPL_WHOISSPECIAL,
				form_str(RPL_WHOISSPECIAL),
				data->target->name, isabotstring);
	}
}

static void h_isabot_whois (hook_data_client *data)
{
	if (IsChatdStyle()) isabot_whois(data);
}

static void h_isabot_high_whois (hook_data_client *data)
{
	if (IsUnrealStyle()) isabot_whois(data);
}

static int
_modinit(void)
{
	user_modes['B'] = find_umode_slot();
	user_mode_names['B'] = "isbot";

	/* add the usermode to the available slot */
	add_top_conf("botmode", NULL, NULL, NULL);
	add_conf_item("botmode", "botstring", CF_QSTRING, conf_set_isabotstring);
	add_conf_item("botmode", "high_loc", CF_YESNO, conf_set_isabotloc);
	construct_umodebuf();

	return 0;
}

static void
_moddeinit(void)
{
	user_modes['B'] = 0;
	user_mode_names['B'] = 0;

	/* disable the umode and remove it from the available list */
	remove_conf_item("botmode", "botstring");
	remove_conf_item("botmode", "high_loc");
	remove_top_conf("botmode");
	construct_umodebuf();
}

DECLARE_MODULE_AV1(whois_isabot, _modinit, _moddeinit, NULL, NULL,
			whois_isabot_hfnlist, "$Revision: 3526 $");

