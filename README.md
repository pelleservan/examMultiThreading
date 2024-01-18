# Examun Multi Threading - Transaction bancaire

Note : Pour facilité le développement du code et le test des différentes fonction, un échentillion du fichier de données à été créé.

## 1. Step 1 - Get data from input file

1. Read `./src/data/transactions.csv`.
2. Store each row in a `std::vector<std::string>`.

## 2. Step 2 - Verify transaction validity

1. Use the given code te define a validation function.
```c++
bool verifyTransaction(int source, int destination, int amount, int signature) {
    bool transaction_valide = false;
    double signature_verif = source * destination;

    for (int j = 0; j < 20000; j++) {
        if (j % 2 == 2) {
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
