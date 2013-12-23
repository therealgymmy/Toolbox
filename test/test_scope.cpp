#include "../src/scope.h"

#include <iostream>
#include <vector>
#include <map>

using namespace std;

class Object {
    int stuff[100];
};

void foo_exit() {
    cout << "----foo_exit----" << endl;

    {
        vector<int*> stuff;
        auto testguard = makeScopeGuardContainer(stuff);
    }

    {
        map<int*, int*> stuff2;
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

    cout << AT << endl;

    try {
        foo_exit();
        foo_success();
        foo_failure();
    }
    catch (...) {
        cout << "exception caught" << endl;
    }
}
