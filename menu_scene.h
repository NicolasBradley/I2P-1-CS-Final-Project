#ifndef menu_scene_h
#define menu_scene_h

#include "utility.h"

extern char name[100];
Scene create_menu_scene(void);
typedef enum _GUN_STATUS {
    BUY,
    BOUGHT,
    EQUIPED,
    UNEQUIPED
} gun_status;
extern int guntype;

#endif /* menu_scene_h */
