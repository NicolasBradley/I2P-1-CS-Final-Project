#ifndef score_scene_h
#define score_scene_h

#include "utility.h"

extern int maplevel;
extern int finished;

Scene create_score_scene(void);

typedef struct {
    char name[51];
    float score;
} LeaderboardEntry;
#endif
