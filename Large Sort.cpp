#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

int MakePartition(int memory) {
    ifstream in("input.txt");
    string s;
    vector<string> cur;
    int cnt = 0;
    long long name = 0;
    while (getline(in, s)) {
        ++cnt;
        cur.push_back(s);
        if (cnt == memory) {
            ofstream out(to_string(name) + ".txt");
            if (cur.size() > 1) {
                sort(cur.begin(), cur.end());
            }
            for (size_t i = 0; i < cur.size(); ++i) {
                out << cur[i];
                if (i + 1 < cur.size()) {
                    out << endl;
                }
            }
            out.close();
            cnt = 0;
            cur.clear();
            ++name;
        }
    }
    if (cnt) {
        if (cur.size() > 1) {
            sort(cur.begin(), cur.end());
        }
        ofstream out(to_string(name) + ".txt");
        for (size_t i = 0; i < cur.size(); ++i) {
            out << cur[i] << endl;
        }
        out.close();
        cnt = 0;
        cur.clear();
        ++name;
    }
    in.close();
    return name;
}

void Merge(long long LastName, long long range, long long FromName = 0) {
    long long rangex = min(range, LastName - FromName);
    vector<ifstream> x(rangex);
    for (int i = 0; i < rangex; ++i) {
        x[i].open(to_string(FromName++) + ".txt");
    }
    vector<bool> used(rangex, false);
    long long wend = 0;
    vector<string> strings(rangex);
    for (int i = 0; i < rangex; ++i) {
        while (!getline(x[i], strings[i])) {
            x[i].close();
            if (FromName <= LastName) {
                x[i].open(to_string(FromName++) + ".txt");
            } else {
                used[i] = true;
                ++wend;
                break;
            }
        }
    }
    ofstream out("output.txt");
    int minx = 0;
    while (wend < rangex) {
        for (int i = 0; i < rangex; ++i) {
            if (!used[i]) {
                minx = i;
                break;
            }
        }
        for (int i = minx; i < rangex; ++i) {
            if (!used[i] && strings[i] < strings[minx]) {
                minx = i;
            }
        }
        out << strings[minx] << endl;
        while (!getline(x[minx], strings[minx])) {
            x[minx].close();
            if (FromName <= LastName) {
                x[minx].open(to_string(FromName++) + ".txt");
            } else {
                used[minx] = true;
                ++wend;
                break;
            }
        }
    }
    out.close();
    for (long long i = 0; i <= LastName; ++i) {
        remove((to_string(i) + ".txt").c_str());
    }
}


int main()
{
    ifstream mem("memory.txt");
    int memory;
    mem >> memory;
    mem.close();
    long long LastName = MakePartition((3 * memory) / 4);
    Merge(LastName, (3 * memory) / 4);
    return 0;
}
