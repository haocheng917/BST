#include <bits/stdc++.h>
using namespace std;
int n, x, y, m, q;
struct Point {
    double x, y;
    bool canDel = true;
    Point() = default;
    Point(double x, double y): x(x), y(y) {}
};
bool operator <(Point a, Point b) {
    return a.x == b.x ? a.y < b.y : a.x < b.y;
}
set<Point> S;
void init() {
    S.insert({0, 0, false});
    S.insert({x, y, false});
    S.insert({0, n, false});
    
}
int main() {
    cin >> n >> x >> y;
    init();
    cin >> m;
    for (int i = 1; i <= m; i++) {
        cin >> a[i].x >> a[i].y;
    }
    cin >> q;
    for (int i = 1; i <= q; i++) {

    }
    return 0;
}