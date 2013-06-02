#include <stdio.h>
#include <string.h>
#include "../common/HPMi.h"
#include "../map/script.h"
#include "../map/pc.h"

HPExport struct hplugin_info pinfo =
{
    "@afk",		// Plugin name
    SERVER_TYPE_MAP,// Which server types this plugin works with?
    "0.1a",			// Plugin version
    HPM_VERSION,	// HPM Version (don't change, macro is automatically updated)
};

ACMD(afk)
{
    sd->state.autotrade = 1;
    sd->state.monster_ignore = 1;
    pc_setsit(sd);
    skill->sit(sd,1);
    clif->sitting(&sd->bl);
    clif->changelook(&sd->bl,LOOK_HEAD_TOP,471);
    clif->specialeffect(&sd->bl, 234,AREA);
    clif->authfail_fd(fd, 15);

    return 0;
}

/* Server Startup */
HPExport void plugin_init (void)
{
    clif = GET_SYMBOL("clif");
    script = GET_SYMBOL("script");
    skill = GET_SYMBOL("skill");

    HPMi->addCommand("afk",ACMD_A(afk));
}

