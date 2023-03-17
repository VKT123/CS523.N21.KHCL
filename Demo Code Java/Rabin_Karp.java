import java.util.Scanner;
public class Rabin_Karp 
{
    private long patHash; // pattern hash value
    private int M; // pattern length
    private long Q; // a large prime
    private int R = 256; // alphabet size
    private long RM; // R^(M-1) % Q 
    public Rabin_Karp(String pat)
    {
        this.M = pat.length();
        Q = 997;
        RM = 1;
        for (int i = 1; i <= M-1; i++) // Compute R^(M-1) % Q for use
            RM = (R * RM) % Q; // in removing leading digit.
        patHash = hash(pat, M);
    }
    public boolean check(int i) // Monte Carlo
    { 
        return true; 
    }
    private long hash(String key, int M) 
    { // Compute hash for key[0..M-1].
        long h = 0;
        for (int j = 0; j < M; j++)
            h = (R * h + key.charAt(j)) % Q;
        return h; 
    }
    private int search(String txt)
    { // Search for hash match in text.
        int N = txt.length();
        long txtHash = hash(txt, M);
        if (patHash == txtHash) return 0; // Match at beginning.
        for (int i = M; i < N; i++)
        { // Remove leading digit, add trailing digit, check for match.
            txtHash = (txtHash + Q - RM*txt.charAt(i-M) % Q) % Q;
            txtHash = (txtHash*R + txt.charAt(i)) % Q;
            if (patHash == txtHash) 
            if (check(i - M + 1)) 
                return i - M + 1; // match
        }
        return N; // no match found
    } 
    public static void main(String[] args) 
    {
        Scanner scanner = new Scanner(System.in);
        System.out.print("Enter a text: ");
        String txt = scanner.nextLine();
        System.out.print("Enter a pattern: ");
        String pat = scanner.nextLine();
        scanner.close();
        Rabin_Karp rabin = new Rabin_Karp(pat);
        int offset = rabin.search(txt);
        if (offset==txt.length())
            System.out.println("Pattern not found");
        else
            System.out.println(pat+" start at " + offset);

    }
}
