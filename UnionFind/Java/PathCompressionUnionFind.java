public class PathCompressionUnionFind implements IUnionFind {
    private int n;
    private int[] parents;
    private int[] ranks;

    public PathCompressionUnionFind(int n) {
        this.n = n;
        parents = new int[n];
        ranks = new int[n];
        for (int i = 0; i < n; i++) {
            parents[i] = i;
        }
    }

    public void union(int i, int j) {
        int rootI = find(i);
        int rootJ = find(j);
        if (rootI != rootJ) {
            if (ranks[rootI] < ranks[rootJ]) {
                parents[rootI] = rootJ;
            } else {
                parents[rootJ] = rootI;
            }

            if (ranks[rootI] == ranks[rootJ]) {
                ranks[rootI]++;
            }
        }        
    }

    public boolean connected(int i, int j) {
        return find(i) == find(j);
    }

    public int find(int i) {
        while (i != parents[i]) {
            i = parents[i] = parents[parents[i]];
        }
        return i;
    }
}
