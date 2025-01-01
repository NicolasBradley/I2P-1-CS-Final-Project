#include <allegro5/allegro.h>
#include "setting_scene.h"
#include "menu_scene.h"
#include "utility.h"
#include "UI.h"
#include "game.h"

static Button backButton;
static bool prev_mouse_press;
static bool cur_mouse_press;

ALLEGRO_SAMPLE* SettingSFX;
ALLEGRO_BITMAP* slider;
ALLEGRO_BITMAP* slider_hovered;
ALLEGRO_BITMAP* bg_settings;

static void init(void) {
    al_rest(0.3);
    backButton = button_create(SCREEN_W / 2 - 200, 650, 400, 100, "UI_Button.png", "UI_Button_hovered.png");
    SettingSFX = al_load_sample("MenuSFX.ogg");
    slider = al_load_bitmap("slider.png");
    slider_hovered = al_load_bitmap("slider_hovered.png");
    bg_settings = al_load_bitmap("bg_shop.png");
}

static void update(void) {

    update_button(&backButton);
    prev_mouse_press = cur_mouse_press;
    cur_mouse_press = mouseState.buttons;
    if(!prev_mouse_press && cur_mouse_press){
        if (mouseState.buttons && backButton.hovered == true) {
            al_play_sample(SettingSFX, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            change_scene(create_menu_scene());
        }
    }
    
    if (mouseState.buttons && mouseState.y > SCREEN_H/3 && mouseState.y < SCREEN_H/3 + 30) {
        
        BGM_VOLUME = (mouseState.x-SCREEN_W/4)/520.0f;
        
        if(mouseState.x <= SCREEN_W/4 && mouseState.buttons){
            BGM_VOLUME = 0.0f;
            
        }
        if(mouseState.x >= SCREEN_W/4 +520 && mouseState.buttons){
            BGM_VOLUME = 1.0f;
           
        }
       
    }
    
    if (mouseState.buttons && mouseState.y > SCREEN_H/2 && mouseState.y < SCREEN_H/2 + 30) {
        
        SFX_VOLUME = (mouseState.x-SCREEN_W/4)/520.0f;
        
        if(mouseState.x <= SCREEN_W/4 && mouseState.buttons){
            SFX_VOLUME = 0.0f;
            
        }
        if(mouseState.x >= SCREEN_W/4 +520 && mouseState.buttons){
            SFX_VOLUME = 1.0f;
            
        }
       
    }
    

}

static void draw(void) {
    al_draw_scaled_bitmap(bg_settings, 0, 0, 900, 900,-150 ,-90, 1024 , 1024, 0);
    
    // button
    draw_button(backButton);
    
    al_draw_text(
        TITLE_FONT,
        al_map_rgb(13, 12, 12), //Bottom layer
        SCREEN_W / 2-3,
        145,
        ALLEGRO_ALIGN_CENTER,
        "SETTINGS"
    );
    al_draw_text(
        TITLE_FONT,
        al_map_rgb(227, 160, 34), //TOP layer
        SCREEN_W / 2,
        140,
        ALLEGRO_ALIGN_CENTER,
        "SETTINGS"
    );
    
    
    al_draw_text(
        P1_FONT,
        al_map_rgb(13, 12, 12), //Bottom layer
        SCREEN_W / 2-3 -300,
        SCREEN_H/3-10,
        ALLEGRO_ALIGN_CENTER,
        "BGM"
    );
    al_draw_text(
        P1_FONT,
        al_map_rgb(227, 160, 34), //TOP layer
        SCREEN_W / 2 -300,
        SCREEN_H/3-10,
        ALLEGRO_ALIGN_CENTER,
        "BGM"
    );
    
    
    al_draw_text(
        P1_FONT,
        al_map_rgb(13, 12, 12), //Bottom layer
        SCREEN_W / 2-3 -300,
        SCREEN_H/2-10,
        ALLEGRO_ALIGN_CENTER,
        "SFX"
    );
    al_draw_text(
        P1_FONT,
        al_map_rgb(227, 160, 34), //TOP layer
        SCREEN_W / 2 -300,
        SCREEN_H/2-10,
        ALLEGRO_ALIGN_CENTER,
        "SFX"
    );
    
    
//    al_draw_text(
//        P2_FONT,
//        al_map_rgb(13, 12, 12), //Bottom layer
//        SCREEN_W / 2-3,
//        SCREEN_H/2+200,
//        ALLEGRO_ALIGN_CENTER,
//        "Hold Shift to Sprint"
//    );
//    al_draw_text(
//        P2_FONT,
//        al_map_rgb(238, 245, 237), //TOP layer
//        SCREEN_W / 2,
//        SCREEN_H/2+200,
//        ALLEGRO_ALIGN_CENTER,
//        "Hold Shift to Sprint"
//    );
    //button text
    al_draw_text(
        P2_FONT,
        al_map_rgb(66, 76, 110),
        SCREEN_W / 2,
        650 + 28 + backButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "APPLY"
    );
    al_draw_text(
        P2_FONT,
        al_map_rgb(225, 225, 225),
        SCREEN_W / 2,
        650 + 31 + backButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "APPLY"
    );
    //BGM
    al_draw_scaled_bitmap(slider, 0, 0, 600, 20, SCREEN_W/4, SCREEN_H/3, 520, 35, 0);//size of background slider
    al_draw_scaled_bitmap(slider_hovered, 0, 0, 600, 20, SCREEN_W/4, SCREEN_H/3, 520*BGM_VOLUME, 35, 0);//slider
    //SFX_VOLUME
    al_draw_scaled_bitmap(slider, 0, 0, 600, 20, SCREEN_W/4, SCREEN_H/2, 520, 35, 0);//size of background slider
    al_draw_scaled_bitmap(slider_hovered, 0, 0, 600, 20, SCREEN_W/4, SCREEN_H/2, 520*SFX_VOLUME, 35, 0);//slider
    
   
}

static void destroy(void) {
    destroy_button(&backButton);
}

Scene create_setting_scene(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    scene.name = "setting";
    scene.init = &init;
    scene.draw = &draw;
    scene.update = &update;
    scene.destroy = &destroy;

    return scene;
}
