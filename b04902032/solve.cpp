#include<bits/stdc++.h>
#define MP make_pair
#define PB push_back
#define F first
#define S second
#define SZ(x) ((int)(x).size())
#define ALL(x) (x).begin(), (x).end()
using namespace std;

typedef pair<int,int> PII;
typedef long long ull;

const int N = 55;
const pair<ull, char> END = MP(0ull, '\0');

const int dx[] = {1, 0, -1, 0};
const int dy[] = {0, 1, 0, -1};
const char alph[] = "drul";

unordered_map<ull, pair<ull, char> > vis[2];

class board {
private:
	bool gg(const int msk) {
		return ((msk&3) | (msk>>2))==3;
	}

public:
	static int n, m;
	static char mat[N][N];
	static vector<PII> goal;
	static bool alive[N];
	static bool reach[N];

	//int eval;
	PII player;
	set<PII> box;
	//string steps;
	char last;
	
	static int pos(const PII &p) {
		return (p.F-1)*m + p.S-1;
	}

	ull hsh() {
		ull ret=0;
		for(PII p : box)
			ret |= (1ull<<pos(p));
		return (ret<<6) | pos(player);
	}

	static board init() {
		board ret;
		ret.player=MP(0, 0);
		ret.last = '\0';
		if(scanf("%d%d", &n, &m) == EOF)
			return ret;
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
					ret.player = MP(i, j);
					buf[j-1] = '-';
				}
				if(buf[j-1] == '$') {
					ret.box.insert(MP(i, j));
					buf[j-1] = '-';
				}
				mat[i][j] = buf[j-1];
			}
		}
		sort(ALL(goal));
		memset(reach, false, sizeof(reach));
		for(int i=1; i<=n; i++)
			for(int j=1; j<=m; j++) {
				if(mat[i][j] == '#')
					continue;
				PII st = MP(i, j);
				bool used[N] = {};
				queue<PII> que;
				que.push(st);
				alive[pos(st)] = false;
				while(!que.empty()) {
					PII p = que.front();
					que.pop();
					reach[pos(p)] = true;
					for(PII q : goal)
						if(p == q)
							alive[pos(st)] = true;
					used[pos(p)] = true;
					for(int k=0; k<4; k++) {
						PII tmp = MP(p.F+dx[k], p.S+dy[k]);
						PII ttmp = MP(p.F-dx[k], p.S-dy[k]);
						if(mat[tmp.F][tmp.S] != '#' and mat[ttmp.F][ttmp.S] != '#' and !used[pos(tmp)])
							que.push(tmp);
					}
				}
			}
		return ret;
	}

	bool valid(const int mv, bool rev=false, bool pull=false) {
		if(rev) {
			int op = mv^2;
			PII tmp = MP(player.F+dx[op], player.S+dy[op]);
			PII ttmp = MP(player.F+dx[mv], player.S+dy[mv]);
			if(mat[tmp.F][tmp.S] == '#' or box.count(tmp))
				return false;
			if(pull and !box.count(ttmp))
				return false;
		} else {
			PII tmp = MP(player.F+dx[mv], player.S+dy[mv]);
			PII ttmp = MP(tmp.F+dx[mv], tmp.S+dy[mv]);
			if(mat[tmp.F][tmp.S] == '#')
				return false;
			if(box.count(tmp) and (mat[ttmp.F][ttmp.S] == '#' or box.count(ttmp)))
				return false;
		}
		return true;
	}

	board move(const int mv, bool rev=false, bool pull=false) const {
		board ret = *this;
		char ch = alph[mv];
		if(rev) {
			int op = mv^2;
			PII tmp = MP(player.F+dx[op], player.S+dy[op]);
			PII ttmp = MP(player.F+dx[mv], player.S+dy[mv]);
			bool flag = box.count(ttmp);
			if(flag and pull) {
				ret.box.erase(ttmp);
				ret.box.insert(player);
				ch -= 32;
			}
			ret.last = ch;
			ret.player = tmp;
		} else {
			PII tmp = MP(ret.player.F+dx[mv], ret.player.S+dy[mv]);
			bool flag=box.count(tmp);
			if(flag) {
				ret.box.erase(tmp);
				ret.box.insert(MP(tmp.F+dx[mv], tmp.S+dy[mv]));
				ch -= 32;
			}
			ret.last = ch;
			ret.player = tmp;
		}
		return ret;
	}

	void print() {
		ull mid = hsh();
		string head, tail;
		for(ull i=mid; vis[0][i].S; i=vis[0][i].F)
			head += vis[0][i].S;
		for(ull i=mid; vis[1][i].S; i=vis[1][i].F)
			tail += vis[1][i].S;
		printf("%d\n", SZ(head)+SZ(tail));
		reverse(ALL(head));
		cout << head+tail << endl;
	}

	static void showmap() {
		for(PII p : goal)
			mat[p.F][p.S] = '.';
		for(int i=0; i<=n; i++) {
			for(int j=0; j<=m; j++)
				putchar(mat[i][j]);
			putchar('\n');
		}
	}

	bool dead() {
		int surr[N] = {};
		for(PII p : box) {
			if(!alive[pos(p)])
				return true;
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
					surr[pos(tmp)] -= 1 << (i^2);
					if(!gg(surr[pos(tmp)]))
						stk.push(tmp);
				}
			}
		}
		int j=0;
		for(PII p : box) {
			while(j<SZ(goal) and goal[j] < p)
				j++;
			if(j<SZ(goal) and p == goal[j])
				continue;
			if(gg(surr[pos(p)]))
				return true;
		}
		return false;
	}

	bool operator<(const board b) const {
		return false;
		//return SZ(steps)+eval > SZ(b.steps)+b.eval;
	}
};

