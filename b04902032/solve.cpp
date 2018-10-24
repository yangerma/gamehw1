/*
#include<bits/stdc++.h>
using namespace std;
#define MP make_pair
#define PB push_back
*/
#include "flow.cpp"
typedef long long ull;
typedef pair<int, int> PII;
#define ALL(x) (x).begin(), (x).end()

const int N = 55;

const int dx[] = {1, 0, -1, 0};
const int dy[] = {0, 1, 0, -1};
const char alph[] = "drul";

unordered_map<ull, ull> vis;

class board {
private:
	bool gg(int msk) {
		return ((msk&3) | (msk>>2))==3;
	}

public:
	static int n, m;
	static char mat[N][N];
	static vector<PII> goal;

	int eval;
	PII player;
	set<PII> box;
	string steps;
	
	static int pos(PII p) {
		return (p.F-1)*m + p.S-1;
	}

	ull hsh() {
		ull ret=0;
		for(PII p : box)
			ret |= (1ull<<pos(p));
		return ret;
	}

	void update() {
		/*
		int num = SZ(goal);
		flow.init(num*2+2);
		for(int i=0; i<num; i++) {
			int j=0;
			for(PII b : box)
				flow.add(1+i, num+1+(j++), 1, abs(goal[i].F-b.F)+abs(goal[i].S-b.S));
		}
		for(int i=0; i<num; i++)
			flow.add(0, 1+i, 1, 0);
		for(int i=0; i<num; i++)
			flow.add(num+1+i, 2*num+1, 1, 0);
		eval = flow.max_flow().S;
		*/
		int j=0;
		eval=0;
		for(set<PII>::iterator ite=box.begin(); ite != box.end(); ite++) {
			eval += abs(goal[j].F - ite->F) + abs(goal[j].S - ite->S);
			j++;
		}
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
		sort(ALL(goal));
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
		PII tmp = MP(ret.player.F+dx[mv], ret.player.S+dy[mv]);
		char ch = alph[mv];
		bool flag=box.count(tmp);
		if(flag) {
			ret.box.erase(tmp);
			ret.box.insert(MP(tmp.F+dx[mv], tmp.S+dy[mv]));
			ch -= 32;
		}
		ret.steps.PB(ch);
		ret.player = tmp;
		ret.update();
		return ret;
	}

	void print() {
		printf("%d\n", SZ(steps));
		for(char ch : steps)
			putchar(ch);
		putchar('\n');
	}

	static void showmap() {
		for(int i=0; i<=n; i++) {
			for(int j=0; j<=m; j++)
				putchar(mat[i][j]);
			putchar('\n');
		}
	}

	bool dead() {
		int surr[N] = {};
		for(PII p : box) {
			for(int i=0; i<4; i++) {
				PII tmp = MP(p.F+dx[i], p.S+dy[i]);
				if(mat[tmp.F][tmp.S] == '#' or box.count(tmp))
					surr[pos(p)] |= (1<<i);
			}
		}
		stack<PII> stk;
		for(PII p : box)
			if(!gg(surr[pos(p)]))
				stk.push(p);
		while(!stk.empty()) {
			PII p = stk.top();
			stk.pop();
			for(int i=0; i<4; i++) {
				PII tmp = MP(p.F+dx[i], p.S+dy[i]);
				if(box.count(tmp) and gg(surr[pos(tmp)])) {
					surr[pos(tmp)] -= 1 << ((i+2)&3);
					if(!gg(surr[pos(tmp)]))
						stk.push(tmp);
				}
			}
		}
		int j=0;
		for(PII p : box) {
			while(j<SZ(goal) and goal[j] < p)
				j++;
			if(p == goal[j])
				continue;
			if(gg(surr[pos(p)]))
				return true;
			/*
			int val=0;
			for(int i=0; i<4; i++) {
				PII tmp = MP(p.F+dx[i], p.S+dy[i]);
				if(mat[tmp.F][tmp.S] == '#')
					val |= 1<<(i&1);
			}
			if(val==3)
				return true;
			*/
		}
		return false;
	}
	
	bool done() {
		ull tar=0;
		for(PII p : goal)
			tar |= (1ull<<pos(p));
		return tar == hsh();
	}

	bool operator<(const board b) const {
		return SZ(steps)+eval > SZ(b.steps)+b.eval;
	}
};

int board::n = 0, board::m=0;
char board::mat[][N] = {};
vector<PII> board::goal = vector<PII>();
priority_queue<board> pque;
//queue<board> pque;

int main() {
	int lev=1;
	while(true) {
		board start = board::init();
		if(start.player == MP(0, 0))
			break;
		if(lev++ >= 100)
			continue;

		vis.clear();
		while(!pque.empty())
			pque.pop();

		pque.push(start);
		int maxsz = 0;
		while(!pque.empty()) {
			maxsz = max(maxsz, SZ(pque));
			board bd = pque.top();
			//printf("%d %d\n", bd.player.F, bd.player.S);
			if(bd.done()) {
				printf("|| %d\n", maxsz);
				bd.print();
				break;
			}
			pque.pop();
			for(int i=0; i<4; i++) {
				if(alph[(i+2)%4] == bd.steps.back())
					continue;
				if(bd.valid(i)) {
					board nxt = bd.move(i);
					if(nxt.dead())
						continue;
					assert(SZ(nxt.box) == SZ(board::goal));
					ull val = nxt.hsh();
					/*
					printf("|| %d: %llu\n", i, val);
					for(auto p : nxt.box)
						printf("** %d %d\n", p.F, p.S);
					*/
					int pos = board::pos(nxt.player);
					if(vis.count(val) and (vis[val]&(1ull<<pos)) != 0)
						continue;
					//printf("## %d: %llu\n", i, val);
					pque.push(nxt);
					if(!vis.count(val))
						vis[val]=0;
					vis[val] |= (1ull<<pos);
				}
			}
		}
		//puts("-----------");
	}
}
