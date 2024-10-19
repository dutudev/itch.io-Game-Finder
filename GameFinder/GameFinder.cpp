#include <iostream>
#include <fstream>
#include <filesystem>
#define CURL_STATICLIB
#include <stdio.h>
#include <cstdlib>
#include <time.h>
#include <chrono>
#include <thread>
#include <vector>
#include <string>
#include <Windows.h>
#include "curl/curl.h"
#include "include/color.hpp"

using namespace std;
using namespace std::filesystem;
using namespace std::chrono;
using namespace std::this_thread;

int gameIndex = 0;
int input = 0;
int gamesNumber = 0;
int GetRandomGame();
bool latestBuild = true;
string localVersionList = "";
string onlineVersionList = "";
string localVersionBuild = "";
string onlineVersionBuild = "";
string gameName = "";
string gameDesc = "";
string gameLink = "";
string link = "";
vector<string> gamesList;
path directoryFiles = "Files";
path listVersionFile = "Files/listVersion.txt";
path buildVersionFile = "Files/buildVersion.txt";
path linksFile = "Files/links.txt";

void ChangeMenu(int inputMenu);
void UpdateList();
void CreateVectorList();
void CheckVersion();
void GetGameInfo(string pickedGame);

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
            UpdateList();
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
                UpdateList();
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
    CreateVectorList();
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
        cout << "\nCurrently " << dye::aqua(gamesNumber) << " games in list!\n";
        cout << "\n\n1.Search for Game\n";
        cin >> input;
        ChangeMenu(input);
        break;
    case 1:
        link = gamesList[GetRandomGame()];
        GetGameInfo(link);
        gameLink = link;
        cout << "Picked Game :\n";
        cout << gameName << endl;
        cout << gameDesc;
        cout << "\n\n0.Go to the main menu\n";
        cout << "1.Find another game\n";
        cout << "2.Open game\n";
        cin >> input;
        ChangeMenu(input);
        break;
    case 2:
        system(std::string("start " + gameLink).c_str());

    default:
        ChangeMenu(0);
        break;
    }
    
}

void CreateVectorList() {
    ifstream file(linksFile);
    string line;
    getline(file, line);

    stringstream ss(line);
    string game;


    while (getline(ss, game, ';')) {
        gamesList.push_back(game);
        gamesNumber++;
    }
    ChangeMenu(0);
}

int GetRandomGame() {
    srand(time(NULL));

    return rand() % gamesList.size();
}

void UpdateList() {
    CURL* curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_ALL);

    string fileContents;
    fileContents = "";
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://drive.google.com/uc?export=download&id=1bpRi9RBe6D38lBkKWD0Jz_VW-hkizNT5");

        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writecallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &fileContents);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            cout << "Code Returned Error", curl_easy_strerror(res);
        }
        
    }
    curl_easy_cleanup(curl);


    curl_global_cleanup();
    ofstream file(linksFile);
    if (file.is_open()) {
        file << fileContents;
        file.close();
    }
    else {
        cout << "Failed to create file";
    }
    
    ofstream filelist(listVersionFile);
    if (filelist.is_open()) {
        filelist << onlineVersionList;
        filelist.close();
        cout << "\nUpdated list at " << listVersionFile << "\n";
        localVersionList = onlineVersionList;
        
    }
    else {
        cout << "Failed to create file";
    }
    CheckVersion();
}

void GetGameInfo(string pickedGame) {
    //cout << "Attempting to fetch URL: " << pickedGame << endl;
    CURL* curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_ALL);

    string htmlContent;
    htmlContent = "";
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, pickedGame.c_str());

        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writecallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &htmlContent);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            cout << "Code Returned Error" << curl_easy_strerror(res);
        }
    }
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    size_t startPos = htmlContent.find("name=\"twitter:site\"/><meta content=");
    if (startPos != string::npos) {
        startPos += 36;

        size_t endPos = htmlContent.find("\" name=\"twitter:title\"", startPos);
        if (endPos != string::npos) {
            gameName = htmlContent.substr(startPos, endPos - startPos);
        }
        
        //check for '
        size_t specialCharacter = gameName.find("&#039;");
        if (specialCharacter != string::npos) {
            gameName = gameName.replace(specialCharacter, 6, "'");
        }

        //check for &
        specialCharacter = gameName.find("&amp;");
        if (specialCharacter != string::npos) {
            gameName = gameName.replace(specialCharacter, 6, "&");
        }
    }

    size_t startPosDesc = htmlContent.find("name=\"twitter:title\"/><meta content=");
    if (startPosDesc != string::npos) {
        startPosDesc += 37;

        size_t endPosDesc = htmlContent.find("Available for Windows", startPosDesc);
        if (endPosDesc != string::npos) {
            gameDesc = htmlContent.substr(startPosDesc, endPosDesc - startPosDesc);
        }
        // add desc for browser games

        //check for '
        size_t specialCharacter = gameDesc.find("&#039;");
        if (specialCharacter != string::npos) {
            gameDesc = gameDesc.replace(specialCharacter, 6, "'");
        }
        //check for &
        specialCharacter = gameDesc.find("&amp;");
        if (specialCharacter != string::npos) {
            gameDesc = gameDesc.replace(specialCharacter, 6, "&");
        }
    }
}

int main()
{
    system("title itch.io Gamefinder");
    CheckVersion();
    return 0;
}