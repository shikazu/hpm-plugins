#include <stdio.h>
#include <string.h>
#include "../common/HPMi.h"
#include "../map/script.h"
#include "../map/pc.h"

HPExport struct hplugin_info pinfo = {
	"autonext",		// Plugin name
	SERVER_TYPE_MAP,// Which server types this plugin works with?
	"1.0",			// Plugin version
	HPM_VERSION,	// HPM Version (don't change, macro is automatically updated)
};

BUILDIN(autonext) 
{
	TBL_PC* sd;
	int timeout;
	
	sd = script->rid2sd(st);
	if( sd == NULL )
		return true;
#ifdef SECURE_NPCTIMEOUT
	sd->npc_idle_type = NPCT_WAIT;
#endif
	script_detach_rid(st);

	timeout=script_getnum(st,2);

	if(st->sleep.tick == 0)
	{
		st->state = RERUNLINE;
		st->sleep.tick = timeout;
	}
	else
	{// sleep time is over
		st->state = RUN;
		st->sleep.tick = 0;
	}

	clif->scriptnext(sd, st->oid);
	return true;
}

/* Server Startup */
HPExport void plugin_init (void) 
{
	clif = GET_SYMBOL("clif");
	script = GET_SYMBOL("script");

	if( HPMi->addScript != NULL ) 
	{
		HPMi->addScript("autonext","i",BUILDIN_A(autonext));
	}
}
