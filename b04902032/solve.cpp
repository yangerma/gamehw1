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
const char alph[] = "drul";

bool solid(const char c) {
	return c=='#' or c=='$';
}



class board {
public:
	static int n, m;
	static char mat[N][N];
	static vector<PII> goal;

	int eval;
	PII player;
	set<PII> box;
	vector<int> steps;

	void update() {
		int num = SZ(goal);
		/*
		flow.init(num*2+2);
		for(int i=0; i<num; i++) {
			int j=0;
			for(PII b : box)
				flow.add(1+i, num+1+(j++), abs(goal[i].F-b.F)+abs(goal[i].S-b.S));
		}
		*/
		for(int i=0; i<num; i++)
			flow.add(0, 1+i, INF);
		for(int i=0; i<num; i++)
			flow.add(num+1+i, 2*num+1, INF);
		eval = flow.max_flow();
	}

	static board init() {
		board st;
		st.player=MP(0, 0);
		if(scanf("%d%d", &n, &m) == EOF)
			return st;
		fill(mat[0], mat[N], '#');
		goal.clear();
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
				if(buf[j-1] == '@') {
					st.player = MP(i, j);
					buf[j-1] = '-';
				}
				if(buf[j-1] == '$') {
					st.box.insert(MP(i, j));
					buf[j-1] = '-';
				}
				mat[i][j] = buf[j-1];
			}
		}
		st.update();
		return st;
	}

	bool valid(const int mv) {
		PII tmp = MP(player.F+dx[mv], player.S+dy[mv]);
		PII ttmp = MP(tmp.F+dx[mv], tmp.S+dy[mv]);
		if(mat[tmp.F][tmp.S] == '#')
			return false;
		if(box.count(tmp) and (mat[ttmp.F][ttmp.S] == '#' or box.count(ttmp)))
			return false;
		return true;
	}

	board move(const int mv) {
		board ret = *this;
		ret.steps.PB(mv);
		PII tmp = MP(ret.player.F+dx[mv], ret.player.S+dy[mv]);
		if(box.count(tmp)) {
			ret.box.erase(tmp);
			ret.box.insert(MP(tmp.F+dx[mv], tmp.S+dy[mv]));
		}
		ret.player = tmp;
		ret.update();
		return ret;
	}

	void print() {
		printf("%d\n", SZ(steps));
		for(int mv : steps)
			putchar(alph[mv]);
		putchar('\n');
	}

	bool operator<(const board b) const {
		return SZ(steps)+eval > SZ(b.steps)+b.eval;
	}
};

int board::n = 0, board::m=0;
char board::mat[][N] = {};
vector<PII> board::goal = vector<PII>();
priority_queue<board> pque;

int main() {
	while(true) {
		board start = board::init();
		if(start.player == MP(0, 0))
			break;
		while(!pque.empty())
			pque.pop();
		pque.push(start);
		while(!pque.empty()) {
			board bd = pque.top();
			//printf("%d %d\n", bd.player.F, bd.player.S);
			if(bd.eval == 0) {
				bd.print();
				break;
			}
			pque.pop();
			for(int i=0; i<4; i++)
				if(bd.valid(i))
					pque.push(bd.move(i));
		}
	}
}
