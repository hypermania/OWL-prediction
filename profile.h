#ifndef PROFILE_H
#define PROFILE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "thread.h"

#define TEAM_NUM 12

extern const char team_names[TEAM_NUM][4];

typedef struct {
  int home_team[3];
  int away_team[3];
  double skill[TEAM_NUM];
  double skill_std[TEAM_NUM];
  double prob[3];
  double prob_std[3];
  MatchProb match_prob[3];
  double diff_entropy;
} Profile;


Profile *profile_create(void);
Profile *profile_from_thread_result(ThreadResult **thread_results, int n);
void profile_print(Profile *profile);
void profile_save(Profile *profile, const char *filename);
Profile *profile_load(const char *filename);

#endif
