/*******************************************************
 * MC658 - Projeto e Analise de Algoritmo III - 1s2017
 * Prof: Flavio Keidi Miyazawa
 * PED: Edson Ticona Zegarra
 ******************************************************/
#include "knapsack.h"
#include <algorithm>
#include <queue>
#include <ctime>

///Preencher aqui para facilitar a correcao. 
// Nome1: Mariane Previde
// RA1: 121192
// Nome2: Sabrina Beck Angelini
// RA2: 157240

/*****************************************************/
/*       DECLARATION FOR PRIVATE USE BY BT           */
/*****************************************************/
typedef struct ResultBt {
	int maxProfit;
	int weight;
	vector<int> addedUsers;
} ResultBt;

void convertIntoSolution(ResultBt resultBt, vector<int> &sol);
bool backtrackingRecursive(int n, int d, int B, vector<int> &p, vector<int> &w, vector<int> &c, int t, clock_t begin,
						ResultBt &bestResult, ResultBt &currentResult);

///
// Bactracking function: 
///
bool bt(int n, int d, int B, vector<int> &p, vector<int> &w, vector<int> &c, vector<int> &sol, int t){

    clock_t begin = clock();
 
 	ResultBt bestResult;
 	bestResult.maxProfit = 0;
 	bestResult.weight = 0;

 	ResultBt currentResult;
 	currentResult.maxProfit = 0;
 	currentResult.weight = 0;

    bool finished = backtrackingRecursive(n, d, B, p, w, c, t, begin, bestResult, currentResult);
 
 	convertIntoSolution(bestResult, sol);

	///
	// Code used to generate the report data
	///
	//clock_t end = clock();
	//double elapsedSecs = double(end - begin) / CLOCKS_PER_SEC;
 	//cout << bestResult.maxProfit << "\n";
	//if(!finished) {
	//	cout << "Timeout\n";
	//}
	//cout << "Execution time: " << elapsedSecs << "\n";



    return finished;
}


/*****************************************************/
/*       DECLARATION FOR PRIVATE USE BY BNB          */
/*****************************************************/
typedef struct UserBnb {
	int id;
	int profit;
	int weight;
	int clazz;
	int profitPerUnitWeight;
} UserBnb;

typedef struct TreeNodeBnb {
	int k; // k represents the k-th user in the sorted vector
	int profit;
	int weight;
	int upperBound;
	vector<UserBnb> addedUsers;
	set<int> addedClasses;
} TreeNodeBnb;

struct CompareTreeNodeBnb {
    bool operator() (TreeNodeBnb node1, TreeNodeBnb node2) {
        // return "true" if "node1" is ordered after "node2", for example:
        return node1.upperBound > node2.upperBound;
    }
};

bool compareByProfitPerUnitWeightDesc(UserBnb user1, UserBnb user2);
bool is_feasible_solution(int d, int B, TreeNodeBnb node);
int upper_bound(TreeNodeBnb node, int n, int d, int B, vector<UserBnb> users);
void convertIntoSolution(vector<UserBnb> addedUsers, vector<int> &sol);

