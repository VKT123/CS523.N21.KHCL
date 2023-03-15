#include <iostream>
#include <cstring>
#include <algorithm>
using namespace std;
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
int Boyer_Moore(string text, string pattern)
{
    int *last = BuildLast(pattern);
    int TextLength=text.length();
    int PatternLength=pattern.length();
    int i=PatternLength-1;
    if (i>TextLength-1)
        return -1; // Không match nếu như pattern > text
    int j=PatternLength-1;
    do
    {
        if (pattern[j]==text[i])
            if (j==0)
                return i; //Match
            else // Looking-Glass Technique
            {
                i--;
                j--;
            }
        else // Character Jump Technique
        {
            int lo=last[text[i]]; //Lần xuất hiện cuối cùng
            i=i+PatternLength-min(j,1+lo);
            j=PatternLength-1;
        }
    }
    while(i<=TextLength-1);
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
    int result=Boyer_Moore(text,pattern);
    if (result==-1)
    {
        cout << pattern << " khong co trong text!" << endl;
    }
    else
        cout << pattern << " nam bat dau tu vi tri thu " << result << " trong text!" << endl;
    return 0;
}