int board::n = 0, board::m=0;
char board::mat[][N] = {};
bool board::alive[N] = {};
bool board::reach[N] = {};
vector<PII> board::goal = vector<PII>();
queue<board> que[2];

int main() {
	int lev=0;
	while(true) {
		board start = board::init();
		if(start.player == MP(0, 0))
			break;
		lev++;
		if(lev >= 100)
			continue;
		
		//board::showmap();
		vis[0].clear(), vis[1].clear();
		while(!que[0].empty())
			que[0].pop();
		while(!que[1].empty())
			que[1].pop();

		que[0].push(start);
		vis[0][start.hsh()] = END;
		board finish = start;
		finish.box.clear();
		for(PII p : board::goal)
			finish.box.insert(p);
		for(PII p : board::goal)
			for(int i=0; i<4; i++) {
				PII tmp = MP(p.F+dx[i], p.S+dy[i]);
				PII ttmp = MP(p.F+dx[i], p.S+dy[i]);
				if(board::mat[tmp.F][tmp.S] != '#' and board::mat[ttmp.F][ttmp.S] != '#' and board::reach[board::pos(tmp)]) {
					bool flag = true;
					for(PII q : board::goal)
						if(tmp==q) {
							flag=false;
							break;
						}
					if(flag) {
						finish.player = tmp;
						vis[1][finish.hsh()] = END;
						que[1].push(finish);
					}
				}
			}
		while(!que[0].empty() and !que[1].empty()) {
			int z = (SZ(que[1]) < SZ(que[0]));
			board bd = que[z].front();
			//printf("%d %d\n", bd.player.F, bd.player.S);
			ull bdhsh = bd.hsh();
			if(vis[1-z].count(bdhsh)) {
				bd.print();
				fflush(stdout);
				break;
			}
			que[z].pop();
			for(int i=0; i<4; i++)
				for(int j=0; j<=z; j++) {
					if(j==0 and alph[i^2] == bd.last)
						continue;
					if(bd.valid(i, z, j)) {
						board nxt = bd.move(i, z, j);
						if(z==0 and nxt.dead())
							continue;
						if(vis[z].count(nxt.hsh()))
							continue;
						que[z].push(nxt);
						vis[z][nxt.hsh()] = MP(bdhsh, nxt.last);
					}
				}
		}
	}
}
