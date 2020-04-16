/* C++ program to simulate a simple UNIX-like file system */

#include <iostream>
#include <fstream>
using namespace std;

typedef int int32; // 4-byte int
typedef char byte; // alias char as byte

class FileSystem {
public:
    void LoadDisk();
    void DumpData();
    bool Create(const char *name, int32 size);
    bool Delete(const char *name);
    void ReadBlock(int32 blockPtr, char *buf);
    void WriteBlock(int32 blockPtr, const char *buf);
    bool Read(const char *name, int32 blockNum, char *buf);
    bool Write(const char *name, int32 blockNum, const char *buf);
    void ls();

    string DISK_NAME;

private:
    byte free_block[128]; 
    struct Inode {
        string name;            // file name
        int32 size;             // file size (in number of blocks)
        int32 blockPointers[8]; // direct block pointers
        int32 used;             // 0 => inode is free; 1 => in use
    };

    Inode inodes[16];
    void CommitToDisk(int inode);
    friend ifstream &operator>>(std::ifstream &, FileSystem::Inode &);
    friend ofstream &operator<<(std::ofstream &, FileSystem::Inode &);
};

ifstream &operator>>(ifstream &inn, FileSystem::Inode &dest) {
    char name[9];
    inn.read(name, 8);
    name[8] = 0;
    dest.name = string(name);
    inn >> dest.size;
    for (int i = 0; i < 8; i++)
        inn >> dest.blockPointers[i];
    inn >> dest.used;
    return inn;
}

ofstream &operator<<(ofstream &onn, FileSystem::Inode &src) {
    onn.write(src.name.c_str(), 8);
    onn << src.size;
    for (int i = 0; i < 8; i++)
        onn << src.blockPointers[i];
    onn << src.used;
    return onn;
}

void FileSystem::LoadDisk() {
    ifstream diskReader(DISK_NAME);

    
    diskReader.read(free_block, 128);
   
    for (int i = 0; i < 16; i++)
        diskReader >> inodes[i];

    diskReader.close();
}

void FileSystem::DumpData() {
    ofstream diskWriter(DISK_NAME);


    diskWriter.write(free_block, 128);
    
    for (int i = 0; i < 16; i++)
        diskWriter << inodes[i];

    diskWriter.close();
}

bool FileSystem::Create(const char *name, int32 size) {
    if (size < 1 || size > 8) {
        cout << "Invalid block size specified!\n";
        return false;
    }

    string filename(name);
    int chosenInode = 16;
    for (int i = 0; i < 16; i++)
        if (inodes[i].used == 1 && filename == inodes[i].name) {
            cout << "A file by name " << filename << " already exists!\n";
            return false;
        }
        else if (inodes[i].used == 0) {
            chosenInode = i;
            break;
        }

    if (chosenInode == 16) {
        cout << "No space left on disk!\n";
        return false;
    }
    inodes[chosenInode].name = filename;
    inodes[chosenInode].size = size;
    inodes[chosenInode].used = 1;
    for (int i = 0; i < size; i++)
        inodes[chosenInode].blockPointers[i] = (chosenInode + 1) * 1024 + i * 127;
    CommitToDisk(chosenInode);
    return true;
}

bool FileSystem::Delete(const char *name) {

    string filename(name);
    for (int i = 0; i < 16; i++)
        if (filename == inodes[i].name) {
            cout << "Deleting file : " << filename << endl;
            inodes[i].used = 0;
            CommitToDisk(i);
            return true;
        }
    cout << filename << " : File not found\n";
    return false;
}

void FileSystem::CommitToDisk(int inode){
    ofstream diskWriter(DISK_NAME, ofstream::binary | ofstream::app);
    diskWriter.seekp(128 + 48 * (inode));
    diskWriter << inodes[inode];
    diskWriter.close();
}

void FileSystem::ReadBlock(int32 blockPtr, char *buf) {
    ifstream diskReader(DISK_NAME, ifstream::binary);
    diskReader.seekg(blockPtr, diskReader.beg);
    diskReader.read(buf, 1024);
    diskReader.close();
}

void FileSystem::WriteBlock(int32 blockPtr, const char *buf) {
    ofstream diskWriter(DISK_NAME, ofstream::binary | ofstream::app);
    diskWriter.seekp(blockPtr, diskWriter.beg);
    diskWriter.write(buf, 1024);
    diskWriter.close();
}

bool FileSystem::Read(const char *name, int32 blockNum, char *buf) {

    if (blockNum < 0 || blockNum > 7) {
        cout << "Invalid block number specified!\n";
        return false;
    }
    string filename(name);
    for (int i = 0; i < 16; i++)
        if (filename == inodes[i].name) {
            if (inodes[i].size < blockNum + 1) {
                cout << "Invalid block number specified!\n";
                return false;
            }
            cout << "Copying block# " << blockNum << " from " << filename << " from disk " << endl;
            ReadBlock(inodes[i].blockPointers[blockNum], buf);
            return true;
        }

    cout << filename << " : File not found\n";
    return false;
}

bool FileSystem::Write(const char *name, int32 blockNum, const char *buf) {

    if (blockNum < 0 || blockNum > 7) {
        cout << "Invalid block number specified!\n";
        return false;
    }
    string filename(name);
    for (int i = 0; i < 16; i++)
        if (filename == inodes[i].name) {
            if (inodes[i].size < blockNum + 1) {
                cout << "Invalid block number specified!\n";
                return false;
            }
            cout << "Writing block# " << blockNum << " from " << filename << " to disk " << endl;
            WriteBlock(inodes[i].blockPointers[blockNum], buf);
            return true;
        }

    cout << filename << " : File not found\n";
    return false;
}

void FileSystem::ls() {

    cout << "Listing all files in the file system!" << endl;
    for (int i = 0; i < 16; i++) {
        if (inodes[i].used == 1)
            cout << inodes[i].name << endl;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "Syntax : " << argv[0] << " <input file>" << endl;
        return 1;
    }

    ifstream input(argv[1]);
    if (!input.is_open()) {
        cout << argv[1] << " : file not found " << endl;
        return 1;
    }

    FileSystem unix;
    input >> unix.DISK_NAME;

    unix.LoadDisk();

    string cmd;
    string filename;
    int blockNum;
    char buf[1024];

    while (!input.eof()) {
        input >> cmd;

        if (cmd == "L") {
            unix.ls();
            continue;
        }

        input >> filename;

        if (cmd == "D") {
            unix.Delete(filename.c_str());
            continue;
        }

        input >> blockNum;

        if (cmd == "C") {
            unix.Create(filename.c_str(), blockNum);
        }
        else if (cmd == "W") {
            unix.Write(filename.c_str(), blockNum, buf);
        }
        else if (cmd == "R") {
            unix.Read(filename.c_str(), blockNum, buf);
        }
    }
    input.close();
    unix.DumpData();
    return 0;
}