#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <algorithm>

using namespace std;

const int MAX_WORDS = 50;
const int MAX_LEN = 25;

vector<string> words;
void readWords(const char* filename) {
    int tyur = 0;
    FILE* file;  
    fopen_s(&file, filename, "r");
    if (!file) {
        cout <<("Ошибка при открытии файла!");
        return ;
    }

    char buffer[MAX_LEN + 1];
    while (fscanf_s(file, "%25s", buffer, (unsigned int)sizeof(buffer)) != EOF) {
        buffer[MAX_LEN] = '\0';
        words.push_back(buffer);
        tyur++;
        //Добавление прочитанного слова в массив words
    }
    if (tyur > MAX_WORDS) {
        cout << ("Ошибка кол-во слов > 50");
        return;

    }

    fclose(file);
}
//2.Функция проверки на корневое слово
bool isPrefix(const string& root, const string& word) {
    return word.compare(0, root.size(), root) == 0; //Если word начинается с root - true
}
//3.Функция, которая ищет самое длинное корневое слово.
void findLongRootWord(const char* outputFile) {
    string longestRoot; 
    vector<string> relatedWords;  

    for (const auto& root : words) {
        vector<string> temp; //Временный список слов, которые начинаются с root
        for (const auto& word : words) {
            if (root != word && isPrefix(root, word)) {
                temp.push_back(word);
            }
        }
        if (!temp.empty() && (root.size() > longestRoot.size())) {
            longestRoot = root;
            relatedWords = temp; 
        }
    }

    ofstream out(outputFile);
    if (!out) {
        cout << "Ошибка при открытии файла!" << endl;
        return;
    }

    if (longestRoot.empty()) {
        out << "NO";
    }
    else {
        out << longestRoot << endl; //Запись корневого слова и его потомков 
        for (const auto& word : relatedWords) {
            out << word << endl;
        }
    }
    out.close();
}

void findMostImportantWord(const char* outputFile) {
    string importantWord;
    int maxCount = 0;

    for (const auto& root : words) {//Подсчёт кол-ва потомков 
        int count = 0;
        for (const auto& word : words) {
            if (root != word && isPrefix(root, word)) {
                count++;
            }
        }
        if (count >= maxCount) { //Если у root нашлось больше потомков чем у прошлого
            maxCount = count;
            importantWord = root;
        }
    }

    FILE* file;
    fopen_s(&file, outputFile, "w");
    if (!file) {
        cout << ("Ошибка при открытии файла !");
        return;
    }

    if (importantWord.empty()) {
        fprintf(file, "NO\n");
    }
    else {
        fprintf(file, "%s\n%d\n", importantWord.c_str(), maxCount);
    }
    fclose(file);
}

int main() {
    setlocale(LC_ALL, "Ru");
    const char* inputFile = "input.txt";
    const char* longestRootFile = "root.txt";
    const char* importantWordFile = "word.txt";

    readWords(inputFile);
    findLongRootWord(longestRootFile);
    findMostImportantWord(importantWordFile);

    return 0;
}