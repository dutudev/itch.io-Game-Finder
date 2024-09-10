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
using namespace std;
using namespace std::filesystem;
using namespace std::chrono;
using namespace std::this_thread;

int gameIndex = 0;
int input = 0;
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
    cout << "Checking current list version";

    CURL* curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_ALL);

    string fileContents;

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
        cout << "\nNewest version : " << fileContents;
    }

    curl_global_cleanup();

    if (!exists(directoryFiles)) {
        create_directories(directoryFiles);
    }

    if (!exists(listVersionFile)) {
        cout << "\nNo local version found, taking newest one\n";
        
        ofstream file(listVersionFile);
        if (file.is_open()) {
            file << fileContents;
            file.close();
            cout << "List up to date at " << listVersionFile;
        }
        else {
            cout << "Failed to create file";
        }
    }
    else {
        cout << "\nComparing local version\n";
        ofstream file(listVersionFile);
        if (file.is_open()) {
            string onlineVersion, localVersion;
            file << onlineVersion;
            if (localVersion == onlineVersion) {
                cout << "Local version up to date " << localVersion;
                sleep_for(seconds(3));
                ChangeMenu(0);
            }
        }
    }
}

void ChangeMenu(int inputMenu) {
    system("cls");
    switch (inputMenu)
    {
    case 0:
        cout << "Itch.io Game Finder\n";
        cout << "\n1.Search for Game\n";
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