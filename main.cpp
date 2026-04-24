#include <bits/stdc++.h>
using namespace std;

using int64 = long long;

static const int DP_THRESHOLD = 400; // Use exact DP up to this n for performance

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];

    if (n == 0) {
        cout << 0 << '\n';
        return 0;
    }

    // Exact interval DP (Burst Balloons style) with virtual 1 at both ends
    if (n <= DP_THRESHOLD) {
        vector<long long> b(n + 2, 1);
        for (int i = 0; i < n; ++i) b[i + 1] = a[i];
        int m = n + 2;
        vector<vector<long long>> dp(m, vector<long long>(m, 0));
        for (int len = 2; len < m; ++len) { // interval [i, j]
            for (int i = 0; i + len < m; ++i) {
                int j = i + len;
                long long best = 0;
                for (int k = i + 1; k <= j - 1; ++k) {
                    long long val = dp[i][k] + dp[k][j] + b[i] * b[k] * b[j];
                    if (val > best) best = val;
                }
                dp[i][j] = best;
            }
        }
        cout << dp[0][m - 1] << '\n';
        return 0;
    }

    // Heuristic fallback for large n to avoid TLE: burst from smallest inward.
    // Maintain a doubly linked list of alive indices and a min-heap keyed by value (with tie-breakers).
    struct Node { int l, r; bool alive; };
    vector<Node> node(n);
    for (int i = 0; i < n; ++i) {
        node[i].l = i - 1;
        node[i].r = i + 1;
        node[i].alive = true;
    }
    node[0].l = -1; node[n-1].r = -1;

    struct Entry {
        long long val; // a[i]
        int idx;
        long long version;
        bool operator<(Entry const& other) const {
            if (val != other.val) return val > other.val; // min-heap by value
            return idx > other.idx;
        }
    };

    vector<long long> vers(n, 0);
    priority_queue<Entry> pq;
    for (int i = 0; i < n; ++i) pq.push({a[i], i, vers[i]});

    long long ans = 0;
    int alive_cnt = n;
    auto contrib = [&](int i) -> long long {
        int L = node[i].l, R = node[i].r;
        if (L == -1 && R == -1) return a[i];
        if (L == -1) return a[i] * a[R];
        if (R == -1) return a[L] * a[i];
        return a[L] * a[i] * a[R];
    };

    while (alive_cnt > 0) {
        // If only one remains, add it and break
        if (alive_cnt == 1) {
            int last = -1;
            for (int i = 0; i < n; ++i) if (node[i].alive) { last = i; break; }
            ans += a[last];
            break;
        }

        // Pop the smallest a[i] still alive
        Entry e;
        do {
            if (pq.empty()) break;
            e = pq.top(); pq.pop();
        } while ((e.idx < 0 || e.idx >= n) || (!node[e.idx].alive) || (vers[e.idx] != e.version));
        int i = e.idx;
        if (!node[i].alive) continue;

        // Add its current contribution and remove it
        ans += contrib(i);
        int L = node[i].l, R = node[i].r;
        node[i].alive = false;
        --alive_cnt;
        if (L != -1) { node[L].r = R; vers[L]++; pq.push({a[L], L, vers[L]}); }
        if (R != -1) { node[R].l = L; vers[R]++; pq.push({a[R], R, vers[R]}); }
    }

    cout << ans << '\n';
    return 0;
}

