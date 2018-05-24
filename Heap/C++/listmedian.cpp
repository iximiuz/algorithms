#include <cassert>
#include <iostream>
#include <set>
#include <sstream>
#include <unordered_map>
#include <vector>

using namespace std;

string toHumanReadableNumber(double n) {
    stringstream ss;
    ss << fixed << n;
    string s = ss.str();

    int pointPos = -1;
    int trailingZeroPos = s[s.size() - 1] == '0' ? s.size() - 1 : -1;

    for (int i = s.size() - 2; i >= 0; i--) {
        if (trailingZeroPos == i + 1  && s[i] == '0') {
            trailingZeroPos = i;
        }

        if (s[i] == '.') {
            pointPos = i;
            break;
        }
    }

    if (pointPos == -1) {
        return s;
    }
    return s.substr(0, trailingZeroPos - (pointPos + 1 == trailingZeroPos));
}

template<typename V>
class IndexedBinaryHeap {
private:
    unordered_map<V, set<int>> index_;
    vector<V> heap_;

public:
    void push(V v) {
        heap_.push_back(v);
        sinkdown_(heap_.size() - 1);
#ifdef _DEBUG
        ensureInvariant_("push");
#endif
    }
    
    void pop() {
        V last = heap_.back();
        heap_.pop_back();
        index_[last].erase(heap_.size());
        if (heap_.size()) {
            index_[heap_[0]].erase(0);
            heap_[0] = last;
            sinkup_(0);
        }
#ifdef _DEBUG
        ensureInvariant_("pop");
#endif
    }
        
    void decr(V oldV, V newV) {
        int pos = *index_[oldV].rbegin();
        index_[oldV].erase(pos);
        heap_[pos] = newV;
        sinkdown_(pos);
#ifdef _DEBUG
        ensureInvariant_("decr");
#endif
    }
    
    V top() const {
        return heap_.at(0);
    }
    
    bool has(V v) {
        return index_[v].size() > 0;
    }
    
    int size() const {
        return heap_.size();
    }

    string toString() {
        if (!heap_.size() && !index_.size()) {
            return "<empty>";
        }

        stringstream ss;
        ss << "#" << heap_.size() << " [";
        for (const auto &v : heap_) {
            ss << toHumanReadableNumber(v) << ", ";
        }
        ss << "]";

        if (index_.size()) {
            ss << "    {";
            for (const auto &pair : index_) {
                if (!pair.second.size()) {
                    continue;
                }
                ss << pair.first << ": {";
                for (const auto &pos: pair.second) {
                    ss << pos << ", ";
                }
                ss << "}, ";
            }
            ss << "}";
        }

        return ss.str();
    }
    
private:
    void sinkdown_(int pos) {
        V newitem = heap_[pos];
        int parent = (pos - 1) / 2;
        while (pos && newitem < heap_[parent]) {
            index_[heap_[pos]].erase(pos);
            heap_[pos] = heap_[parent];
            index_[heap_[pos]].insert(pos);
            pos = parent;
            parent = (pos - 1) / 2;
        }
        
        index_[heap_[pos]].erase(pos);
        heap_[pos] = newitem;
        index_[newitem].insert(pos);
    }
    
