import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Scanner;

public class Maze {

    static HashSet<Long> visited;
    static String[] dirString = { "left", "up", "right", "down" };
    static int[][] dirs = { { 1, 0 }, { 0, 1 }, { -1, 0 }, { 0, -1 } };
    static Scanner input;

    public static void main(String[] args) {
        // TODO Auto-generated method stub
        visited = new HashSet<Long>();
        input = new Scanner(System.in);
        dfs(0, 0, -1000);
        System.out.println("no way out");
        System.exit(0);
    }

    private static void dfs(int i, int j, int dirFrom) {
        dirFrom = dirFrom + 2;
        dirFrom %= 4;
        ArrayList<Integer> dirsToDo = new ArrayList<Integer>();
        for (int dir = 0; dir < 4; dir++) {
            dirsToDo.add(dir);
        }
        dirsToDo.remove((Integer) dirFrom);
        if (dirFrom >= 0)
            dirsToDo.add(dirFrom);
        for (int dir : dirsToDo) {
            if (!visited.contains(hash(i, j, dir))) {
                visited.add(hash(i, j, dir));
                System.out.println(dirString[dir]);
                String verdict = input.next();
                if (verdict.equals("wall")) {
                    continue;
                }
                if (verdict.equals("solved")) {
                    System.exit(0);
                }
                if (verdict.equals("ok")) {
                    if (dir != dirFrom)
                        dfs(i + dirs[dir][0], j + dirs[dir][1], dir);
                }
            }
        }

    }

    private static long hash(int x, int y, int dir) {
        return x * 10000000000l + y * 100 + dir;
    }

}
