#!/bin/bash
assert() {
    expected="$1"
    input="$2"
    
    ./9cc "${input}" "${debug_mode}" > tmp.s
    cc -o tmp tmp.s helper.o
    ./tmp
    
    actual="$?"
    
    if [ "${actual}" = "${expected}" ];then
        echo "${input} => ${actual}"
    else
        echo "${input} => ${expected} expected, but got ${actual}"
        exit 1
    fi
}

debug_mode="$1"

assert 0 "int main(){ 0; }"
assert 42 "int main(){ 42; }"
assert 21 "int main(){ 5+20-4; }"
assert 41 "int main(){ 12 + 34 - 5 ; }"
assert 47 "int main(){ 5 + 6*7 ; }"
assert 15 "int main(){ 5 * (9 - 6); }"
assert 4 "int main(){ (3 + 5) / 2; }"
assert 5 "int main(){ -3 + 8; }"
assert 2 "int main(){ 8 + -3*+2; }"
assert 10 "int main(){ - - +10; }"

assert 1 "int main(){ 3 == 3; }"
assert 0 "int main(){ 5 == 3; }"
assert 1 "int main(){ 5 != 3; }"
assert 0 "int main(){ 3 != 3; }"
assert 1 "int main(){ 2 < 3; }"
assert 0 "int main(){ 3 < 3; }"
assert 1 "int main(){ 3 <= 3; }"
assert 0 "int main(){ 4 <= 3; }"
assert 1 "int main(){ 4 > 3; }"
assert 0 "int main(){ 3 > 3; }"
assert 1 "int main(){ 3 >= 3; }"
assert 0 "int main(){ 2 >= 3; }"

assert 7 "int main() {
    int foo;
    int bar;
    foo=4;
    bar=3;
    return foo+bar;
}
"
assert 5 "int main() {
    return 5;
    return 8;
}"
assert 3 "int main() {
    int return_var;
    return_var = 3;
    return return_var;
}"

assert 100 "int main() {
    int a;
    a = 10;
    if (a < 0)
        return 0;
    else if (a < 5)
        return 5;
    else
        return 100;

    return 127;
}"

assert 10 "int main() {
    int a;
    a = 0;
    while (a < 10)
        a = a + 1;
    return a;
}"

assert 5 "int main() {
    int a;
    a = 100;
    while (a > 5)
        a = a - 1;

    return a;
}"

assert 45 "int main() {
    int sum;
    int i;
    sum = 0;
    for (i=0; i < 10; i = i + 1)
        sum = sum + i;
    return sum;
}"
assert 10 "int main() {
    int i;
    i = 0;
    for (; i < 10; i = i + 1)
        i = i + 1;
    return i;
}"
assert 12 "int main() {
    int sum;
    int i;
    sum = 0;
    for (i=3; sum < 10;)
        sum = sum + i;
    return sum;
}"

assert 50 "int main() {
    int foo;
    int bar;
    int i;
    foo = 0;
    bar = 0;
    for (i=0; i < 10; i = i+1) {
        foo = foo + 2;
        bar = bar + 3;
    }
    return foo + bar;
}
"
assert 15 "int main() {
    int foo;
    int bar;
    foo = 0;
    bar = 0;
    for (;;) {
        foo = foo + 2;
        bar = bar + 3;
        if (foo + bar > 13) {
            return foo + bar;
        }
    }
}"

assert 10 "
int main() {
    return foo();
}

int foo() {
    return 10;
}
"
assert 13 "int main() {
    return foo(2, 11);
}

int foo(int x, int y) {
    return x + y;
}
"

assert 55 "int main() {
    return fib(10);
}

int fib(int n) {
    if (n == 1) return 1;
    if (n == 2) return 1;

    return fib(n-1) + fib(n-2);
}
"

assert 3 "int main() {
    int x;
    int *y;
    x = 3;
    y = &x;
    return *y;
}"

assert 3 "int main() {
    int x;
    int *y;
    int **z;
    z = &y;
    y = &x;
    **z = 3;
    return x;
}"

assert 0 "int main() {
    int *p;
    alloc4(&p, 9, 10, 11, 12);
    int *q;

    q = p + 2;
    if (*q != 11) {
        return 1;
    }

    q = p + 3;
    if (*q != 12) {
        return 2;
    }
    return 0;
}"

echo "OK"
