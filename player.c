#include "player.h"
#include "game.h"
#include "utility.h"
#include "map.h"
#include "enemy.h"

#include <math.h>

static bool isCollision(Player* player, Map* map);

int stamina = 100;
int tintgreen = 0;
Player create_player(char * path, int row, int col){
    Player player;
    memset(&player, 0, sizeof(player));
    
    player.coord = (Point){
        col * TILE_SIZE,
        row * TILE_SIZE
    };
    
    player.speed = 4;
    player.health = 60;
    
    player.image = al_load_bitmap(path);
    if(!player.image){
        game_abort("Error Load Bitmap with path : %s", path);
    }
    
    player.slap_audio = al_load_sample("slap_audio.ogg");
    player.Knight_audio = al_load_sample("Knight_attack.ogg");
    stamina = 100;
    //player.animation_tick = 0;
    return player;
}

void update_player(Player * player, Map* map){

    Point original = player->coord;
    if(player->status == PLAYER_DYING){
        player->animation_tick = (player->animation_tick + 1) % 128;
        //if(player->animation_tick == 127)return;
    }
    else if(/*player->health > 0 ||*/ player->status != PLAYER_DYING){
        player->status = PLAYER_IDLE;
        player->animation_tick = (player->animation_tick + 1) % 64;
    }
    // Knockback effect
    
    
        
    
    
    if(player->knockback_CD > 0){

        player->knockback_CD--;
        int next_x = player->coord.x + player->speed * cos(player->knockback_angle);
        int next_y = player->coord.y + player->speed * sin(player->knockback_angle);
        Point next;
        next = (Point){next_x, player->coord.y};
        
        if(!isCollision(player, map)){
            player->coord = next;
        }
        
        next = (Point){player->coord.x, next_y};
        if(!isCollision(player, map)){
            player->coord = next;
        }
    }
    
    /*
        [TODO HACKATHON 1-1]D
        
        Player Movement
        Adjust the movement by player->speed

        if (keyState[ALLEGRO_KEY_W]) {
            player->coord.y = ...
            player->direction = ...
        }
        if (keyState[ALLEGRO_KEY_S]) {
            player->coord.y = ...
            player->direction = ...
        }
    */
    if (keyState[ALLEGRO_KEY_LSHIFT] && stamina > 0) {
        player->speed = 12;
        stamina--;
    }else{
        player->speed = 4;
    }
    if(player->status != PLAYER_DYING){
        if (keyState[ALLEGRO_KEY_S] || keyState[ALLEGRO_KEY_DOWN]) {
            player->coord.y += player->speed;
            player->direction += player->coord.y;
            player->status = PLAYER_WALKING;
        }
        if (keyState[ALLEGRO_KEY_W] || keyState[ALLEGRO_KEY_UP]) {
            player->coord.y -= player->speed;
            player->direction -= player->coord.y;
            player->status = PLAYER_WALKING;
        }
    }
    
    
    
    // if Collide, snap to the grid to make it pixel perfect
    if(isCollision(player, map)){
        player->coord.y = round((float)original.y / (float)TILE_SIZE) * TILE_SIZE;
    }
    
    /*
        [TODO HACKATHON 1-2]D
        
        Player Movement
        Add/Subtract the movement by player->speed

        if (keyState[ALLEGRO_KEY_A]) {
            player->coord.y = ...
            player->direction = ...
        }
        if (keyState[ALLEGRO_KEY_D]) {
            player->coord.y = ...
            player->direction = ...

    }
    */
    
    if(player->status != PLAYER_DYING){
        if (keyState[ALLEGRO_KEY_D] || keyState[ALLEGRO_KEY_RIGHT]) {
            player->coord.x += player->speed;
            player->direction += RIGHT;
            player->status = PLAYER_WALKING;
            player->direction = RIGHT;
        }
        if (keyState[ALLEGRO_KEY_A]|| keyState[ALLEGRO_KEY_LEFT]) {
            player->coord.x -= player->speed;
            player->direction -= LEFT;
            player->status = PLAYER_WALKING;
            player->direction = LEFT;
        }
    }
    if(isCollision(player, map)){
        player->coord.x = round((float)original.x / (float)TILE_SIZE) * TILE_SIZE;
    }
    
    /*
        [TODO Homework] Check
        
        Calculate the animation tick to draw animation later
    */
    
    
    
        
    if(player->animation_hit_tick > 0){
        player->animation_tick = (player->animation_tick + 1) % 64;
        player->animation_hit_tick--;
    }
    if(heal > 0){
        tintgreen = 2000;
    }
}

