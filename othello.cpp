#include<iostream>
#include<cstdio>
#include<cmath>
#include<vector>
#include<list>
#include<set>
#include<map>
#include<stack>
#include<queue>
#include<string>
#include<cstring>
#include<utility>
#include<algorithm>
#include<functional>
#include<deque>
#include<cctype>
#include<time.h>
#include<cstdlib>
#include<iomanip>
/*#include<windows.h>*/
using namespace std;

#define is_inboard(x, y) (x>=1 && x<=8 && y>=1 && y<=8)

int v[10][10];  //v[1][1]~v[8][8]‚ðŽg‚¤
int player = 1;
int point_human = 2, point_com = 2;

void rule();
void rand_set();
int GetRandom(int, int);
char disp_convert(int, int);
void disp_board();
int choose_square();
void after_choose_square(int);
bool is_putable(int, int, int);
bool or_path(int);
void turn_stone(int, int);
void point_sum();
void human();
void computer();


int main(){
   for (int x = 1; x <= 8; x++){
      for (int y = 1; y <= 8; y++){
         v[x][y] = 0;
      }
   }
   v[4][4] = v[5][5] = 1;
   v[4][5] = v[5][4] = -1;

   rule();
   rand_set();
   disp_board();
   while (!or_path(1) || !or_path(2)){
      human();
      if (!or_path(1) || !or_path(2)) computer();
      else break;
   }
   point_sum();
   printf("Game Finish!\n");
   printf("Result  -->   You: %2d   Com: %2d\n", point_human, point_com);
   if (point_human > point_com) printf("Winner: You!! Congratulations!!\n\n");
   else if (point_human < point_com) printf("Winner: Computer!!\nLet's try again!!\n\n");
   else printf("Draw...\nPlease play this game once again!\n\n");
}


void rule(){
   printf("\n  Welcome to Othello !!\n\n");
   printf(" ~Rule~\n");
   printf("  * Your stone is 'O', and computer's stone is 'X'.\n");
   printf("  * Please select a square in this way\n");
   printf("     --> for example, \"3e\"\n\n");
}


void rand_set(){
   clock_t start = clock();
   printf("  Please push Enter key...");
   cin.ignore();
   clock_t end = clock();
   for (int i = 0; i < (end-start)/10; i++){
      GetRandom(1, 8);
   }
}


int GetRandom(int min, int max){
	return min + (int)(rand()*(max-min+1.0)/(1.0+RAND_MAX));
}


char disp_convert(int x, int y){
   if (v[x][y] == 1) return 'O';
   else if (v[x][y] == -1) return 'X';
   else return ' ';
}


void disp_board(){
   puts("");
   printf("       a  b  c  d  e  f  g  h\n");
   for (int x = 1; x <= 8; x++){
      printf("    %d ", x);
      for (int y = 1; y <= 8; y++){
         switch(v[x][y]){
            case 1: printf(" O ");  break;
            case -1: printf(" X ");  break;
            default: printf(" - ");  break;
         }
      }
      printf(" %d\n", x);
   }
   printf("       a  b  c  d  e  f  g  h\n");
   puts("");
   point_sum();
   printf("    You: %2d\n    Com: %2d\n\n", point_human, point_com);
}


int choose_square(){
   string ss;  bool flag;
   do {
      switch(player){
         case 1: printf(">>Player 1: You\n");  break;
         case 2: printf(">>Player 2: Computer\n");  break;
      }
      printf(" Please choose a square: ");
      getline(cin, ss);
      flag = true;
      if (ss.size() != 2) flag = false;
      if (!(ss[0]>='1' && ss[0]<='8' && ss[1]>='a' && ss[1]<='h')) flag = false;
      if (!is_putable(ss[0]-'0', ss[1]-'a'+1, player)) flag = false;
      if (flag == false) printf(" Error!\n");
   } while (flag == false);
   return 10*(ss[0]-'0') + (ss[1]-'a'+1);
}


void after_choose_square(int square){
   v[square/10][square%10] = (player==1) ? 1 : -1;
   turn_stone(square/10, square%10);
   disp_board();
}


bool is_putable(int x, int y, int player){
   if (v[x][y]!=0) return false;
   for (int dx = -1; dx <= 1; dx++){
      for (int dy = -1; dy <= 1; dy++){
         int nx = x + dx, ny = y + dy;
         if (v[nx][ny]==((player==1)?-1:1) && !(dx==0 && dy==0) && is_inboard(nx, ny)){
            while(v[nx][ny]!=0 && is_inboard(nx, ny)){
               nx += dx;  ny += dy;
               if (v[nx][ny]==((player==1)?1:-1) && is_inboard(nx, ny)) return true;
            }
         }
      }
   }
   return false;
}


//path‚·‚é‚µ‚©‚È‚¢ê‡‚Étrue‚ð•Ô‚·
bool or_path(int player){
   for (int x = 1; x <= 8; x++){
      for (int y = 1; y <= 8; y++){
         if (is_putable(x, y, player)) return false;
      }
   }
   return true;
}


void turn_stone(int x, int y){
   for (int dx = -1; dx <= 1; dx++){
      for (int dy = -1; dy <= 1; dy++){
         int nx = x + dx, ny = y + dy;
         if (v[nx][ny]==((player==1)?-1:1) && !(dx==0 && dy==0) && is_inboard(nx, ny)){
            while(v[nx][ny]!=0 && is_inboard(nx, ny)){
               nx += dx;  ny += dy;
               if (v[nx][ny]==((player==1)?1:-1) && is_inboard(nx, ny)){
                  nx = x + dx;  ny = y + dy;
                  while (v[nx][ny]==((player==1)?-1:1) && is_inboard(nx, ny)){
                     v[nx][ny] = (player==1)?1:-1;
                     nx += dx;  ny += dy;
                  }
                  break;
               }
            }
         }
      }
   }
}


void point_sum(){
   point_human = point_com = 0;
   for (int x = 1; x <= 8; x++){
      for (int y = 1; y <= 8; y++){
         if (v[x][y] == 1) point_human++;
         else if (v[x][y] == -1) point_com++;
      }
   }
}


void human(){
   player = 1;
   if (or_path(1)){
      printf(">>Player 1: You\n path\n");
      return;
   }
   int square = choose_square();
   after_choose_square(square);
}


void computer(){
   player = 2;
   if (or_path(2)){
      printf(">>Player 2: Computer\n path\n");
      return;
   }
   bool flag;  int square;
   do {
      square = GetRandom(11, 88);
      flag = true;
      if (!(square/10>=1 && square/10<=8 && square%10>=1 && square%10<=8)) flag = false;
      if (!is_putable(square/10, square%10, player)) flag = false;
   } while (!flag);
   printf(">>Player 2: Computer\n");
   /*Sleep(3000);*/
   printf(" Computer selected: %d%c\n", square/10, square%10+'a'-1);
   after_choose_square(square);
}
