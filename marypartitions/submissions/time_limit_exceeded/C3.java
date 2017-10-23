/**
 * Created by Alyssa on 10/21/2017.
 */

import java.lang.reflect.Array;
import java.util.*;
import java.io.*;

import static java.lang.Math.*;

public class C3 {
    static StringBuilder sb;

    public static void main(String[] args) {
        FastScanner sc = new FastScanner();
        //Scanner sc = new Scanner(System.in);
        sb = new StringBuilder();
        int T = sc.nextInt();

//        for(int trial = 0; trial < T; trial++) {
//            sc.nextInt();
//
//            int m = sc.nextInt();
//            int n = sc.nextInt();
//            Integer[] vals = getVals(n, m);
//            long[] dp = new long[n + 1];
//            dp[0] = 1;
//            for(int cents = 1; cents <= n; cents++) {
//                for(int j = 0; j < vals.length; j++) {
//                    if(vals[j] <= cents) {
//                        dp[cents] += dp[cents - vals[j]];
//                    }
//                }
//            }
//            sb.append(trial + 1).append(' ').append(dp[n]).append('\n');
//        }
//        sb.deleteCharAt(sb.length() - 1);

        for(int trial = 0; trial < T; trial++) {
            sc.nextInt();

            int m = sc.nextInt();
            int n = sc.nextInt();
            Integer[] vals = getVals(n, m);
            int V = vals.length;
            long[][] dp = new long[n + 1][V];
            Arrays.fill(dp[0], 1);
            for(int cents = 0; cents <= n; cents++) {
                 for(int i = 0; i < V; i++) {
                    int currValCent = vals[i];
                    while(cents - currValCent >= 0) {
                        if(i == 0) {
                            if (cents - currValCent == 0) {
                                dp[cents][i] += 1;
                            }
                            else {
                                //do nothing
                            }
                        }
                        else {
                            dp[cents][i] += dp[cents - currValCent][i - 1];
                        }
                        currValCent += vals[i];
                    }
                    if(i > 0 && cents > 0)
                        dp[cents][i] += dp[cents][i - 1];
                }
            }
            sb.append(trial + 1).append(' ').append(dp[n][vals.length - 1]).append("\n");
        }
        sb.deleteCharAt(sb.length() - 1);

        System.out.println(sb);
    }

    public static Integer[] getVals(int n, int m) {
        ArrayList<Integer> pows = new ArrayList<>();

        long pow = 1;
        while(pow <= n) {
            pows.add((int)pow);
            pow *= m;
        }
        Integer[] arr = new Integer[pows.size()];
        for(int i = 0; i < pows.size(); i++) {
            arr[i] = pows.get(pows.size() - i - 1);
        }
        return arr;
    }

    public static class FastScanner {
        BufferedReader br;
        StringTokenizer st;

        public FastScanner(Reader in) {
            br = new BufferedReader(in);
        }

        public FastScanner() {
            this(new InputStreamReader(System.in));
        }

        String next() {
            while (st == null || !st.hasMoreElements()) {
                try {
                    st = new StringTokenizer(br.readLine());
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            return st.nextToken();
        }

        int nextInt() {
            return Integer.parseInt(next());
        }

        long nextLong() {
            return Long.parseLong(next());
        }

        double nextDouble() {
            return Double.parseDouble(next());
        }

        String readNextLine() {
            String str = "";
            try {
                str = br.readLine();
            } catch (IOException e) {
                e.printStackTrace();
            }
            return str;
        }

        int[] readIntArray(int n) {
            int[] a = new int[n];
            for (int idx = 0; idx < n; idx++) {
                a[idx] = nextInt();
            }
            return a;
        }

        long[] readLongArray(int n) {
            long[] a = new long[n];
            for (int idx = 0; idx < n; idx++) {
                a[idx] = nextLong();
            }
            return a;
        }
    }
}
