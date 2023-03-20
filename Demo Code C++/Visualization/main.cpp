// Includes, defines and global variables
#include <algorithm>
#include <iostream>
#include <sstream>
#include <cstring>
#include <vector>
#include <random>
#include <cmath>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
using namespace std;

#define debug(x) cout << #x << " = " << x << endl
#define R 256 // Number Of Characters In The Input Alphabet
#define MAX 1123456 // Maximum Number Of Characters

// Change console font size declarations
void FontSize(int,int);

// String search methods declarations

int brute_force();
int dfa[MAX][256];
int kmpmatrix();
int boyer_moore();
int rabin_karp();

// Colours define
typedef enum
{
    DEFAULT = 0,
    BLACK = 30,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE
}Colour;

string text, pattern;
long Q;
Colour text_colour[MAX];
Colour pattern_colour[MAX];
int dynamic_option=0;

// Printing methods
inline void ColourText(int index, Colour c)
{
    text_colour[index] = c;
}

inline void ColourPattern(int index, Colour c)
{
    pattern_colour[index] = c;
}

inline void ResetTextColour()
{
    for (int i = 0; i < text.size(); i++)
        ColourText(i, DEFAULT);
}

inline void ResetPatternColour()
{
    for (int i = 0; i < pattern.size(); i++)
        ColourPattern(i, DEFAULT);
}

void PrintStrings(int text_index, int pattern_index)
{
    if (dynamic_option == 0)
    {
        FontSize(100,100);
        system("CLS");
    }
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
    for (int i = 0; i <= text_print.size(); i++)
        if (text_print[i] == '\n')
    text_print[i] = ' ';
    pattern_print = pattern.substr(pattern_start, pattern_end);
    pattern_index -= pattern_start;

  // Inserting space in the pattern if it's needed for it to be aligned with the
  // text
    int num_espaces = 0;
    while (pattern_index < text_index)
    {
        pattern_print.insert(0, " ");
        num_espaces++;
        pattern_index++;
    }

  // Finally printing the strings
    for (int i = 0; i < text_print.size(); i++)
        printf("\033[1;%dm%c\033[0m", text_colour[i + text_start], text_print[i]);
    printf("\n");

    for (int i = 0; i < pattern_print.size(); i++)
        printf("\033[1;%dm%c\033[0m", pattern_colour[i - num_espaces], pattern_print[i]);
    printf("\n\n");
    if (dynamic_option == 0)
        Sleep(800);
}

