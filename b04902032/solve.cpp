#include "flow.cpp"
typedef long long ull;
typedef pair<int, int> PII;
#define ALL(x) (x).begin(), (x).end()

const int N = 55;

const int dx[] = {1, 0, -1, 0};
const int dy[] = {0, 1, 0, -1};
const char alph[] = "drul";

unordered_map<ull, ull> vis[2];

class board {
private:
	bool gg(const int msk) {
		return ((msk&3) | (msk>>2))==3;
	}

public:
	static int n, m;
	static char mat[N][N];
	static vector<PII> goal;
	static int cost[N][N];

	int eval;
	PII player;
	set<PII> box;
	string steps;
	
	static int pos(const PII &p) {
		return (p.F-1)*m + p.S-1;
	}

	ull hsh() {
		ull ret=0;
		for(PII p : box)
			ret |= (1ull<<pos(p));
		return ret;
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
		for(int i=1; i<=n; i++)
			for(int j=1; j<=m; j++) {
				if(mat[i][j] == '#')
					continue;
				bool used[N] = {};
				queue<PII> que;
				que.push(MP(i, j));
				cost[i][j] = INF;
				int cnt=0;
				while(cost[i][j]==INF and !que.empty()) {
					int sz = SZ(que);
					for(int _=0; _<sz; _++) {
						PII p = que.front();
						que.pop();
						assert(p.F>0 and p.S>0);
						for(PII q : goal)
							if(p == q) {
								cost[i][j] = cnt;
								break;
							}
						if(cost[i][j] < INF)
							break;
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
		return st;
	}

	bool valid(const int mv, bool rev=false, bool pull=false) {
		if(rev) {
			int op = (mv+2)%4;
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

	board move(const int mv, bool rev=false, bool pull=false) {
		board ret = *this;
		char ch = alph[mv];
		if(rev) {
			int op = (mv+2)&3;
			PII tmp = MP(player.F+dx[op], player.S+dy[op]);
			PII ttmp = MP(player.F+dx[mv], player.S+dy[mv]);
			bool flag = box.count(ttmp);
			if(flag and pull) {
				ret.box.erase(ttmp);
				ret.box.insert(player);
				ch -= 32;
			}
			ret.steps += ch;
			ret.player = tmp;
		} else {
			PII tmp = MP(ret.player.F+dx[mv], ret.player.S+dy[mv]);
			bool flag=box.count(tmp);
			if(flag) {
				ret.box.erase(tmp);
				ret.box.insert(MP(tmp.F+dx[mv], tmp.S+dy[mv]));
				ch -= 32;
			}
			ret.steps += ch;
			ret.player = tmp;
		}
		return ret;
	}

	void print() {
		printf("%d\n", SZ(steps));
		for(char ch : steps)
			putchar(ch);
		putchar('\n');
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
			if(cost[p.F][p.S] == INF)
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
			if(j<SZ(goal) and p == goal[j])
				continue;
			if(gg(surr[pos(p)]))
				return true;
		}
		return false;
	}
	
	bool done(unordered_map<ull, ull> mp) {
		return (mp[hsh()] & (1ull<<pos(player)));
	}

	bool operator<(const board b) const {
		return SZ(steps)+eval > SZ(b.steps)+b.eval;
	}
};

int board::n = 0, board::m=0;
char board::mat[][N] = {};
int board::cost[][N] = {};
vector<PII> board::goal = vector<PII>();
queue<board> que[2];

int main() {
	int lev=1;
	while(true) {
		board start = board::init();
		if(start.player == MP(0, 0))
			break;
		if(lev++ >= 100)
			continue;
		
		//board::showmap();
		vis[0].clear(), vis[1].clear();
		while(!que[0].empty())
			que[0].pop();
		while(!que[1].empty())
			que[1].pop();

		que[0].push(start);
		vis[0][start.hsh()] = (1ull<<board::pos(start.player));
		board finish = start;
		finish.box.clear();
		for(PII p : board::goal)
			finish.box.insert(p);
		int goalhsh = finish.hsh();
		vis[1][goalhsh] = 0;
		for(PII p : board::goal)
			for(int i=0; i<4; i++) {
				PII tmp = MP(p.F+dx[i], p.S+dy[i]);
				PII ttmp = MP(p.F+dx[i], p.S+dy[i]);
				if(board::mat[tmp.F][tmp.S] != '#' and board::mat[ttmp.F][ttmp.S] != '#') {
					bool flag = true;
					for(PII q : board::goal)
						if(tmp==q) {
							flag=false;
							break;
						}
					if(flag) {
						finish.player = tmp;
						vis[1][goalhsh] |= (1ull<<board::pos(finish.player));
						que[1].push(finish);
					}
				}
			}
		puts("start search");
		int states1=0;
		int states2=0;
		int states3=0;
		while(!que[0].empty() and !que[1].empty()) {
			assert(!que[1].empty());
			//states++;
			int z = (SZ(que[1])<=SZ(que[0]));
			board bd = que[z].front();
			//printf("%d %d\n", bd.player.F, bd.player.S);
			if(bd.done(vis[1-z])) {
				printf("|| %d %d %d\n", states1, states2, states3);
				//bd.print();
				fflush(stdout);
				break;
			}
			que[z].pop();
			for(int i=0; i<4; i++) {
				for(int j=0; j<=z; j++) {
					/*
					if(j==0 and alph[(i+2)%4] == bd.steps.back())
						continue;
					*/
					if(bd.valid(i, z, j)) {
						//if(z == 1)
							//printf("|| (%d, %d), dir: %d, pull: %d\n", bd.player.F, bd.player.S, i, j);
						board nxt = bd.move(i, z, j);
						states1++;
						if(z==0 and nxt.dead())
							continue;
						states2++;
						assert(SZ(nxt.box) == SZ(board::goal));
						ull val = nxt.hsh();
						/*
						printf("|| %d: %llu\n", i, val);
						for(auto p : nxt.box)
							printf("** %d %d\n", p.F, p.S);
						*/
						int pos = board::pos(nxt.player);
						if(vis[z].count(val) and (vis[z][val]&(1ull<<pos)) != 0)
							continue;
						states3++;
						//printf("## %d: %llu\n", i, val);
						que[z].push(nxt);

						if(!vis[z].count(val))
							vis[z][val]=0;
						vis[z][val] |= (1ull<<pos);
					}
				}
			}
		}
		//puts("-----------");
	}
}
