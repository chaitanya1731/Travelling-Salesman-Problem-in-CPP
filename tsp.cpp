#include <bits/stdc++.h>
#define N 4
#define MAX 9999

using namespace std;

int path[N+1];
bool isVisited[N];
int cost = MAX;

string printVertex(int vertex){
	switch(vertex){
		case 0: return "A";
		case 1: return "B";
		case 2: return "C";
		case 3: return "D";
		default: return "Vertex Unknown";
	}
}

void printPath(){
	for (int i = 0; i <= N-1; i++) {
		string vertex = printVertex(path[i]);
		printf("%s -> ", vertex.c_str());
	}
	printf("%s\n", printVertex(path[N]).c_str());
}

void finalPath(int tempPath[]) {
	for (int i = 0; i < N; i++) {
		path[i] = tempPath[i];
	}
	path[N] = tempPath[0];
}

int frstMinDistance(int pathMatrix[N][N], int i) {
	int minDist = MAX;
	for (int k = 0; k < N; k++) {
		if (pathMatrix[i][k] < minDist && i != k) {
			minDist = pathMatrix[i][k];
		}
	}
	return minDist;
}

int scndMinDistance(int pathMatrix[N][N], int i) {
	int firstEdgeCost = MAX, secondEdgeCost = MAX;
	for (int j = 0; j < N; j++) {
		if (i == j)
			continue;
		if (pathMatrix[i][j] <= firstEdgeCost) {
			secondEdgeCost = firstEdgeCost;
			firstEdgeCost = pathMatrix[i][j];
		}
		else if (pathMatrix[i][j] <= secondEdgeCost && pathMatrix[i][j] != firstEdgeCost) {
			secondEdgeCost = pathMatrix[i][j];
		}
	}
	return secondEdgeCost;
}

void TSPbranchNbound(int pathMatrix[N][N], int currentBound, int currentCost, int level, int tempPath[]) {
	if (level == N) {
		if (pathMatrix[tempPath[level-1]][tempPath[0]] != 0) {
			int curr_res = currentCost + pathMatrix[tempPath[level-1]][tempPath[0]];
			if (curr_res < cost) {
				finalPath(tempPath);
				cost = curr_res;
			}
		}
		return;
	}

	for (int i=0; i<N; i++) {
		if (pathMatrix[tempPath[level-1]][i] != 0 && isVisited[i] == false) {
			int temp = currentBound;
			currentCost += pathMatrix[tempPath[level-1]][i];
			if (level==1)
				currentBound -= ((frstMinDistance(pathMatrix, tempPath[level-1]) + frstMinDistance(pathMatrix, i))/2);
			else
				currentBound -= ((scndMinDistance(pathMatrix, tempPath[level-1]) + frstMinDistance(pathMatrix, i))/2);

			if (currentBound + currentCost < cost) {
				tempPath[level] = i;
				isVisited[i] = true;

				TSPbranchNbound(pathMatrix, currentBound, currentCost, level+1, tempPath);
			}

			currentCost -= pathMatrix[tempPath[level-1]][i];
			currentBound = temp;

			memset(isVisited, false, sizeof(isVisited));
			for (int j=0; j<=level-1; j++) {
				isVisited[tempPath[j]] = true;
			}
		}
	}
}

void TSP(int pathMatrix[N][N]) {
	int tempPath[N+1];
	int currentBound = 0;
	memset(tempPath, -1, sizeof(tempPath));
	memset(isVisited, 0, sizeof(tempPath));

	for (int i = 0; i < N; i++){
		currentBound += (frstMinDistance(pathMatrix, i) + scndMinDistance(pathMatrix, i));
	}

	if(currentBound & 1){
		currentBound = currentBound/2 + 1;
	}
	else {
		currentBound = currentBound/2;
	}

	isVisited[0] = true;
	tempPath[0] = 0;

	TSPbranchNbound(pathMatrix, currentBound, 0, 1, tempPath);
}

int main() {
	int pathMatrix[N][N] = { { 0, 4, 3, 2 },
							 { 4, 0, 1, 2 },
							 { 3, 1, 0, 5 },
							 { 2, 2, 5, 0 } };
	TSP(pathMatrix);
	printf("Minimum cost of TSP : %d\n", cost);
	printf("Path Taken by the Salesman: ");
	printPath();
	return 0;
}
