#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main() {

    // Get input file content : transactions
    std::string inputFile = "../data/transactions.csv";
    
    std::ifstream file(inputFile);
    
    if (!file.is_open()) {
        std::cerr << "Error can't reach to open " << inputFile << std::endl;
        return 1;
    }
    
    std::vector<std::string> transactions;
    
    std::string transaction;
    while (std::getline(file, transaction)) {
        
        transactions.push_back(transaction);
    }
    
    file.close();
    
    // Print transactions
    for (const auto& l : transactions) {
        std::cout << l << std::endl;
    }

    return 0;
}
