#include <iostream>
#include <fstream>
#include <filesystem>
#define CURL_STATICLIB
#include <stdio.h>
#include <cstdlib>
#include <time.h>
#include <chrono>
#include <thread>
#include "curl/curl.h"
#include "include/color.hpp"

using namespace std;
using namespace std::filesystem;
using namespace std::chrono;
using namespace std::this_thread;

int gameIndex = 0;
int input = 0;
bool latestBuild = true;
string localVersionList = "";
string onlineVersionList = "";
string localVersionBuild = "";
string onlineVersionBuild = "";
path directoryFiles = "Files";
path listVersionFile = "Files/listVersion.txt";
path buildVersionFile = "Files/buildVersion.txt";
path linksFile = "Files/links.txt";

void PerformAction(int inputMenu);
void ChangeMenu(int inputMenu);
void UpdateList();
void CheckVersion();

size_t writecallback(void* content, size_t size, size_t nmemb, string* data) {
    int length = size * nmemb;
    data->append((char*)content, length);
    return length;
}

void CheckVersion() {
    //check if version file exists
    cout << "Checking current list and build version";

    CURL* curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_ALL);

    string fileContents;
    fileContents = "";
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://drive.google.com/uc?export=download&id=1NvJlECyt_R54QDYzHj8ubBcNvjDngML4");

        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        //https://drive.google.com/uc?export=download&id=1NvJlECyt_R54QDYzHj8ubBcNvjDngML4

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writecallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &fileContents);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            cout << "Code Returned Error", curl_easy_strerror(res);
        }
        cout << "\nNewest list version : " << fileContents;
        onlineVersionList = fileContents;
    }
    curl_easy_cleanup(curl);
    curl = curl_easy_init();
    fileContents = "";
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://drive.google.com/uc?export=download&id=1NcxWeFlu2SLb5mCcD1Zp7MK1qe-ZmGDo");

        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writecallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &fileContents);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            cout << "Code Returned Error", curl_easy_strerror(res);
        }
        cout << "\nNewest build version : " << fileContents;
        onlineVersionBuild = fileContents;
    }
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    if (!exists(directoryFiles)) {
        create_directories(directoryFiles);
    }

    if (!exists(listVersionFile)) {
        cout << "\nNo local version for list found, taking newest one\n";
        
        ofstream file(listVersionFile);
        if (file.is_open()) {
            file << onlineVersionList;
            file.close();
            cout << "List up to date at " << listVersionFile;
            localVersionList = onlineVersionList;
        }
        else {
            cout << "Failed to create file";
        }
    }
    else {
        cout << "\nComparing local list version\n";
        ifstream file(listVersionFile);
        if (file.is_open()) {
            getline(file, localVersionList);
            if (localVersionList == onlineVersionList) {
                cout << "Local list version up to date " << localVersionList;
            }
            else {
                cout << dye::green("Updating game list");
                //UpdateList();
            }
            file.close();
        }
    }
    sleep_for(seconds(1));
    if (!exists(buildVersionFile)) {
        cout << "\nNo local version for build found, taking newest one\n";

        ofstream file(buildVersionFile);
        if (file.is_open()) {
            file << fileContents;
            file.close();
            cout << "List up to date at " << buildVersionFile;
            localVersionBuild = onlineVersionBuild;
        }
        else {
            cout << "Failed to create file";
        }
    }
    else {
        cout << "\nComparing local build version\n";
        ifstream file(buildVersionFile);
        if (file.is_open()) {
            getline(file, localVersionBuild);
            if (localVersionBuild == onlineVersionBuild) {
                cout << "Local build version up to date " << localVersionBuild;
            }
            else {
                cout << dye::green("New build available on dutudev github!");
                latestBuild = false;
            }
            file.close();
        }
    }
    sleep_for(seconds(3));
    ChangeMenu(0);
}

void ChangeMenu(int inputMenu) {
    system("cls");
    switch (inputMenu)
    {
    case 0:
        cout << "Itch.io Game Finder\n";
        cout << "Write the number of the command you wish to run and press enter\n";
        cout << "Currently on\n";
        cout << "Build Version : " << localVersionBuild;
        if (!latestBuild) {
            cout << dye::green(" *new build available*");
        }
        cout << "\nList Version : " << localVersionList;
        cout << "\n\n1.Search for Game\n";
        cin >> input;
        PerformAction(input);
        break;
    case 1:
        break;
    default:
        break;
    }
    
}

void PerformAction(int inputMenu) {
    srand(time(NULL));
    
    switch (inputMenu)
    {
    case 1:
        gameIndex = rand();
        ChangeMenu(0);
        break;
    default:
        break;
    }
}
int main()
{
    CheckVersion();
    return 0;
}