#include "standing.h"


void init_standing(Standing *standing){
  memset(standing, 0, sizeof(Standing));
}

void update_standing(Standing *standing, MatchResult *match_results, int start, int end){
  int i;
  for(i = start; i <= end; i++){
    int home_team = match_results[i].home_team;
    int away_team = match_results[i].away_team;
    int diff = match_results[i].home_score - match_results[i].away_score;
    standing->team[home_team].diff += diff;
    standing->team[away_team].diff -= diff;
    if(diff > 0){
      standing->team[home_team].w++;
      standing->team[away_team].l++;
    } else {
      standing->team[home_team].l++;
      standing->team[away_team].w++;
    }
  }
}

void print_standing(Standing *standing){

  int compare(const void *p1, const void *p2){
    int r1 = *((int *)p1);
    int r2 = *((int *)p2);
    if(standing->team[r1].w > standing->team[r2].w) return -1;
    if(standing->team[r1].w == standing->team[r2].w){
      if(standing->team[r1].diff > standing->team[r2].diff) return -1;
      if(standing->team[r1].diff == standing->team[r2].diff) return 0;
      if(standing->team[r1].diff < standing->team[r2].diff) return 1;
    }
    if(standing->team[r1].w < standing->team[r2].w) return 1;
    return 0;
  }
  
  int ranking[TEAM_NUM];
  int i;
  for(i = 0; i < TEAM_NUM; i++){
    ranking[i] = i;
  }
  
  qsort(&ranking, TEAM_NUM, sizeof(int), compare);

  
  for(i = 0; i < TEAM_NUM; i++){
    int team_num = ranking[i];
    printf("%s  %2d  %2d  %2d\n", team_names[team_num], standing->team[team_num].w, standing->team[team_num].l, standing->team[team_num].diff);
  }
}
