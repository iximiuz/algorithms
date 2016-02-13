public class NaiveUnionFind implements IUnionFind {
    private int n;
    private int[] members;

    public NaiveUnionFind(int n) {
        this.n = n;
        members = new int[n];
        for (int i = 0; i < n; i++) {
            members[i] = i;
        }
    }

    public void union(int i, int j) {
        int rootI = find(i);
        int rootJ = find(j);
        if (rootI != rootJ) {
            for (int x = 0; x < n; x++) {
                if (members[x] == rootI) {
                    members[x] = rootJ;
                }
            }   
        }        
    }

    public boolean connected(int i, int j) {
        return find(i) == find(j);
    }

    public int find(int i) {
        return members[i];
    }
}