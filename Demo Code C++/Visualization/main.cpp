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

using namespace std;

#define debug(x) cout << #x << " = " << x << endl
#define Char 256 // Number Of Characters In The Input Alphabet

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

const int N = 1123456;
string text, pattern;
long Prime;
Colour text_colour[N];
Colour pattern_colour[N];

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
    for (long unsigned int i = 0; i < text.size(); i++)
        ColourText(i, DEFAULT);
}

inline void ResetPatternColour()
{
    for (long unsigned int i = 0; i < pattern.size(); i++)
        ColourPattern(i, DEFAULT);
}

void PrintStrings(int text_index, int pattern_index)
{
    system("CLS");
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
    while (pattern_index < text_index)
    {
        pattern_print.insert(0, " ");
        num_espaces++;
        pattern_index++;
    }

  // Finally printing the strings
    for (long unsigned int i = 0; i < text_print.size(); i++)
        printf("\033[1;%dm%c\033[0m", text_colour[i + text_start], text_print[i]);
    printf("\n");

    for (long unsigned int i = 0; i < pattern_print.size(); i++)
        printf("\033[1;%dm%c\033[0m", pattern_colour[i - num_espaces], pattern_print[i]);
    printf("\n\n");
    Sleep(1000);
}

// String search methods declarations

int brute_force();
int dfa[N][256];
int kmpmatrix();
int boyer_moore();
int rabin_karp();

