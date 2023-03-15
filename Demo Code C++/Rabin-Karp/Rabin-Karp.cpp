#include <iostream>
#define Prime 101 //Prime Number
#define Char 256 // Number Of Characters In The Input Alphabet
using namespace std;
int Rabin_Karb(string text, string pattern)
{
    int i,j;
    int TextLength=text.length();
    int PatternLength=pattern.length();
    int hashText=0;// hash value for text
    int hashPattern=0;// hash value for pattern
    int h=1;
    for (i=0; i<PatternLength-1;i++)
        h=(h*Char)%Prime;
    for (i=0; i<PatternLength;i++)
    {
        hashPattern=(Char * hashPattern + pattern[i]) % Prime;
        hashText=(Char * hashText + text[i]) % Prime;
    }
    for (i=0; i<=TextLength-PatternLength; i++)
    {
        if (hashPattern==hashText)
        {
            for (j=0; j<PatternLength; j++)
                if (text[i+j] != pattern[j])
                    break;
        }
        if (j==PatternLength)
            return i;
        if (i<TextLength-PatternLength)
        {
            hashText=(Char*(hashText-text[i]*h)+text[i+PatternLength])%Prime;
            if (hashText<0)
                hashText=hashText+Prime;
        }
    }
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
    int result=Rabin_Karb(text,pattern);
    if (result==-1)
    {
        cout << pattern << " khong co trong text!" << endl;
    }
    else
        cout << pattern << " nam bat dau tu vi tri thu " << result << " trong text!" << endl;
    return 0;
}
