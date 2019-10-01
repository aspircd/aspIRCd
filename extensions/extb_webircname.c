/* webirc extban type: matches web IRC clients. */

#include "stdinc.h"
#include "modules.h"
#include "client.h"
#include "ircd.h"

static int _modinit(void);
static void _moddeinit(void);
static int eb_webircname(const char *data, struct Client *client_p, struct Channel *chptr, long mode_type);

DECLARE_MODULE_AV1(extb_webircname, _modinit, _moddeinit, NULL, NULL, NULL, "$Revision$");

static int
_modinit(void)
{
	extban_table['w'] = eb_webircname;

	return 0;
}

static void
_moddeinit(void)
{
	extban_table['w'] = NULL;
}

static int eb_webircname(const char *data, struct Client *client_p,
		struct Channel *chptr, long mode_type)
{
	struct Metadata *md = user_metadata_find(client_p, "WEBIRCNAME");
	int isweb;
	if (md == NULL) isweb = 0;
	else isweb = 1;

	(void)chptr;
	(void)mode_type;
	if (data != NULL)
	{
		if (!isweb) return EXTBAN_NOMATCH;
		return !irccmp(data, md->value) ? EXTBAN_MATCH : EXTBAN_NOMATCH;
	}
	return isweb?EXTBAN_MATCH:EXTBAN_NOMATCH;
}
