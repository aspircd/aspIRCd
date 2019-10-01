/* SCTP extban type: matches non-TCP users */

#include "stdinc.h"
#include "modules.h"
#include "client.h"
#include "ircd.h"

static int _modinit(void);
static void _moddeinit(void);
static int eb_sctp(const char *data, struct Client *client_p, struct Channel *chptr, long mode_type);

DECLARE_MODULE_AV1(extb_sctp, _modinit, _moddeinit, NULL, NULL, NULL, "$Revision$");

static int
_modinit(void)
{
	extban_table['t'] = eb_sctp;

	return 0;
}

static void
_moddeinit(void)
{
	extban_table['t'] = NULL;
}

static int eb_sctp(const char *data, struct Client *client_p,
		struct Channel *chptr, long mode_type)
{

	(void)chptr;
	(void)mode_type;
	if (data != NULL)
		return EXTBAN_INVALID;
	return (client_p->umodes & UMODE_SCTPCLIENT) ? EXTBAN_MATCH : EXTBAN_NOMATCH;
}
