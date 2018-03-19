#include <algorithm>
#include <cmath>
#include <complex>
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

const int base = 100;
const double PI = 3.141592653589793238463;

typedef complex<double> based;

vector<int> read(string num) {
    vector<int> lnum;
    for (int i = (int) num.length(); i > 0; i -= 2) {
        if (i < 2) {
            lnum.push_back(atoi(num.substr(0, i).c_str()));
        } else {
            lnum.push_back(atoi(num.substr(i - 2, 2).c_str()));
        }
    }
    return lnum;
}

vector<int> read(vector<int>& from) {
    vector<int> to;
    copy(from.begin(), from.end(), to.begin());
    return to;
}

vector<int> read(int x) {
    return read(to_string(x));
}

void print(ostream& fout, vector<int>& lnum) {
    fout << (lnum.empty() ? 0 : lnum.back());
    for (int i = (int) lnum.size() - 2; i >= 0; --i) {
        string x = to_string(lnum[i]);
        for (int j = 0; j + int(x.length()) < 2; ++j) {
            fout << "0";
        }
        fout << x;
    }
}

vector<int> sum(vector<int>& a, vector<int>& b) {
    vector<int> c;
    int carry = 0;
    for (size_t i = 0; i < max(a.size(), b.size()) || carry; ++i) {
        if (i == c.size()) {
            c.push_back(0);
        }
        c[i] += carry + ((i < b.size()) ? b[i] : 0);
        carry = c[i] / base;
        c[i] /= base;
    }
    while (c.size() > 1 && c.back() == 0) {
        c.pop_back();
    }
    return c;
}

vector<int> mult(vector<int>& a, vector<int>& b) {
    vector<int> c(a.size() + b.size() + 1, 0);
    for (size_t i = 0; i < a.size(); ++i) {
        int carry = 0;
        for (int j = 0; j < (int)b.size() || carry; ++j) {
            int cur = c[i+j] + a[i] * (j < (int)b.size() ? b[j] : 0) + carry;
            c[i+j] = (cur % base);
            carry = (cur / base);
        }
    }
    while (c.size() > 1 && c.back() == 0) {
        c.pop_back();
    }
    return c;
}

typedef complex<double> cd;
typedef vector<cd> vcd;

vcd fft(const vcd &as) {
    int n = as.size();
    int k = 0;
    while ((1 << k) < n) {
        k++;
    }
    vector<int> rev(n);
    rev[0] = 0;
    int high1 = -1;
    for (int i = 1; i < n; i++) {
        if ((i & (i - 1)) == 0) {
            high1++;
        }
        rev[i] = rev[i ^ (1 << high1)];
        rev[i] |= (1 << (k - high1 - 1));
    }

    vcd roots(n);
    for (int i = 0; i < n; i++) {
        double alpha = 2 * M_PI * i / n;
        roots[i] = cd(cos(alpha), sin(alpha));
    }

    vcd cur(n);
    for (int i = 0; i < n; i++)
        cur[i] = as[rev[i]];

    for (int len = 1; len < n; len <<= 1) {
        vcd ncur(n);
        int rstep = roots.size() / (len * 2);
        for (int pdest = 0; pdest < n;) {
            int p1 = pdest;
            for (int i = 0; i < len; i++) {
                cd val = roots[i * rstep] * cur[p1 + len];
                ncur[pdest] = cur[p1] + val;
                ncur[pdest + len] = cur[p1] - val;
                pdest++, p1++;
            }
            pdest += len;
        }
        cur.swap(ncur);
    }
    return cur;
}

vcd fft_rev(const vcd &as) {
    vcd res = fft(as);
    for (int i = 0; i < (int)res.size(); i++) {
        res[i] /= as.size();
    }
    reverse(res.begin() + 1, res.end());
    return res;
}

void multiply (const vector<int> & a, const vector<int> & b, vector<int> & res) {
    vcd fa (a.begin(), a.end()), fb (b.begin(), b.end());
    size_t n = 1;
    while (n < max (a.size(), b.size())) {
        n <<= 1;
    }
    n <<= 1;
    fa.resize (n),  fb.resize (n);

    fa = fft(fa);
    fb = fft(fb);
    for (size_t i=0; i<n; ++i) {
        fa[i] *= fb[i];
    }
    fa = fft_rev(fa);

    res.resize (n);
    for (size_t i=0; i<n; ++i) {
        res[i] = int (fa[i].real() + 0.5);
    }
    int carry = 0;
    for (size_t i=0; i<n; ++i) {
        res[i] += carry;
        carry = res[i] / base;
        res[i] %= base;
    }
    while (res.size() > 1 && res.back() == 0) {
        res.pop_back();
    }
}

vector<int> ProdTree(int l, int r) {
    if (l > r) {
        return read(1);
    }
    if (l == r) {
        return read(l);
    }
    if (r - l == 1) {
        auto a = read(l);
        auto b = read(r);
        vector<int> res;
        multiply(a, b, res);
        return res;
    }
    int m = (l + r) / 2;
    auto a = ProdTree(l, m);
    auto b = ProdTree(m + 1, r);
    vector<int> res;
    multiply(a, b, res);
    return res;
}

vector<int> FactTree(int n) {
    if (n < 0) {
        return read(0);
    }
    if (n == 0) {
        return read(1);
    }
    if (n == 1 || n == 2) {
        return read(n);
    }
    return ProdTree(2, n);
}

int main()
{
    ifstream fin("input.txt");
    ofstream fout("output.txt");
    int n;
    fin >> n;
    vector<int> ans = FactTree(n);
    print(fout, ans);
    fin.close();
    fout.close();
    return 0;
}
