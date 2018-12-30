#include "playoff.h"


dsfmt_t dsfmt_playoff;

void simulate_playoff(Standing *standing, Profile *profile, int start, int end){
  int i;
  for(i = start; i <= end; i++){
    int home_team = match_schedule[i].home_team;
    int away_team = match_schedule[i].away_team;
    double map_winrate = 1 / (1 + profile->skill[away_team] / profile->skill[home_team]);

    int w = 0;
    int j;
    for(j = 0; j < 4; j++){
      double r = dsfmt_genrand_open_close(&dsfmt_playoff);
      if(r < map_winrate){
	w++;
      }
    }

    if(w == 2){
      double r = dsfmt_genrand_open_close(&dsfmt_playoff);
      if(r < map_winrate){
	w++;
      }
    }
    
    standing->team[home_team].diff += w - (4 - w);
    standing->team[away_team].diff -= w - (4 - w);
    
    if(w >= 3){
      standing->team[home_team].w++;
      standing->team[away_team].l++;
    } else {
      standing->team[home_team].l++;
      standing->team[away_team].w++;
    }
  }
}