///
// Branch and Bound function
///
bool bnb(int n, int d, int B, vector<int> &p, vector<int> &w, vector<int> &c, vector<int> &sol, int t){ 
	clock_t begin = clock();

	// Joining all data about a user into one struct and all the users into one array
	vector<UserBnb> users;
	for(int i = 0; i < n; i++) {
		UserBnb user;
		user.id = i;
		user.profit = p[i];
		user.weight = w[i];
		user.clazz = c[i];
		user.profitPerUnitWeight = p[i]/w[i];

		users.push_back(user);
	}

	// Sort all users considering its ratio of profit per unit weight. our greedy approach uses the idea of the fractional
	// knapsack problem, where we can partially accept a user, imagine that the user is an item and the total bandwidth
	// is a knapsack, in the fractional knapsack problem we can put a fraction of the item into the knapsack
	sort(users.begin(), users.end(), compareByProfitPerUnitWeightDesc);

	// Our initial maxProfit will be zero, because we haven't decided wich items to put into the knapsack
	int maxProfit = 0;
	vector<UserBnb> solForMaxProfit;

	// Initialize the priority queue
	priority_queue<TreeNodeBnb, vector<TreeNodeBnb>, CompareTreeNodeBnb> nodesQueue;

	TreeNodeBnb initialDummyNode;
	initialDummyNode.profit = 0;
	initialDummyNode.weight = 0;
	initialDummyNode.k = -1;
	nodesQueue.push(initialDummyNode);

	while(!nodesQueue.empty()) {
		// Dequeue - the priority queue will always give the node with maximum upper bound
		TreeNodeBnb current = nodesQueue.top();
		nodesQueue.pop();

		// Cut solutions that does not give us an upper bound greater than our current solution
		if(current.upperBound < maxProfit) {
			continue;
		}

		// if it is a leaf we can't build new solutions with it
		if(current.k == n-1) {
			continue;
		}

		TreeNodeBnb nodeWithKthUser;
		nodeWithKthUser.k = current.k + 1;

		// Simulating the solution with the k-th user added to the total bandwidth
		// Add the k-th user to the solution
		nodeWithKthUser.profit = current.profit + users[nodeWithKthUser.k].profit;
		nodeWithKthUser.weight = current.weight + users[nodeWithKthUser.k].weight;		
		nodeWithKthUser.addedUsers.insert(nodeWithKthUser.addedUsers.end(), current.addedUsers.begin(), current.addedUsers.end());
		nodeWithKthUser.addedUsers.push_back(users[nodeWithKthUser.k]);
		nodeWithKthUser.addedClasses.insert(users[nodeWithKthUser.k].clazz);

		// If the solution with the k-th user is feasible and better than the current solution we have 
		if(is_feasible_solution(d, B, nodeWithKthUser) && nodeWithKthUser.profit > maxProfit) {
			// We keep the solution with the k-th user
			maxProfit = nodeWithKthUser.profit;
			solForMaxProfit = nodeWithKthUser.addedUsers;
		}

		// Calculate the upper bound of a any other possible solution including the k-th user
		nodeWithKthUser.upperBound = upper_bound(nodeWithKthUser, n, d, B, users);
		// If the upper calculated bound is promising than we add the node with the k-th user
		// to future evaluation
		if(nodeWithKthUser.upperBound > maxProfit) {
			nodesQueue.push(nodeWithKthUser);
		}
		
		// Considering the possible solutions without the k-th user doing the same process
		TreeNodeBnb nodeWithoutKthUser;
		nodeWithoutKthUser.k = current.k + 1;
		nodeWithoutKthUser.profit = current.profit;
		nodeWithoutKthUser.weight = current.weight;
		nodeWithoutKthUser.addedUsers = current.addedUsers;
		nodeWithoutKthUser.addedClasses = current.addedClasses;
		nodeWithoutKthUser.upperBound = upper_bound(nodeWithoutKthUser, n, d, B, users);
		if(nodeWithoutKthUser.upperBound > maxProfit) {
			nodesQueue.push(nodeWithoutKthUser);
		}

		clock_t end = clock();
		double elapsedSecs = double(end - begin) / CLOCKS_PER_SEC;
		if(elapsedSecs > t) {
			convertIntoSolution(solForMaxProfit, sol);
			///
			// Code used to generate the report data
			///
			//cout << maxProfit << "\n";
			//cout << "Timeout\n";
			//cout << "Execution time: " << elapsedSecs << "\n";
			return false;
		}

	}

	convertIntoSolution(solForMaxProfit, sol);

	///
	// 	Code used to generate the report data
	///
	//clock_t end = clock();
	//double elapsedSecs = double(end - begin) / CLOCKS_PER_SEC;
	//cout << maxProfit << "\n";
	//cout << "Execution time: " << elapsedSecs << "\n";

	return true;
}

/**********************************************/
/*        COMMON PRIVATE FUNCTIONS            */
/**********************************************/

int consumedCapacity(int d, int usedClasses, int weight) {
	return weight + d * (usedClasses - 1);
}

/**********************************************/
/*      BACKTRACKING PRIVATE FUNCTIONS        */
/**********************************************/

void convertIntoSolution(ResultBt resultBt, vector<int> &sol) {

	for(int i = 0; i < resultBt.addedUsers.size(); i++) {
		int userId = resultBt.addedUsers[i];
		sol[userId] = 1;
	}

}

