#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <windows.h>

#include "Font.h"
#include "LinkedList.h"


#define VERSION "1.0.0.1"
#define MAIN_COMMAND_COUNT 6
#define SETTINGS_COMMAND_COUNT 1
#define SAVED_FONTS "SAVED_FONTS"

void printMainCommands();
void printSettingsCommands();
bool readSavedFolder(LinkedList*);
Font* retFont(std::string& filePathG);
bool printFontIndex(LinkedList* listG);

int main()
{
    std::cout << "[CONSOLE]: INITIALIZING TEXTMAKE...";
    //init stuff
    system((std::string("title TEXTMAKE ") + std::string(VERSION) + std::string(" BY LegendsZ#1526 @ DISCORD")).c_str());
    bool run = true;
    std::string temp;
    std::string addon = "";
    LinkedList fontList;
    readSavedFolder(&fontList);

    std::cout << "OK!\n";
    std::cout << "[CONSOLE]: WELCOME TO TEXTMAKE " << VERSION << "!\n\n";

    std::cout << "[CONSOLE]: TYPE 'CMDS' FOR COMMANDS!\n\n";

    while (true) {
        std::cout << "MAIN>" << addon;
        temp.clear();
        std::getline(std::cin, temp);

        if (addon == "") {
            if (temp == std::string("CMDS") || temp == std::string("cmds")) {
                printMainCommands();
            }
            else if (std::atoi(temp.c_str()) - 48 < MAIN_COMMAND_COUNT + 1) {
                switch (std::atoi(temp.c_str())) {
                case 1:
                    run = false;
                    break;
                case 2:
                    std::cout << "[CONSOLE]: ADD FONT: ENTER FILE PATH: ";
                    temp.clear();
                    std::getline(std::cin, temp);
                    {
                        Font* newFont = retFont(temp);
                        if (newFont != nullptr) {
                            fontList.addNodeByPtr(newFont);
                            std::cout << "[CONSOLE]: ADDED FONT SUCCESSFULLY!\n\n";
                        }
                        else {
                            std::cout << "[CONSOLE]: ERROR!\n\n";
                        }
                    }
                    break;
                case 3:
                    std::cout << "[CONSOLE]: DELETE FONT: ENTER FONT INDEX: ";
                    temp.clear();
                    std::getline(std::cin, temp);
                    Font* tempF;
                    if (std::atoi(temp.c_str()) < 1) {
                        std::cout << "[CONSOLE]: ERROR: CANNOT DELETE DEFAULT FONT!\n\n";
                    }
                    else if (fontList.delNodeAndObjectByIndex<Font>(std::atoi(temp.c_str()))) {
                        std::cout << "[CONSOLE]: FONT DELETED SUCCESSFULLY!\n\n";
                    }
                    else {
                        std::cout << "[CONSOLE]: ERROR!\n\n";
                    }
                    break;
                case 4:
                    printFontIndex(&fontList);
                    break;
                case 5:
                    std::cout << "[CONSOLE]: PRODUCE TEXT: ENTER FONT INDEX (TYPE F TO VIEW FONT INDEXES): ";
                    temp.clear();
                    std::getline(std::cin, temp);

                    if (temp == std::string("F") || temp == std::string("f")) {
                        printFontIndex(&fontList);
                    }
                    else if (std::atoi(temp.c_str()) >= 0 && std::atoi(temp.c_str()) < fontList.retNodeCount()) {
                        Font* asd = (Font*)fontList.retPtrByIndex(std::atoi(temp.c_str()));
                        std::cout << "[CONSOLE]: PRODUCE TEXT: ENTER TEXT: ";
                        temp.clear();
                        std::getline(std::cin, temp);

                        std::string producedText = asd->produceText(temp);
                        std::cout << "\n[CONSOLE]: OUTPUT:\n<===>\n" << producedText << "\n<===>\n";
                        std::cout << "[CONSOLE]: WOULD YOU LIKE TO COPY TO CLIPBOARD? (Y/y for yes / Any other key for no): ";
                        temp.clear();
                        std::getline(std::cin, temp);
                        if (temp == "Y" || temp == "y") {
                            const size_t length = strlen(producedText.c_str()) + 1; //copies to clipboard.
                            HGLOBAL mem = GlobalAlloc(GMEM_MOVEABLE, length);
                            memcpy(GlobalLock(mem), producedText.c_str(), length);
                            GlobalUnlock(mem);
                            OpenClipboard(0);
                            EmptyClipboard();
                            SetClipboardData(CF_TEXT, mem);
                            CloseClipboard();
                            std::cout << "[CONSOLE]: SUCCESSFULLY COPIED TO CLIPBOARD!\n\n";
                        }
                    }
                    break;
                case 6:
                    addon += "SETTINGS>";
                    break;
                default:
                    std::cout << "[CONSOLE]: ERROR - INPUT NOT RECOGNIZED!\n\n";
                    break;
                }
                if (run == false) {
                    break;
                }
            }
            else {
                std::cout << "[CONSOLE]: ERROR - INPUT NOT RECOGNIZED / TYPE 'CMDS' FOR HELP!\n\n";
            }
        }
        else if (addon == "SETTINGS>") {
            if (temp == std::string("CMDS") || temp == std::string("cmds")) {
                printSettingsCommands();
            }
            else {
                if (std::atoi(temp.c_str()) - 48 < SETTINGS_COMMAND_COUNT + 1) {
                    switch (std::atoi(temp.c_str())) {
                    case 1:
                        addon = "";
                        break;
                    default:
                        std::cout << "[CONSOLE]: ERROR - INPUT NOT RECOGNIZED!\n\n";
                        break;
                    }
                }
            }
        }
    }
}