void draw_player(Player * player, Point cam){
    int dy = player->coord.y - cam.y; // destiny y axis
    int dx = player->coord.x - cam.x; // destiny x axis
    
    /*
        [TODO Homework] Check
        
        Draw Animation of Dying, Walking, and Idle
    */
    
    if(player->status == PLAYER_IDLE){
        int xcord = 32 * (int)(player->animation_tick / 32);
        if(player->animation_hit_tick > 0){
                // Hint: check the asset Panda's sprite size
                // Multiply by the anim frame you want to change n, by (n - 1)
            // Here I want to loop 2 frames hence n = 2 and I put 1
            xcord += 32 * 1;
        }
        int tint = player->knockback_CD > 0 ? 0 : 255;
//        if(heal > 0){
//            tint = 2000;
//        }
        int change_direction = player->direction == RIGHT ? 1 : 0;
//94, 255, 0
        al_draw_tinted_scaled_rotated_bitmap_region(player->image, xcord, 0, 32, 32, al_map_rgb(255, tint, tint),
            0, 0, dx, dy, TILE_SIZE / 32, TILE_SIZE / 32,
            0, change_direction);
    }
    else if (player->status == PLAYER_WALKING)
    {
        int offset = 32 * (int)(player->animation_tick / 32);
        if(player->animation_hit_tick > 0){

            offset += 32 * 2;
        }
        int change_direction = player->direction == LEFT ? 0 : 1;
        int tint = player->knockback_CD > 0 ? 0 : 255;
        //when heal tint green
        
        if(tintgreen > 0){
            al_draw_tinted_scaled_rotated_bitmap_region(player->image, offset, 32, 32, 32, al_map_rgb(117, 242, 99),
                0, 0, dx, dy, TILE_SIZE / 32, TILE_SIZE / 32,
                0, change_direction);
        }
        al_draw_tinted_scaled_rotated_bitmap_region(player->image, offset, 32, 32, 32, al_map_rgb(255, tint, tint),
            0, 0, dx, dy, TILE_SIZE / 32, TILE_SIZE / 32,
            0, change_direction);
    }
    else if (player->status == PLAYER_DYING)
    {
        int offset = 32 * (int)(player->animation_tick / 18);//16
//        printf("ofs = %d\n", offset);
//        int cry = 0;
//        if(cry == 0){
//            if(player->animation_tick == 63) cry++;
//        }else{
//            offset = 64;
//        }
//            if(player->animation_hit_tick > 0){
//                    // I have 4 frames, hence n - 1 = 3
//                offset += 32 * 2;
//            }
//        printf("%d\n", player->animation_tick);
//        printf("Mark = %d\n", cry);
//

        al_draw_tinted_scaled_rotated_bitmap_region(player->image, offset, 64, 32, 32, al_map_rgb(255, 255, 255),
            0, 0, dx, dy, TILE_SIZE / 32, TILE_SIZE / 32,
            0, 0);
    }
    //----------

//    al_draw_tinted_scaled_bitmap(player->image, al_map_rgb(255, 255, 255),
//        0, 0, 32, 32, // source image x, y, width, height
//        dx, dy, TILE_SIZE, TILE_SIZE, // destiny x, y, width, height
//        flag // Flip or not
//    );

    
#ifdef DRAW_HITBOX
    al_draw_rectangle(
        dx, dy, dx + TILE_SIZE, dy + TILE_SIZE,
        al_map_rgb(255, 0, 0), 1
    );
#endif
    
}

void delete_player(Player * player){
    al_destroy_bitmap(player->image);
}

static bool isCollision(Player* player, Map* map){

    if (player->coord.x < 0 ||
        player->coord.y < 0 ||
        (player->coord.x + TILE_SIZE - 1) / TILE_SIZE >= map->col ||
        (player->coord.y + TILE_SIZE - 1) / TILE_SIZE >= map->row)
        return true;

    /*

        [TODO HACKATHON 2-1]D
    
        Check every corner of enemy if it's collide or not

        We check every corner if it's collide with the wall/hole or not
        You can activate DRAW_HITBOX in utility.h to see how the collision work

        if(!isWalkable(map->map[...][...])) return true;
        if(!isWalkable(map->map[...][...])) return true;
        if(!isWalkable(map->map[...][...])) return true;
        if(!isWalkable(map->map[...][...])) return true;w

    */
    if(!isWalkable(map->map[(player->coord.y)/TILE_SIZE][(player->coord.x) / TILE_SIZE])) return true; //top
    if(!isWalkable(map->map[(player->coord.y+TILE_SIZE-1)/TILE_SIZE][(player->coord.x)/TILE_SIZE])) return true; //left
    if(!isWalkable(map->map[(player->coord.y)/TILE_SIZE][(player->coord.x+TILE_SIZE-1)/TILE_SIZE])) return true; //right
    if(!isWalkable(map->map[(player->coord.y+TILE_SIZE-1)/TILE_SIZE][(player->coord.x+TILE_SIZE-1)/TILE_SIZE])) return true;//bottom
    return false;
}

void hitPlayer(Player * player, Point enemy_coord, int damage, int enemydx, int enemydy, int die){
    if(player->knockback_CD == 0){
        float dY = player->coord.y - enemy_coord.y;
        float dX = player->coord.x - enemy_coord.x;
        float angle = atan2(dY, dX);

        /*
            [TODO Homework]D
            
            Game Logic when the player get hit or die

            player->knockback_angle = angle;
            player->knockback_CD = 32;

            player->health = ...
            if(player->health <= 0){

                player->health = 0;
                player->status = ...

                ...
            }
        */
        player->knockback_angle = angle;
      
        float dx = player->coord.x - enemydx;  // Difference in x-coordinates
        float dy = player->coord.y - enemydy;  // Difference in y-coordinates
        float distance = sqrt(dx * dx + dy * dy); // Euclidean distance

        // Check if the player is within the radius
        if (distance <= 180 && die == 1) {
            player->knockback_CD = 60;
            player->health -= damage;
        }
//        if((player->coord.x >= enemydx && player->coord.y >= enemydy) || (player->coord.x <= enemydx+180 && player->coord.y<=enemydy+180)){
//            player->knockback_CD = 32;
//            player->health -= damage;
//        }
//        if(enemyType == slime){
          //  player->knockback_CD = 32;
            
        if(!die){
            player->knockback_CD = 32;
            player->health -= damage;
        }
        
        
        
        
        if(player->health <= 0){
            player->health = 0;
            player->status = PLAYER_DYING;
        }
    }
}
