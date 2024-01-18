# Examun Multi Threading - Transaction bancaire

Note : Pour facilité le développement du code et le test des différentes fonction, un échentillion du fichier de données à été créé.

## 1. Step 1 - Get data from input file

1. Read `./src/data/transactions.csv`.
2. Store each row in a `std::vector<std::string>`.
3. Store each row data in a struct :
```c++
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
```

## 2. Step 2 - Verify transaction validity

1. Use the given code te define a validation function :
```c++
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

    return transaction_valide;
}
```

## 3. Step 3 - Start to processed transaction

1. To prossed transaction I developed a function whish will be execute for each transaction :
```c++
void processTransaction(Transaction transaction, StatusData& status) {
    int source = transaction.sourceAccount;
    int destination = transaction.destinationAccount;
    float amount = transaction.amount;
    float signature = transaction.signature;
}
```

2. We declare a `struct` to define a transaction :
```c++
struct Transaction {
    int sourceAccount;
    int destinationAccount;
    int amount;
    int signature;
};
```
3. We declare a `struct` to store all status data :
```c++
struct StatusData {
        int totalTransactionPressed = 0;
        int lastTransactionProssed = 0;
        int validCount = 0;
        int totalValidAmount = 0;
        ...
    };
```

4. We read all transaction in `main()` and run the above function.
```c++
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
```

## 4. Step 4 - Displays status data

1. To display status data we develop a function :
```c++
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
```

2. We use a dedictae thread to run this function.
