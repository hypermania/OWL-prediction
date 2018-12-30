#ifndef CODE_H
#define CODE_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <dlfcn.h>
#include <stdbool.h>

#include "profile.h"

typedef struct {
  int home_team;
  int away_team;
  int home_score;
  int away_score;
} MatchResult;

extern MatchResult *match_results;
extern MatchResult *match_schedule;

void write_code(const char *filename, int start, int end);

double (*get_dist(int start, int end))(double, double, double, double, double, double, double, double, double, double, double, double);

int load_matches(const char *filename);

int load_schedule(const char *filename);

int get_team_num(const char *team_name);

#endif
