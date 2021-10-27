#include <bits/stdc++.h>

using namespace std;

#define print_op(...) ostream& operator<<(ostream& out, const __VA_ARGS__& u)
template<typename A, typename B> print_op(pair<A, B>) { return out << "(" << u.first << ", " << u.second << ")"; }
template<typename T_container, typename T = typename enable_if<!is_same<T_container, string>::value, typename T_container::value_type>::type> print_op(T_container) { out << "{"; string sep; for (const T &x : u) out << sep << x, sep = ", "; return out << "}"; }
template<typename T> void dbg_out(string s, T x) {cerr << "\033[1;35m" << s << "\033[0;32m = \033[33m" << x << "\033[0m\n";}
template<typename T, typename... Args> void dbg_out(string s, T x, Args... args) {for (int i=0, b=0; i<(int)s.size(); i++) if (s[i] == '(' || s[i] == '{') b++; else
if (s[i] == ')' || s[i] == '}') b--; else if (s[i] == ',' && b == 0) {cerr << "\033[1;35m" << s.substr(0, i) << "\033[0;32m = \033[33m" << x << "\033[31m | "; dbg_out(s.substr(s.find_first_not_of(' ', i + 1)), args...); break;}}
#ifdef LOCAL
#define dbg(...) dbg_out(#__VA_ARGS__, __VA_ARGS__)
#else
#define dbg(...)
#endif

using ll = long long;
using ld = long double;
#define ar array
#define sz(x) ((int)x.size())
#define rep(i, a, b) for (int i = (int)(a); i < (int)(b); i++) 
#define all(a) (a).begin(), (a).end()

const int MAX_N = 2e5 + 5;
const int MAX_L = 20;
const int MAX_C = 26;
const ll MOD = 1e9 + 7;
const ll INF = 1e9;
const ld EPS = 1e-9;

struct DSU {
    int num; vector<int> par, sza;
    void init(int _n) {
        num = _n;
        par = vector<int>(_n); iota(par.begin(), par.end(), 0);
        sza = vector<int>(_n, 1);
    }
    int find(int u) { return u == par[u] ? u : par[u] = find(par[u]); }
    bool same(int u, int v) { return find(u) == find(v); }
    bool merge(int u, int v) {
        u = find(u), v = find(v);
        if (u == v) return false;
        if (sza[u] > sza[v]) swap(u, v);
        par[u] = v;
        sza[v] += sza[u];
        num--;
        return true;
    }
};
 
int n, m, q, par[MAX_N][MAX_L], dep[MAX_N], mx[MAX_N][MAX_L];
bool vis[MAX_N];
vector<ar<int,2>> adj[MAX_N];
DSU dsu;
 
void dfs(int u, int p = 0, int c = 0) {
    vis[u] = true;
    par[u][0] = p;
    mx[u][0] = c;
    for (int i = 1; i < MAX_L; i++) {
        par[u][i] = par[par[u][i - 1]][i - 1];
        mx[u][i] = max(mx[u][i - 1], mx[par[u][i - 1]][i - 1]);
    }
    for (auto [v, w] : adj[u]) {
        if (v == p) continue;
        dep[v] = dep[u] + 1;
        dfs(v, u, w);
    }
}
 
ar<int,2> kthAnc(int u, int k) {
    int c = 0;
    for (int i = 0; i < MAX_L; i++) { 
        if (k & (1 << i)) {
            c = max(c, mx[u][i]);
            u = par[u][i];
        }
    }
    return {u, c};
}
 
int query(int uu, int v) {
    if (dep[uu] < dep[v]) swap(uu, v);
    auto [u, c] = kthAnc(uu, dep[uu] - dep[v]);
    if (u == v) return c;
    for (int i = MAX_L - 1; i >= 0; i--) {
        if (par[u][i] != par[v][i]) {
            c = max(c, mx[u][i]);
            c = max(c, mx[v][i]);
            u = par[u][i];
            v = par[v][i];
        }
    }
    c = max(c, mx[u][0]);
    c = max(c, mx[v][0]);
    return c;
}

// Maintain a forest of minimal spanning trees (cost is the time)
// Then for each spanning tree, use binary lifting to find the max cost between u and v
 
void solve(int tc = 0) {
    cin >> n >> m >> q;
    dsu.init(n);
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v; 
        if (dsu.merge(u, v)) {
            adj[u].push_back({v, i + 1});
            adj[v].push_back({u, i + 1});
        }
    }
    for (int i = 1; i <= n; i++) {
        if (vis[i]) continue;
        dfs(i);
    }
    while (q--) {
        int u, v; cin >> u >> v; 
        if (u == v) {
            cout << 0 << "\n";
        } else if (dsu.same(u, v)) {
            cout << query(u, v) << "\n";
        } else {
            cout << -1 << "\n";
        }
    }
}

signed main() {
    ios_base::sync_with_stdio(false); cin.tie(NULL);
    int tc = 1;
    // cin >> tc;
    for (int t = 1; t <= tc; t++) {
        // cout << "Case #" << t << ": ";
        solve(t);
    }
}