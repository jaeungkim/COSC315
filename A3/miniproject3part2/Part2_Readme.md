# COSC 315 - MiniProject 3

### Jae Ung Kim / Joseph Choi / Jason Yoon

#### Part II: File Systems

Tasks: write a simple UNIX-like file system with assumptions: 

1. The file system resides on a disk that is 128KB in size.
2. There is only oe root directory. No subdirectories are allowed.
3. The file System supports a maximum of 16 files.
4. The maximum size of a file is 8 blocks; each block is 1KB in size.
5. Each file has a unique name, the file name can be no longer than 8 characters.

6. Implement the following operations for our file system.

![](/Users/jaeungkim/Desktop/Screen Shot 2020-04-09 at 5.43.08 PM.png)



In order to run our program these steps are needed.

1. First, download all the required source codes. (create_fs, sample input.txt, etc.)
2. Run the create_fs file using your choice of compiler. We used VScode and it's C/C++ compiler extension to run our program.
3. "gcc lab3.create_fs.c -o createFsC" in terminal will create a executable createFsC file 
4. ./createFsC disk0 to create a file with the name
5. this will "format" our file system by initializing all blocks in the super block to be free and marking all 16 inodes to be free.
6. Next, we will run our file based made from template code.
7. We noticed the template code was somewhat explained in java format, so we converted it to C++ to run our program.
8. "g++ COSC315_main.cpp -o main2" will run our program. 
9. "./part2 lab3input.txt" will compile our file
10. please note that we referenced our methods based from internet, due to not having extensive knowledge of input/ouptut design of C++
