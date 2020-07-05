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

assert 0 "0;"
assert 42 "42;"
assert 21 "5+20-4;"
assert 41 " 12 + 34 - 5 ;"
assert 47 "5 + 6*7 ;"
assert 15 "5 * (9 - 6);"
assert 4 "(3 + 5) / 2;"
assert 5 "-3 + 8;"
assert 2 "8 + -3*+2;"
assert 10 "- - +10;"

assert 1 "3 == 3;"
assert 0 "5 == 3;"
assert 1 "5 != 3;"
assert 0 "3 != 3;"
assert 1 "2 < 3;"
assert 0 "3 < 3;"
assert 1 "3 <= 3;"
assert 0 "4 <= 3;"
assert 1 "4 > 3;"
assert 0 "3 > 3;"
assert 1 "3 >= 3;"
assert 0 "2 >= 3;"

assert 7 "foo=4;bar=3;foo+bar;"

assert 5 "return 5; return 8;"
assert 3 "return_var = 3; return return_var;"

assert 100 "
a = 10;
if (a < 0)
    return 0;
else if (a < 5)
    return 5;
else
    return 100;

return 127;
"

assert 10 "
a = 0;
while (a < 10)
    a = a + 1;
return a;
"

assert 5 "
a = 100;
while (a > 5)
    a = a - 1;

return a;
"

assert 45 "
sum = 0;
for (i=0; i < 10; i = i + 1)
    sum = sum + i;
return sum;
"
assert 10 "
i = 0;
for (; i < 10; i = i + 1)
    i = i + 1;
return i;
"
assert 12 "
sum = 0;
for (i=3; sum < 10;)
    sum = sum + i;
return sum;
"

assert 50 "
foo = 0;
bar = 0;
for (i=0; i < 10; i = i+1) {
    foo = foo + 2;
    bar = bar + 3;
}
return foo + bar;
"

assert 15 "
foo = 0;
bar = 0;
for (;;) {
    foo = foo + 2;
    bar = bar + 3;
    if (foo + bar > 13) {
        return foo + bar;
    }
}
"

assert 10 "
foo();
return 10;
"

assert 10 "
return bar(3, 7);
"

echo "OK"