// Methods implementation
// Main Brute_Force algorithm function
int brute_force()
{
    ResetTextColour();
    ResetPatternColour();
    int i, j;
    // Get the length of the pattern and text strings
    int M = pattern.size();
    int N = text.size();
    // Iterate through the text string
    for (i = 0, j = 0; j < M && i < N; i++, j++)
    {
        // Color the current positions in the text and pattern strings blue
        ColourText(i, BLUE);
        ColourPattern(j, BLUE);
        // Print the current strings with colors
        PrintStrings(i, j);
        // Check if the current character in the text matches the current character in the pattern
        while (text[i] != pattern[j])
        {
            // If they don't match, color the positions in red
            ColourText(i, RED);
            ColourPattern(j, RED);
            // Print the current strings with colors
            PrintStrings(i, j);
            // Move back in the text string to the next possible starting position for a match
            i -= j - 1;
            // Reset the pattern index to 0
            j = 0;
            // Reset the colors of the text and pattern strings
            ResetPatternColour();
            ResetTextColour();
            // Color the current positions in the text and pattern strings green
            ColourText(i, BLUE);
            ColourPattern(j, BLUE);
            // Print the current strings with colors
            PrintStrings(i, j);
        }
        // If the characters match, color the positions in blue
        ColourText(i, GREEN);
        ColourPattern(j, GREEN);
        // Print the current strings with colors
        PrintStrings(i, j);
    }
    // If the entire pattern was found in the text, return the starting position of the match
    if (j == M)
        return i - M;
    // If the pattern was not found in the text, return -1
    else
        return -1;
}
// Inicializing the dfa matrix used in the method kmpmatrix()
void initmatrix()
{
    int M = pattern.size();  // get the size of the pattern to be searched for.

    // Set all entries of the DFA to 0.
    for (int c = 0; c < R; c++)
        dfa[c][0] = 0;

    // Set the first entry of the DFA based on the first character of the pattern.
    dfa[pattern[0]][0] = 1;

    // Compute the rest of the DFA.
    for (int i = 0, j = 1; j < M; j++)
    {
        // Copy mismatch cases.
        for (int c = 0; c < R; c++)
            dfa[c][j] = dfa[c][i];

        // Set match case.
        dfa[pattern[j]][j] = j+1;

        // Update restart state.
        i = dfa[pattern[j]][i];
    }
}
// Main KMP algorithm function
int kmpmatrix()
{
    initmatrix();
    ResetTextColour();
    ResetPatternColour();
    int i, j, M, N;
    // Get the length of the pattern and text strings
    M = pattern.size();
    N = text.size();

    // Apply the DFA to the text to find the pattern.
    for (i = 0, j = 0; j < M && i < N; i++)
    {
        // Color the matched portion of the text and pattern blue.
        ColourText(i, BLUE);
        ColourPattern(j, BLUE);
        PrintStrings(i, j);

        // Check if the current character in the text matches the current character in the pattern.
        if (dfa[(int)text[i]][j] == j + 1)
        {
            // If match, color the matched portion of the text and pattern green.
            ColourText(i, GREEN);
            ColourPattern(j, GREEN);
            PrintStrings(i, j);
        }
        else
        {
            // If mismatch, color the mismatched portion of the text and pattern red and reset the colors.
            ColourText(i, RED);
            ColourPattern(j, RED);
            PrintStrings(i, j);
            ResetTextColour();
            ResetPatternColour();
        }

        // Transition to the next state.
        j = dfa[(int)text[i]][j];
    }

    // If the pattern was found, return the starting position of the first occurrence.
    if (j == M)
        return i - M;
    // If the pattern was not found in the text, return -1
    else
        return -1;
}
// Function to build last array, which contains the index of the last occurrence of each character in the pattern
int *BuildLast(string pattern)
{
    int *last=new int[128];
    for (int i=0; i<128; i++)
        last[i]=-1;
    for (int i=0; i<pattern.size(); i++)
        last[int(pattern[i])]=i;
    return last;
}
// Main Boyer-Moore algorithm function
int boyer_moore()
{
    ResetTextColour();
    ResetPatternColour();
    // Build last array
    int *last = BuildLast(pattern);

    // Get the length of the text and pattern
    int N=text.size();
    int M=pattern.size();

    // Start comparing from the end of the pattern and text
    int i=M-1;
    if (i>N-1)
        return -1; // If pattern is longer than text, return -1 (no match)
    int j=M-1;

    // Compare pattern and text until either the end of the pattern or the end of the text is reached
    do
    {
        // Color the matched portion of the text and pattern blue.
        ColourText(i, BLUE);
        ColourPattern(j, BLUE);
        PrintStrings(i, j);
        // If the characters match, move both pointers back one position
        if (pattern[j]==text[i])
        {
            // If match, color the matched portion of the text and pattern green.
            ColourText(i, GREEN);
            ColourPattern(j, GREEN);
            PrintStrings(i, j);
            if (j==0)
                return i; // If the entire pattern has been matched, return the index of the match
            else
            {
                i--;
                j--;
            }
        }
        // If the characters do not match, use character jump technique to move the pattern and text pointers forward
        else
        {
            // If mismatch, color the mismatched portion of the text and pattern red and reset the colors.
            ColourText(i, RED);
            ColourPattern(j, RED);
            PrintStrings(i, j);
            ResetTextColour();
            ResetPatternColour();

            // Get the last occurrence of the current text character in the pattern
            int lo=last[text[i]];

            // Move the text pointer forward by the amount needed to align the pattern character with the last occurrence of the text character
            // If the last occurrence of the text character is not in the pattern, move the text pointer forward by the entire pattern length
            i=i+M-min(j,1+lo);
            j=M-1;
        }
    }
    while(i<=N-1);

    return -1; // If no match is found, return -1
}
// Check Prime Number Function
bool is_prime(int n) {
    if (n <= 1) {
        return false;
    }

    int limit = sqrt(n);

    for (int i = 2; i <= limit; i++) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}
