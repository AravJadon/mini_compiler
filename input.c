#include<stdio.h>
int main(){

int base;
int exp;
int result;
int i;
int j;
int temp;
base = 3;
exp = 4;
result = 1;
j++;
i = 0;
while (i < exp) {
    temp = 0;
    j = 0;
    while (j < base) {

        if(i==0) return 0;
        temp = temp + result;
        j = j + 1;
    }
    result = temp;
    i = i + 1;
}
}
/* ============================================================
   TEST CASE H2: GCD using Subtraction Method
   Phase: All 4
   Expected: PASS
   Logic: Euclidean GCD without modulo - keep subtracting smaller
          from larger until equal
   TAC: two nested while loops with comparisons
   ============================================================ */

int a;
int b;
int gcd;
a = 48;
b = 18;
while (a != b) {
    if (a > b) {
        a = a - b;
    } else {
        b = b - a;
    }
}
gcd = a;




/* ============================================================
   TEST CASE H3: Bubble Sort Pass on 5 hardcoded values
   Phase: All 4
   Expected: PASS
   Logic: one full pass of bubble sort using if-else swaps
   TAC: 4 swap blocks each with 3 assignments + conditionals
   ============================================================ */

int a;
int b;
int c;
int d;
int e;
int temp;
a = 5;
b = 3;
c = 8;
d = 1;
e = 4;
if (a > b) {
    temp = a;
    a = b;
    b = temp;
} else {
    temp = 0;
}
if (b > c) {
    temp = b;
    b = c;
    c = temp;
} else {
    temp = 0;
}
if (c > d) {
    temp = c;
    c = d;
    d = temp;
} else {
    temp = 0;
}
if (d > e) {
    temp = d;
    d = e;
    e = temp;
} else {
    temp = 0;
}




/* ============================================================
   TEST CASE H4: Count Digits of a Number
   Phase: All 4
   Expected: PASS
   Logic: keep dividing by 10 until 0, count iterations
   TAC: while loop with division and comparison
   ============================================================ */

int n;
int count;
int temp;
n = 123456;
count = 0;
temp = n;
while (temp > 0) {
    temp = temp / 10;
    count = count + 1;
}




/* ============================================================
   TEST CASE H5: Sum of Digits
   Phase: All 4
   Expected: PASS
   Logic: extract last digit via n - (n/10)*10, accumulate
   TAC: loop with /, *, - chain to simulate modulo
   ============================================================ */

int n;
int sum;
int digit;
int temp;
int q;
n = 9876;
sum = 0;
temp = n;
while (temp > 0) {
    q = temp / 10;
    digit = temp - q * 10;
    sum = sum + digit;
    temp = q;
}




/* ============================================================
   TEST CASE H6: Fibonacci using Loop
   Phase: All 4
   Expected: PASS
   Logic: compute nth Fibonacci number iteratively
   TAC: loop with 3-variable rolling assignment
   ============================================================ */

int n;
int a;
int b;
int c;
int i;
n = 10;
a = 0;
b = 1;
i = 2;
while (i < n + 1) {
    c = a + b;
    a = b;
    b = c;
    i = i + 1;
}




/* ============================================================
   TEST CASE H7: Prime Check (Trial Division without modulo)
   Phase: All 4
   Expected: PASS
   Logic: for each divisor d from 2 to n-1, check if n = (n/d)*d
          if yes, not prime (isPrime = 0)
   TAC: nested loop logic with multiplication comparison
   ============================================================ */

int n;
int d;
int isPrime;
int q;
int prod;
n = 29;
isPrime = 1;
d = 2;
while (d < n) {
    q = n / d;
    prod = q * d;
    if (prod == n) {
        isPrime = 0;
    } else {
        isPrime = isPrime;
    }
    d = d + 1;
}




/* ============================================================
   TEST CASE H8: Running Min and Max in a Loop
   Phase: All 4
   Expected: PASS
   Logic: simulate array values using formula i*i - 5*i + 7
          track min and max across all values
   TAC: complex expression per iteration + two conditionals
   ============================================================ */

