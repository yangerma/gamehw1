/*
#include<bits/stdc++.h>
using namespace std;
#define MP make_pair
#define PB push_back
*/
#include "dinic.cpp"
typedef pair<int, int> PII;
#define F first
#define S second

const int N = 55;

const int dx[] = {1, 0, -1, 0};
const int dy[] = {0, 1, 0, -1};

int n, m;
char mat[N][N];
vector<PII> goal;
PII player;

struct board {
	int eval;
	PII player;
	vector<PII> box;
	bool operator<(const board b) const {
		return eval > b.eval;
	}
};

bool solid(const char c) {
	return c=='#' or c=='$';
}

int move(const PII cur, const int mv) {
	PII tmp = MP(cur.F+dx[mv], cur.S+dy[mv]);
	if(mat[tmp.F][tmp.S] == '#')
		return -1;
	if(mat[tmp.F][tmp.S] == '$' and solid(mat[tmp.F+dx[mv]][tmp.S+dy[mv]]))
		return -1;
	int ret=0;
	if(mat[tmp.F][tmp.S] == '$') {
		mat[tmp.F+dx[mv]][tmp.S+dy[mv]] = '$';
		return 1;
	}
	mat[tmp.F][tmp.S] = '@';
	return ret;
}

void reverse(const PII cur, const int mv) {
	int rv = (mv+2)&3;
	PII tmp = MP(cur.F+dx[rv], cur.S+dy[rv]);
	assert(!solid(mat[tmp.F][tmp.S]) and mat[cur.F][cur.S] == '@');
	mat[tmp.F][tmp.S] = '@';
	if(mat[cur.F+dx[mv]][cur.S+dy[mv]] == '$') {
		mat[cur.F][cur.S] = '$';
		mat[cur.F+dx[mv]][cur.S+dy[mv]] = '.';
	}
}

int eval() {
	vector<PII> box;
	for(int i=1; i<=n; i++)
		for(int j=1; j<=m; j++) {
			if(mat[i][j] == '@')
				box.PB(MP(i, j));
		}
	assert(SZ(goal) == SZ(box));
	int num = SZ(goal);
	flow.init(num*2+2);
	for(int i=0; i<num; i++)
		for(int j=0; j<num; j++)
			flow.add(1+i, num+1+j, abs(goal[i].F-box[j].F)+abs(goal[i].S-box[j].S));
	for(int i=0; i<num; i++)
		flow.add(0, 1+i, INF);
	for(int i=0; i<num; i++)
		flow.add(num+1+i, 2*num+1, INF);
	return flow.max_flow();
}

board zip(int _eval = -1) {
	board ret;
	ret.box.clear();
	for(int i=1; i<=n; i++)
		for(int j=1; j<=m; j++) {
			if(mat[i][j] == '@')
				ret.box.PB(MP(i, j));
		}
	ret.player=player;
	if(_eval == -1)
		ret.eval = eval();
	else
		ret.eval = _eval;
	return ret;
}

void init() {
	goal.clear();
}

int main() {
	while(scanf("%d%d", &n, &m) != EOF) {
		fill(mat[0], mat[N], '#');
		init();
		for(int i=1; i<=n; i++) {
			char buf[N];
			scanf("%s", buf);
			for(int j=1; j<=m; j++) {
				if(buf[j-1] == '*' or buf[j-1] == '+' or buf[j-1] == '.') {
					goal.PB(MP(i, j));
					if(buf[j-1] == '*')	buf[j-1] = '$';
					else if(buf[j-1] == '+') buf[j-1] = '@';
					else buf[j-1] = '-';
				}
				if(buf[j-1] == '@')
					player = MP(i, j);
				mat[i][j] = buf[j-1];
			}
		}
		priority_queue<board> pque;
		pque.push(zip());
		while(true) {
			board bd = pque.top();
			pque.pop();
			for(int i=0; i<4; i++) {
				if(change
			}
		}
	}
}
