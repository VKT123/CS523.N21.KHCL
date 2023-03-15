#include <iostream>
#include <cstring>
using namespace std;
int *ComputeFail(string pattern)
{
    int *fail=new int[pattern.length()];
    fail[0]=0;
    int PatternLength=pattern.length();
    int j=0;
    int i=1;
    while (i<PatternLength)
    {
        if (pattern[j]==pattern[i])
        {
            // j+1 Chars match
            fail[i]=j+1;
            i++;
            j++;
        }
        else
            if (j>0) // j follows matching prefix
                j=fail[j-1];
            else
            {
                fail[i]=0;
                i++;
            }
    }
    return fail;
}
int KMP(string text, string pattern)
{
    int TextLength=text.length();
    int PatternLength=pattern.length();
    int *fail=ComputeFail(pattern);
    int i=0;
    int j=0;
    while (i<TextLength)
    {
        if (pattern[j]==text[i])
        {
            if (j==PatternLength-1)
                return i-PatternLength+1; //Match
            i++;
            j++;
        }
        else
            if (j>0)
                j=fail[j-1];
            else
                i++;
    }
    return -1; //Không match
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
    int result=KMP(text,pattern);
    if (result==-1)
    {
        cout << pattern << " khong co trong text!" << endl;
    }
    else
        cout << pattern << " nam bat dau tu vi tri thu " << result << " trong text!" << endl;
    return 0;
}
