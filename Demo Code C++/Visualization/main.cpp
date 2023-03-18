// Includes, defines and global variables
#include <algorithm>
#include <iostream>
#include <sstream>
#include <cstring>
#include <vector>
#include <random>
#include <cmath>

using namespace std;

#define debug(x) cout << #x << " = " << x << endl
#define Char 256 // Number Of Characters In The Input Alphabet

// Colours define
typedef enum {
  DEFAULT = 0,
  BLACK = 30,
  RED,
  GREEN,
  YELLOW,
  BLUE,
  MAGENTA,
  CYAN,
  WHITE
} Colour;

const int N = 1123456;
string text, pattern;
long Prime;
Colour text_colour[N];
Colour pattern_colour[N];

// General methods

void HitEnterClearingConsoleAndShowingMessage() {
  cin.ignore();
  fflush(stdin);
  printf("Hit enter key to continue...\n");
  getchar();
  system("clear");
}

void HitEnterToContinue() { getchar(); }

string stream_to_string(const istream &stm) {
  ostringstream oss;
  oss << stm.rdbuf();
  return oss.str();
}

// Printing methods

inline void ResetTextColour();
inline void ResetPatternColour();
inline void ColourText(int index, Colour c);
inline void ColourPattern(int index, Colour c);

inline void ColourText(int index, Colour c) { text_colour[index] = c; }

inline void ColourPattern(int index, Colour c) { pattern_colour[index] = c; }

inline void ResetTextColour() {
  for (long unsigned int i = 0; i < text.size(); i++)
    ColourText(i, DEFAULT);
}

inline void ResetPatternColour() {
  for (long unsigned int i = 0; i < pattern.size(); i++)
    ColourPattern(i, DEFAULT);
}

void PrintStrings(int text_index, int pattern_index) {
  // The strings the program will print
  // Each one will be printed in one line and the pattern must be aligned with
  // the text accordingly with the indexes
  string text_print;
  string pattern_print;

  // The indexes of the start and end of the substring of the text which we'll
  // print on the screen
  int text_start, text_end;
  text_start = max(0, text_index - 40);
  text_end = min((int)text.size(), 80);

  // The indexes of the start and end of the substring of the pattern which
  // we'll print on the screen
  int pattern_start, pattern_end;
  pattern_start = max(0, pattern_index - 40);
  pattern_end = min((int)pattern.size(), 80);

  text_print = text.substr(text_start, text_end);
  text_index -= text_start;

  // Removing line breaks of the text.
  for (long unsigned int i = 0; i <= text_print.size(); i++)
    if (text_print[i] == '\n')
      text_print[i] = ' ';

  pattern_print = pattern.substr(pattern_start, pattern_end);
  pattern_index -= pattern_start;

  // Inserting space in the pattern if it's needed for it to be aligned with the
  // text
  int num_espaces = 0;
  while (pattern_index < text_index) {
    pattern_print.insert(0, " ");
    num_espaces++;
    pattern_index++;
  }

  // Finally printing the strings
  for (long unsigned int i = 0; i < text_print.size(); i++)
    printf("\033[1;%dm%c\033[0m", text_colour[i + text_start], text_print[i]);
  printf("\n");

  for (long unsigned int i = 0; i < pattern_print.size(); i++)
    printf("\033[1;%dm%c\033[0m", pattern_colour[i - num_espaces],
           pattern_print[i]);
  printf("\n\n");
}

// String search methods declarations

int bruteforce();
int kmp();
int dfa[N][256];
int kmpmatrix();
int boyer_moore();
int rabin_karp();

// Main method

int main() {
    fflush(stdin);
    cout << "Nhap chuoi text: ";
    getline(cin, text);
    cout << "Nhap pattern can tim: ";
    getline(cin, pattern);
    text+=' \0';
    rabin_karp();
    return 0;
}

// Methods implementation

int bruteforce() {
  int i, j, M, N;
  M = pattern.size();
  N = text.size();

  for (i = 0, j = 0; j < M && i < N; i++, j++) {
    ColourText(i, GREEN);
    ColourPattern(j, GREEN);
    PrintStrings(i, j);

    while (text[i] != pattern[j]) {
      ColourText(i, RED);
      ColourPattern(j, RED);
      PrintStrings(i, j);
      i -= j - 1;
      j = 0;
      ResetPatternColour();
      ResetTextColour();
      ColourText(i, GREEN);
      ColourPattern(j, GREEN);
      PrintStrings(i, j);
    }
    ColourText(i, BLUE);
    ColourPattern(j, BLUE);
    PrintStrings(i, j);
  }

  if (j == M)
    return i - M;
  else
    return -1;
}

