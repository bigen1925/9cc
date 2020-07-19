#!/bin/bash
assert() {
    expected="$1"
    input="$2"
    
    ./9cc "${input}" "${debug_mode}" > tmp.s
    cc -o tmp tmp.s func.c
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

# assert 0 "main(){ 0; }"
# assert 42 "main(){ 42; }"
# assert 21 "main(){ 5+20-4; }"
# assert 41 "main(){ 12 + 34 - 5 ; }"
# assert 47 "main(){ 5 + 6*7 ; }"
# assert 15 "main(){ 5 * (9 - 6); }"
# assert 4 "main(){ (3 + 5) / 2; }"
# assert 5 "main(){ -3 + 8; }"
# assert 2 "main(){ 8 + -3*+2; }"
# assert 10 "main(){ - - +10; }"

# assert 1 "main(){ 3 == 3; }"
# assert 0 "main(){ 5 == 3; }"
# assert 1 "main(){ 5 != 3; }"
# assert 0 "main(){ 3 != 3; }"
# assert 1 "main(){ 2 < 3; }"
# assert 0 "main(){ 3 < 3; }"
# assert 1 "main(){ 3 <= 3; }"
# assert 0 "main(){ 4 <= 3; }"
# assert 1 "main(){ 4 > 3; }"
# assert 0 "main(){ 3 > 3; }"
# assert 1 "main(){ 3 >= 3; }"
# assert 0 "main(){ 2 >= 3; }"

assert 7 "main() {
    int foo;
    int bar;
    foo=4;
    bar=3;
    return foo+bar;
}
"
assert 5 "main() {
    return 5;
    return 8;
}"
assert 3 "main() {
    int return_var;
    return_var = 3;
    return return_var;
}"

assert 100 "main() {
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

assert 10 "main() {
    int a;
    a = 0;
    while (a < 10)
        a = a + 1;
    return a;
}"

assert 5 "main() {
    int a;
    a = 100;
    while (a > 5)
        a = a - 1;

    return a;
}"

assert 45 "main() {
    int sum;
    int i;
    sum = 0;
    for (i=0; i < 10; i = i + 1)
        sum = sum + i;
    return sum;
}"
assert 10 "main() {
    int i;
    i = 0;
    for (; i < 10; i = i + 1)
        i = i + 1;
    return i;
}"
assert 12 "main() {
    int sum;
    int i;
    sum = 0;
    for (i=3; sum < 10;)
        sum = sum + i;
    return sum;
}"

assert 50 "main() {
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
assert 15 "main() {
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
main() {
    return foo();
}

foo() {
    return 10;
}
"
assert 13 "main() {
    return foo(2, 11);
}

foo(x, y) {
    return x + y;
}
"

assert 55 "main() {
    return fib(10);
}

fib(n) {
    if (n == 1) return 1;
    if (n == 2) return 1;

    return fib(n-1) + fib(n-2);
}
"

assert 3 "main() {
    int x;
    int y;
    x = 3;
    y = *x;
    return &y;
}"

echo "OK"
