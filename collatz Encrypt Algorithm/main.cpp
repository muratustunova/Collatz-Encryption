#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <locale>
#include <windows.h>
#include <ctime>
#include <iomanip>
#include <openssl/evp.h>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>
#include <future>
using namespace std;

vector<int> findNumbersAfterPercent(const string &text)
{
    vector<int> numbers;
    size_t pos = 0;
    while ((pos = text.find('%', pos)) != string::npos)
    {
        pos++;
        string currentNumber = "";

        while (pos < text.length() && isdigit(text[pos]))
        {
            currentNumber += text[pos];
            pos++;
        }
        if (!currentNumber.empty())
        {
            numbers.push_back(stoi(currentNumber));
        }
    }

    return numbers;
}

pair<vector<int>, vector<int>> findNumbers(const string &text)
{
    vector<int> numbers, detailStepVector;
    string currentNumber = "";
    detailStepVector = findNumbersAfterPercent(text);
    for (char c : text)
    {
        if (isdigit(c))
        {
            currentNumber += c;
        }
        else
        {
            if (!currentNumber.empty())
            {
                numbers.push_back(stoi(currentNumber));
                currentNumber = "";
            }
        }
    }
    if (!currentNumber.empty())
    {
        numbers.push_back(stoi(currentNumber));
    }
    return make_pair(numbers, detailStepVector);
}

char randomLetter()
{
    int rastgeleSayi = 65 + (rand() % 26);
    char harf = static_cast<char>(rastgeleSayi);
    return harf;
}

int findOriginalNumber(vector<int> &allSteps, vector<int> &detailSteps)
{
    vector<int> reverseDetailSteps;
    int step = 1;
    int starterNumber = 1;
    int beforeValuesIndex = 0, afterValuesIndex = 0;
    reverse(detailSteps.begin(), detailSteps.end());

    int size = allSteps[allSteps.size() - 1], deepStep = 0;

    for (int j = size; j > 0; j--)
    {
        if (deepStep < detailSteps.size() && j == detailSteps[deepStep])
        {
            reverseDetailSteps.push_back(step);
            deepStep++;
        }
        step++;
    }

    step = 1;
    for (int i = 0; i < reverseDetailSteps.size(); i++)
    {
        while (reverseDetailSteps[i] != step)
        {
            starterNumber *= 2;
            step++;
        }
        while (reverseDetailSteps[i] == step)
        {
            starterNumber = (starterNumber - 1) / 3;
            step++;
        }
    }

    if (reverseDetailSteps[reverseDetailSteps.size() - 1] < allSteps[allSteps.size() - 1])
    {
        int extraSteps = reverseDetailSteps[reverseDetailSteps.size() - 1];
        while (extraSteps != allSteps[allSteps.size() - 1])
        {
            starterNumber *= 2;
            extraSteps++;
        }
    }
    return starterNumber;
}

string encryptedASCIIChar(int byteValue, const vector<string> &dynamicStepsArr)
{
    string finalCharValue, hashText;
    byteValue = byteValue + 32;
    finalCharValue += string(1, static_cast<char>(byteValue));

    for (size_t i = 0; i < dynamicStepsArr.size(); ++i)
    {
        finalCharValue += dynamicStepsArr[i];
        finalCharValue += randomLetter();
    }
    return finalCharValue;
}

string decryptedASCIIChar(const string &encryptedContent)
{
    string charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

    if (encryptedContent == "+")
    {
        return " ";
    }
    vector<int> beginOfchar;
    istringstream stream(encryptedContent);
    string crackText, line, originalNumber = "";

    for (size_t i = 0; i < encryptedContent.length(); i++)
    {
        if (encryptedContent[i] == '!')
        {
            beginOfchar.push_back(i);
        }
    }

    for (size_t i = 0; i < beginOfchar.size(); i++)
    {
        size_t start = beginOfchar[i];
        size_t end = (i + 1 < beginOfchar.size()) ? beginOfchar[i + 1] : encryptedContent.size();
        string parseText = encryptedContent.substr(start, end - start);

        auto [steps, detailSteps] = findNumbers(parseText);
        originalNumber += to_string(findOriginalNumber(steps, detailSteps)) + " ";
    }

    istringstream numberStream(originalNumber);
    string finalText;
    string asciiValueStr;

    while (getline(numberStream, asciiValueStr, ' '))
    {
        if (!asciiValueStr.empty())
        {
            int asciiValue = stoi(asciiValueStr);
            char character = static_cast<char>(asciiValue);
            finalText += character;
        }
    }

    return finalText;
}

int main()
{
    setlocale(LC_ALL, "tr_TR.UTF-8");
    SetConsoleOutputCP(65001);

    string fileName = "a.txt";

    cout << "1-)Ecryption\n2-)Decryption\nChoose: (1/2): ";
    int choose;
    cin >> choose;

    if (choose == 1)
    {
        ifstream inputFile(fileName, ios::in | ios::binary);
        if (!inputFile)
        {
            cerr << "File could not be read: " << fileName << endl;
            return 1;
        }

        ofstream tempFile("temp.txt", ios::out | ios::binary);
        if (!tempFile)
        {
            cerr << "Failed to create temporary file." << endl;
            return 1;
        }

        string line;
        while (getline(inputFile, line))
        {
            string encryptedLine = "";
            for (char c : line)
            {
                if (c != 32)
                {
                    int step = 1;
                    int byteNumber = static_cast<unsigned char>(c);
                    vector<string> Steps;

                    while (byteNumber != 1)
                    {
                        if (byteNumber % 2 == 0)
                        {
                            byteNumber = byteNumber / 2;
                            Steps.push_back(to_string(step));
                        }
                        else
                        {
                            byteNumber = (byteNumber * 3) + 1;
                            Steps.push_back("%" + to_string(step));
                        }
                        step++;
                    }

                    encryptedLine += encryptedASCIIChar(byteNumber, Steps) + "\n";
                }
                else
                {
                    encryptedLine += "+\n";
                }
            }
            tempFile.write(encryptedLine.c_str(), encryptedLine.size());
        }
        inputFile.close();
        tempFile.close();

        remove(fileName.c_str());
        rename("temp.txt", fileName.c_str());

        cout << "Encryption is complete and written to file." << endl;
    }
    else if (choose == 2)
    {
        ifstream inputFile(fileName, ios::in | ios::binary);
        if (!inputFile)
        {
            cerr << "File could not be read: " << fileName << endl;
            return 1;
        }

        ofstream tempFile("temp.txt", ios::out | ios::binary);
        if (!tempFile)
        {
            cerr << "Failed to create temporary file." << endl;
            return 1;
        }

        string decryptedContent = "";
        string line;

        while (getline(inputFile, line))
        {
            string decryptedLine = decryptedASCIIChar(line);
            decryptedContent += decryptedLine;
        }

        inputFile.close();
        tempFile.write(decryptedContent.c_str(), decryptedContent.size());
        tempFile.close();
        remove(fileName.c_str());
        rename("temp.txt", fileName.c_str());

        cout << "The analysis is completed and written to the file." << endl;
    }
    else
    {
        cout << "You have selected the wrong parameter." << endl;
    }
    return 0;
}
