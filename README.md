# Design Document and Observation
- Created separate while loops for parallel and sequential executions.
- Used fork() and exec() to implement different executions
- parallel and sequential executions look almost identical but one line of code, due to it being almost similar but sequential requires parent processes to finish; while parallel doesn't. Only needed waitpid() in sequential to make it work.
- timeout is not ideally right as we got stuck on how to implement clock() and how it works 
- commented almost all lines of codes to make it clear what we are doing
- Please refer to photos for sample outputs

# REFERENCES

-  [https://github.com/qimingfang/closh/blob/master/closh.c](https://github.com/qimingfang/closh/blob/master/closh.c)

- [https://github.com/AkshitMurthy/CloneShell/blob/master/vas.c](https://github.com/AkshitMurthy/CloneShell/blob/master/vas.c)

  