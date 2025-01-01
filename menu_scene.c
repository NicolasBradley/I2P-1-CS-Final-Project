#include <allegro5/allegro.h>
#include <string.h>
#include "menu_scene.h"
#include "game_scene.h"
#include "setting_scene.h"
#include "loading_scene.h"
#include "utility.h"
#include "UI.h"
#include "game.h"
#include "map.h"
#include "Leaderboard.h"

Scene create_shop_scene(void);

static bool prev_mouse_press;
static bool cur_mouse_press;

//Menu
static Button ShopButton;
static Button settingButton;
static Button RightButton;
static Button LeftButton;
static Button ScoreButton;


//Shop
static Button backButton;


ALLEGRO_SAMPLE* MenuSFX;
ALLEGRO_SAMPLE* BuySFX;
ALLEGRO_SAMPLE* ErrorSFX;

ALLEGRO_BITMAP* bg_panda;
ALLEGRO_BITMAP* bg_nthu;



int slide = 0;


static void init(void) {
    slide = 0;
    ShopButton = button_create(SCREEN_W / 2 - 200, 600, 400, 100, "UI_Button.png", "UI_Button_hovered.png");
    settingButton = button_create(SCREEN_W / 2 - 200, 600, 400, 100, "UI_Button.png", "UI_Button_hovered.png");
    ScoreButton = button_create(SCREEN_W / 2 - 200, 600, 400, 100, "UI_Button.png", "UI_Button_hovered.png");
    
    RightButton = button_create(SCREEN_W / 2+180, 580, 150, 150, "Right.png", "Right_Hovered.png");
    LeftButton = button_create(SCREEN_W - 740, 580, 150, 150, "Left.png", "Left_Hovered.png");
  
    MenuSFX = al_load_sample("MenuSFX.ogg");
    BuySFX = al_load_sample("sfx_buy.ogg");
    ErrorSFX = al_load_sample("sfx_error.ogg");
    
    bg_panda = al_load_bitmap("menu_bg.png");
    bg_nthu = al_load_bitmap("nthu_no_background.png");
    
    change_bgm("menu_bgm.ogg");
}

