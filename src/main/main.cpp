#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>

struct Transaction {
    int sourceAccount;
    int destinationAccount;
    int amount;
    int signature;
};

struct StatusData {
        int totalTransactionPressed = 0;
        int lastTransactionProssed = 0;
        int validCount = 0;
        int totalValidAmount = 0;
    };

std::vector<Transaction> getData(const std::string& inputFilePath) {
    std::ifstream file(inputFilePath);

    if (!file.is_open()) {
        std::cerr << "Error: Unable to open " << inputFilePath << std::endl;
        return std::vector<Transaction>();
    }

    std::vector<Transaction> transactions;

    std::string line;
    int lineCount = 0;

    while (std::getline(file, line)) {
        if (lineCount > 0) {
            std::stringstream ss(line);
            Transaction transaction;

            if ((ss >> transaction.sourceAccount) &&
                (ss.ignore(1, ';')) &&
                (ss >> transaction.destinationAccount) &&
                (ss.ignore(1, ';')) &&
                (ss >> transaction.amount) &&
                (ss.ignore(5, ';')) &&
                (ss >> transaction.signature)) {
                transactions.push_back(transaction);
            } else {
                std::cerr << "Error parsing line: " << line << std::endl;
            }
        }

        lineCount++;
    }

    file.close();

    return transactions;
}


void printMessageStatus(int refreshTime, StatusData& status) {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(refreshTime));

        float validTransactionRatio = static_cast<float>(status.validCount) / status.totalTransactionPressed;

        std::cout << "Status..." << std::endl;
        std::cout << "Last transaction prossed : " << status.lastTransactionProssed << std::endl;
        std::cout << "Unvalide transaction : " << status.totalTransactionPressed - status.validCount << " / " << status.totalTransactionPressed << " = "  << validTransactionRatio << "%" <<std::endl;
        std::cout << "Total transaction prossed : " << status.totalTransactionPressed << std::endl;
        if (status.validCount > 0) {
            std::cout << "Average amount : " << status.totalValidAmount / status.validCount << std::endl;
        }
        else {
            std::cout << "Average amount : 0" << std::endl;
        }
        std::cout << "------------------------" << std::endl;

        status.lastTransactionProssed = 0;
    }
}

void printTransaction(Transaction transaction) {
    std::cout << "Source Account: " << transaction.sourceAccount << std::endl;
    std::cout << "Destination Account: " << transaction.destinationAccount << std::endl;
    std::cout << "Amount: " << transaction.amount << std::endl;
    std::cout << "Signature: " << transaction.signature << std::endl;
    std::cout << "------------------------" << std::endl;
}

bool verifyTransaction(int source, int destination, int amount, int signature) {
    bool transaction_valide = false;
    double signature_verif = source * destination;

    for (int j = 0; j < 20000; j++) {
        if (j % 2 == 0) {
            signature_verif -= j / 32;
            signature_verif += 1;
        }
        else {
            signature_verif += amount / 32;
        }
    }
    if (signature == signature_verif) {
        transaction_valide = true;
    }

    // std::cout << signature << " == " << signature_verif << std::endl;

    return transaction_valide;
}

void processTransaction(Transaction transaction, StatusData& status) {
    int source = transaction.sourceAccount;
    int destination = transaction.destinationAccount;
    int amount = transaction.amount;
    int signature = transaction.signature;

    printTransaction(transaction);

    bool transactionCorrect = verifyTransaction(source, destination, amount, signature);

    if (transactionCorrect) {
        status.validCount++;
        status.totalValidAmount += transaction.amount;
    }

    status.totalTransactionPressed++;
    status.lastTransactionProssed++;
}

int main() {
    int refreshTime = 1;

    StatusData status;
    
    std::vector<Transaction> transactions = getData("../data/transactions.csv");

    std::thread messageThread(printMessageStatus, refreshTime, std::ref(status));

    for (const auto& transaction : transactions) {
        processTransaction(transaction, status);
    }
    
    messageThread.join();

    return 0;
}
