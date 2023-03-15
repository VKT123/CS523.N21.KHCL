#include <iostream>
#include <cstring>
using namespace std;

int Brute_Force(string text, string pattern)
{
    int TextLength=text.length();
    int PatternLength=pattern.length();
    for (int i=0; i<TextLength-PatternLength;i++)
    {
        int j;
        for (j=0; j<PatternLength;j++)
        {
            if (text[i+j]!=pattern[j])
            {
                break;
            }
        }
        if (j==PatternLength)
            return i;   //Match
    }
    return -1; //Mismatch
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
    int result=Brute_Force(text,pattern);
    if (result==-1)
    {
        cout << pattern << " khong co trong text!" << endl;
    }
    else
        cout << pattern << " nam bat dau tu vi tri thu " << result << " trong text!" << endl;
    return 0;
}
