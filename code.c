#include "code.h"

char *program_head =
  "#include \"distribution.h\"\ndouble p(double SEO, double LDN, double NYE, double LAV, double LAG, double HOU, double BOS, double PHI, double SFS, double DAL, double FLA, double SHD){return ";
//(1/(SEO*LDN*NYE*LAV*LAG*HOU*BOS*PHI*SFS*DAL*FLA*SHD))*;

char *program_tail =
  "}";


int match_num;
MatchResult *match_results;
MatchResult *match_schedule;

void write_code(const char *filename, int start, int end){
  FILE *file = fopen(filename, "w");
  fputs(program_head, file);
  int i;
  for(i = start; i <= end; i++){
    fprintf(file, "LIKELIHOOD_%d_%d(%s, %s)",
	    match_results[i].home_score,
	    match_results[i].away_score,
	    team_names[match_results[i].home_team],
	    team_names[match_results[i].away_team]
	    );
    if(i < end){
      fputs("*", file);
    }else{
      fputs(";", file);
    }
  }
  fputs(program_tail, file);
  fclose(file);
}

double (*get_dist(int start, int end))(double, double, double, double, double, double, double, double, double, double, double, double){
  write_code("temp.c", start, end);
  system("gcc temp.c -msse4.2 -msse2 -mpopcnt -mbmi -mbmi2 -mavx -mavx2 -lm -O3 -ffast-math -shared -fPIC -o dynamic.so");
  void *handle = dlopen("./dynamic.so", RTLD_NOW);
  system("rm temp.c");
  double (*dist)(double, double, double, double, double, double, double, double, double, double, double, double) = dlsym(handle, "p");

  return dist;
}

int get_team_num(const char *team_name){
  int i;
  for(i = 0; i < TEAM_NUM; i++){
    int cmp = strcmp(team_name, team_names[i]);
    if(cmp == 0){
      return i;
    }
  }
  return -1;
}

int load_matches(const char *filename){
  match_results = malloc(1024 * sizeof(MatchResult));
  match_num = 0;
  FILE *file = fopen(filename, "r");
  while(true){
    char home_str[4];
    char away_str[4];
    int read = fscanf(file, "%s %s %d %d",
		      home_str,
		      away_str,
		      &match_results[match_num].home_score,
		      &match_results[match_num].away_score
		      );
    match_results[match_num].home_team = get_team_num(home_str);
    match_results[match_num].away_team = get_team_num(away_str);
    if(read > 0){
      match_num++;
    }else{
      break;
    }
  }
  fclose(file);
  return match_num;
}

int load_schedule(const char *filename){
  match_schedule = malloc(1024 * sizeof(MatchResult));
  match_num = 0;
  FILE *file = fopen(filename, "r");
  while(true){
    char home_str[4];
    char away_str[4];
    int read = fscanf(file, "%s %s", home_str, away_str);
    match_schedule[match_num].home_team = get_team_num(home_str);
    match_schedule[match_num].away_team = get_team_num(away_str);
    if(read > 0){
      match_num++;
    }else{
      break;
    }
  }
  fclose(file);
  return match_num;
}
