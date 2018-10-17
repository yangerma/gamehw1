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
vector<PII> goal

struct board {
	int eval;
	PII player;
	vector<PII> box;
	bool operator<(const board b) const {
		return eval > b.eval;
	}
} cur;

bool solid(const char c) {
	return c=='#' or c=='$';
}

bool valid(const int mv) {
	PII tmp = MP(cur.player.F+dx[mv], cur.player.S+dy[mv]);
	unzip();
	if(mat[tmp.F][tmp.S] == '#')
		return false;
	if(mat[tmp.F][tmp.S] == '$' and solid(mat[tmp.F+dx[mv]][tmp.S+dy[mv]]))
		return false;
	return true;
}

void move(const int mv) {
	PII tmp = MP(cur.player.F+dx[mv], cur.player.S+dy[mv]);
	if(mat[tmp.F][tmp.S] == '$') {
		mat[tmp.F+dx[mv]][tmp.S+dy[mv]] = '$';
		for(int i=0; i<SZ(cur.box); i++) {
			if(cur.box[i] == tmp) {
				cur.box[i] = MP(tmp.F+dx[mv], tmp.S+dy[mv]);
				break;
			}
		}
	}
	mat[cur.player.F][cur.player.S] = '-';
	cur.player = tmp;
	mat[tmp.F][tmp.S] = '@';
}
// undo mv
void reverse(const int mv) {
	int rv = (mv+2)&3;
	PII tmp = MP(cur.player.F+dx[rv], cur.player.S+dy[rv]);
	PII tmp2 = MP(cur.player.F+dx[mv], cur.player.S+dy[mv]);
	assert(!solid(mat[tmp.F][tmp.S]) and mat[cur.F][cur.S] == '@');
	if(mat[tmp2.F][tmp2.S] == '$') {
		mat[cur.player.F][cur.player.S] = '$';
		for(int i=0; i<SZ(cur.box); i++) {
			if(cur.box[i] == tmp2) {
				cur.box[i] = cur.player;
				break;
			}
		}
		mat[cur.F+dx[mv]][cur.S+dy[mv]] = '-';
	}
	mat[tmp.F][tmp.S] = '@';
	cur.player=tmp;
}

int eval() {
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
	ret.player=player;
	ret.box = box;
	if(_eval == -1)
		ret.eval = eval();
	else
		ret.eval = _eval;
	return ret;
}

void init() {
	goal.clear();
	box.clear();
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
		while(!pque.empty()) {
			board bd = pque.top();
			pque.pop();
			for(int i=0; i<4; i++) {
				if(valid(i)) {
					move(i);
					pque.push(zip());
					reverse(i);
				}
			}
		}
	}
}
