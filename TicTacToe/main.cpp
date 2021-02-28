//
//  main.cpp
//  TicTacToe
//
//  Created by Ad1n0d1n on 26.02.2021.
//

#define CHK_DOT(x, sz) ((x) >= 0 && (x) < (sz))
#define POINT_ITEM(a, r, c) (*((*(a + r)) + c))

#include <iostream>
#include <string>

using namespace std;

enum PLAYER {HUMAN='X', AI='O', EMPTY='_'};

typedef struct {
   int szY;
   int szX;
   PLAYER** map;
   int towin;
} Field;

void tictactoe();
int gamechk(Field &field, PLAYER dot, const string &winString);
int humwinchk(Field &field);
int aiwinchk(Field &field);
void init(Field &field, int size);
void print(Field &field);
void human(Field &field);
void ai(Field &field);
int winchk(Field &field, PLAYER c);
int linechk(Field &field, int y, int x, int vx, int vy, int len, PLAYER c);
char getval(PLAYER** array, const int row, const int col);
void setval(PLAYER** array, const int row, const int col, PLAYER value);
int isvalid(Field &field, int x, int y);
int isvalid(string x, string y);
int isempty(Field &field, int x, int y);
int isdraw(Field &field);

int main(int argc, const char * argv[]) {
    srand(time(0));
    
    tictactoe();
    return 0;
}

void tictactoe() {
    cout << "Enter field size: ";
    string field_size;
    
    while(true) {
        cin >> field_size;
        try {
            if(stoi(field_size) >= 2) break;
        } catch (exception){}
    
        cout << "Enter correct field size: ";
    }
    
    Field field;
    // C-style
    // while (1) {
    while (true) {
        init(field, stoi(field_size));
        print(field);
        while (true) {
            human(field);
            print(field);
            if (gamechk(field, HUMAN, "Human win!")) break;
            ai(field);
            print(field);
            if (gamechk(field, AI, "AI win!")) break;
        }
        // C-style
        // char answer[1];
        // scanf("%s", answer);
        // if (strcasecmp(answer, "y") != 0)
        //    break;

        string answer;
        cout << "Play again? ";
        // one word to separator
        cin >> answer;


        // STL <algorithm>
        transform(answer.begin(), answer.end(), answer.begin(), ::tolower);

        if (answer.find('y') != 0)
           break;
    }
}

int gamechk(Field &field, PLAYER dot, const string &winString) {
   if (winchk(field, dot)) {
       cout << winString << endl;
       return 1;
   }
   if (isdraw(field)) {
       cout << "draw" << endl;
       return 1;
   }
   return 0;
}

void init(Field &field, int size) {
   field.towin = size;
   field.szY = size;
   field.szX = size;
   field.map = (PLAYER **) calloc(sizeof(PLAYER *), field.szY);
   for (int y = 0; y < field.szY; ++y) {
       *(field.map + y) = (PLAYER *) calloc(sizeof(PLAYER), field.szX);
       for (int x = 0; x < field.szX; ++x) {
           setval(field.map, y, x, EMPTY);
       }
   }
}

void print(Field &field) {
   //#include <windows.h>
   //system("cls")
   std::system("clear");
   printf("-------\n");
   for (int i = 0; i < field.szY; ++i) {
       cout << "|";
       for (int j = 0; j < field.szX; ++j) {
           printf("%c|", getval(field.map, i, j));
       }
       cout << endl;
   }
}

void human(Field &field) {
    string strx;
    string stry;
    int x, y;
    
    do {
        while(true) {
            printf("Введите координаты хода X и Y (от 1 до %d) через пробел >>", field.szY);
            // C-style
            // scanf("%d %d", &x, &y);
            cin >> strx >> stry;
            
            if(isvalid(strx, stry)){
                x = stoi(strx) - 1; //cause array index
                y = stoi(stry) - 1;
                break;
            }
        }

    } while (!isvalid(field, x, y) || !isempty(field, x, y));
    setval(field.map, y, x, HUMAN);
}

void ai(Field &field) {
    int x, y;
    //C++11 <random>
    //   random_device rd;
    //   mt19937 mt(rd());
    //   uniform_real_distribution<double> dist(0, 3);
    if(aiwinchk(field) || humwinchk(field)) return;
    
    do {
        // C-style
        x = rand() % field.szX;
        y = rand() % field.szY;
    //       x = dist(mt);
    //       y = dist(mt);
    } while (!isempty(field, x, y));
    setval(field.map, y, x, AI);
}

int winchk(Field &field, PLAYER c) {
   for (int y = 0; y < field.szX; y++) {
       for (int x = 0; x < field.szY; x++) {
           if (linechk(field, y, x, 1, 0, field.towin, c)) return 1;
           if (linechk(field, y, x, 1, 1, field.towin, c)) return 1;
           if (linechk(field, y, x, 0, 1, field.towin, c)) return 1;
           if (linechk(field, y, x, 1, -1, field.towin, c)) return 1;
       }
   }
   return 0;
}

int aiwinchk(Field &field) {
   for (int y = 0; y < field.szY; y++) {
       for (int x = 0; x < field.szX; x++) {
           if (isempty(field, x, y)) {
               setval(field.map, y, x, AI);
               if (winchk(field, AI))
                   return 1;
               setval(field.map, y, x, EMPTY);
           }
       }
   }
   return 0;
}

int humwinchk(Field &field) {
   for (int y = 0; y < field.szY; y++) {
       for (int x = 0; x < field.szX; x++) {
           if (isempty(field, x, y)) {
               setval(field.map, y, x, HUMAN);
               if (winchk(field, HUMAN)) {
                   setval(field.map, y, x, AI);
                   return 1;
               }
               setval(field.map, y, x, EMPTY);
           }
       }
   }
   return 0;
}

int linechk(Field &field, int y, int x, int vx, int vy, int len, PLAYER c) {
   const int endx = x + (len - 1) * vx;
   const int endy = y + (len - 1) * vy;
   if (!isvalid(field, endx, endy))
       return 0;
   for (int i = 0; i < len; i++)
       if (getval(field.map, (y + i * vy), (x + i * vx)) != c)
           return 0;
   return 1;
}

int isdraw(Field &field) {
   for (int y = 0; y < field.szY; y++)
       for (int x = 0; x < field.szX; x++)
           if (isempty(field, x, y))
               return 0;
   return 1;
}

char getval(PLAYER** array, const int row, const int col) {
    return POINT_ITEM(array, row, col);
}

void setval(PLAYER** array, const int row, const int col, PLAYER value) {
    POINT_ITEM(array, row, col) = value;
}

int isvalid(Field &field, int x, int y) {
    return CHK_DOT(x, field.szX) && CHK_DOT(y, field.szY);
}

int isvalid(string x, string y) {
    try {
        stoi(x);
        stoi(y);
        return 1;
    } catch (exception) {
        cout << "Incorrect input" << endl;
        return 0;
    }
}

int isempty(Field &field, int x, int y) {
    return getval(field.map, y, x) == EMPTY;
}
