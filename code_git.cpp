#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <filesystem>
#include <iterator>
#include <map>
#include <vector>
#include <dirent.h>
#define MAX 1024
using namespace std;

string path = "/Users/divya/Desktop/PROJECT GIT";

// INIT
void init() {

    //.git
    if(mkdir("./git", 0777) != 0) {
        cerr << "Error : directory not created " << strerror(errno)<<"\n";
        return ;
    }
    else {

        //files
        ofstream file1("./git/main index.txt");
        file1.close();
        ofstream file2("./git/log.txt");
        file2.close();

        //zero folder
        if(mkdir("./git/zero", 0777) != 0) {
            cerr << "Error : directory not created " << strerror(errno)<<"\n";
            return ;
        }

        else {

            //inside zero folder
            ofstream file3("./git/zero/index.txt");
            file3.close();
            cout<<"Git Initialized."<<"\n";
        }   
    }  

    //copy file to base folder
    int src_fd, dst_fd, n, err;
    unsigned char buffer[4096];
    char current_path[] = "/Users/divya/Desktop/PROJECT GIT";
    char final_path[] = "/Users/divya/Desktop/PROJECT GIT/git/zero"; 
    struct dirent* files_name;
    DIR* open_file = NULL;
    DIR* read_file = NULL;
    open_file = opendir(current_path); 

    //dir not open
    if(!open_file) printf("\n ERROR! Directory can not be open");

    //dir open
    else {    
        int nErrNo = 0;
        while((files_name = readdir(open_file)) != NULL) {
            if(nErrNo == 0) {nErrNo = errno;}
            string current_file = files_name->d_name;

            // other than these files
            if (current_file == "." || current_file == ".." || current_file == ".git" || current_file == "git"||current_file == ".DS_Store" || current_file == ".vscode" || current_file == "code_git" || current_file == "code_git.cpp") {continue;}
            
            struct stat st_buf;
            stat(files_name->d_name, &st_buf);
            if (S_ISDIR (st_buf.st_mode)) {continue;}
            else if (S_ISREG (st_buf.st_mode)) {

                //read file
                FILE* pnReadFile = fopen(files_name->d_name,"r");
                if(pnReadFile) {
                    char strDestFileName[MAX] = {0};
                    sprintf(strDestFileName, "%s/%s", final_path, files_name->d_name);

                    //write file
                    FILE* pnWriteFile  = fopen(strDestFileName, "w");
                    if(pnWriteFile) {
                        char buffer[MAX] = {0};  
                            while(fgets(buffer, MAX, pnReadFile)) {fputs(buffer, pnWriteFile);}
                        fclose(pnWriteFile);
                    }
                    else {printf("\n Unable to open file %s", strDestFileName);}
                    fclose(pnReadFile);
                }
                else {printf ("\nERROR! File Could not be open for reading");}
            }
        }
        if(nErrNo != errno) {printf ("ERROR Occurred!\n");}
    }
    closedir(open_file);
}

/*
// MKDIR
void makeDir() {
    string file_name;
    cin>>file_name;
    file_name = path+"/"+file_name;
    if(mkdir(file_name.c_str(), 0777) != 0) {
        cerr << "Error : directory not created " << strerror(errno) << endl;
        return ;
    }
}
*/

/*
void listoffiles() {
    for (const auto & file : directory_iterator(path))
        cout << file.path()<<"\n";
}
*/

// SHA
string sha(string file) {   
    string f1 = "shasum " + file + " > temp.txt";
    system(f1.c_str());
    FILE *temp = fopen("temp.txt", "r+");
    char sha1[100];
    fread(sha1, sizeof(char), 100, temp);
    string input = sha1;
    fclose(temp);
    string sha = input.substr(0, input.find(' '));
    cout<<sha<<"\n";
    int result = remove("temp.txt");
    if (result != 0) {cout << "Error: File not deleted." << endl;}
    return sha;
}

// STORE FILE
vector <string> storeFile(string path) {

    //dirent.h header file
    vector <string> files;
    struct dirent *total_files;
    DIR *dir = opendir(path.c_str());
    if(!dir) {
        fprintf(stderr,"Unable to open directory\n");
		exit(EXIT_FAILURE);
    }
    while ((total_files = readdir(dir)) != NULL) {
        string current_file = total_files->d_name;
        if (current_file == "." || current_file == ".." || current_file == "git" || current_file == ".git" ||current_file == ".DS_Store" || current_file == ".vscode" || current_file=="code_git" || current_file=="code_git.cpp") {continue;}
        else {files.push_back(current_file);}   
    }
    closedir(dir);
    return files;
}


// MAP 
map <string, string> files_map(string path) {
    map<string, string> file_info;
    vector <string> files = storeFile(path);
    int size = files.size(), i=0;

    // stores file name and sha
    while(size--) {
        file_info[files[i]] = sha(files[i]);
        i++;
    }
    return file_info;
}


// STATUS
void status() {
    vector <string> deleted;
    vector <string> modified;
    vector <string> untracked;
    map <string, string> filename_sha;
    filename_sha = files_map(path + "/git/zero");
    vector<string> files = storeFile(path);

    // more...

}

int main() {
    string x;
    cin>>x;
    if(x == "-1") {return 0;}
    else if(x == "init") {init();}
    else if(x == "files") {storeFile(path);}
    else if(x == "filemap") {files_map(path);}    
    return 0;
}