#ifndef FFT_HPP
#define FFT_HPP

#include <vector>
using std::vector;
#include <complex>
using std::complex;
typedef complex<double> cd;

const double pi = std::acos(-1);

void fft(vector<cd> &a, bool inv) {
    int n = a.size(), logn = std::__lg(n);
    for (int i = 0; i < n; ++i) {
        int j = 0;
        for (int bit = 0; bit < logn; ++bit) {
            j |= (i >> bit & 1) << (logn - bit - 1);
        }
        if (i < j) {
            swap(a[i], a[j]);
        }
    }

    for (int len = 2; len <= n; len <<= 1) {
        int k = len >> 1;
        cd wlen = std::polar(1.0, (inv ? -1 : 1) * 2.0 * pi / len);
        for (int i = 0; i < n; i += len) {
            cd w = 1.0f;
            for (int j = 0; j < k; ++j) {
                cd u = a[i + j], v = w * a[i + j + k];
                a[i + j] = u + v;
                a[i + j + k] = u - v;
                w *= wlen;
            }
        }
    }

    if (inv) {
        for (auto &z : a) {
            z /= n;
        }
    }
}

vector<cd> mult(const vector<cd> &a, const vector<cd> &b) {
    auto ffa = a, ffb = b;
    int n = 1;
    while (n < ffa.size() + ffb.size()) {
        n <<= 1;
    }
    ffa.resize(n); ffb.resize(n);
    fft(ffa, false); fft(ffb, false);
    for (int i = 0; i < n; ++i) {
        ffa[i] *= ffb[i];
    }
    fft(ffa, true);
    return ffa;
}

vector<cd> czt(const vector<cd> &a, bool inv) {
    int n = a.size();
    vector<cd> x(n), y(2 * n);
    for (int i = 0; i < n; ++i) {
        y[n - i] = y[n + i] = std::polar(1.0, (inv ? 1 : -1) * pi * i * i / n);
    }
    for (int i = 0; i < n; ++i) {
        x[i] = a[i] * std::conj(y[n + i]);
    }

    vector<cd> _ft = mult(x, y), ft(n);
    for (int i = 0; i < n; ++i) {
        ft[i] = _ft[n + i] * std::conj(y[n + i]);
        if (inv) {
            ft[i] /= n;
        }
    }

    return ft;
}

struct epicycle {
    double r, phi, v;
};

vector<epicycle> convert(const vector<cd> &a) {
    int n = a.size();
    vector<epicycle> e(n);
    for (int i = 0; i < n; ++i) {
        e[i] = {
            abs(a[i]),
            std::atan2(a[i].imag(), a[i].real()),
            (float)i,
        };
    }

    return e;
}

#endif