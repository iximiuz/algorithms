public class WeightedByHeightUnionFind implements IUnionFind {
    private int n;
    private int[] parents;
    private int[] heigths;

    public WeightedByHeightUnionFind(int n) {
        this.n = n;
        parents = new int[n];
        heigths = new int[n];
        for (int i = 0; i < n; i++) {
            parents[i] = i;
        }
    }

    public void union(int i, int j) {
        int rootI = find(i);
        int rootJ = find(j);
        if (rootI != rootJ) {
            if (heigths[rootI] < heigths[rootJ]) {
                parents[rootI] = rootJ;
                heigths[rootJ] = Math.max(heigths[rootJ], heigths[rootI] + 1);
            } else {
                parents[rootJ] = rootI;
                heigths[rootI] = Math.max(heigths[rootI], heigths[rootJ] + 1);
            }
        }        
    }

    public boolean connected(int i, int j) {
        return find(i) == find(j);
    }

    public int find(int i) {
        while (i != parents[i]) {
            i = parents[i];
        }
        return i;
    }
}
