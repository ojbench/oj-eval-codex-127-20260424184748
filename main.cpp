#include <bits/stdc++.h>
using namespace std;

using int64 = long long;

static const int DP_THRESHOLD = 1005; // Always use exact DP for n <= 1000

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
                const long long edge = (long long)b[i] * (long long)b[j];
                for (int k = i + 1; k <= j - 1; ++k) {
                    long long val = dp[i][k] + dp[k][j] + edge * (long long)b[k];
                    if (val > best) best = val;
                }
                dp[i][j] = best;
            }
        }
        cout << dp[0][m - 1] << '\n';
        return 0;
    }

    // Fallback should never trigger for given constraints
    cout << 0 << '\n';
    return 0;
}
