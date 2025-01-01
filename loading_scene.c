#include <allegro5/allegro.h>
#include <string.h>
#include "menu_scene.h"
#include "loading_scene.h"
#include "game_scene.h"
#include "setting_scene.h"
#include "utility.h"
#include "UI.h"
#include "game.h"
#include "map.h"


int maplevel = 1;
ALLEGRO_BITMAP* trophy_img;
ALLEGRO_BITMAP* textbg_img;
static int trophy_animation = 0;

static int timer_countdown;

static ALLEGRO_BITMAP* player_bitmap;
static ALLEGRO_BITMAP* bg_bitmap;

static void init(void) {
    timer_countdown = 120;
    bg_bitmap = al_load_bitmap("Load_bg.jpg");
    player_bitmap = al_load_bitmap("panda2.png");
    if (!player_bitmap) {
        game_abort("Failed to load player bitmap");
    }
    trophy_img = al_load_bitmap("coins.png");
    textbg_img = al_load_bitmap("UI_Button.png");
    change_bgm("None");
}

static void update(void) {
    timer_countdown--;
    if (timer_countdown == 0) {
        change_scene(create_game_scene());
    }
    
    trophy_animation = (trophy_animation + 1) % 64/*(64 * 2)*/;
}

static void draw(void) {
    al_draw_scaled_bitmap(bg_bitmap, 0, 0, 1350, 1300,0 ,0, 1064 , 1024, 0);
    al_draw_textf(
        TITLE_FONT,
        al_map_rgb(255, 255, 255),/*al_map_rgb(146, 161, 185),*/
        SCREEN_W / 2 + 40,
        SCREEN_H / 2-50,
        ALLEGRO_ALIGN_CENTER,
//        "LEVEL 1"
        "LEVEL %d", maplevel
    );
    //Back text
    al_draw_tinted_scaled_bitmap(textbg_img, al_map_rgba_f(0.5, 0.5, 0.5, 0.5), 0, 0, 222, 108, (SCREEN_W / 2)-340 ,SCREEN_H/2+280, 700 , 108, 0);
    al_draw_textf(
        P1_FONT,
        al_map_rgb(0, 0, 0),
        (SCREEN_W / 2)+4,
        /*150*/SCREEN_H/2+305,
        ALLEGRO_ALIGN_CENTER,
        "GET ALL COINS TO WIN!"
                 );
    //front text
    al_draw_textf(
        P1_FONT,
        al_map_rgb(255, 235, 84),
        SCREEN_W / 2,
        /*150*/SCREEN_H/2+300,
        ALLEGRO_ALIGN_CENTER,
        "GET ALL COINS TO WIN!"
                 );
    
    int /*offsetx = 32 * (int)(trophy_animation / (64 / 9));*/ offsetx = 16 * (int)(trophy_animation / 8);
//    al_draw_tinted_scaled_bitmap(trophy_img,
//                            al_map_rgb(255, 255, 255),
//                            offsetx, 0, 32, 32,
//                                 SCREEN_W/2-50 , SCREEN_H/2+100, 100, 100,
//                            0);
    al_draw_scaled_bitmap(trophy_img,
        offsetx, 0, 16, 16,
        SCREEN_W/2-30 , /*SCREEN_H/2+*/200, 100, 100,
        0);
//    for(int i =0; i < timer_countdown; i++){
//       // al_rest(1.0);
//        al_draw_scaled_bitmap(al_load_bitmap("nthu_no_background.png"), 0, 0, 32, 32, i*70, 0, 100 , 100, 1);
//    }
    
//    al_draw_textf(
//        TITLE_FONT,
//        al_map_rgb(146, 161, 185),
//        SCREEN_W / 2 - 150,
//        SCREEN_H / 2+150,
//        ALLEGRO_ALIGN_CENTER,
////        "LEVEL 1"
//        "Loading..."
//    );
    
    
//    al_draw_scaled_bitmap(player_bitmap,
//        0, 0, 32, 32,
//        SCREEN_W / 4, SCREEN_H / 2-50, 64, 64, 0);
}

static void destroy(void) {
 //   al_destroy_bitmap(player_bitmap);
    al_destroy_bitmap(bg_bitmap);
}


Scene create_loading_scene(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    scene.name = "loading";
    scene.init = &init;
    scene.draw = &draw;
    scene.update = &update;
    scene.destroy = &destroy;

    return scene;
}