    void sinkup_(int pos) {
        V newitem = heap_[pos];
        int child = pos * 2 + 1;
        while (child < heap_.size()) {
            if (child + 1 < heap_.size() && heap_[child] > heap_[child + 1]) {
                child = child + 1;
            }
            
            index_[heap_[pos]].erase(pos);
            heap_[pos] = heap_[child];
            index_[heap_[pos]].insert(pos);
            pos = child;
            child = pos * 2 + 1;
        }
        
        index_[heap_[pos]].erase(pos);
        heap_[pos] = newitem;
        sinkdown_(pos);
    }
 
#ifdef _DEBUG
    void ensureInvariant_(const string &op) {
        for (int i = 0, n = heap_.size(); i < n / 2; i++) {
            int childpos = 2 * i + 1;
            if (!(childpos >= n || heap_[i] <= heap_[childpos])) {
                cerr << "op=" << op << " parent=" << heap_[i] << " at " << i 
                     << ", child=" << heap_[childpos] << " at " << childpos << endl;
                assert(false);
            }

            childpos++;

            if (!(childpos >= n || heap_[i] <= heap_[childpos])) {
                cerr << "op=" << op << " parent=" << heap_[i] << " at " << i 
                     << " child=" << heap_[childpos] << " at " << childpos << endl;
                assert(false);
            }
        }

        for (const auto &x : heap_) {
            assert(index_[x].size());
        }

        int heapSize = 0;
        for (const auto &pair : index_) {
            heapSize += pair.second.size();
            for (const auto &pos : pair.second) {
                assert(heap_.at(pos) == pair.first);
            }
        }

        assert(heapSize == heap_.size());
    }
#endif
};

template<typename T>
class List {
private:
    IndexedBinaryHeap<T> lower_;
    IndexedBinaryHeap<T> upper_;
    
public:
    void add(T v) {        
        if (lower_.size() && v <= -lower_.top()) {
            lower_.push(-v);
        } else if (upper_.size() && v >= upper_.top()) {
            upper_.push(v);
        } else {
            lower_.push(-v);
        }
        
        rebalance_();
#ifdef _DEBUG
        ensureInvariant_();
#endif
    }

    bool remove(T v) {
        if (lower_.has(-v)) {
            lower_.decr(-v, -(-lower_.top() + 1));
            lower_.pop();
            rebalance_();
#ifdef _DEBUG
            ensureInvariant_();
#endif
            return true;
        }        
        if (upper_.has(v)) {
            upper_.decr(v, upper_.top() - 1);
            upper_.pop();
            rebalance_();
#ifdef _DEBUG
            ensureInvariant_();
#endif
            return true;
        }        
        return false;
    }

    int size() const {
        return lower_.size() + upper_.size();
    }
    
    double median() const {
        if (lower_.size() > upper_.size()) {
            return -lower_.top();
        }
        if (lower_.size() < upper_.size()) {
            return upper_.top();
        }
        return (-lower_.top() + upper_.top()) / 2.0;
    }

    string toString() {
        return "lower=" + lower_.toString() + "\nupper=" + upper_.toString();
    }

private:
    void rebalance_() {
        while (abs(lower_.size() - upper_.size()) > 1) {
            if (lower_.size() > upper_.size()) {
                T x = lower_.top();
                lower_.pop();
                upper_.push(-x);
            } else {
                T x = upper_.top();
                upper_.pop();
                lower_.push(-x);
            }
        }
    }

#ifdef _DEBUG
    void ensureInvariant_() {
        assert(abs(lower_.size() - upper_.size()) <= 1);
        if (lower_.size() && upper_.size()) {
            assert(-lower_.top() <= upper_.top());
        }
    }
#endif
};

void median(const vector<char> &commands, const vector<int> &args) {
    List<int64_t> lst;
    
    for (int i = 0, l = commands.size(); i < l; i++) {
        char command = commands[i];
        int arg = args[i];

        if ('a' == command) {
            lst.add(arg);
            cout << toHumanReadableNumber(lst.median()) << endl;
        }
        
        if ('r' == command) {
            if (lst.remove(arg) && lst.size()) {
                cout << toHumanReadableNumber(lst.median()) << endl;
            } else {
                cout << "Wrong!" << endl;
            }
        }
    }
}

int main() {
    int N;
    cin >> N;

    vector<char> commands;
    vector<int> args;
    char cmd;
    int arg;
	for (int i = 0; i < N; i++) {
        cin >> cmd >> arg;
        commands.push_back(cmd);
        args.push_back(arg);
    }

    median(commands, args);
    return 0;
}

