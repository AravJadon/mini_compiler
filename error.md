### 


-> printf scanf to be added
-> syntax tree in the op of phase2: convert syntax tree to ast
-> test11: local and global scope declaration of same variable not being handled see int a line 1 and line3.
-> test16: register allocation is incorrect like overwritten
-> test19: variable reduction logical error, in code optimistion phase:as in if (10 > 20) is false but still the goto is poting to the if code block and not to else
-> test20: switch case not directly jumping to desired label but starting from very first label
-> test24: variable local and global scope redefinition of same variables throwing errors
-> test25: if(a+b){body} // if (a+b);

To Check:
-> are all logical operators part of the lexing, test10 && ig missing

new errors:
-> hard2: loop invariant propagation not happening recursively up the rec tree(here rec tree == nested loops)
-> hard3: was based on cse but 0 cse done rather constant propagation done 13 times or so.


