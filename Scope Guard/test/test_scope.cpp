#include "../src/scope.h"
#include "../src/scope_container.h"

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <utility>

using namespace std;

struct Object {
    int stuff[100];
};

void foo_exit() {
    cout << "----foo_exit----" << endl;

    {
        vector<int*> stuff;
        auto testguard = makeScopeGuardContainer(stuff);
    }

    {
        vector<Object*> stuff;
        auto testguard = makeScopeGuardContainer(stuff);
    }

    {
        map<Object*, Object*> stuff2;
        auto testguard2 = makeScopeGuardContainer(stuff2);
    }

    {
        map<int*, int> stuff3;
        auto testguard3 = makeScopeGuardContainer(stuff3);
    }

    {
        map<int, int*> stuff3;
        auto testguard3 = makeScopeGuardContainer(stuff3);
    }

    {
        set<Object*> stuff;
        auto testguard = makeScopeGuardContainerCustom(stuff,
                [](set<Object*>::iterator itr) { delete *itr; });
    }

    {
        map<Object*, int*> stuff;
        auto testguard = makeScopeGuardContainerCustom(stuff,
                [](map<Object*, int*>::const_iterator p) { delete p->first; delete p->second; });
    }

    map<Object*, int*> s1;
    vector<Object*> s2;
    set<Object*> s3;
    map<int*, pair<int*, Object*>> s4;

    scope(exit, {
        cout << "exit" << endl;

        release_container(s1);

        release_container(s2);

        release_container_with_deleter(s3, itr, {
            delete *itr;
        });

        release_container_with_deleter(s4, itr, {
            delete itr->first;
            delete itr->second.first;
            delete itr->second.second;
        });
    });
    scope(success, {
        cout << "success" << endl;
    });
    scope(failure, {
        cout << "failure" << endl;
    });

    int *t1 = new int;
    Object *t2 = new Object;
    t2->stuff[5] = 0;

    int *t3 = new int;

    s4[t1] = make_pair(new int, new Object);
    s4[t3] = make_pair(new int, t2);
}

void foo_success() {
    cout << "----foo_success----" << endl;

    scope(exit, {
        cout << "exit" << endl;
    });
    scope(success, {
        cout << "success" << endl;
    });
    scope(failure, {
        cout << "failure" << endl;
    });
}

void foo_failure() {
    cout << "----foo_failure----" << endl;

    scope(exit, {
        cout << "exit" << endl;
    });
    scope(success, {
        cout << "success" << endl;
    });
    scope(failure, {
        cout << "failure" << endl;
    });

    throw "error";
}

int main() {

    foo_exit();
    foo_success();

    try {
        foo_failure();
    }
    catch (...) {
        cout << "exception caught" << endl;
    }
}
