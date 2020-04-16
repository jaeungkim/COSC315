#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;
void extract(int n, int m, unsigned int v[]){
	int size = 5;
	int mask1 = 0;
	cout<<size<<endl;
	for(int i = 0; i < n; i++){			//creating a mask
		mask1 = mask1*2 + 1;
	}
	for(int i = 0; i < size ;i++){
		int originalV = v[i];
		int offset = v[i] & mask1;		//uses mask to extract
							//offset
		v[i] = v[i] >> m; //shifting v[i] by m;
		printf("Virtual address %d is in page number %d and offset %d\n",originalV,v[i],offset);				//display
	}	
	

}

int main(){
	int m = 8;
	int n = 8;
	unsigned int v[5]; 
	ifstream myfile;
	myfile.open("sampleInput.txt"); 	//reads a input file
	string line;
	int counter = 0;
	while (std::getline(myfile, line)){	//store them as ary of 
		int temp = 0;			//unsigned int array
		stringstream geek(line);
		geek >> temp;
		v[counter] = temp;
		counter++;
	}
	myfile.close();
	extract(m, n, v);			//extract function to extract
						//page number and offset
	return 0;
}
