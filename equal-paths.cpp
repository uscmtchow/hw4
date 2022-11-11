#include "equal-paths.h"
#include <iostream>
using namespace std;


// You may add any prototypes of helper functions here
int equalPathsHelper(Node* root);

bool equalPaths(Node * root)
{
    // Add your code below
    if (equalPathsHelper(root) <= -1) return false;
    return true;
}

int equalPathsHelper(Node* root) {
  if (root == nullptr) return 0;

	int lh = 0, rh = 0;

	if (root->left != nullptr) lh = equalPathsHelper(root->left) + 1;
	if (root->right != nullptr) rh = equalPathsHelper(root->right) + 1;

    if (lh != 0 && rh != 0) {
        if (lh == rh) return lh;
        return -1000;
    }   
    else if (rh == 0 && lh == 0) return 0;
    else if (rh == 0 && lh != 0) return lh;
    else if (lh == 0 && rh != 0) return rh;

	return -1000;
}
