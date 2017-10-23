/**
 * Solution to GNY 2016/Tight-Fit Sudoku
 *
 * @author godmar
 */
import java.util.*;
import java.util.stream.*;

@SuppressWarnings("unchecked")
public class TFSudokuGodmar
{
    static char [][][] B = new char [6][6][];
    static Set<Character> ALL() {
        return Stream.of('1','2','3','4','5','6','7','8','9').collect(Collectors.toSet());
    }
    static Set<Character> [] inrow;   // choices left available to place in row
    static Set<Character> [] incol;   // choices left available to place in col
    static Set<Character> [] insqr;   // choices left available to place in square/block

    static {
        inrow = new Set[6];
        incol = new Set[6];
        insqr = new Set[6];
        for (int i = 0; i < 6; i++) {
            inrow[i] = ALL();
            incol[i] = ALL();
            insqr[i] = ALL();
        }
    }

    public static void main(String []av) {
        Scanner s = new Scanner(System.in);
        int dashes = 0;
        for (int r = 0; r < 6; r++) {
            for (int c = 0; c < 6; c++) {
                String w = s.next();
                if (w.contains("/"))
                    B[r][c] = new char[] { w.charAt(0), w.charAt(2) };
                else
                    B[r][c] = new char[] { w.charAt(0) };

                for (char v : B[r][c]) {
                    if (v != '-') {
                        inrow[r].remove(v);
                        incol[c].remove(v);
                        insqr[r/2*2+c/3].remove(v);
                    } else {
                        dashes++;
                    }
                }
            }
        }

        sudoku(dashes);
    }

    static boolean sudoku(int dashes) {
        if (dashes == 0) {
            outputBoard();
            return true;
        }

        int bestr = -1;
        int bestc = -1;
        int besti = -1;
        Set<Character> bestchoices = null;

        /* Find the r, c, i location for which the smallest number of choices exists. */
        for (int r = 0; r < 6; r++)
            for (int c = 0; c < 6; c++)
                for (int i = 0; i < B[r][c].length; i++)
                    if (B[r][c][i] == '-') {
                        Set<Character> left = new HashSet<>(inrow[r]);
                        left.retainAll(incol[c]);
                        left.retainAll(insqr[r/2*2+c/3]);

                        if (bestchoices == null || left.size() < bestchoices.size()) {
                            bestr = r;
                            bestc = c;
                            besti = i;
                            bestchoices = left;
                        }
                    }

        /* Now try all options to the (r, c, i) locations with the smallest number
         * of choices.  If the solution so far has led to a contradiction, this could
         * be none.  If an entry is now determined, it will be 1.
         */
        for (char k : bestchoices) {
            char [] cell = B[bestr][bestc];
            cell[besti] = k;

            // check x/y x < y condition
            if (cell.length == 2 && cell[1-besti] != '-' && cell[0] > cell[1]) {
                cell[besti] = '-';
                continue;
            }

            inrow[bestr].remove(k);
            incol[bestc].remove(k);
            insqr[bestr/2*2+bestc/3].remove(k);
            if (sudoku(dashes-1))
                return true;

            // undo
            cell[besti] = '-';
            inrow[bestr].add(k);
            incol[bestc].add(k);
            insqr[bestr/2*2+bestc/3].add(k);
        }
        return false;
    }

    static void outputBoard() {
        for (int r = 0; r < 6; r++) {
            for (int c = 0; c < 6; c++) {
                StringJoiner sj = new StringJoiner("/");
                for (char v : B[r][c])
                    sj.add(v+"");
                System.out.print(sj.toString() + " ");
            }
            System.out.println();
        }
    }
}
