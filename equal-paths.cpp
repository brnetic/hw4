#include "equal-paths.h"
#include <iostream>
using namespace std;


// You may add any prototypes of helper functions here

int maxheight(Node *root){
	if(root == NULL){
		return 0;
	}
	if(maxheight(root->left)>maxheight(root->right)){
		return 1 + maxheight(root->left);
	}
	else{
		return 1 + maxheight(root->right);
	}
}



bool equalPaths(Node * root)
{   
    // Add your code below
    if(root == NULL){
			return true;
		}
		if(root->left && root->right){
			return maxheight(root->left) == maxheight(root->right);
		}
		else if(root->left){
			return equalPaths(root->left);
		}
		else if(root->right){
			return equalPaths(root->right);
		}
		else{
			return true;
		}
}

