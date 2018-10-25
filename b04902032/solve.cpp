#include<bits/stdc++.h>
#include<signal.h>
#include<unistd.h>
#define MP make_pair
#define PB push_back
#define F first
#define S second
#define SZ(x) ((int)(x).size())
#define ALL(x) (x).begin(), (x).end()
using namespace std;

typedef pair<int,int> PII;
typedef long long ull;

const int INF = 1e9;
const int N = 55;
const int T = 15;
const pair<ull, char> END = MP(0ull, '\0');

const int dx[] = {1, 0, -1, 0};
const int dy[] = {0, 1, 0, -1};
const char alph[] = "drul";

int arrn[T], arrm[T];
char arrmat[T][N][N];
int kase, finished;
bool timeup;

unordered_map<ull, pair<ull, char> > vis[2];
unordered_map<ull, int > Cost;

class board {
private:
	bool gg(const int msk) {
		return ((msk&3) | (msk>>2))==3;
	}

public:
	static int n, m;
	static char mat[N][N];
	static vector<PII> goal;
	static int cost[N];
	static ull reach[N];
	static bool astar;

	int eval;
	PII player;
	set<PII> box;
	char last;
	int len;
	
	static int pos(const PII &p) {
		return (p.F-1)*m + p.S-1;
	}

	ull hsh() {
		ull ret=0;
		for(PII p : box)
			ret |= (1ull<<pos(p));
		return (ret<<6) | pos(player);
	}

	void update() {
		eval=0;
		int cnt=0;
		ull ky=0;
		for(PII p : box) {
			ky |= (1ull<<pos(p));
			if(++cnt == 2) {
				eval += Cost[ky];
				cnt=0;
				ky=0;
			}
		}
		if(cnt != 0)
			eval += cost[pos(*prev(box.end()))];
		eval *= 10;
	}