int consumedCapacity(int d, vector<int> &c, ResultBt resultBt) {
	set<int> addedClasses;

	for(int i = 0; i < resultBt.addedUsers.size(); i++) {
		int userId = resultBt.addedUsers[i];
		addedClasses.insert(c[i]);
	}

	return consumedCapacity(d, addedClasses.size(), resultBt.weight);
}

bool backtrackingRecursive(int n, int d, int B, vector<int> &p, vector<int> &w, vector<int> &c, int t, clock_t begin,
						ResultBt &bestResult, ResultBt &currentResult) {
	
	// base case
	if(n == 0 || consumedCapacity(d, c, currentResult) >= B) {
		if(bestResult.maxProfit < currentResult.maxProfit) {
			bestResult = currentResult;
		}

		currentResult.maxProfit = 0;
		currentResult.weight = 0;
		currentResult.addedUsers.clear();

		clock_t end = clock();
		double elapsedSecs = double(end - begin) / CLOCKS_PER_SEC;
		if(elapsedSecs > t) {
			return false;
		}

		return true;
	}

	// try to add the n-th user
	ResultBt resultWithNthUser;
	resultWithNthUser.maxProfit = currentResult.maxProfit + p[n-1];
	resultWithNthUser.weight = currentResult.weight + w[n-1];
	resultWithNthUser.addedUsers.insert(resultWithNthUser.addedUsers.end(), currentResult.addedUsers.begin(), currentResult.addedUsers.end());
	resultWithNthUser.addedUsers.push_back(n-1);

	// if the user requirements are too big, so we can't include this user in the optimal solution
	if(consumedCapacity(d, c, resultWithNthUser) > B) {
		// continue without N-th user
		return backtrackingRecursive(n-1, d, B, p, w, c, t, begin, bestResult, currentResult);
	}

	// otherwise, we calculate wich is the best, the solution with or without n-th user
	bool finishedWithNthUser = backtrackingRecursive(n-1, d, B, p, w, c, t, begin, bestResult, resultWithNthUser);

	if(finishedWithNthUser) {
		return backtrackingRecursive(n-1, d, B, p, w, c, t, begin, bestResult, currentResult);
	}

	return false;

}

/**********************************************/
/*    BRANCH AND BOUND PRIVATE FUNCTIONS      */
/**********************************************/

bool compareByProfitPerUnitWeightDesc(UserBnb user1, UserBnb user2) {
	return user1.profitPerUnitWeight > user2.profitPerUnitWeight;
}

bool is_feasible_solution(int d, int B, int usedClasses, int weight) {;
	return consumedCapacity(d, usedClasses, weight) <= B;
}

bool is_feasible_solution(int d, int B, TreeNodeBnb node) {
	int usedClasses = node.addedClasses.size();
	return is_feasible_solution(d, B, usedClasses, node.weight);
}

int upper_bound(TreeNodeBnb node, int n, int d, int B, vector<UserBnb> users) {

	// If the maximum capacity has been reached on the current node level
	// or if the maximum capacity has been overcome
	// than the upper bound will be zero because we will
	// waste our time with non feasible solutions
	if(consumedCapacity(d, node.addedClasses.size(), node.weight) >= B) {
		return 0;
	}

	// The upper bound starts with the node profit
	int upperBound = node.profit;
	int totalWeight = node.weight;

	// We start to simulate the upper bound by putting the users ussing the greedy approach
	// of descending order of profit per weight unit starting on the user that came after
	// the k-th user
	int j;
	for(j = node.k + 1; j < n && is_feasible_solution(d, B, node.addedClasses.size(), totalWeight); j++){
		totalWeight += users[j].weight;
		upperBound += users[j].profit;
	}

	// If j is not n, we know that we can add the j-th user partially
	if(j < n) {
		upperBound += (B - consumedCapacity(d, node.addedClasses.size(), totalWeight)) * users[j].profitPerUnitWeight;
	}

	return upperBound;

}

void convertIntoSolution(vector<UserBnb> addedUsers, vector<int> &sol) {
	// transform the added users into the solution vector
	for(int i = 0; i < addedUsers.size(); i++) {
		UserBnb addedUser = addedUsers[i];
		sol[addedUser.id] = 1;
	}
}