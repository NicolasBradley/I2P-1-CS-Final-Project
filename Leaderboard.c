#include <allegro5/allegro.h>
#include <string.h>
#include "game_scene.h"
#include "menu_scene.h"
#include "loading_scene.h"
#include "game.h"
#include "player.h"
#include "map.h"
#include "enemy.h"
#include "weapon.h"
#include "UI.h"
#include <math.h>

#include "Leaderboard.h"



static Button backButton;
static Button EnterButton;
static bool prev_mouse_press;
static bool cur_mouse_press;

ALLEGRO_SAMPLE* LeaderSFX;
ALLEGRO_BITMAP* bg_board;
ALLEGRO_BITMAP* bg_name;
ALLEGRO_BITMAP* titlebg_img;

Scene create_score_scene(void);
Scene create_name_scene(void);

const char *filename = "/Users/nicolasbradleychandra/Documents/C projects NTHU/i2p_final/i2p_final/leaderboard.txt";
int count = 0;
int error =0;
int askname = 1;
char name[100];


static int timer_countdown;


#define MAX_NAME_LENGTH 50
#define MAX_ENTRIES 20

LeaderboardEntry leaderboard[MAX_ENTRIES];

void save_leaderboard(LeaderboardEntry leaderboard[], int count, const char *filename) {
//    if(finished){
//        FILE *file = fopen(filename, "w");
//        
//        if (file == NULL) {
//            printf("Error.\n");
//            return;
//        }
//        fprintf(file, "%s %.2lf\n", leaderboard[count-1].name, leaderboard[count-1].score);
//
//        fclose(file);
//    }
//    else{
        FILE *file = fopen(filename, "a");
        
        if (file == NULL) {
            printf("Error.\n");
            return;
        }
        fprintf(file, "%s %.2lf\n", leaderboard[count-1].name, leaderboard[count-1].score);

        fclose(file);
//    }
    
    

}

void bubbleSort(LeaderboardEntry leaderboard[], int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count -i - 1; j++) {
            if (leaderboard[j].score > leaderboard[j + 1].score) {
                // Swap scores
                int tempScore = leaderboard[j].score;
                leaderboard[j].score = leaderboard[j + 1].score;
                leaderboard[j + 1].score = tempScore;

                // Swap names using strcpy
                char tempName[51];
                strcpy(tempName, leaderboard[j].name);
                strcpy(leaderboard[j].name, leaderboard[j + 1].name);
                strcpy(leaderboard[j + 1].name, tempName);
            }
        }
    }
}

void load_leaderboard(LeaderboardEntry leaderboard[], int *count, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error Empty File\n");
        return;
    }

    *count = 0;
    printf("%d", finished);
    while (fscanf(file, "%s %f", leaderboard[*count].name, &leaderboard[*count].score) != EOF && *count < MAX_ENTRIES ) {
        if(finished){
            if(leaderboard[*count].name == name){
                leaderboard[*count].score = total_time;
                save_leaderboard(leaderboard, *count, filename);
            }
        }
        (*count)++;
    }

    fclose(file);
    
}

void add_score(LeaderboardEntry leaderboard[], int *count, const char *name, int score) {
    
    if (*count < MAX_ENTRIES) {
        strcpy(leaderboard[*count].name, name);
        leaderboard[*count].score = score;
        (*count)++;
    } else {
        printf("Leaderboard is full.\n");
    }
}

void print_leaderboard(LeaderboardEntry leaderboard[], int count) {
    for (int i = 0; i < 7; i++) {
        al_draw_textf(
                     P1_FONT,
                     al_map_rgb(/*255, 224, 64*/8, 8, 8),
                     SCREEN_W/2-230,
                     SCREEN_H/2-140+i*50,
                     ALLEGRO_ALIGN_LEFT,
                     "%d. ",i+1
                     );
        al_draw_textf(
                     P1_FONT,
                     al_map_rgb(255, 224, 64),
                     SCREEN_W/2-235,
                     SCREEN_H/2-143+i*50,
                     ALLEGRO_ALIGN_LEFT,
                     "%d. ",i+1
                     );
        al_draw_textf(
                     P1_FONT,
                     al_map_rgb(/*255, 224, 64*/8, 8, 8),
                     SCREEN_W/2-190,
                     SCREEN_H/2-140+i*50,
                     ALLEGRO_ALIGN_LEFT,
                     "%s", leaderboard[i].name
                     );
        al_draw_textf(
                     P1_FONT,
                     al_map_rgb(255, 224, 64),
                     SCREEN_W/2-185,
                     SCREEN_H/2-143+i*50,
                     ALLEGRO_ALIGN_LEFT,
                     "%s", leaderboard[i].name
                     );
        al_draw_textf(
                     P1_FONT,
                     al_map_rgb(/*255, 224, 64*/8, 8, 8),
                     SCREEN_W/2+160,
                     SCREEN_H/2-140+i*50,
                     ALLEGRO_ALIGN_CENTER,
                     "%.2lf", leaderboard[i].score
                     );
        al_draw_textf(
                     P1_FONT,
                     al_map_rgb(255, 224, 64),
                     SCREEN_W/2+155,
                     SCREEN_H/2-143+i*50,
                     ALLEGRO_ALIGN_CENTER,
                     "%.2lf", leaderboard[i].score
                     );
    }
}