// Methods implementation
// Main Brute_Force algorithm function
int brute_force()
{
    int i, j, M, N;
    // Get the length of the pattern and text strings
    int PatternLength = pattern.size();
    int TextLength = text.size();
    // Iterate through the text string
    for (i = 0, j = 0; j < PatternLength && i < TextLength; i++, j++)
    {
        // Color the current positions in the text and pattern strings green
        ColourText(i, GREEN);
        ColourPattern(j, GREEN);
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
            ColourText(i, GREEN);
            ColourPattern(j, GREEN);
            // Print the current strings with colors
            PrintStrings(i, j);
        }
        // If the characters match, color the positions in blue
        ColourText(i, BLUE);
        ColourPattern(j, BLUE);
        // Print the current strings with colors
        PrintStrings(i, j);
    }
    // If the entire pattern was found in the text, return the starting position of the match
    if (j == PatternLength)
        return i - PatternLength;
    // If the pattern was not found in the text, return -1
    else
        return -1;
}
// Inicializing the dfa matrix used in the method kmpmatrix()
void initmatrix()
{
    int preflen = 0;
    int c;
    long unsigned int j;

    // Set all entries of the DFA to 0.
    for (c = 0; c < 256; c++)
        dfa[c][0] = 0;

    // Set the first entry of the DFA based on the first character of the pattern.
    dfa[(int)pattern[0]][0] = 1;

    // Compute the DFA transition function for the remaining entries.
    for (j = 1; j < pattern.size(); j++)
    {
        // Copy the transition function from the previous state for all characters except the pattern character.
        for (c = 0; c < 256; c++)
            dfa[j][c] = dfa[preflen][c];

        // Update the transition function for the pattern character.
        dfa[(int)pattern[j]][j] = j + 1;

        // Update the prefix length for the next iteration.
        preflen = dfa[(int)pattern[j]][preflen];
    }
}
// Main KMP algorithm function
int kmpmatrix()
{
    initmatrix();
    int i, j, PatternLength, TextLength;
    // Get the length of the pattern and text strings
    PatternLength = pattern.size();
    TextLength = text.size();

    // Apply the DFA to the text to find the pattern.
    for (i = 0, j = 0; j < PatternLength && i < TextLength; i++)
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
    if (j == PatternLength)
        return i - PatternLength;
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
    // Build last array
    int *last = BuildLast(pattern);

    // Get the length of the text and pattern
    int TextLength=text.size();
    int PatternLength=pattern.size();

    // Start comparing from the end of the pattern and text
    int i=PatternLength-1;
    if (i>TextLength-1)
        return -1; // If pattern is longer than text, return -1 (no match)
    int j=PatternLength-1;

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
            i=i+PatternLength-min(j,1+lo);
            j=PatternLength-1;
        }
    }
    while(i<=TextLength-1);

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
    mt19937 rng(random_device{}());
    uniform_int_distribution<int> dist(0, 1);
    long prime;
    do {
        prime = pow(2, 31) * dist(rng) + (dist(rng) ? pow(2, 30) : 0);
    } while (!is_prime(prime));
    return prime;
}
// Main Rabin_Karp algorithm function
int rabin_karp()
{
    // Get the length of the text and pattern
    int PatternLength = pattern.size();
    int TextLength = text.size();
    int i, j, k;
    Prime=997;                           // Choose a prime number for the hash function
    // Prime=longRandomPrime();
    long p = 0, t = 0, h = 1;

    // Calculate h = Char^(m-1) % Prime where Char is the base of the hash function (e.g., 256 for ASCII)
    for (i = 0; i < PatternLength - 1; i++)
        h = (h * Char) % Prime;

    // Calculate hash value for pattern and text
    for (i = 0; i < PatternLength; i++)
    {
        p = (Char * p + pattern[i]) % Prime;  // Calculate hash value for pattern
        t = (Char * t + text[i]) % Prime;     // Calculate hash value for the first m characters of the text
    }

    // Find the match
    for (i = 0; i < TextLength - PatternLength; i++)
    {
        // Color the matched portion of the text and pattern blue.
        ColourText(i, BLUE);
        ColourPattern(j, BLUE);
        PrintStrings(i, j);

        // If the hash values of pattern and text match, compare the strings character by character
        if (p == t)
        {
            for(k = 0; k < PatternLength; k++)
            {
                    ColourText(i + k, CYAN);
                    ColourPattern(j + k, CYAN);
            }
            PrintStrings(i + k, j + k);
            ResetTextColour();
            ResetPatternColour();
            for (j = 0; j < PatternLength; j++)
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
            if (j == PatternLength)  // If all characters match, return the starting index of the match
                return i;
        }

        else  // If the hash values don't match, skip to the next index in the text
        {
            for(k = 0; k < PatternLength; k++)
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
        if (i < TextLength - PatternLength)
        {
            t = (Char * (t - text[i] * h) + text[i + PatternLength]) % Prime;
            if (t < 0)
                t = (t + Prime);
        }
    }
    return -1;  // If no match is found, return -1
}
int main()
{
    system("CLS");
    fflush(stdin);
    cout << "----------------------------------------------------------------" << endl;
    cout << "|             Chuong trinh visualize substring search          |" << endl;
    cout << "----------------------------------------------------------------" << endl;
    cout << "Nhap chuoi text: ";
    getline(cin, text);
    cout << "Nhap pattern can tim: ";
    getline(cin, pattern);
    text+=' \0';
    system("CLS");
    int x;
    do
    {
        cout << "----------------------------------------------------------------" << endl;
        cout << "|             Chuong trinh visualize substring search          |" << endl;
        cout << "----------------------------------------------------------------" << endl;
        cout << "----------------------------------------------------------------" << endl;
        cout << "|                 4 thuat toan substring search                |" << endl;
        cout << "| 1. Brute Force                                               |" << endl;
        cout << "| 2. Boyer Moore                                               |" << endl;
        cout << "| 3. Knute Morris Prath                                        |" << endl;
        cout << "| 4. Rabin Karp                                                |" << endl;
        cout << "| 5. Thoat                                                     |" << endl;
        cout << "----------------------------------------------------------------" << endl;
        cout << "Hay chon thuat toan de thuc hien substring search: ";
        cin >> x;
        if (x<=4)
            switch(x)
            {
            case 1:
                brute_force();
                break;
            case 2:
                boyer_moore();
                break;
            case 3:
                kmpmatrix();
                break;
            case 4:
                rabin_karp();
                break;
            }
    }while(x<=4);
    return 0;
}
