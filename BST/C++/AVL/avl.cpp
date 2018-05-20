#include <cassert>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

struct Node
{
    int data;
    struct Node *left;
    struct Node *right;
    int height;
};

inline int height(Node *node) {
    return node ? node->height : 0;
}

void updateHeights(vector<Node*> &path, int start = -1) {
    if (start == -1) {
        start = path.size() - 1;
    }

    for (int i = start; i >= 0; i--) {
        path[i]->height = 1 + max(height(path[i]->left), 
                                  height(path[i]->right));
    }
}

Node *rotateLeft(Node *root) {
    Node *newRoot = root->right;
    
    Node *tmp = newRoot->left;
    newRoot->left = root;
    root->right = tmp;
    
    root->height = 1 + max(height(root->left), height(root->right));
    newRoot->height = 1 + max(height(newRoot->left), height(newRoot->right));
    
    return newRoot;
}

Node *rotateRight(Node *root) {
    Node *newRoot = root->left;
    
    Node *tmp = newRoot->right;
    newRoot->right = root;
    root->left = tmp;
    
    root->height = 1 + max(height(root->left), height(root->right));
    newRoot->height = 1 + max(height(newRoot->left), height(newRoot->right));
    
    return newRoot;
}

Node *insertToAVL(Node *root, int data) {
    if (!root) {
      return new Node { data, NULL, NULL, 1 };
    }

    Node *node = root;
    vector<Node*> path;
    while (node) {
        path.push_back(node);

        if (node->data == data) {
            return root;  // noop
        } 
        
        if (node->data > data) {
            if (node->left) {
                node = node->left;
            } else {
                node->left = new Node { data, NULL, NULL, 1 };
                node = node->left;
                break;
            }
        } else {
            if (node->right) {
                node = node->right;
            } else {
                node->right = new Node{ data, NULL, NULL, 1 };
                node = node->right;
                break;
            }
        }
    }

    updateHeights(path);
    
    Node *unbalanced = NULL;
    int unbalancedPos;
    for (int i = path.size() - 1; i >= 0; i--) {
        if (abs(height(path[i]->left) - height(path[i]->right)) > 1) {
            unbalanced = path[i];
            unbalancedPos = i;
            break;
        }
    }
    if (!unbalanced) {
        return root;
    }

    assert(path.size() >= 2);
    bool changeRoot = (unbalanced == root);
    
    if (height(unbalanced->left) - height(unbalanced->right) > 1) {
        Node *child = unbalanced->left;
        if (height(child->left) - height(child->right) == 1) {
            // Left Left
            node = rotateRight(unbalanced);
        } else {
            // Left Right
            assert(height(child->left) - height(child->right) == -1);
            unbalanced->left = rotateLeft(child);
            node = rotateRight(unbalanced);
        }

        if (unbalancedPos > 0) {
            if (path[unbalancedPos - 1]->left == unbalanced) {
                path[unbalancedPos - 1]->left = node;
            } else {
                path[unbalancedPos - 1]->right = node;
            }
        }
    } else {
        assert(height(unbalanced->left) - height(unbalanced->right) < -1);
        Node *child = unbalanced->right;
        if (height(child->left) - height(child->right) == -1) {
            // Right Right
            node = rotateLeft(unbalanced);
        } else {
            // Right Left
            assert(height(child->left) - height(child->right) == 1);
            unbalanced->right = rotateRight(child);
            node = rotateLeft(unbalanced);
        }

        if (unbalancedPos > 0) {
            if (path[unbalancedPos - 1]->left == unbalanced) {
                path[unbalancedPos - 1]->left = node;
            } else {
                path[unbalancedPos - 1]->right = node;
            }
        }
    }

    updateHeights(path, unbalancedPos - 1);
    
    return changeRoot ? node : root;
}


// Testing facilities
int _height(Node *root) {
  if (!root) {
    return 0;
  }

  return 1 + max(_height(root->left), _height(root->right));
}

bool isBalanced(Node *root) {
  if (!root) {
    return true;
  }

  return abs(_height(root->left) - _height(root->right)) <= 1
    && isBalanced(root->left)
    && isBalanced(root->right);
}

void walk_inorder(Node *root, vector<int> &out) {
  if (!root) {
    return;
  }

  walk_inorder(root->left, out);
  out.push_back(root->data);
  walk_inorder(root->right, out);
}

void test_insertion(const vector<int> &fixture) {
  Node *root = NULL;
  for (const auto &x : fixture) {
      root = insertToAVL(root, x);
  }

  assert(root);
  assert(isBalanced(root));

  vector<int> out;
  walk_inorder(root, out);

  set<int> uniq(fixture.begin(), fixture.end());
  assert(out.size() == uniq.size());
  assert(equal(out.begin(), out.end(), uniq.begin()));
}

int main() {
    test_insertion({ 988, 857, 744, 492, 228, 366, 860, 937, 433, 552, 438, 229, 276, 408, 475 });
    test_insertion({ 859, 396, 30, 238, 236, 794, 819, 429, 144, 12, 929, 
                     530, 777, 405, 444, 764, 614, 539, 607, 841, 905, 819 });
}

