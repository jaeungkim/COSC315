
g++ miniProjectPart1.cpp
./a.out

My program reads a line from the text file(sampleInput.txt)
and stores it in form of unsigned integer array. (unsigned int[]);
n and m variables is customized on the main function.

extract(int n, int m, unsigned int[] v) function 
creates a mask and the size of the mask is depended by
the size of n.

mask is used to extract offset from each virtual address (v[i])
and the virtual address get shifted right by m.

Shifted virtual address doesn't need to be masked as it only 
contains page number.

Extracted offset and page number is displayed as 
documented on the guideline.
