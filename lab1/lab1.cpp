#include <iostream> 
#include <vector> 
#include <algorithm>

using namespace std;

class BinaryNumber {
private:
    unsigned short bitSize;
    vector<short> num;

public:
    BinaryNumber(int countBit) {
        bitSize = countBit;
        num.resize(bitSize);
    }

    void setNum(int number) {
        int n = number;
        if (n <= pow(2, bitSize - 1) - 1 && n > -pow(2, bitSize - 1)) {
            if (n < 0) {
                num[0] = 1; // Задаем знак числа
                n = -n; // Делаем число положительным для перевода в двоичное представление
            }
            else {
                num[0] = 0; // Задаем знак числа 0, если число положительное
            }

            int i = bitSize-1;
            while (n > 0) {
                num[i] = n % 2;
                n = n / 2;
                i -= 1;
            }
        }
        else {
            cout << "ERROR: NUM IS BIG" << endl;
        }
    }

    void setBinNum(vector<short> binNumber) {
        if (binNumber.size() == num.size()) {
            for (int i = 0; i < binNumber.size(); i++)
                num[i] = binNumber[i];
        }
    }


    void setZero() {
        num[0] = 0;
    }

    void setMinus() {
        num[0] = 1;
    }
    

    unsigned short getBitSize() {
        return bitSize;
    }
    vector<short> getBinNumber() {
        return num;
    }


    int getNumber() {
        int number = 0;
        int k = 0;
        for (int i = bitSize - 1; i >= 1; i--) {
            if (num[i] == 1) {
                number += (int)pow(2, k);
            }
            k += 1;
        }
        if (num[0] == 1) {
            return -number;
        }
        return number;
    }

    void print() {
        for (int i = 0; i < bitSize ; i++) {
            cout << num[i] << " ";
        }
        cout << endl;
    }
};

enum Operations {
    plus,
    minus,
    multiply
};


class Action {
private:
public:
    BinaryNumber sum(BinaryNumber firstNumber, BinaryNumber secondNumber) {
        unsigned short size = firstNumber.getBitSize();
        short add = 0;
        BinaryNumber result(size);
        firstNumber.print();
        secondNumber.print();

        vector<short> binResult(size);
        vector<short> firstBinNumber = firstNumber.getBinNumber();
        vector<short> secondBinNumber = secondNumber.getBinNumber();

        binResult[0] = firstBinNumber[0];
        bool flag = false;
        short additive = 0;
        for (int i = size - 1; i > 0; i--) {
            short sum = firstBinNumber[i] + secondBinNumber[i] + additive;
            if (i == 1 &&  (sum == 2 || sum == 3)) {
                cout << "--- ERROR: OVERFLOW ----" << endl;
                flag = true;
                break;
            }
            if (sum == 1) {
                binResult[i] = 1;
                additive = 0;
            }
            else if (sum == 2) {
                binResult[i] = 0;
                additive = 1;
            }
            else if (sum == 3) {
                binResult[i] = 1;
                additive = 1;
            }
            else {
                binResult[i] = 0;
            }
        }
        
        
        if (flag == false) {
            result.setBinNum(binResult);
            return result;
        }
        else {
            return result;
        }
    }



    BinaryNumber minus(BinaryNumber firstNumber, BinaryNumber secondNumber) {
        unsigned short size = firstNumber.getBitSize();
        BinaryNumber result(size);
        if (firstNumber.getNumber() - secondNumber.getNumber() >= 0) {
            
            short borrow = 0;
            
            firstNumber.print();
            secondNumber.print();

            vector<short> binResult(size);
            vector<short> firstBinNumber = firstNumber.getBinNumber();
            vector<short> secondBinNumber = secondNumber.getBinNumber();

            for (int i = size - 1; i >= 1; i--) {
                short diff = firstBinNumber[i] - secondBinNumber[i] - borrow;

                if (diff < 0) {
                    diff += 2;
                    borrow = 1;
                }
                else {
                    borrow = 0;
                }
                binResult[i] = diff;
            }

            result.setBinNum(binResult);
            return result;
        }
        else {
            result.setNum(firstNumber.getNumber() - secondNumber.getNumber());
            return result;
        }
    }

    BinaryNumber multiply(BinaryNumber firstNumber, BinaryNumber secondNumber) {
        unsigned short size = firstNumber.getBitSize() ;
        BinaryNumber result(size);
        vector<short> binResult(size * 2);
        vector<short> firstBinNumber = firstNumber.getBinNumber();
        vector<short> secondBinNumber = secondNumber.getBinNumber();
        
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                binResult[i + j] += firstBinNumber[size - i - 1] * secondBinNumber[size - j - 1];
              
            }
        }
        
        int addition = 0;
        for (int i = 0; i < size * 2; i++) {
            binResult[i] += addition;
            addition = binResult[i] / 2;
            binResult[i] %= 2;
        }
       
        reverse(binResult.begin(),binResult.end());
        bool flag = false;


        for (int i = 0; i <= size; i++) {
            if (binResult[i] != 0) {
                flag = true;
            }
        }


        if (flag == true) {
            cout << "---ERROR: OVERFLOW ---" << endl;
            return result;
        }
        else {
            vector<short> newResult;
            newResult.push_back(0);
            for (int i = binResult.size() - 1; i > size; i--) {
                newResult.push_back(binResult[i]);
            }
            result.setBinNum(newResult);
            return result;
        }
        
    }
};


class BinaryCalculator {
private:
    Action action;
public:
    BinaryCalculator(BinaryNumber firstNumber, BinaryNumber secondNumber, char operation) {
        if (firstNumber.getBitSize() != secondNumber.getBitSize()) {
            cout << "--- ERROR: BIT SIZE IS NOT EQUAL ---" << endl;
        }
        vector<short> firstBinNumber = firstNumber.getBinNumber();
        vector<short> secondBinNumber = secondNumber.getBinNumber();

        if (operation == Operations::plus) {
            if (firstBinNumber[0] == secondBinNumber[0]) {
                BinaryNumber result = action.sum(firstNumber, secondNumber);
                result.print();
                int ans = result.getNumber();
                cout << ans << endl;
            }
        }
        else if (operation == Operations::minus) {
            if (firstBinNumber[0] == secondBinNumber[0] && firstBinNumber[0] == 0) {
                BinaryNumber result = action.minus(firstNumber, secondNumber);
                result.print();
                int ans = result.getNumber();
                cout << ans << endl;
            }
        }
        else if (operation == Operations::multiply) {
            if (firstBinNumber[0] == secondBinNumber[0] && firstBinNumber[0] == 0) {
                BinaryNumber result = action.multiply(firstNumber, secondNumber);
                result.print();
           
            }
            else if (firstBinNumber[0] == secondBinNumber[0] && firstBinNumber[0] == 1) {
                firstNumber.setZero();
                secondNumber.setZero();
                BinaryNumber result = action.multiply(firstNumber, secondNumber);
                result.print();
            }
            else if (firstBinNumber[0] == 1 || secondBinNumber[0] == 1) {
                firstNumber.setZero();
                secondNumber.setZero();
                BinaryNumber result = action.multiply(firstNumber, secondNumber);
                result.setMinus();
                result.print();
            } 

            
        }
    }
};

int main()
{
    BinaryNumber a(8);
    BinaryNumber b(8);
    a.setNum(4);
    b.setNum(25);
    BinaryCalculator calc(a,b, Operations::multiply);

}
