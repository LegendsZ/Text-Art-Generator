#pragma once

class Letter {
	friend class Font;
private:
	std::string letterLine;
	int spaceCount;
	std::string* indivLine;
public:
	Letter(): letterLine(""), spaceCount(0), indivLine(nullptr) {
	}
	~Letter() {
		delete[] indivLine;
	}
	bool feedLine(std::string& lineG) {
		letterLine.append(lineG);
		return false;
	}
	bool finishLetter() {
		indivLine = new std::string[spaceCount];
		int x = 0;
		int index_start = 0;
		for (int k = 0; k < letterLine.length(); k++) {
			if (letterLine[k] == '\n') {
				indivLine[x++] = letterLine.substr( index_start, k-index_start ); //work here
				index_start = k + 1;
			}
		}
		return true;
	}
	std::string& produceLetter() {
		return letterLine;
	}
	bool incrSC() {
		return ++spaceCount;
	}
	bool decrSC() {
		spaceCount--;
		return true;
	}
	const int& retSC() const{
		return spaceCount;
	}
};