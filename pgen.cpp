#ifdef ONPC
    #define _GLIBCXX_DEBUG
#endif
#include <bits/stdc++.h>

#ifndef ONPC
    #pragma GCC optimize("Ofast")
    #pragma GCC target("avx2")
    #pragma GCC target("popcnt")

    #define cerr if (false) cerr
#endif

#define all(v) v.begin(), v.end()

using namespace std;

typedef long long ll;
typedef unsigned long long ull;

typedef pair<int, int> Pint;
typedef pair<ll, ll> Pll;

mt19937_64 gen64(chrono::steady_clock::now().time_since_epoch().count());
inline ll rnd(ll l, ll r) {
    return uniform_int_distribution<ll>(l, r)(gen64);
}

void solve() {
    double step;
    cin >> step;
    string file;
    cin >> file;
    
    auto sq = [](auto x) {
        return x * x;
    };

    auto f = [&](double x, double y) {
        return sq(x) + sq(y - sqrt(abs(x)));
    };

    auto findY = [&](double x) {
        // x**2 + y**2 - 2 * sqrt(abs(x)) * y + x ** 2 = 1
        // y**2 - 2 * sqrt(abs(x)) * y + (2 * x**2 - 1) = 0
        double a = 1.0, b = -2 * sqrt(abs(x)), c = 2 * sq(x) - 1;
        double d = sq(b) - 4 * a * c;
        pair<double, double> ans;
        if (d < 0) {
            ans = {-100, -100};   
        } else {
            ans = {
                (-b - sqrt(d)) / (2 * a),
                (-b + sqrt(d)) / (2 * a)
            };
        }
        return ans;
    };

    vector<pair<double, double>> pts, t;
    for (double x = -1.0; x <= 1.0; x += step) {
        auto [y1, y2] = findY(x);
        if (y1 != -100 && y2 != -100) {
            pts.push_back({x, y1});
            pts.push_back({x, y2});
        }
        
        // for (double y = -2.0; y <= 2.0; y += step) {
        //     if (abs(f(x, y) - 1) <= 1e-6) {
        //         pts.push_back({x, y});
        //     }
        // }
    }
    sort(all(pts), [](pair<double, double> a, pair<double, double> b) {
        return atan2(a.second, a.first) < atan2(b.second, b.first);
    });

    int idx = 0;
    double pi = acos(-1);
    for (int i = 0; i < pts.size(); ++i) {
        if (atan2(pts[i].second, pts[i].first) >= pi / 2) {
            idx = i;
            break;
        }
    }

    t.reserve(pts.size());
    for (int i = idx; i < pts.size(); ++i) {
        t.push_back(pts[i]);
    }
    for (int i = 0; i < idx; ++i) {
        t.push_back(pts[i]);
    }
    swap(t, pts);

    cout << pts.size() << '\n';
    ofstream out(file);
    for (auto [x, y] : pts) {
        out << "\t{" << x << ", " << y << "},\n";
    }
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 1;
    // cin >> t;
    while (t--) {
        solve();
    }

    return 0;
}