int i;
int val;
int minVal;
int maxVal;
int t1;
int t2;
int t3;
i = 1;
t1 = i * i;
t2 = 5 * i;
t3 = t1 - t2;
val = t3 + 7;
minVal = val;
maxVal = val;
i = 2;
while (i < 9) {
    t1 = i * i;
    t2 = 5 * i;
    t3 = t1 - t2;
    val = t3 + 7;
    if (val < minVal) {
        minVal = val;
    } else {
        minVal = minVal;
    }
    if (val > maxVal) {
        maxVal = val;
    } else {
        maxVal = maxVal;
    }
    i = i + 1;
}




/* ============================================================
   TEST CASE H9: Collatz Conjecture Steps
   Phase: All 4
   Expected: PASS
   Logic: if n is even: n = n/2, else n = 3*n+1, count steps
          even check: n - (n/2)*2 == 0
   TAC: complex if-else inside while, heavy expression use
   ============================================================ */

int n;
int steps;
int half;
int check;
n = 27;
steps = 0;
while (n != 1) {
    half = n / 2;
    check = n - half * 2;
    if (check == 0) {
        n = n / 2;
    } else {
        n = n * 3 + 1;
    }
    steps = steps + 1;
}




/* ============================================================
   TEST CASE H10: Triangle Number Classifier
   Phase: All 4
   Expected: PASS
   Logic: given sides a, b, c - classify triangle type
          equilateral, isosceles, or scalene
          also check valid triangle (sum of two sides > third)
   TAC: 4 chained if-else blocks
   ============================================================ */

int a;
int b;
int c;
int valid;
int kind;
a = 5;
b = 5;
c = 8;
valid = 0;
kind = 0;
if (a + b > c) {
    if (b + c > a) {
        if (a + c > b) {
            valid = 1;
        } else {
            valid = 0;
        }
    } else {
        valid = 0;
    }
} else {
    valid = 0;
}
if (valid == 1) {
    if (a == b) {
        if (b == c) {
            kind = 3;
        } else {
            kind = 2;
        }
    } else {
        if (b == c) {
            kind = 2;
        } else {
            if (a == c) {
                kind = 2;
            } else {
                kind = 1;
            }
        }
    }
} else {
    kind = 0;
}




/* ============================================================
   TEST CASE H11: Accumulate with Conditional Skip (no continue)
   Phase: All 4
   Expected: PASS
   Logic: sum all numbers 1-20 EXCEPT multiples of 3
          simulated by checking i - (i/3)*3 == 0
   TAC: loop + expression-based modulo + conditional add
   ============================================================ */

int i;
int sum;
int q;
int rem;
i = 1;
sum = 0;
while (i < 21) {
    q = i / 3;
    rem = i - q * 3;
    if (rem == 0) {
        sum = sum + 0;
    } else {
        sum = sum + i;
    }
    i = i + 1;
}




/* ============================================================
   TEST CASE H12: Expression with All Operators in One Statement
   Phase: All 4
   Expected: PASS - TAC generates ~8 temporaries for one line
   ============================================================ */

int a;
int b;
int c;
int d;
int result;
a = 10;
b = 3;
c = 2;
d = 4;
result = a * b + c * d - a / b + d - c * a;




/* ============================================================
   TEST CASE H13: Two Loops sharing variables, result fed into third
   Phase: All 4
   Expected: PASS - variable liveness across multiple loops in TAC
   ============================================================ */

int i;
int sumA;
int sumB;
int diff;
int product;
i = 1;
sumA = 0;
while (i < 6) {
    sumA = sumA + i * i;
    i = i + 1;
}
i = 1;
sumB = 0;
while (i < 6) {
    sumB = sumB + i;
    i = i + 1;
}
diff = sumA - sumB * sumB;
if (diff > 0) {
    product = diff * sumA;
} else {
    product = diff * sumB;
}




/* ============================================================
   TEST CASE H14: Nested Loop - Multiplication Table Check
   Phase: All 4
   Expected: PASS - finds all pairs where i*j == 24
   ============================================================ */

int i;
int j;
int found;
int fi;
int fj;
found = 0;
i = 1;
while (i < 25) {
    j = 1;
    while (j < 25) {
        if (i * j == 24) {
            if (found == 0) {
                fi = i;
                fj = j;
                found = found + 1;
            } else {
                found = found + 1;
            }
        } else {
            found = found;
        }
        j = j + 1;
    }
    i = i + 1;
}




