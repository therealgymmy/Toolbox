#include "../src/scope.h"

#include <iostream>

using namespace std;

class Object {
    int stuff[100];
};

void foo_exit() {
    cout << "----foo_exit----" << endl;

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
