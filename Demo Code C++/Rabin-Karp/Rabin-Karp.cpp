#include <iostream>
#define Char 256 // Number Of Characters In The Input Alphabet
using namespace std;
int Rabin_Karp(string text, string pattern)
{
    int PatternLength = pattern.size();
    int TextLength = text.size();
    int i, j;
    long Prime=997;
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
                if (text[i + j] != pattern[j])
                {
                    break;
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

int main()
{
    string text, pattern;
    fflush(stdin);
    cout << "Nhap chuoi text: ";
    getline(cin, text);
    cout << "Nhap pattern can tim: ";
    getline(cin, pattern);
    text+=' \0';
    int result=Rabin_Karp(text,pattern);
    if (result==-1)
    {
        cout << pattern << " khong co trong text!" << endl;
    }
    else
        cout << pattern << " nam bat dau tu vi tri thu " << result << " trong text!" << endl;
    return 0;
}