	static board init(const int t) {
		board ret;
		ret.len=0;
		ret.last = '\0';
		n=arrn[t],m=arrm[t];
		fill(mat[0], mat[N], '#');
		goal.clear();
		for(int i=1; i<=n; i++) {
			char buf[N];
			strcpy(buf, arrmat[t][i-1]);
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
		memset(reach, 0, sizeof(reach));
		for(int i=1; i<=n; i++)
			for(int j=1; j<=m; j++) {
				PII st = MP(i, j);
				cost[pos(st)] = INF;
				if(mat[i][j] == '#')
					continue;
				bool used[N] = {};
				queue<PII> que;
				que.push(st);
				int cnt=0;
				while(!que.empty()) {
					int sz=SZ(que);
					while(sz--) {
						PII p = que.front();
						que.pop();
						reach[pos(p)] |= (1ull<<pos(st));
						for(PII q : goal)
							if(p == q)
								cost[pos(st)] = cnt;
						used[pos(p)] = true;
						for(int k=0; k<4; k++) {
							PII tmp = MP(p.F+dx[k], p.S+dy[k]);
							PII ttmp = MP(p.F-dx[k], p.S-dy[k]);
							if(mat[tmp.F][tmp.S] != '#' and mat[ttmp.F][ttmp.S] != '#' and !used[pos(tmp)])
								que.push(tmp);
						}
					}
					cnt++;
				}
			}
		if(astar)
			ret.update();
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
		ret.len++;
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
		if(astar)
			ret.update();
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
		ull val = hsh()>>6;
		for(PII p : goal) {
			if(!(reach[pos(p)] & val))
				return true;
		}
		for(PII p : box) {
			if(cost[pos(p)] == INF)
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

	static int difficulty() {
		const int vx[] = {1, 1, 0, -1, -1, -1, 0, 1};
		const int vy[] = {0, 1, 1, 1, 0, -1, -1, -1};
		int ret=0;
		for(int i=1; i<=n; i++)
			for(int j=1; j<=m; j++) {
				if(mat[i][j] == '#')
					continue;
				for(int k=0; k<7; k++) {
					int x=i+vx[k], y=j+vy[k];
					int u=i+vx[k+1], v=j+vy[k+1];
					if(mat[x][y] != mat[u][v])
						ret++;
				}
			}
		return ret;
	}

	bool done() {
		ull val=0, bdhsh = hsh()>>6;
		for(PII p : goal)
			val |= (1ull<<pos(p));
		return (val&bdhsh) == bdhsh;
	}

	bool operator<(const board b) const {
		return len+eval > b.len+b.eval;
	}
};

int bfs(board start) {
	static queue<board> que[2];
	vis[0].clear();
	while(!que[0].empty())
		que[0].pop();
	while(!que[1].empty())
		que[1].pop();

	que[0].push(start);
	vis[0][start.hsh()] = END;
	while(!que[0].empty()) {
		const int z=0;
		board bd = que[z].front();
		que[z].pop();
		//printf("%d %d\n", bd.player.F, bd.player.S);
		ull bdhsh = bd.hsh();
		if(bd.done())
			return bd.len;
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
	return INF;
}

void precal(board start) {
	Cost.clear();
	int n=board::n, m=board::m;
	for(int i=0; i<n*m; i++)
		for(int j=i+1; j<n*m; j++) {
			//for(int k=j+1; k<n*m; k++) {
				//if(board::cost[i]==INF or board::cost[j]==INF or board::cost[k]==INF)
				if(board::cost[i]==INF or board::cost[j]==INF)
					continue;
				start.box.clear();
				//ull val=(1ull<<i)|(1ull<<j)|(1ull<<k);
				ull val=(1ull<<i)|(1ull<<j);
				start.box.insert(MP(i/m+1, i%m+1));
				start.box.insert(MP(j/m+1, j%m+1));
				//start.box.insert(MP(k/m+1, k%m+1));
				Cost[val] = bfs(start);
			}
}

int board::n = 0, board::m=0;
char board::mat[][N] = {};
int board::cost[N] = {};
ull board::reach[N] = {};
vector<PII> board::goal = vector<PII>();
bool board::astar = false;

void handler(int val) {
	for(int _=finished; _<kase; _++)
		printf("0\n\n");
	timeup=true;
}

priority_queue<board> pque[2];
queue<board> que[2];

int main() {
	alarm(59);
	signal(SIGALRM, handler);
	for(kase=0; scanf("%d%d", arrn+kase, arrm+kase) != EOF; kase++)
		for(int i=0; i<arrn[kase]; i++)
			scanf("%s", arrmat[kase][i]);
	finished=0;
	timeup=false;

	for(int t=0; t<kase; t++) {

		//printf("%d\n", t+1);
		board start = board::init(t);
		//printf("||| %d %d\n", board::difficulty(), SZ(start.box));
		vis[0].clear(), vis[1].clear();

		if(board::difficulty() >= 90) {
			precal(start);
			
			board::astar=true;
			while(!pque[0].empty())
				pque[0].pop();
			while(!pque[1].empty())
				pque[1].pop();

			pque[0].push(start);
			vis[0][start.hsh()] = END;
			board finish = start;
			finish.box.clear();
			for(PII p : board::goal)
				finish.box.insert(p);
			for(PII p : board::goal)
				for(int i=0; i<4; i++) {
					PII tmp = MP(p.F+dx[i], p.S+dy[i]);
					PII ttmp = MP(p.F+dx[i], p.S+dy[i]);
					if(board::mat[tmp.F][tmp.S] != '#' and board::mat[ttmp.F][ttmp.S] != '#' and (board::reach[board::pos(tmp)]&(start.hsh()>>6)) ) {
						bool flag = true;
						for(PII q : board::goal)
							if(tmp==q) {
								flag=false;
								break;
							}
						if(flag) {
							finish.player = tmp;
							vis[1][finish.hsh()] = END;
							pque[1].push(finish);
						}
					}
				}
			while(!pque[0].empty() and !pque[1].empty()) {
				int z = (SZ(pque[1]) < SZ(pque[0]));
				board bd = pque[z].top();
				//printf("%d %d\n", bd.player.F, bd.player.S);
				ull bdhsh = bd.hsh();
				if(timeup)
					return 0;
				if(vis[1-z].count(bdhsh)) {
					bd.print();
					finished++;
					fflush(stdout);
					break;
				}
				pque[z].pop();
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
							pque[z].push(nxt);
							vis[z][nxt.hsh()] = MP(bdhsh, nxt.last);
						}
					}
			}
			board::astar=false;
		} else {
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
					if(board::mat[tmp.F][tmp.S] != '#' and board::mat[ttmp.F][ttmp.S] != '#' and (board::reach[board::pos(tmp)]&(start.hsh()>>6)) ) {
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
				if(timeup)
					return 0;
				if(vis[1-z].count(bdhsh)) {
					bd.print();
					finished++;
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
}
