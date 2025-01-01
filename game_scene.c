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

Player player; // Player
Map map; // Map
enemyNode * enemyList; // Enemy List
BulletNode * bulletList; // Bullet List

Enemy enemy;

static Button QuitButton;
static Button RestartButton;
static Button ContinueButton;
static Button backButton;

ALLEGRO_SAMPLE* gameSFX;
ALLEGRO_SAMPLE* shieldSFX;
//float record[100000];
//int total_coins;

ALLEGRO_BITMAP* lose_panda;
ALLEGRO_BITMAP* win_panda;
ALLEGRO_BITMAP* coin_count;

//double al_get_time(void);

double current_time, elapsed_time, ts = 0;

static int timer_countdown;
int enemy_countdown;



int overall_coins = 0;
float total_time = 0;
// Weapon
Weapon weapon; 

int coins_obtained;
int pause_play = 0;
//int maplevel = 1;
int finished = 0;
Scene create_lose_scene(void);
Scene create_win_scene(void);
Scene create_name_scene(void);
//Scene create_winning_scene(void);
ALLEGRO_BITMAP* hearts_bitmap;
ALLEGRO_BITMAP* bosshp_bitmap;


int hint = 200;
int play_sound = 0;

static void init(void){
    boss_countdown = 600;
    hint = 200;
    play_sound = 0;
    finished = 0;
    unlock = 0;
    pause_play = 0;
    timer_countdown = 48;//32
    enemy_countdown = 1000;
    ts = al_get_time();
    initEnemy();
    //if(maplevel >3) maplevel = 1;

    
    if(maplevel == 1) {
        map = create_map("map1.txt", 0);
        change_bgm("DOOM.ogg");
    }
    if(maplevel == 2){
        map = create_map("map5.txt", 0);
        change_bgm("Thor.ogg");
    }
    if(maplevel == 3){
        map = create_map("map3.txt", 0);//map3
        change_bgm("DOOM_Gore.ogg");
    }
    
    hearts_bitmap = al_load_bitmap("heart.png");
    bosshp_bitmap = al_load_bitmap("UI_Button.png");
    backButton = button_create(SCREEN_W / 2 - 200, 650, 400, 100, "UI_Button.png", "UI_Button_hovered.png");

    player = create_player("panda2.png", map.Spawn.x, map.Spawn.y);

    enemyList = createEnemyList();
    bulletList = createBulletList();
    
    //create_weapon(char * weapon_path, char * bullet_path, int cooldown, int speed, int damage)
    if(guntype == 0){
        weapon = create_weapon("guns.png", "yellow_bullet.png", 16, 8, 10);
    }
    else if(guntype == 1){
        weapon = create_weapon("sniper.png", "orange_bullet.png", 30, 30, 50);
        
    }
   
    for(int i=0; i<map.EnemySpawnSize; i++){
        Enemy enemy = createEnemy(map.EnemySpawn[i].x, map.EnemySpawn[i].y, map.EnemyCode[i]);
        insertEnemyList(enemyList, enemy);
    }

    game_log("coord x:%d \n coords y:%d \n", map.Spawn.x, map.Spawn.y);
    //change_bgm("game_bgm.ogg");
    
    coin_count = al_load_bitmap("coin_icon.png");
    gameSFX = al_load_sample("MenuSFX.ogg");
    shieldSFX = al_load_sample("shield_break.ogg");
}

