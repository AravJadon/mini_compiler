int global_var;



int main() {
    /* 1. Declarations and Basic Assignments */
    int a;
    int b = 10;
    int c = 20;
    int d;

    /* 2. Arithmetic and Compound assignments */
    d = a + b * c - (b / 2) % 3;
    d += 5;
    d -= 2;
    d *= 3;
    d /= 2;
    d %= 4;

    /* 3. Increment and Decrement Statements */
    ++a;
    b++;
    --c;
    d--;

    /* 4. Boolean and Relational Expressions */
    int truthy;
    int falsy;
    
    /* Notice the boolean assignment rule in parser.y: ID ASSIGN LPAREN rel_bool RPAREN */
    truthy = (a < b);
    falsy = !(a > b);

    if (a <= 10 && b >= 5 || !(c == 20)) {
        d = 1;
    } else {
        d = 0;
    }

    /* 5. Loops: while and do-while */
    int count = 0;
    while (count < 10) {
        count++;
        if (count == 5) {
            continue;
        }
        if (count == 8) {
            break;
        }
    }

    do {
        count--;
    } while (count > 0);

    /* 6. Loops: for loop 
       Note: According to parser.y, the third condition of for-loop requires a rel_bool. 
       Syntax: FOR LPAREN decl_stmt SEMI M bexpr SEMI M rel_bool N RPAREN M matched_stmt 
    */
    for (int i = 0; i < 5; i > 0) {
        printf("Loop running");
    }

    /* 7. Switch-Case Statement */
    int choice = 2;
    switch(choice) {
        case 1: {
            a = 100;
        }
        case 2: {
            a = 200;
        }
        default: {
            a = 0;
        }
    }

    /* 8. I/O Functions */
    int input_val;
    printf("Enter value:");
    scanf("%d", &input_val);
    printf("You entered:", input_val);

    /* 9. Function Calls */
    int result;

    return 0;
}
