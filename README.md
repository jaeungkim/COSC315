<<<<<<< HEAD
# Design Document and Observation
=======
# COSC315
- closh.c - COSC 315, Winter 2020
- ASSIGNMENT 1
- Jae Ung Kim (37007135)
- Deokhee Yoon (67262030)
- Joseph Choi (58178138)

Design Document and Observation
>>>>>>> b1e55ac10e99e5f8c2c0773b302d04ac977cf245

- Created separate while loops for parallel and sequential executions.
- Used fork() and exec() to implement different executions
- parallel and sequential executions look almost identical but one line of code, due to it being almost similar but sequential requires parent processes to finish; while parallel doesn't. Only needed waitpid() in sequential to make it work.
- timeout is not ideally right as we got stuck on how to implement clock() and how it works 
- commented almost all lines of codes to make it clear what we are doing
<<<<<<< HEAD
- Please refer to photos for sample outputs

# REFERENCES

-  [https://github.com/qimingfang/closh/blob/master/closh.c](https://github.com/qimingfang/closh/blob/master/closh.c)

- [https://github.com/AkshitMurthy/CloneShell/blob/master/vas.c](https://github.com/AkshitMurthy/CloneShell/blob/master/vas.c)

  
=======


- REFERENCES: [https://github.com/qimingfang/closh/blob/master/closh.c](https://github.com/qimingfang/closh/blob/master/closh.c)
- [https://github.com/AkshitMurthy/CloneShell/blob/master/vas.c](https://github.com/AkshitMurthy/CloneShell/blob/master/vas.c)

>>>>>>> b1e55ac10e99e5f8c2c0773b302d04ac977cf245
