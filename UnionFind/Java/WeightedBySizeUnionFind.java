public class WeightedBySizeUnionFind implements IUnionFind {
    private int n;
    private int[] parents;
    private int[] sizes;

    public WeightedBySizeUnionFind(int n) {
        this.n = n;
        parents = new int[n];
        sizes = new int[n];
        for (int i = 0; i < n; i++) {
            parents[i] = i;
            sizes[i] = 1;
        }
    }

    public void union(int i, int j) {
        int rootI = find(i);
        int rootJ = find(j);
        if (rootI != rootJ) {
            if (sizes[rootI] < sizes[rootJ]) {
                parents[rootI] = rootJ;
                sizes[rootJ] += sizes[rootI];
            } else {
                parents[rootJ] = rootI;
                sizes[rootI] += sizes[rootJ];
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