#include <iostream>
#include <fstream>
#include <filesystem>
#define CURL_STATICLIB
//#include <curl/curl.h>
#include <stdio.h>
#include <cstdlib>
#include <time.h>
using namespace std;
namespace fs = filesystem;

int gameIndex = 0;
int input = 0;

void PerformAction(int inputMenu);
void ChangeMenu(int inputMenu);
void UpdateList();
void CheckVersion();

void CheckVersion() {
    //check if version file exists
    cout << "Checking current list version";
    /*if (!fs::exists("version.txt")) {
        CURL* curl;
        CURLcode res;
        curl_global_init(CURL_GLOBAL_ALL);

        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, "https://dutu.dev/");

            res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                cout << "Code Returned Error" , curl_easy_strerror(res);
            }
        }

        curl_global_cleanup();

    }*/
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