void printMainCommands() {
    std::cout << "\n-->COMMANDS<--\n1 = EXIT\n2 = LOAD FONT\n3 = DELETE FONT\n4 = FONT INDEX LIST\n5 = PRODUCE TEXT\n6 = SETTINGS\n\n";
}
void printSettingsCommands() {
    std::cout << "\n-->COMMANDS<--\n1 = EXIT SETTINGS\n\n";
}

bool printFontIndex(LinkedList* listG) {
    std::cout << "\n-->FONT INDEXES<--\n";
    Font** fonts = (Font**)listG->retAllPtrsAsPtr();
    for (int i = 0; i < listG->retNodeCount(); i++) {
        std::cout << i << " = " << fonts[i]->getName() << "\n";
    }
    std::cout << "\n";
    return true;
}

bool readSavedFolder(LinkedList* listG) {
    for (const std::filesystem::path dirEntry : std::filesystem::recursive_directory_iterator(SAVED_FONTS)) {
        std::string fpath = std::string(dirEntry.string());
        Font* newFont = retFont(fpath);
        listG->addNodeByPtr(newFont);
        //std::cout << "\n" << dirEntry.string() << "\n"; //DEBUG PURPOSES
    }
    return true;
}

Font* retFont(std::string& filePathG) {
    std::ifstream readFile;
    readFile.open(filePathG.c_str());

    if (!readFile) {
        return nullptr;
    }
    Font* newFont = nullptr;

    int lineCount = 0;
    int section = -1;
    std::string line;
    while (std::getline(readFile, line)) {
        if (newFont == nullptr) {
            newFont = new Font(line);
            continue;
        }
        int x = 0;
        try {
            x = std::stoi(line);

            if (x >= 0 /*0*/ && x < LETTER_COUNT /*+1*/) {
                section = std::atoi(line.c_str()); //to compensate for the sections starting from 1, add -1
                continue;
            }
        }
        catch (std::exception& err) {
            newFont->feedLetter(line, section);
        }
        line.clear();
        lineCount++;
    }
    newFont->formatLetters();
    readFile.close();
    return newFont;
}