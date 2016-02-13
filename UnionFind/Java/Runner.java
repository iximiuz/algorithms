import java.util.Random;


public class Runner {    
    private static final int N = 100000;

    private static IUnionFind createUnionFind(String implName, int n) {
        switch (implName) {
        case "naive":            
            return new NaiveUnionFind(n);
        case "size": 
            return new WeightedBySizeUnionFind(n);
        case "height":
            return new WeightedByHeightUnionFind(n);
        case "compress":
            return new PathCompressionUnionFind(n);
        }

        throw new IllegalArgumentException("Unkwon Union-Find implementation [" + implName + "]");
    }

    public static void main(String[] args) {
        String impl = args[0];
        IUnionFind uf = createUnionFind(impl, N);
        System.out.println("Testing " + uf.getClass().getSimpleName() + "...");

        for (int i = 0; i < N - 1; i++) {
            for (int j = i + 1; j < N; j++) {
                if (uf.connected(i, j)) {
                    System.out.println("Error! Two vertices are unexpectedly connected (" + i + ", " + j + ")");
                    return;
                }
            }
        }

        Random rand = new Random();        

        final long startTime = System.currentTimeMillis();

        for (int i = 0; i < N*10; i++) {
            int x = rand.nextInt(N);
            int y = rand.nextInt(N);
            uf.union(x, y);
            if (!uf.connected(x, y)) {
                System.out.println("Error! Two vertices aren't connected (" + x + ", " + y + ")");
                return;
            }
        }    

        System.out.println("Success!");    
        System.out.println("Total execution time: " + (System.currentTimeMillis() - startTime) + " ms");
    }
}
