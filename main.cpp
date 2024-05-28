/*
 * File: textGenerator.cpp
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include "myrandom.h"

using namespace std;
using Ngram = vector<string>;
using NgramMap = map<Ngram, vector<string>>;

vector<string> readFromFile(string &filename){
    vector<string> words;
    bool isValid = false;
    cout<<"Input file name?  ";

    while(!isValid){
        cin >> filename;

        ifstream inputfile(filename);

        if(!inputfile){
            cout<< "Invalid file, try again:  ";
        }
        else{
        string word;
        while(inputfile >> word){
         words.push_back(word);
         }
         isValid = true;
        }
    }
    return words;
}

NgramMap buildNgrams(const vector<string> &words, int N){
    NgramMap ngrams;
    int numWords = words.size();
    for (int i = 0; i < numWords - N + 1 ; ++i){
        Ngram ngram(words.begin() + i, words.begin() + i + N - 1 );
        string nextWord = words[i + N - 1];
        ngrams[ngram].push_back(nextWord);
    }

    for(int i = numWords - N + 1; i < numWords; ++i){
        Ngram ngram(words.begin()+ i, words.end());
        int wrapWords = i - numWords + N - 1;
        ngram.insert(ngram.end(),words.begin(), words.begin() + wrapWords);
        string nextWord = words[wrapWords];
        ngrams[ngram].push_back(nextWord);
    }
    return ngrams;
}

void printMap (const NgramMap &ngrams){
    for(const auto &pos : ngrams){
        const Ngram &ngram = pos.first;
        const vector<string> &nextWords = pos.second;

        cout<< "{";
        for(size_t i = 0; i< ngram.size();++i){
            cout<< ngram[i];
            if(i<ngram.size()-1){
                cout<< " ";
            }
        }
        cout << "} -> ";
        cout << "{";
        for(size_t i = 0; i < nextWords.size(); ++i){
            cout<<nextWords[i];
            if(i<nextWords.size() - 1){
                cout<< " ";
            }
        }
        cout << "}" << endl;
    }
    cout << endl;

}

int getRandom(int maxInd){
    return randomInteger(0,maxInd - 1);
}

string generateText(const NgramMap &ngrams, int N, int numWords){
    auto randword = ngrams.begin();
    advance(randword, getRandom(ngrams.size()));

    Ngram currNgram = randword -> first;
    string genText;
    genText.reserve(numWords);

    for(const string &word : currNgram){
        genText += word + " ";
    }

    for(int i = 0; i < numWords - N + 1; ++i){
        const vector<string> &nextWords = ngrams.at(currNgram);
        int randInd = getRandom(nextWords.size());
        string nextWord = nextWords[randInd];

        genText += nextWord + " ";

        currNgram.erase(currNgram.begin());
        currNgram.push_back(nextWord);
    }
    return genText;
}

int main() {
    cout<< "Welcome to the Text Generator." << endl;
    cout<< "This program makes random text based on a document." << endl;

    string inputFile;
    int N;
    int numWords;
    vector<string> words;
    
    words = readFromFile(inputFile);
    if(words.empty()){
        cout<< "No words to build N-grams. Exiting."<< endl;
        return 0;
    }
    while(true){
        cout<<"Value of N?  ";
        cin >> N;
    
        if(N > 1){
            break;
        }
        else{
            while(N <= 1){
            cout<< "N must be > 1, try again: ";
            cin >> N;
            }
            break;
        }
    }
    while(true){
    cout<< "Total words you'd like to generate?  ";
    cin >> numWords;
    if(numWords >= N){
        break;
    }
    else{
        while(numWords < N){
            cout << "Total words must be at least N, try again:  ";
            cin >> numWords;
        }
        break;
    }
    }
    string userInput;
    bool isMapBuilt = false;

    NgramMap ngrams;

    while(true){
        cout<< "Type b-build map, p-print map, g-generate text, s-start over, x-to exit: ";
        cin >> userInput;

        if(userInput == "x"){
            break;
        }
        else if(userInput == "s"){
            isMapBuilt = false;
            ngrams.clear();
            continue;
        }
        else if (userInput == "b"){
            ngrams.clear();
            if(words.empty()){
                continue;
            }
            ngrams = buildNgrams(words,N);
            isMapBuilt = true; 
            cout<< "\n...Building map: " << inputFile << "...\n\n";
        }
        else if (userInput == "p"){
            if(isMapBuilt){
                printMap(ngrams);
            }
        }
        else if (userInput == "g"){
            if(isMapBuilt){
                string genText = generateText(ngrams, N, numWords);
                cout << "..."<<genText<< "..."<<endl;
            }
        }
    }

    return 0;
}