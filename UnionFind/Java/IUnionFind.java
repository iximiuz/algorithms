public interface IUnionFind {
    public void union(int i, int j);
    public int find(int i);
    public boolean connected(int i, int j);
}