// A random 31-bit prime
long longRandomPrime() {
    long prime;
    do {
        prime=(rand()%5001) + 5000;
    } while (!is_prime(prime));
    return prime;
}
// Main Rabin_Karp Las Vegas version algorithm function
int rabin_karp()
{
    ResetTextColour();
    ResetPatternColour();
    // Get the length of the text and pattern
    int M = pattern.size();
    int N = text.size();
    int i, j = 0, k;
    //Q=997;                           // Choose a prime number for the hash function
    Q=longRandomPrime();
    long p = 0, t = 0, h = 1;

    // Calculate h = R^(m-1) % Q where R is the base of the hash function (e.g., 256 for ASCII)
    for (i = 1; i <= M - 1; i++)
        h = (h * R) % Q;

    // Calculate hash value for pattern and text
    for (i = 0; i < M; i++)
    {
        p = (R * p + pattern[i]) % Q;  // Calculate hash value for pattern
        t = (R * t + text[i]) % Q;     // Calculate hash value for the first m characters of the text
    }

    // Find the match
    for (i = 0; i <= N - M; i++)
    {
        // Color the matched portion of the text and pattern blue.
        ColourText(i, BLUE);
        ColourPattern(j, BLUE);
        PrintStrings(i, j);

        // If the hash values of pattern and text match, compare the strings character by character
        if (p == t)
        {
            for(k = 0; k < M; k++)
            {
                    ColourText(i + k, CYAN);
                    ColourPattern(j + k, CYAN);
            }
            PrintStrings(i + k, j + k);
            ResetTextColour();
            ResetPatternColour();
            for (j = 0; j < M; j++)
            {
                if (text[i + j] != pattern[j])  // If characters don't match, break the loop
                {
                    // If mismatch, color the mismatched portion of the text and pattern red and reset the colors.
                    ColourText(i + j, RED);
                    ColourPattern(j, RED);
                    PrintStrings(i + j, j);
                    ResetTextColour();
                    ResetPatternColour();
                    break;
                }
                else  // If characters match, continue the loop
                {
                    // If match, color the matched portion of the text and pattern green.
                    ColourText(i + j, GREEN);
                    ColourPattern(j, GREEN);
                    PrintStrings(i + j, j);
                }
            }
            if (j == M)  // If all characters match, return the starting index of the match
                return i;
        }

        else  // If the hash values don't match, skip to the next index in the text
        {
            for(k = 0; k < M; k++)
            {
                    // If mismatch, color the mismatched portion of the text and pattern red and reset the colors.
                    ColourText(i + k, RED);
                    ColourPattern(j + k, RED);
            }
            PrintStrings(i + k, j + k);
            ResetTextColour();
            ResetPatternColour();
        }

        // Calculate hash value for the next m characters of the text
        if (i < N - M)
        {
            t = (R * (t - text[i] * h) + text[i + M]) % Q;
            if (t < 0)
                t = (t + Q);
        }
    }
    return -1;  // If no match is found, return -1
}
// Change Console Font Size
void FontSize(int x, int y)
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX cfi = { sizeof(cfi) };
    GetCurrentConsoleFontEx(hOut, FALSE, &cfi);
    cfi.dwFontSize.X = x; // set X size to x pixels
    cfi.dwFontSize.Y = y; // set Y size to y pixels
    SetCurrentConsoleFontEx(hOut, FALSE, &cfi);
}
int main()
{
    int idx;
    system("CLS");
    int option;
    do
    {
        system("CLS");
        FontSize(8,16);
        cout << "----------------------------------------------------------------" << endl;
        cout << "|              SUBSTRING SEARCH VISUALIZATION PROGRAM          |" << endl;
        cout << "----------------------------------------------------------------" << endl;
        cout << "----------------------------------------------------------------" << endl;
        cout << "|                 4 SUBSTRING SEARCH ALGORITHMS                |" << endl;
        cout << "| 0. Exit                                                      |" << endl;
        cout << "| 1. Brute Force                                               |" << endl;
        cout << "| 2. Boyer Moore                                               |" << endl;
        cout << "| 3. Knute Morris Prath                                        |" << endl;
        cout << "| 4. Rabin Karp                                                |" << endl;
        cout << "----------------------------------------------------------------" << endl;
        cout << "Select option: ";
        cin >> option;
        if (!option) return 0;
        fflush(stdin);
        cout << "Enter text string: ";
        getline(cin, text);
        cout << "Enter pattern string: ";
        getline(cin, pattern);
        if (text.length() < pattern.length())
            option = -1; // indicates error
        for (dynamic_option=0; dynamic_option<2; dynamic_option++)
        {
            switch(option)
                {
                case 1:
                    idx=brute_force();
                    break;
                case 2:
                    idx=boyer_moore();
                    break;
                case 3:
                    idx=kmpmatrix();
                    break;
                case 4:
                    idx=rabin_karp();
                    break;
                default:
                    cout << "INVALID INPUT PLEASE TRY AGAIN!" << endl;
                    system("PAUSE");
                    break;
                }
            if (idx!=-1)
                cout << "-->" << pattern << " found at: " << idx << endl;
            else
                cout << "-->" << pattern << " not found!!!" << endl;
            system("PAUSE");
            system("CLS");
            FontSize(8,16);
        }
    }while(1);
    return 0;
}
