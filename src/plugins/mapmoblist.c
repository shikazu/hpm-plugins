#include <stdio.h>
#include <string.h>
#include "../common/HPMi.h"
#include "../map/script.h"
#include "../map/pc.h"
#include "../map/map.h"

HPExport struct hplugin_info pinfo = {
	"mapmoblist",		// Plugin name
	SERVER_TYPE_MAP,// Which server types this plugin works with?
	"1.0",			// Plugin version
	HPM_VERSION,	// HPM Version (don't change, macro is automatically updated)
};

static int count_mob(struct block_list *bl, va_list ap) // [FE]
{
    struct mob_data *md = (struct mob_data*)bl;
    int id = va_arg(ap, int);
    if (md->class_ == id)
        return 1;
    return 0;
}

ACMD(mapmoblist)
{
	char temp[100];
    bool mob_searched[MAX_MOB_DB];
    bool mob_mvp[MAX_MOB_DB]; // Store mvp data..
    struct s_mapiterator* it;
    unsigned short count = 0, i, mapindex = 0;
    int m = 0;

    memset(mob_searched, 0, MAX_MOB_DB);
    memset(mob_mvp, 0, MAX_MOB_DB);

    if (message && *message) {
        // Player input map name, search mob list for that map
        mapindex = mapindex_name2id(message);
        if (!mapindex) {
            clif->message(fd, "Map not found");
            return -1;
        }
        m = map_mapindex2mapid(mapindex);
    } else {
        // Player doesn't input map name, search mob list in player current map
        mapindex = sd->mapindex;
        m = sd->bl.m;
    }

    clif->message(fd, "--------Monster List--------");

    sprintf(temp, "Mapname: %s", mapindex_id2name(mapindex));
    clif->message(fd, temp);

    clif->message(fd, "Monsters: ");

    //Looping and search for mobs
    it = mapit_geteachmob();
    while (true) {
        TBL_MOB* md = (TBL_MOB*)mapit->next(it);
        if (md == NULL)
            break;

        if (md->bl.m != m || md->status.hp <= 0)
            continue;
        if (mob_searched[md->class_] == true)
            continue; // Already found, skip it
        if (mob_db(md->class_)->mexp) {
            mob_searched[md->class_] = true;
            mob_mvp[md->class_] = true; // Save id for later
            continue; // It's MVP!
        }

        mob_searched[md->class_] = true;
        count = map_foreachinmap(count_mob, m, BL_MOB, md->class_);

        sprintf(temp, " %s[%d] : %d", mob_db(md->class_)->jname, md->class_, count);

        clif->message(fd, temp);
    }
    mapit->free(it);

    clif->message(fd, "MVP: ");

    // Looping again and search for mvp, not sure if this is the best way..
    for (i = 1000; i < MAX_MOB_DB; i++) { //First monster start at 1001 (Scorpion)
        if (mob_mvp[i] == true) {
            count = map_foreachinmap(count_mob, m, BL_MOB, i);
            sprintf(temp, " %s[%d] : %d", mob_db(i)->jname, i, count);
            clif->message(fd, temp);
        }
    }

	return true;
}

/* Server Startup */
HPExport void plugin_init (void)
{
	clif = GET_SYMBOL("clif");
    script = GET_SYMBOL("script");
    battle = GET_SYMBOL("battle");

    if( HPMi->addCommand != NULL ) {//link our '@sample' command
		HPMi->addCommand("mapmoblist",ACMD_A(mapmoblist));
	}
}

