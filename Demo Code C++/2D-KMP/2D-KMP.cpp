#include <iostream>
#include <cstring>
#define CHAR 256
#define MAX 2000
using namespace std;
class KMP
{
    private:
        string pattern;
        int dfa[CHAR][MAX];
    public:
        KMP(string pattern)
        {
            this -> pattern=pattern;
            int PatternLength=pattern.length();
            dfa[pattern[0]][0]=1;
            for (int i=0,j=1;j<PatternLength;j++)
            {
                for (int c=0;c<CHAR;c++)
                    dfa[c][j]=dfa[c][i];
                dfa[pattern[j]][j]=j+1;
                i=dfa[pattern[j]][i];
            }
        }
        int Search(string text)
        {
            int i,j;
            int PatternLength=pattern.length();
            int TextLength=text.length();
            for (i=0, j=0; i<TextLength && j<PatternLength; i++)
                j=dfa[text[i]][j];
            if (j==PatternLength)
                return i-PatternLength;
            else
                return TextLength;
        }
};
int main()
{
    string text, pattern;
    fflush(stdin);
    cout << "Nhap chuoi text: ";
    getline(cin, text);
    cout << "Nhap pattern can tim: ";
    getline(cin, pattern);
    text+=' \0';
    KMP kmp(pattern);
    int result=kmp.Search(text);
    if (result==text.length())
    {
        cout << pattern << " khong co trong text!" << endl;
    }
    else
        cout << pattern << " nam bat dau tu vi tri thu " << result << " trong text!" << endl;
    return 0;
}
