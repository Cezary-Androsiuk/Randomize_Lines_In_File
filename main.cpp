#include <cstdio>
#include <windows.h>
#include <ctime>

#include <string>
#include <vector>
#include <fstream>

// compile: g++ main.cpp -o main.exe -lcomdlg32

// from directory removed .git, .gitignore, .vscode, example_file.txt, main.exe

// i decided to use pointers 

std::string OpenFileDialog();
bool readFile(std::vector<std::string> *content, const std::string *file);
void randomizeContent(std::vector<std::string> *content);
void createSaveFileName(std::string *file);
bool saveFile(const std::vector<std::string> *content, const std::string *file);

int main()
{
    std::string file = OpenFileDialog();
    printf("Selected file: %s\n", file.c_str());

    std::vector<std::string> content;

    if(!readFile(&content, &file)) return 1;
    randomizeContent(&content);
    createSaveFileName(&file);
    if(!saveFile(&content, &file)) return 1;
    return 0;
}

std::string OpenFileDialog()
{
    OPENFILENAME ofn;       // common dialog box structure
    char szFile[260];       // buffer for file name
    HWND hwnd = NULL;       // owner window
    HANDLE hf;              // file handle

    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrTitle = "Select a file to randomize lines";  // Set the dialog title
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // Display the Open dialog box.
    if (GetOpenFileName(&ofn) == TRUE) 
    {
        return std::string(ofn.lpstrFile);
    }
    return std::string();
}

bool readFile(std::vector<std::string> *content, const std::string *file)
{
    std::ifstream f(*file);
    if(!f.good())
    {
        printf("error while reading file %s to read\n", file->c_str());
        return false;
    }

    std::string buffer;
    while (std::getline(f, buffer))
    {
        content->push_back(buffer);
    }
    
    f.close();
    return true;
}

void randomizeContent(std::vector<std::string> *content)
{
    std::vector<std::string> newContent;
    newContent.reserve(content->size());

    std::srand(std::time(nullptr));

    for(int i=content->size()-1; i>=0; --i)
    {
        int pos = i == 0 ? 0 : std::rand() % i;
        newContent.push_back((*content)[pos]);
        content->erase(content->begin() + pos);
    }
    
    *content = newContent;
}

void createSaveFileName(std::string *fileName)
{
    constexpr const char *postfix = "_randomized";

    size_t last_dot = fileName->find_last_of('.');
    if(last_dot == std::string::npos)
    {
        // fileName not contains dot -> just add at text the end
        *fileName += postfix;
        return;
    }

    // fileName is for example: "some.text_title.someExt"
    std::string extension = fileName->substr(last_dot, fileName->length() - last_dot); // gets ".someExt"
    std::string name = fileName->substr(0, last_dot); // gets "some.text_title"

    *fileName = name + postfix + extension;
}

bool saveFile(const std::vector<std::string> *content, const std::string *file)
{
    std::ofstream f(*file);
    if(!f.good())
    {
        printf("error while saving file %s to read\n", file->c_str());
        return false;
    }
    
    bool first = true;
    for(const auto &i : *content)
    {
        if(first)
        {
            f << i;
            first = false;
        }
        else
        {
            f << std::endl << i;
        }
    }

    f.close();
    return true;
}