import java.util.Scanner;
public class Brute_Force {
    public Brute_Force()
    {}
    public static int search(String pat, String txt) 
    {
        int M = pat.length();
        int N = txt.length();
        for (int i = 0; i <= N - M; i++)
        { 
            int j;
            for (j = 0; j < M; j++)
                if (txt.charAt(i+j) != pat.charAt(j))
                    break;
            if (j == M) return i; // found
        }
        return N; // not found 
    }
    public static void main(String[] args) 
    {
        Scanner scanner = new Scanner(System.in);
        System.out.print("Enter a text: ");
        String txt = scanner.nextLine();
        System.out.print("Enter a pattern: ");
        String pat = scanner.nextLine();
        scanner.close();
        Brute_Force BF = new Brute_Force();
        int offset = BF.search(pat,txt);
        if (offset==txt.length())
            System.out.println("Pattern not found");
        else
            System.out.println(pat+" start at " + offset);

    }
}
