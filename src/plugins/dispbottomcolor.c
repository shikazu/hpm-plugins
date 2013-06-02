#include <stdio.h>
#include <string.h>
#include "../common/HPMi.h"
#include "../map/script.h"
#include "../map/pc.h"

HPExport struct hplugin_info pinfo = {
	"dispbottomcolor",		// Plugin name
	SERVER_TYPE_MAP,// Which server types this plugin works with?
	"1.0",			// Plugin version
	HPM_VERSION,	// HPM Version (don't change, macro is automatically updated)
};

BUILDIN(dispbottomcolor) 
{
	TBL_PC *sd = script->rid2sd(st);
	const char *message;
	int color;
	
	message=script_getstr(st,2);

	if(script_hasdata(st,3))
		color=script_getnum(st,3);
	else 
		color = COLOR_DEFAULT;

	if(sd)
		clif->colormes(sd->fd,color,message);
	return true;
}

/* Server Startup */
HPExport void plugin_init (void) 
{
	clif = GET_SYMBOL("clif");
	script = GET_SYMBOL("script");

	if( HPMi->addScript != NULL ) 
	{
		HPMi->addScript("dispbottomcolor","s?",BUILDIN_A(dispbottomcolor));
	}
}