static void update(void) {
    update_button(&settingButton);
    update_button(&ShopButton);
    update_button(&ScoreButton);

    update_button(&RightButton);
    update_button(&LeftButton);

    if(keyState[ALLEGRO_KEY_ENTER]){
        change_scene(create_loading_scene());
    }
  
//    slide = 0;
    /*
        [TODO HACKATHON 4-3] 
        
        Change scene to setting scene when the button is pressed
    */
   
    if ((mouseState.buttons && RightButton.hovered) || keyState[ALLEGRO_KEY_RIGHT]) {
        
        if(slide>=2)slide+=0;
        else{
            al_play_sample(MenuSFX, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            slide+=1;
        }
        al_rest(0.5);
        
    }
    if ((mouseState.buttons && LeftButton.hovered) || keyState[ALLEGRO_KEY_LEFT]) {
        
        if(slide==0)slide+=0;
        else{
            slide--;
            al_play_sample(MenuSFX, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        }
        al_rest(0.5);
    }
    if (mouseState.buttons && ScoreButton.hovered && slide == 0) {
        al_play_sample(MenuSFX, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
//        if(finished){
//            LeaderboardEntry leaderboard;
//            while()
//            leaderboard[]
//        }
        change_scene(create_score_scene());
    }
    if (mouseState.buttons && ShopButton.hovered && slide == 1) {
        al_play_sample(MenuSFX, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        change_scene(create_shop_scene());
    }
    if (mouseState.buttons && settingButton.hovered && slide == 2) {
        al_play_sample(MenuSFX, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        change_scene(create_setting_scene());
    }
   
    
}

static void draw(void) {
    
    // Title Text
    al_draw_scaled_bitmap(bg_panda, 0, 0, 1300, 1300,0 ,0, 1024 , 1024, 0);
    al_draw_text(
        TITLE_FONT,
        al_map_rgb(239, 205, 250), //Bottom layer
        SCREEN_W / 2-3,
        145,
        ALLEGRO_ALIGN_CENTER,
        "PANDA\'S REVENGE"
//        "NTHU-RPG ADVENTURE"
    );
    al_draw_scaled_bitmap(bg_nthu, 0, 0, 302, 303, SCREEN_W/2-50, 30/*SCREEN_H/2-100*/, 100 , 100, 0);
    al_draw_text(
        TITLE_FONT,
        al_map_rgb(196, 8, 255), //TOP layer
        SCREEN_W / 2,
        140,
        ALLEGRO_ALIGN_CENTER,
        "PANDA\'S REVENGE"
//        "NTHU-RPG ADVENTURE"
    );
    al_draw_text(
        P2_FONT,
        al_map_rgb(10,10,10),
        SCREEN_W / 2+3,
        510,
        ALLEGRO_ALIGN_CENTER,
        "PRESS [ENTER] TO PLAY"
    );
    al_draw_text(
        P2_FONT,
        al_map_rgb(255, 255, 255),
        SCREEN_W / 2,
        500,
        ALLEGRO_ALIGN_CENTER,
        "PRESS [ENTER] TO PLAY"
    );

    if(slide == 0){
        draw_button(ScoreButton);
        draw_button(RightButton);
        //draw_button(LeftButton);
        // button text
        al_draw_text(
                     P2_FONT,
                     al_map_rgb(66, 76, 110),
                     SCREEN_W / 2,
                     600 + 28 + ScoreButton.hovered * 11,
                     ALLEGRO_ALIGN_CENTER,
                     "SCOREBOARD"
                     );
        al_draw_text(
                     P2_FONT,
                     al_map_rgb(225, 225, 225),
                     SCREEN_W / 2,
                     600 + 31 + ScoreButton.hovered * 11,
                     ALLEGRO_ALIGN_CENTER,
                     "SCOREBOARD"
                     );
   
    }
    else if(slide == 1){
        
        draw_button(ShopButton);
        draw_button(LeftButton);
        draw_button(RightButton);
        // button text
        al_draw_text(
                     P2_FONT,
                     al_map_rgb(66, 76, 110),
                     SCREEN_W / 2,
                     600 + 28 + ShopButton.hovered * 11,
                     ALLEGRO_ALIGN_CENTER,
                     "SHOP"
                     );
        al_draw_text(
                     P2_FONT,
                     al_map_rgb(225, 225, 225),
                     SCREEN_W / 2,
                     600 + 31 + ShopButton.hovered * 11,
                     ALLEGRO_ALIGN_CENTER,
                     "SHOP"
                     );
    }
    else if(slide == 2){
         //button
        draw_button(settingButton);
        draw_button(LeftButton);
        // button text
        al_draw_text(
            P2_FONT,
            al_map_rgb(66, 76, 110),
            SCREEN_W / 2,
            600 + 28 + settingButton.hovered * 11,
            ALLEGRO_ALIGN_CENTER,
            "SETTING"
        );
        al_draw_text(
            P2_FONT,
            al_map_rgb(225, 225, 225),
            SCREEN_W / 2,
            600 + 31 + settingButton.hovered * 11,
            ALLEGRO_ALIGN_CENTER,
            "SETTING"
        );
    }
  
  
}

static void destroy(void) {
    destroy_button(&settingButton);
    destroy_button(&ShopButton);
    destroy_button(&RightButton);
    destroy_button(&LeftButton);
    destroy_button(&ScoreButton);
}


Scene create_menu_scene(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    scene.name = "menu";
    scene.init = &init;
    scene.draw = &draw;
    scene.update = &update;
    scene.destroy = &destroy;

    return scene;
}


//
//SHOP==============================
//

int guntype = 0;
int sniper_price = 3;

ALLEGRO_BITMAP* bg_shop;
ALLEGRO_BITMAP* DOMO;
ALLEGRO_BITMAP* Gun;
ALLEGRO_BITMAP* Sniper;
ALLEGRO_BITMAP* Coin_bitmap;
static Button SniperButton;
static Button GunButton;
static Button BuyButton;

gun_status ak_EQstatus= EQUIPED;
gun_status sniper_EQstatus= UNEQUIPED;
gun_status sniper_Buystatus=BUY;

static void init_shop(void) {
    al_rest(0.3);
    slide = 0;
    //guntype = 0;
    //Images
    bg_shop = al_load_bitmap("bg_shop.png");
    DOMO = al_load_bitmap("DOMO.png");
    
    Gun = al_load_bitmap("guns.png");
    Sniper = al_load_bitmap("sniper.png");
    Coin_bitmap = al_load_bitmap("coin_icon.png");
   
    //Buttons
    RightButton = button_create(SCREEN_W / 2+180, SCREEN_H/2-90, 150, 150, "Right.png", "Right_Hovered.png");
    LeftButton = button_create(SCREEN_W - 740, SCREEN_H/2-90, 150, 150, "Left.png", "Left_Hovered.png");
    backButton = button_create(SCREEN_W / 2 - 200, 650, 400, 100, "UI_Button.png", "UI_Button_hovered.png");
    
    GunButton = button_create(SCREEN_W / 2 - 200, 540, 400, 100, "UI_Button.png", "UI_Button_hovered.png");
    SniperButton = button_create(SCREEN_W / 2 - 200, 540, 400, 100, "UI_Button.png", "UI_Button_hovered.png");
    BuyButton = button_create(SCREEN_W / 2 - 200, 540, 400, 100, "UI_Button.png", "UI_Button_hovered.png");
}

static void update_shop(void) {
    update_button(&RightButton);
    update_button(&LeftButton);
    update_button(&backButton);
    
    update_button(&GunButton);
    update_button(&SniperButton);

    prev_mouse_press = cur_mouse_press;
    cur_mouse_press = mouseState.buttons;
    
    if(!prev_mouse_press && cur_mouse_press){
        if (mouseState.buttons && backButton.hovered == true) {
            al_play_sample(MenuSFX, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            change_scene(create_menu_scene());
        }
    }
    if (mouseState.buttons && RightButton.hovered) {
        
        if(slide>=1)slide+=0;
        else{
            al_play_sample(MenuSFX, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            slide+=1;
        }
        al_rest(0.5);
        
    }
    if (mouseState.buttons && LeftButton.hovered) {
        
        if(slide==0)slide+=0;
        else{
            slide--;
            al_play_sample(MenuSFX, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        }
        al_rest(0.5);
    }
    if (mouseState.buttons && GunButton.hovered == true && slide == 0) {

        if(ak_EQstatus == UNEQUIPED){
            al_play_sample(MenuSFX, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            ak_EQstatus = EQUIPED;
            sniper_EQstatus = UNEQUIPED;
            guntype = 0;
        }
        else{
            return;
        }
        
    }
    if (mouseState.buttons && SniperButton.hovered == true && slide == 1) {
        al_rest(0.5);
        if(sniper_Buystatus == BUY && overall_coins >= sniper_price){
            al_play_sample(BuySFX, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            sniper_Buystatus = BOUGHT;
            overall_coins -= sniper_price;
        }else if(sniper_Buystatus == BUY && overall_coins <= sniper_price){
            al_play_sample(ErrorSFX, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            al_rest(1.0);
        }
        if(sniper_Buystatus == BOUGHT && sniper_EQstatus == UNEQUIPED){
                al_play_sample(MenuSFX, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                sniper_EQstatus = EQUIPED;
                ak_EQstatus = UNEQUIPED;
                guntype = 1;
        }else{
            return;
        }
        

    }
    
  
    
    
}

static void draw_shop(void) {
    
    al_draw_scaled_bitmap(bg_shop, 0, 0, 1250, 1200,0 ,0, 1024 , 1024, 0);
    al_draw_scaled_bitmap(DOMO, 0, 0, 500, 500, SCREEN_W/2+150,SCREEN_H/2+100, 300 , 300, 0);
    
    
    al_draw_text(
        TITLE_FONT,
        al_map_rgb(13, 12, 12), //Bottom layer
        SCREEN_W / 2-3,
        145,
        ALLEGRO_ALIGN_CENTER,
        "DOMO\'S SHOP"

    );
    al_draw_text(
        TITLE_FONT,
        al_map_rgb(227, 160, 34), //TOP layer
        SCREEN_W / 2,
        140,
        ALLEGRO_ALIGN_CENTER,
        "DOMO\'S SHOP"
    );
    
    
    al_draw_scaled_bitmap(Coin_bitmap, 0, 0, 16, 16, 0, 100, 100 , 100, 0);//coin
    al_draw_textf(
        P1_FONT,
        al_map_rgb(225, 225, 225),
        110,
        130,
        ALLEGRO_ALIGN_LEFT,
        "%d", overall_coins
    );
    
    if(slide == 0){
        draw_button(GunButton);
        draw_button(RightButton);
        
        al_draw_scaled_bitmap(Gun, 0, 0, 32, 32, SCREEN_W/2-145 ,SCREEN_H/2-150, 250 , 250, 0);
        al_draw_text(
                     P1_FONT,
                     al_map_rgb(247, 212, 54),
                     SCREEN_W / 2,
                     440 + 28,
                     ALLEGRO_ALIGN_CENTER,
                     "AK-47"
                     );
        al_draw_text(
                     P1_FONT,
                     al_map_rgb(255, 224, 64),
                     SCREEN_W / 2,
                     240,
                     ALLEGRO_ALIGN_CENTER,
                     "Damage : 10"
                     );
        if(ak_EQstatus == UNEQUIPED){
            al_draw_text(
                         P2_FONT,
                         al_map_rgb(66, 76, 110),
                         SCREEN_W / 2,
                         540 + 28 + GunButton.hovered * 11,
                         ALLEGRO_ALIGN_CENTER,
                         "Equip"
                         );
            al_draw_text(
                         P2_FONT,
                         al_map_rgb(225, 225, 225),
                         SCREEN_W / 2,
                         540 + 31 + GunButton.hovered * 11,
                         ALLEGRO_ALIGN_CENTER,
                         "Equip"
                         );
        }else{
            al_draw_text(
                         P2_FONT,
                         al_map_rgb(66, 76, 110),
                         SCREEN_W / 2,
                         540 + 28 + GunButton.hovered * 11,
                         ALLEGRO_ALIGN_CENTER,
                         "Equiped"
                         );
//            al_draw_text(
//                         P2_FONT,
//                         al_map_rgb(225, 225, 225),
//                         SCREEN_W / 2,
//                         540 + 31 + GunButton.hovered * 11,
//                         ALLEGRO_ALIGN_CENTER,
//                         "Equiped"
//                         );
        }
       
   
    }
    else if(slide == 1){
        
        draw_button(SniperButton);
        draw_button(LeftButton);
       // draw_button(RightButton);
        al_draw_text(
                     P1_FONT,
                     al_map_rgb(247, 212, 54),
                     SCREEN_W / 2,
                     440 + 28,
                     ALLEGRO_ALIGN_CENTER,
                     "Blaster"
                     );
        al_draw_text(
                     P1_FONT,
                     al_map_rgb(255, 224, 64),
                     SCREEN_W / 2,
                     240,
                     ALLEGRO_ALIGN_CENTER,
                     "Damage : 50"
                     );
        if(sniper_Buystatus == BOUGHT){
            al_draw_scaled_bitmap(Sniper, 0, 0, 32, 32, SCREEN_W/2-80 ,SCREEN_H/2-150, 280 , 280, 0);
        }else{
            al_draw_tinted_scaled_bitmap(Sniper, al_map_rgba_f(0.5, 0.5, 0.5, 0.5), 0, 0, 32, 32, SCREEN_W/2-100 ,SCREEN_H/2-150, 280 , 280, 0);
            al_draw_text(
                         P1_FONT,
                         al_map_rgb(255, 224, 64),
                         SCREEN_W/2,
                         SCREEN_H/2-50,
                         ALLEGRO_ALIGN_CENTER,
                         "3 Coins"
                         );
        }
        
        if(sniper_Buystatus == BUY){
            al_draw_text(
                         P2_FONT,
                         al_map_rgb(66, 76, 110),
                         SCREEN_W / 2,
                         540 + 28 + BuyButton.hovered * 11,
                         ALLEGRO_ALIGN_CENTER,
                         "Buy"
                         );
            al_draw_text(
                         P2_FONT,
                         al_map_rgb(225, 225, 225),
                         SCREEN_W / 2,
                         540 + 31 + BuyButton.hovered * 11,
                         ALLEGRO_ALIGN_CENTER,
                         "Buy"
                         );
        }else{
            if(sniper_EQstatus == UNEQUIPED){
                al_draw_text(
                             P2_FONT,
                             al_map_rgb(66, 76, 110),
                             SCREEN_W / 2,
                             540 + 28 + SniperButton.hovered * 11,
                             ALLEGRO_ALIGN_CENTER,
                             "Equip"
                             );
                al_draw_text(
                             P2_FONT,
                             al_map_rgb(225, 225, 225),
                             SCREEN_W / 2,
                             540 + 31 + SniperButton.hovered * 11,
                             ALLEGRO_ALIGN_CENTER,
                             "Equip"
                             );
            }
            else{
                al_draw_text(
                             P2_FONT,
                             al_map_rgb(66, 76, 110),
                             SCREEN_W / 2,
                             540 + 28 + SniperButton.hovered * 11,
                             ALLEGRO_ALIGN_CENTER,
                             "Equiped"
                             );
//                al_draw_text(
//                             P2_FONT,
//                             al_map_rgb(225, 225, 225),
//                             SCREEN_W / 2,
//                             540 + 31 + SniperButton.hovered * 11,
//                             ALLEGRO_ALIGN_CENTER,
//                             "Equiped"
//                             );
            }
            
        }
       
    }
    
    
    
    
    
    
    
    
    draw_button(backButton);

    al_draw_text(
        P2_FONT,
        al_map_rgb(66, 76, 110),
        SCREEN_W / 2,
        650 + 28 + backButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "BACK"
    );
    al_draw_text(
        P2_FONT,
        al_map_rgb(225, 225, 225),
        SCREEN_W / 2,
        650 + 31 + backButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "BACK"
    );
  
}

static void destroy_shop(void) {

    destroy_button(&backButton);
    
}
Scene create_shop_scene(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    scene.name = "shop";
    scene.init = &init_shop;
    scene.draw = &draw_shop;
    scene.update = &update_shop;
    scene.destroy = &destroy_shop;

    return scene;
}