// Inicializing the dfa matrix used in the method kmpmatrix()
void initmatrix() {
  int preflen = 0;
  int c;
  long unsigned int j;
  for (c = 0; c < 256; c++)
    dfa[c][0] = 0;
  dfa[(int)pattern[0]][0] = 1;

  for (j = 1; j < pattern.size(); j++) {
    for (c = 0; c < 256; c++)
      dfa[j][c] = dfa[preflen][c];
    dfa[(int)pattern[j]][j] = j + 1;
    preflen = dfa[(int)pattern[j]][preflen];
  }
}
int kmpmatrix() {
  initmatrix();
  int i, j, M, N;
  M = pattern.size();
  N = text.size();

  for (i = 0, j = 0; j < M && i < N; i++) {
    ColourText(i, BLUE);
    ColourPattern(j, BLUE);
    PrintStrings(i, j);
    if (dfa[(int)text[i]][j] == j + 1) {
      ColourText(i, GREEN);
      ColourPattern(j, GREEN);
      PrintStrings(i, j);
    } else {
      ColourText(i, RED);
      ColourPattern(j, RED);
      PrintStrings(i, j);
      ResetTextColour();
      ResetPatternColour();
    }
    j = dfa[(int)text[i]][j];
  }

  if (j == M)
    return i - M;
  else
    return -1;
}
int *BuildLast(string pattern)
{
    /*Trả Về Một Mảng Chứa Index Của Các Kí Tự
    Trong Pattern Xuất Hiện Lần Cuối Cùng*/
    int *last=new int[128];
    for (int i=0; i<128; i++)
        last[i]=-1;
    for (int i=0; i<pattern.length(); i++)
        last[int(pattern[i])]=i;
    return last;
}
int boyer_moore()
{
    int *last = BuildLast(pattern);
    int TextLength=text.size();
    int PatternLength=pattern.size();
    int i=PatternLength-1;
    if (i>TextLength-1)
        return -1; // Không match nếu như pattern > text
    int j=PatternLength-1;
    do
    {
        ColourText(i, BLUE);
        ColourPattern(j, BLUE);
        PrintStrings(i, j);
        if (pattern[j]==text[i])
        {
            ColourText(i, GREEN);
            ColourPattern(j, GREEN);
            PrintStrings(i, j);
            if (j==0)
                return i; //Match
            else // Looking-Glass Technique
            {
                i--;
                j--;
            }
        }
        else // Character Jump Technique
        {
            ColourText(i, RED);
            ColourPattern(j, RED);
            PrintStrings(i, j);
            ResetTextColour();
            ResetPatternColour();
            int lo=last[text[i]]; //Lần xuất hiện cuối cùng
            i=i+PatternLength-min(j,1+lo);
            j=PatternLength-1;
        }
    }
    while(i<=TextLength-1);
    return -1; //Không match
}
int rabin_karp()
{
    int PatternLength = pattern.size();
    int TextLength = text.size();
    int i, j;
    Prime=997;
    long p = 0, t = 0, h = 1;
    for (i = 0; i < PatternLength - 1; i++)
        h = (h * Char) % Prime;
    // Calculate hash value for pattern and text
    for (i = 0; i < PatternLength; i++)
    {
        p = (Char * p + pattern[i]) % Prime;
        t = (Char * t + text[i]) % Prime;
    }

    // Find the match
    for (i = 0; i <= TextLength - PatternLength; i++)
    {
        if (p == t)
        {
            for (j = 0; j < PatternLength; j++)
            {
                ColourText(i, BLUE);
                ColourPattern(j, BLUE);
                PrintStrings(i, j);
                if (text[i + j] != pattern[j])
                {
                    ColourText(i, RED);
                    ColourPattern(j, RED);
                    PrintStrings(i, j);
                    ResetTextColour();
                    ResetPatternColour();
                    break;
                }
                else
                {
                    ColourText(i, GREEN);
                    ColourPattern(j, GREEN);
                    PrintStrings(i, j);
                }
            }
        if (j == PatternLength)
            return i;
        }

    if (i < TextLength - PatternLength)
    {
        t = (Char * (t - text[i] * h) + text[i + PatternLength]) % Prime;
        if (t < 0)
            t = (t + Prime);
    }
    }
    return -1;
}
