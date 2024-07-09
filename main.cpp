#include <cstdio>
#include <windows.h>
#include <string>

// compile: g++ main.cpp -o main.exe -lcomdlg32

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
        printf("Selected file: %s\n", ofn.lpstrFile);
        return std::string(ofn.lpstrFile);
    }
    return std::string();
}

int main()
{
    std::string file = OpenFileDialog();
    printf("Selected file: %s\n", file.c_str());
}