/* ============================================================
   TEST CASE H15: Long Arithmetic Expression Stress Test
   Phase: All 4
   Expected: PASS - single statement expands to 15+ TAC lines
   ============================================================ */

int a;
int b;
int c;
int d;
int e;
int f;
int g;
int result;
a = 1;
b = 2;
c = 3;
d = 4;
e = 5;
f = 6;
g = 7;
result = a + b * c - d * e + f * g - a * b + c - d + e * f * g;




/* ----------------------------------------------------------------
   ================================================================
   HARD ERROR TEST CASES - TRICKY ONES
   ================================================================
   ---------------------------------------------------------------- */


/* ============================================================
   TEST CASE HE1: [SEMANTIC ERROR] Undeclared var used as loop bound
   Phase: Semantic
   Expected: ERROR - 'n' not declared, used in while condition
   Tricky: all other vars declared correctly, only n is missing
   ============================================================ */

int i;
int sum;
i = 0;
sum = 0;
while (i < n) {
    sum = sum + i;
    i = i + 1;
}




/* ============================================================
   TEST CASE HE2: [SEMANTIC ERROR] Undeclared var on RHS of complex expr
   Phase: Semantic
   Expected: ERROR - 'base' not declared
   Tricky: buried deep inside a multi-operator expression
   ============================================================ */

int result;
int exp;
int i;
result = 1;
exp = 3;
i = 0;
while (i < exp) {
    result = result * base;
    i = i + 1;
}




/* ============================================================
   TEST CASE HE3: [SEMANTIC ERROR] Undeclared on LHS inside nested if
   Phase: Semantic
   Expected: ERROR - 'output' not declared, assigned inside nested if
   Tricky: declared vars used everywhere else, only output missing
   ============================================================ */

int score;
int bonus;
score = 80;
bonus = 10;
if (score > 70) {
    if (bonus > 5) {
        output = score + bonus;
    } else {
        output = score;
    }
} else {
    output = 0;
}




/* ============================================================
   TEST CASE HE4: [SEMANTIC ERROR] Variable declared AFTER use
   Phase: Semantic
   Expected: ERROR - 'total' used before declaration
   Tricky: declaration comes later in the code, not before use
   ============================================================ */

int i;
i = 0;
while (i < 5) {
    total = total + i;
    i = i + 1;
}
int total;
total = 0;




/* ============================================================
   TEST CASE HE5: [SEMANTIC ERROR] Duplicate declaration deep in loop
   Phase: Semantic
   Expected: ERROR - 'temp' declared twice (second one inside loop)
   Tricky: first declaration is valid, second is inside while body
   ============================================================ */

int i;
int temp;
i = 0;
temp = 0;
while (i < 5) {
    int temp;
    temp = i * i;
    i = i + 1;
}




/* ============================================================
   TEST CASE HE6: [SYNTAX ERROR] Assignment inside condition
   Phase: Syntax
   Expected: ERROR - '=' used instead of '==' in while condition
   Tricky: easy to miss, common real-world mistake
   ============================================================ */

int x;
x = 5;
while (x = 0) {
    x = x - 1;
}




/* ============================================================
   TEST CASE HE7: [SYNTAX ERROR] Missing operator in expression
   Phase: Syntax
   Expected: ERROR - two identifiers adjacent with no operator
   ============================================================ */

int a;
int b;
int c;
a = 5;
b = 3;
c = a b;




/* ============================================================
   TEST CASE HE8: [SYNTAX ERROR] if with no body braces
   Phase: Syntax
   Expected: ERROR - missing '{' after if condition
   ============================================================ */

int x;
x = 10;
if (x > 5)
    x = 1;
else {
    x = 0;
}




/* ============================================================
   TEST CASE HE9: [LEXICAL ERROR] Float literal (not supported)
   Phase: Lexical
   Expected: ERROR - '3.14' is not a valid token in Toy-C (no float)
   ============================================================ */

int x;
x = 3.14;




/* ============================================================
   TEST CASE HE10: [LEXICAL + SEMANTIC] Double error in one program
   Phase: Lexical first, then Semantic
   Expected: LEXICAL ERROR on '#' symbol first
   Tricky: even if lexer skips it, 'y' is also undeclared
   ============================================================ */

int x;
x = 5;
x = x + #y;



}