static void update(void){
    update_button(&backButton);
    /*
        [TODO Homework]check
        
        Change the scene if winning/losing to win/lose scene
    */
    Point Camera;
    /*
        [TODO HACKATHON 1-3]D
        
        Calcualte the formula for the Camera
        Camera.x = ...
        Camera.y = ...

        Hint: Adjust it based on player position variable, then subtract it with half of the gameplay screen
    */
    //Camera = (Point){ 0, 0 };

    Camera.x = player.coord.x - SCREEN_W/2; //set camera to centre of player
    Camera.y = player.coord.y - SCREEN_H/2; //screenW/w is centre of map
    
    if(keyState[ALLEGRO_KEY_ESCAPE]){
        pause_play = 1;
    }
    if(pause_play){
        if(keyState[ALLEGRO_KEY_SPACE]){
            pause_play = 0;
        }
 
        if (mouseState.buttons && backButton.hovered == true) {
            al_play_sample(gameSFX, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            coins_obtained = 0;
            change_scene(create_menu_scene());
            return;
        }
        return;
    }
    elapsed_time = al_get_time() - ts;
    if(player.status == PLAYER_DYING){
        timer_countdown--;
        if(timer_countdown == 0){
            coins_obtained = 0;
            coin_counter = 0;
            change_scene(create_lose_scene());
            change_bgm("dead.ogg");
            return;
        }
    }
    
    
    //BOSS MECHANICS
    enemy_countdown--;
    
    if(maplevel == 3){
        if(/*boss_health <= 1000 && boss_health >= 200 && */enemy_countdown == 0){
            for(int i=0; i<3; i++){
                Enemy enemy = createEnemy(map.EnemySpawn[i].x, map.EnemySpawn[i].y, 'K');
                insertEnemyList(enemyList, enemy);
            }
           // enemy_countdown = 500;
        }
        if(/*boss_health <= 1500 && boss_health >= 1000 &&*/enemy_countdown == 0){

            for(int i=0; i<4; i++){
                Enemy enemy = createEnemy(map.EnemySpawn[i].x, map.EnemySpawn[i].y, 'S');
                insertEnemyList(enemyList, enemy);
            }
            enemy_countdown = 1500;
        }
        
    }
  
    


    
    
    
    if(coins_obtained == coin_counter){
       // printf("%lf", elapsed_time);
        total_time += elapsed_time-0.01;
//        printf("time taken = %lf", record[maplevel]);
        overall_coins += coins_obtained;
        coins_obtained = 0;
        coin_counter = 0;
        
        maplevel++;
        al_rest(1.0);
        change_scene(create_win_scene());
        change_bgm("win_sfx.ogg");
        return;
    }
    if(heal > 0 && player.health <= 100){
        player.health += heal;
        heal = 0;
    }
    
    
    if(player.status != PLAYER_DYING){
        updateEnemyList(enemyList, &map, &player);
        update_weapon(&weapon, bulletList, player.coord, Camera);
        updateBulletList(bulletList, enemyList, &map);
    }
    
    update_player(&player, &map);
    update_map(&map, player.coord, &coins_obtained);
    
}
int temp = 60;
static void draw(void){
    
    
    Point Camera;
    /*
        [TODO HACKATHON 1-4]D
        
        Calcualte the formula for the Camera
        Camera.x = ...
        Camera.y = ...

        Hint: Just copy from the [TODO HACKATHON 1-3]D
    */
    
    Camera.x = player.coord.x - SCREEN_W/2; //set camera to centre of player
    Camera.y = player.coord.y - SCREEN_H/2; //screenW/w is centre of map
    //Camera = (Point){ 0, 0 };
    
    // Draw
    
    draw_map(&map, Camera);
    draw_player(&player, Camera);
    
    
    
    if(player.status != PLAYER_DYING){
        draw_weapon(&weapon, player.coord, Camera);
        drawEnemyList(enemyList, Camera);
        drawBulletList(bulletList, Camera);
    }
    /*
        [TODO Homework]Check
        
        Draw the UI of Health and Total Coins
    */
    al_draw_scaled_bitmap(coin_count, 0, 0, 16, 16, 0, 100, 100 , 100, 0);//coin
        al_draw_textf(
            P1_FONT,
            al_map_rgb(225, 225, 225),
            110,
            130,
            ALLEGRO_ALIGN_LEFT,
                      "%d/%d", coins_obtained,coin_counter
        );
    al_draw_textf(
        P1_FONT,
        al_map_rgb(225, 225, 225),
        750,
        20,
        ALLEGRO_ALIGN_RIGHT,
        "%.2lf", al_current_time() - ts
    );
    al_draw_textf(
        P1_FONT,
        al_map_rgb(225, 225, 225),
        620,
        20,
        ALLEGRO_ALIGN_RIGHT,
        "Time:"
    );
    
    if(player.health <= 60){
        temp = 60;
        if(play_sound && player.health == 60 ){
            al_play_sample(shieldSFX, 4*SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            play_sound = 0;
        }
        for(int i =0; i < player.health/10; i++){
                al_draw_scaled_bitmap(hearts_bitmap, 0, 0, 32, 32, i*70, 0, 100 , 100, 1);
            //hearts
        }
    }else{
        for(int i =0; i < (player.health-60)/10; i++){
            al_draw_tinted_scaled_bitmap(hearts_bitmap, al_map_rgba_f(/*0.8, 2, 0.5, 0.5*/0.3, 1, 1, 1), 0, 0, 32, 32, i*70, 0, 100, 100, 1);
        }
        play_sound = 1;
        if(temp != 0){
            al_draw_text(
                P2_FONT,
                al_map_rgb(225, 225, 225),
                SCREEN_W / 2,
                300 + 31,
                ALLEGRO_ALIGN_CENTER,
                "Shield Activated"
            );
            temp--;
        }
        
    }
    
    
    
//    al_draw_scaled_bitmap(slider, 0, 0, 600, 20, SCREEN_W/4, SCREEN_H/3, 520, 20, 0);//size of background slider
//    al_draw_scaled_bitmap(slider_hovered, 0, 0, 600, 20, SCREEN_W/4, SCREEN_H/3, 520*BGM_VOLUME, 20, 0);//slider
    al_draw_line(30, 100, 400, 100, al_map_rgba_f(0.5, 0.5, 0.5, 0.5), 20);
    al_draw_line(30, 100, 30+stamina*4, 100, al_map_rgb(123, 252, 116), 20);
  
        
        
    if(maplevel == 3){
//        al_draw_tinted_scaled_bitmap(bosshp_bitmap, al_map_rgba_f(/*0.8, 2, 0.5, 0.5*/0.3, 1, 1, 1), 0, 0, 222, 108, (SCREEN_W / 2)-300 ,700, 580 , 80, 0);
        
        al_draw_line(150, 750, 150+500, 750, al_map_rgba_f(0.5, 0.5, 0.5, 0.5), 40);
        al_draw_line(150, 750, 150+boss_health/3, 750, al_map_rgb(235, 32, 21), 40);
        al_draw_text(
            P1_FONT,
            al_map_rgb(/*227, 36, 36*/28, 18, 18),
            SCREEN_W / 2,
            735,
            ALLEGRO_ALIGN_CENTER,
            "NECROMANCER"
                     );
        
        if(hint != 0){
            al_draw_text(
                P1_FONT,
                al_map_rgb(252, 23, 3),
                SCREEN_W / 2,
                200,
                ALLEGRO_ALIGN_CENTER,
                "KILL THE BOSS"
            );
            hint--;
        }
    }
    
    if(pause_play){
        draw_button(backButton);
        al_draw_text(
            P2_FONT,
            al_map_rgb(66, 76, 110),
            SCREEN_W / 2,
            650 + 28 + backButton.hovered * 11,
            ALLEGRO_ALIGN_CENTER,
            "QUIT"
        );
        al_draw_text(
            P2_FONT,
            al_map_rgb(225, 225, 225),
            SCREEN_W / 2,
            650 + 31 + backButton.hovered * 11,
            ALLEGRO_ALIGN_CENTER,
            "QUIT"
        );
        al_draw_text(
            P2_FONT,
            al_map_rgb(225, 225, 225),
            SCREEN_W / 2,
            300 + 31,
            ALLEGRO_ALIGN_CENTER,
            "Press [SPACE] to Continue"
        );
    }

}

static void destroy(void){
    delete_player(&player);
    delete_weapon(&weapon);
    destroy_map(&map);
    destroyBulletList(bulletList);
    destroyEnemyList(enemyList);
    terminateEnemy();
}


Scene create_game_scene(void){
    Scene scene;
    memset(&scene, 0, sizeof(Scene));
    
    scene.name = "game";
    scene.init = &init;
    scene.draw = &draw;
    scene.update = &update;
    scene.destroy = &destroy;
    
    return scene;
}



//LOSE SCENE

static void init_lose(void){
//    losing_panda = al_load_bitmap("Assets/panda_lose.png");
    QuitButton = button_create(SCREEN_W / 2 - 200, 600, 400, 100, "UI_Button.png", "UI_Button_hovered.png");
    RestartButton = button_create(SCREEN_W / 2 - 200, 450, 400, 100, "UI_Button.png", "UI_Button_hovered.png");
    lose_panda = al_load_bitmap("panda_lose.png");
    
}

static void update_lose(void){
    update_button(&QuitButton);
    update_button(&RestartButton);
    al_draw_text(
        TITLE_FONT,
        al_map_rgb(227, 36, 36),
        SCREEN_W / 2,
        85,
        ALLEGRO_ALIGN_CENTER,
        "GAME OVER"
                 );
    al_draw_textf(
        P1_FONT,
        al_map_rgb(146, 161, 185),
        SCREEN_W / 2,
        150,
        ALLEGRO_ALIGN_CENTER,
        "Time Taken: %.2lf", elapsed_time-0.01
                 );
    if (mouseState.buttons && RestartButton.hovered) {
        change_scene(create_loading_scene());
    }
    if (mouseState.buttons && QuitButton.hovered) {
        change_scene(create_menu_scene());
    }
}

static void draw_lose(void){
    Point Camera;
    
    Camera.x = player.coord.x - SCREEN_W/2; //set camera to centre of player
    Camera.y = player.coord.y - SCREEN_H/2; //screenW/w is centre of map
    //Camera = (Point){ 0, 0 };
    
    // Draw
    
        draw_button(RestartButton);
        // button text
        al_draw_text(
            P2_FONT,
            al_map_rgb(66, 76, 110),
            SCREEN_W / 2,
            450 + 28 + RestartButton.hovered * 11,
            ALLEGRO_ALIGN_CENTER,
            "Restart"
        );
        
        draw_button(QuitButton);
        // button text
        al_draw_text(
            P2_FONT,
            al_map_rgb(66, 76, 110),
            SCREEN_W / 2,
            600 + 28 + QuitButton.hovered * 11,
            ALLEGRO_ALIGN_CENTER,
            "Quit"
                     );
        al_draw_scaled_bitmap(lose_panda, 0, 0, 64, 64, SCREEN_W/2-150, 175, 300 , 300, 0);
    
}

static void destroy_lose(void){
    al_destroy_bitmap(lose_panda);
    destroy_button(&RestartButton);
    destroy_button(&QuitButton);

}

Scene create_lose_scene(void){
    Scene scene;
    memset(&scene, 0, sizeof(Scene));
    
    scene.name = "lose";
    scene.init = &init_lose;
    scene.draw = &draw_lose;
    scene.update = &update_lose;
    scene.destroy = &destroy_lose;
    
    return scene;
}

//WIN SCENE

static void init_win(void){

    QuitButton = button_create(SCREEN_W / 2 - 200, 600, 400, 100, "UI_Button.png", "UI_Button_hovered.png");
    ContinueButton = button_create(SCREEN_W / 2 - 200, 450, 400, 100, "UI_Button.png", "UI_Button_hovered.png");
    win_panda = al_load_bitmap("panda_win.png");
    
}

static void update_win(void){
    update_button(&QuitButton);
    update_button(&ContinueButton);
    
    if(maplevel == 4){
        al_draw_text(
            TITLE_FONT,
            al_map_rgb(30, 230, 43),
            SCREEN_W / 2,
            85,
            ALLEGRO_ALIGN_CENTER,
            "GAME COMPLETED"
                     );
        al_draw_text(
            TITLE_FONT,
            al_map_rgb(30, 230, 43),
            SCREEN_W / 2,
            500,
            ALLEGRO_ALIGN_CENTER,
            "THANKS FOR PLAYING"
                     );
    }else{
        al_draw_text(
            TITLE_FONT,
            al_map_rgb(30, 230, 43),
            SCREEN_W / 2,
            85,
            ALLEGRO_ALIGN_CENTER,
            "LEVEL COMPLETED"
                     );
    }
   
    al_draw_textf(
        P1_FONT,
        al_map_rgb(146, 161, 185),
        SCREEN_W / 2,
        150,
        ALLEGRO_ALIGN_CENTER,
        "Time Taken: %.2lf", elapsed_time-0.01
                 );
    
    if (mouseState.buttons && ContinueButton.hovered && maplevel <= 3) {
        change_scene(create_loading_scene());
    }
    else if(maplevel == 4){
        finished  = 1;
    }
    if (mouseState.buttons && QuitButton.hovered) {
        if(maplevel == 4){
            change_scene(create_name_scene());
            maplevel = 1;
        }else{
            change_scene(create_menu_scene());
        }
    }

}

static void draw_win(void){
    Point Camera;
    
    Camera.x = player.coord.x - SCREEN_W/2; //set camera to centre of player
    Camera.y = player.coord.y - SCREEN_H/2; //screenW/w is centre of map
    //Camera = (Point){ 0, 0 };
    
    // Draw
    if(maplevel <= 3){
        draw_button(ContinueButton);
        // button text
        al_draw_text(
            P2_FONT,
            al_map_rgb(66, 76, 110),
            SCREEN_W / 2,
            450 + 28 + ContinueButton.hovered * 11,
            ALLEGRO_ALIGN_CENTER,
            "Continue"
        );
    }
        
        
        draw_button(QuitButton);
        // button text
        al_draw_text(
            P2_FONT,
            al_map_rgb(66, 76, 110),
            SCREEN_W / 2,
            600 + 28 + QuitButton.hovered * 11,
            ALLEGRO_ALIGN_CENTER,
            "Quit"
                     );
        al_draw_scaled_bitmap(win_panda, 0, 0, 64, 64, SCREEN_W/2-150, 150, 300 , 300, 0);
    
}

static void destroy_win(void){
    al_destroy_bitmap(win_panda);
    destroy_button(&ContinueButton);
    destroy_button(&QuitButton);

}

Scene create_win_scene(void){
    Scene scene;
    memset(&scene, 0, sizeof(Scene));
    
    scene.name = "win";
    scene.init = &init_win;
    scene.draw = &draw_win;
    scene.update = &update_win;
    scene.destroy = &destroy_win;
    
    return scene;
}