static void init_score(void) {
    al_rest(0.3);
    
    
    bg_board = al_load_bitmap("Board_bg.png");
    LeaderSFX = al_load_sample("MenuSFX.ogg");
    
    titlebg_img = al_load_bitmap("UI_Button.png");
    backButton = button_create(SCREEN_W / 2 - 200, 650, 400, 100, "UI_Button.png", "UI_Button_hovered.png");
//    bg_shop = al_load_bitmap("bg_shop.png");
//    DOMO = al_load_bitmap("DOMO.png");
    
    count = 0;

//    add_score(leaderboard, &count, "Alice", 100);
//    add_score(leaderboard, &count, "Bob", 200);

//    save_leaderboard(leaderboard, count, filename);
    
//    compute();
    load_leaderboard(leaderboard, &count, filename);
}

static void update_score(void) {
    
    update_button(&backButton);
    
    
//    if(askname == 1){
//        change_scene(create_name_scene());
//    }
    
    prev_mouse_press = cur_mouse_press;
    cur_mouse_press = mouseState.buttons;
    
    if(!prev_mouse_press && cur_mouse_press){
        if (mouseState.buttons && backButton.hovered == true) {
            al_play_sample(LeaderSFX, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            change_scene(create_menu_scene());
        }
    }
    


    
    
}

static void draw_score(void) {
    al_draw_scaled_bitmap(bg_board, 0, 0, 1300, 1300,0 ,0, 1024 , 1024, 0);
    al_draw_filled_rounded_rectangle(0+100, 0+100, 720, 620, 10, 10, al_map_rgba_f(0.3, 0.5, 0.5, 0.5));
    
    bubbleSort(leaderboard, count);
    print_leaderboard(leaderboard, count);

    draw_button(backButton);
    
    al_draw_tinted_scaled_bitmap(titlebg_img, al_map_rgba_f(0.8, 0.8, 0.8, 0.8), 0, 0, 222, 108, (SCREEN_W / 2)-330 ,80, 680 , 108, 0);
    al_draw_text(
        TITLE_FONT,
        al_map_rgb(13, 12, 12), //Bottom layer
        SCREEN_W / 2-3+10,
        105,
        ALLEGRO_ALIGN_CENTER,
        "LEADER BOARD"

    );

    al_draw_text(
        TITLE_FONT,
        al_map_rgb(227, 160, 34), //TOP layer
        SCREEN_W / 2+10,
        100,
        ALLEGRO_ALIGN_CENTER,
        "LEADER BOARD"

    );
    
    al_draw_textf(
                 P1_FONT,
                 al_map_rgb(/*255, 224, 64*/8, 8, 8),
                 SCREEN_W/2-100,
                 SCREEN_H/2-200,
                 ALLEGRO_ALIGN_RIGHT,
                 "Name"
                 );
    al_draw_textf(
                 P1_FONT,
                 al_map_rgb(237, 14, 193),
                 SCREEN_W/2-95,
                 SCREEN_H/2-197,
                 ALLEGRO_ALIGN_RIGHT,
                 "Name"
                 );
    al_draw_textf(
                 P1_FONT,
                 al_map_rgb(/*255, 224, 64*/8, 8, 8),
                 SCREEN_W/2+170,
                 SCREEN_H/2-200,
                 ALLEGRO_ALIGN_CENTER,
                 "Time Taken"
                 );
    al_draw_textf(
                 P1_FONT,
                 al_map_rgb(237, 14, 193),
                 SCREEN_W/2+165,
                 SCREEN_H/2-197,
                 ALLEGRO_ALIGN_CENTER,
                 "Time Taken"
                 );
    
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

static void destroy_score(void) {

    destroy_button(&backButton);
    
}
Scene create_score_scene(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    scene.name = "score";
    scene.init = &init_score;
    scene.draw = &draw_score;
    scene.update = &update_score;
    scene.destroy = &destroy_score;

    return scene;
}



//SUBMIT SCORE



ALLEGRO_SAMPLE* ErrorrSFX;
static void init_name(void) {
    al_rest(0.3);
    ErrorrSFX = al_load_sample("sfx_error.ogg");
    bg_board = al_load_bitmap("Board_bg.png");
   // bg_name = al_load_bitmap("name_bg.png");
    LeaderSFX = al_load_sample("MenuSFX.ogg");
   // backButton = button_create(SCREEN_W / 2 - 200, 650, 400, 100, "UI_Button.png", "UI_Button_hovered.png");
    titlebg_img = al_load_bitmap("UI_Button.png");
    EnterButton = button_create(SCREEN_W / 2 - 200, 550, 400, 100, "UI_Button.png", "UI_Button_hovered.png");
}

static void update_name(void) {
    if (timer_countdown == 0){
        error = 0;
    }
    if(error == 1){
        timer_countdown--;
    }
    //update_button(&backButton);
    update_button(&EnterButton);
    
    prev_mouse_press = cur_mouse_press;
    cur_mouse_press = mouseState.buttons;
    
//    if(!prev_mouse_press && cur_mouse_press){
//        if (mouseState.buttons && backButton.hovered == true) {
//            al_play_sample(LeaderSFX, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
//            change_scene(create_menu_scene());
//        }
//    }
    

    if (mouseState.buttons && EnterButton.hovered == true && finished) {
        al_play_sample(LeaderSFX, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        askname = 0;
        add_score(leaderboard, &count, name, total_time);
        save_leaderboard(leaderboard, count, filename);
        change_scene(create_score_scene());
    }
    else if(mouseState.buttons && EnterButton.hovered == true && !finished){
        timer_countdown = 60;
        al_play_sample(ErrorrSFX, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        al_rest(1.0);
        error = 1;
    }
    al_draw_text(
        TITLE_FONT,
        al_map_rgb(13, 12, 12), //Bottom layer
        SCREEN_W / 2-3,
        145,
        ALLEGRO_ALIGN_CENTER,
        "LEADER BOARD"

    );

    al_draw_text(
        TITLE_FONT,
        al_map_rgb(227, 160, 34), //TOP layer
        SCREEN_W / 2,
        140,
        ALLEGRO_ALIGN_CENTER,
        "LEADER BOARD"

    );
    
    if(keyState[ALLEGRO_KEY_BACKSPACE] && strlen(name) > 0){
        name[strlen(name) - 1] = '\0';
        keyState[ALLEGRO_KEY_BACKSPACE] = false;
    }
    
    for(int i = ALLEGRO_KEY_A; i <= ALLEGRO_KEY_Z && strlen(name) <= 5; i++){
        if(keyState[i]){
            name[strlen(name)] = i -1 + 'A';
            keyState[i] = false;
        }
    }
    
    
}

static void draw_name(void) {
    al_draw_scaled_bitmap(bg_board, 0, 0, 1300, 1300,0 ,0, 1024 , 1024, 0);
//    draw_button(backButton);
    draw_button(EnterButton);
    al_draw_tinted_scaled_bitmap(titlebg_img, al_map_rgba_f(0.8, 0.8, 0.8, 0.8), 0, 0, 222, 108, (SCREEN_W / 2)-330 ,320, 680 , 108, 0);
    al_draw_text(
        TITLE_FONT,
        al_map_rgb(13, 12, 12), //Bottom layer
        SCREEN_W / 2-3+10,
        135,
        ALLEGRO_ALIGN_CENTER,
        "New WINNER Detected"

    );
    al_draw_text(
        TITLE_FONT,
        al_map_rgb(247, 245, 245),
        SCREEN_W / 2,
        140,
        ALLEGRO_ALIGN_CENTER,
        "New WINNER Detected"
    );
    if(error == 1){
        al_draw_text(
            P2_FONT,
            al_map_rgb(13, 12, 12), //Bottom layer
            SCREEN_W / 2-3,
            355,
            ALLEGRO_ALIGN_CENTER,
            "Complete All Levels To unlock!"

        );
        al_draw_text(
            P2_FONT,
            al_map_rgb(250, 246, 37),
            SCREEN_W / 2,
            350,
            ALLEGRO_ALIGN_CENTER,
            "Complete All Levels To Unlock!"
        );
    }
    
    if(error != 1){
        al_draw_text(
            TITLE_FONT,
            al_map_rgb(235, 231, 40),
            SCREEN_W / 2+20,
            340,
            ALLEGRO_ALIGN_CENTER,
            name

        );
    }
    
    al_draw_text(
        P2_FONT,
        al_map_rgb(66, 76, 110),
        SCREEN_W / 2,
        550 + 28 + EnterButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "SAVE"
    );
    al_draw_text(
        P2_FONT,
        al_map_rgb(225, 225, 225),
        SCREEN_W / 2,
        550 + 31 + EnterButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "SAVE"
    );
    
//    al_draw_text(
//        P2_FONT,
//        al_map_rgb(66, 76, 110),
//        SCREEN_W / 2,
//        650 + 28 + backButton.hovered * 11,
//        ALLEGRO_ALIGN_CENTER,
//        "BACK"
//    );
//    al_draw_text(
//        P2_FONT,
//        al_map_rgb(225, 225, 225),
//        SCREEN_W / 2,
//        650 + 31 + backButton.hovered * 11,
//        ALLEGRO_ALIGN_CENTER,
//        "BACK"
//    );
  
}

static void destroy_name(void) {
    destroy_button(&EnterButton);
//    destroy_button(&backButton);
    
}
Scene create_name_scene(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    scene.name = "name";
    scene.init = &init_name;
    scene.draw = &draw_name;
    scene.update = &update_name;
    scene.destroy = &destroy_name;

    return scene;
}
