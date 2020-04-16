
#include <fstream>
#include <iostream>

using namespace std;

typedef int int32;

class myFileSystem
{
    public:
    
    string diskName;
    int Create(const char *name, int32 size);
    int Delete(const char *name);
    void ls();
    int Read(const char *name, int32 blockNum, char buf[1024]);
    int Write(const char *name, int32 blockNum, char buf[1024]);
    void loadDisk();
    void trashData();
    
    // superblcok
    public:
    
    char freeBlock[128];
    
    //Construct inodes
    
    struct Inode{
        string name;//file name
        int32 size;//file size(in terms of blocks)
        int32 blockPointers[8]; //direct pointers to block
        int32 used;//0 indicates its free, 1 indicates it is in use

    };
    
    Inode inodes[16];
    void sendInodes(int inode);
    
    //Got help from code on internet for ifstream  and offstream
    
    friend ifstream &operator>>(std::ifstream &, myFileSystem::Inode &);
    friend ofstream &operator<<(std::ofstream &, myFileSystem::Inode &);
    
};

//Got help from code on internet for ifstream  and offstream

ifstream &operator>>(ifstream &inn, myFileSystem::Inode &dest){
    char name[9];
    inn.read(name, 8);
    name[8] = 0;
    dest.name = (string) name;
    inn >> dest.size;
    for(int i =0;i<8;i++)
        inn >>dest.blockPointers[i];
    inn >> dest.used;
    
    return inn;
}

ofstream &operator<<(ofstream &onn, myFileSystem::Inode &src){
    onn.write(src.name.c_str(), 8);
    onn << src.size;
    for (int i =0; i < 8; i++)
        onn << src.blockPointers[i];
    onn <<src.used;
    
    return onn;
}
    
    void myFileSystem::sendInodes(int inode){
    ofstream diskWriter(diskName, ofstream::binary | ofstream::app);
    diskWriter.seekp(48*(inode) + 128);
    diskWriter << inodes[inode];
    diskWriter.close();
    }

    int myFileSystem::Create(const char *name,int32 size){
    
    //step1: look for a free inode
    
    string filename(name);
    int freeInode = 16;
    for(int i =0; i<16; i++)
        if(filename==inodes[i].name && inodes[i].used == 1){
        cout<< "File with same name exists! Error!\n";
        return false;
    } else if(inodes[i].used ==0){
        freeInode = i;
        break;
    }
    
    //step2: look for a number fof free blocks
    if(freeInode == 16){
        cout<< "No free Blocks!\n";
        return false;
    }
    
    //step3: mark the inode and blocks
    inodes[freeInode].name = filename;
    inodes[freeInode].size = size;
    inodes[freeInode].used = 1;
    
    //step4: write the entire super block back to disk
    
    for(int i = 0; i < size; i++)
        inodes[freeInode].blockPointers[i] = i * 127 + (freeInode + 1) *1024;
    sendInodes(freeInode);
    return 1;
    }
    
    int myFileSystem::Delete(const char *name){
    
    string filename(name);
    //Step 1: look for a inode with given name by seraching
    
    for(int i = 0; i < 16; i++)
        if(filename == inodes[i].name){
            //Step 2: Freeblocks of file being deleted
            cout << "Deleting file: " << filename<< endl;
            //Step 3: Mark inode as free
            inodes[i].used = 0;
            //Step 4: write superblock back to disk
            sendInodes(i);
            return 1;
    
    }//step 1: if it does not exist, then return an error
    cout<< "File is not found\n";
    return 0;
    }
    
    void myFileSystem::ls(){
    for(int i = 0; i < 16; i++){
        if(inodes[i].used == 1)
            cout << "file name: " + inodes[i].name <<endl;
    }
    
    }
    
    int myFileSystem::Read(const char *name, int32 blockNum, char buf[1024]){
    
    //step1: locate inode for this file
    
    string filename(name);
    for(int i = 0; i < 16; i++)
        if(filename == inodes[i].name){
            //blockNum does not exceed size - 1
            if(inodes[i].size < blockNum + 1){
                cout << "Invalid Block Number!\n";
                return false;
    }
    //step2: seek to blockpointers and read the blcok from disk to buf
            ifstream diskReader(diskName,ifstream::binary);
            diskReader.seekg(inodes[i].blockPointers[blockNum], diskReader.beg);
            diskReader.read(buf, 1024);
            diskReader.close();
            return true;
    }
    if(blockNum < 0 || blockNum > 7){
        cout << "Invalid Blcok Number!\n";
        return false;
        
    }
    else
        cout << "File not found!\n";
    
        return 0;
    }
    
    int myFileSystem::Write(const char *name, int32 blockNum, char buf[1024]){
       
    //step1: locate inode for this file

    string filename(name);
    for(int i = 0; i < 16 ;i++)
        if(filename == inodes[i].name){
            if(inodes[i].size <blockNum+1){
                cout << "Invalid Block Number!\n";
                return false;
    }
    
    //step2: seek to blockpointers and write buf to disk
    
            ofstream diskWriter(diskName, ofstream::binary | ofstream::app);
            diskWriter.seekp(inodes[i].blockPointers[blockNum], diskWriter.beg);
            diskWriter.write(buf,1024);
            diskWriter.close();
            return true;
    
    }
     if(blockNum < 0 || blockNum > 7){
           cout << "Invalid Blcok Number!\n";
           return false;
           
       }
       else
           cout << "File not found!\n";
       
           return 0;
       }
    
   // Load the Disk reading freeblock and inodes
    void myFileSystem::loadDisk(){
    ifstream diskReader(diskName);
    diskReader.read(freeBlock,128);
    for(int i = 0; i < 16; i++)
        diskReader >> inodes[i];
    diskReader.close();
    }
    
    //Trash the daata writing freeblock and inodes
    void myFileSystem::trashData(){
    ofstream diskWriter(diskName);
    diskWriter.write(freeBlock,128);
    for(int i = 0; i < 16; i++)
        diskWriter << inodes[i];
    diskWriter.close();
    }

    
    int main(int argc, char *argv[]){
    if(argc < 2) {
    cout << "Syntax: " << argv[0] << "input file" << endl;
        return 1;
    }
    ifstream input(argv[1]);
    if(!input.is_open()){
    cout << argv[1] << ": fill not found " << endl;
        return 1;
    }
    
    myFileSystem unix;
    input >> unix.diskName;
    unix.loadDisk();
    
    int blockNum;
    char buf[1024];
    string letter;
    string fileName;
    
    while(!input.eof()){
    input >> letter;
    
    if (letter == "L") {
        unix.ls();
        continue;
    }
    
    input >> fileName;
    
    if(letter == "D"){
        unix.Delete(fileName.c_str());
        continue;
    }
    
    input >> blockNum;
    
    if(letter == "C"){
        unix.Create(fileName.c_str(), blockNum);
    }
    if(letter == "W"){
        unix.Write(fileName.c_str(), blockNum,buf);
    }
    
    if(letter == "R"){
        unix.Write(fileName.c_str(), blockNum,buf);
    }
        
    }
    input.close();
    unix.trashData();
    return 0;
    }
