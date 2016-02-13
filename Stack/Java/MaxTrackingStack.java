import java.util.EmptyStackException;


class MaxTrackingStack<Item extends Comparable<? super Item>> {
    private class Node {
        public Item value;
        public Node prev;
        public Node maxSoFar;

        public Node(Item val) {
            value = val;
            prev = null;
            maxSoFar = this;
        }
    }

    private Node top;
    private int sz;

    public MaxTrackingStack() {
        top = null;
        sz = 0;
    }

    public void push(Item item) {
        Node node = new Node(item);
        if (null != top) {
            node.prev = top;
            if (item.compareTo(top.maxSoFar.value) < 0) {
                node.maxSoFar = top.maxSoFar;
            }            
        }

        top = node;
        sz++;
    }

    public Item pop() {
        if (null == top) {
            throw new EmptyStackException();
        }
        Item retval = top.value;
        top = top.prev;
        sz--;
        return retval;
    }

    public Item max() {
        if (null == top) {
            throw new EmptyStackException();
        }
        return top.maxSoFar.value;
    }

    public boolean isEmpty() {
        return size() == 0;
    }

    public int size() {
        return sz;
    }

    public static void main(String[] args) {
        System.out.println("Testing MaxTrackingStack...");

        MaxTrackingStack<Integer> s = new MaxTrackingStack<Integer>();
        int popped;

        for (int i = 0; i < 2; i++) {
           if (!s.isEmpty()) {
                System.out.println("Error! Stack must be empty");
                return;
            }

            s.push(2);
            if (2 != s.max()) {
                System.out.println("Error!");
                return;
            }

            s.push(3);
            if (3 != s.max()) {
                System.out.println("Error!");
                return;
            }

            s.push(1);
            if (3 != s.max()) {
                System.out.println("Error!");
                return;
            }

            s.push(0);
            if (3 != s.max()) {
                System.out.println("Error!");
                return;
            }

            s.push(5);
            if (5 != s.max()) {
                System.out.println("Error!");
                return;
            }

            if (5 != s.size()) {
                System.out.println("Error! Stack size expected to be 3, but actually is [" + s.size() + "]");
                return;
            }

            popped = s.pop();
            if (5 != popped) {
                System.out.println("Error! Expected 5, got [" + popped + "]");
                return;
            }

            popped = s.pop();
            if (0 != popped) {
                System.out.println("Error! Expected 0, got [" + popped + "]");
                return;
            }

            popped = s.pop();
            if (1 != popped) {
                System.out.println("Error! Expected 0, got [" + popped + "]");
                return;
            }

            popped = s.pop();
            if (3 != popped) {
                System.out.println("Error! Expected 0, got [" + popped + "]");
                return;
            }

            popped = s.pop();
            if (2 != popped) {
                System.out.println("Error! Expected 0, got [" + popped + "]");
                return;
            }
        }
        
        System.out.println("Success!");   
    }
}
