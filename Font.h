#pragma once
#include "Letter.h"

#define SPACE " "
#define LETTER_COUNT 26
#define REDUCTION 65
class Font {
private:
	std::string name;
	Letter* alphabets;
public:
	Font(std::string& nameG): name(nameG), alphabets(new Letter[LETTER_COUNT]) {
	}

	~Font() {
		delete[] alphabets;
	}

	bool feedLetter(std::string& letterG, int value) {
		std::string newLine = "\n";
		alphabets[value].feedLine(letterG);
		alphabets[value].feedLine(newLine);
		alphabets[value].incrSC();
		return true;
	}

	bool formatLetters() {
		for (int i = 0; i < LETTER_COUNT; i++) {
			alphabets[i].finishLetter();
			alphabets[i].letterLine = alphabets[i].letterLine.substr(0, alphabets[i].letterLine.length() - 1); //removes the last '\n'
		}
		return true;
	}

	const std::string& getName() const {
		return name;
	}

	const std::string produceText(std::string& textG) const{
		std::string text;
		int textGHIGHEST=0;

		for (int i = 0; i < textG.length(); i++) {
			if (textG[i] == ' ') {
				continue;
			}
			int c = alphabets[textG[i] - REDUCTION].retSC();
			if (c > textGHIGHEST) {
				textGHIGHEST = alphabets[textG[i] - REDUCTION].retSC();
			}
		}

		std::string* finalText = new std::string[textGHIGHEST];
		for (int i = 0; i < textGHIGHEST; i++) {
			finalText[i] = "";
		}

		for (int i = 0; i < textG.length(); i++) {
			for (int k = 0; k < textGHIGHEST; k++) {
				if (textG[i] == ' ' || alphabets[textG[i] - REDUCTION].retSC() - 1 < k) {
					finalText[k].append(SPACE);
					continue;
				}
				finalText[k].append(alphabets[textG[i] - REDUCTION].indivLine[k]);
			}
		}

		for (int i = 0; i < textGHIGHEST; i++) {
			text.append(finalText[i] + "\n");
		}

		delete[]finalText; //dealloc
		return text.substr(0, text.length()-1);
	}
};