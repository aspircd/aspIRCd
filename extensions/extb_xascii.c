/* SSL extban type: matches ssl users */

#include "stdinc.h"
#include "modules.h"
#include "client.h"
#include "ircd.h"

static int _modinit(void);
static void _moddeinit(void);
static int eb_xascii(const char *data, struct Client *client_p, struct Channel *chptr, long mode_type);

DECLARE_MODULE_AV1(extb_xascii, _modinit, _moddeinit, NULL, NULL, NULL, "$Revision$");

static int
_modinit(void)
{
	extban_table['u'] = eb_xascii;

	return 0;
}

static void
_moddeinit(void)
{
	extban_table['u'] = NULL;
}

static int eb_xascii(const char *data, struct Client *client_p,
		struct Channel *chptr, long mode_type)
{

	(void)chptr;
	(void)mode_type;
	if (data != NULL)
		return EXTBAN_INVALID;
	return IsXAscii(client_p) ? EXTBAN_MATCH : EXTBAN_NOMATCH;
}
