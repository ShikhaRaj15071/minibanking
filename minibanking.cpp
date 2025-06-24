#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

class Account {
    int accNo;
    char name[50];
    char type;
    double balance;

public:
    void createAccount();
    void showAccount() const;
    void modify();
    void deposit(double);
    void withdraw(double);
    int getAccNo() const;
    double getBalance() const;
    char getType() const;
    void report() const;
};

// ---------------- Method Definitions ----------------

void Account::createAccount() {
    cout << "Enter Account No: ";
    cin >> accNo;
    cin.ignore();
    cout << "Enter Name: ";
    cin.getline(name, 50);
    cout << "Enter Account Type (C/S): ";
    cin >> type;
    type = toupper(type);
    cout << "Enter Initial Balance (min 500 for Savings, 1000 for Current): ";
    cin >> balance;
    cout << "Account Created.\n";
}

void Account::showAccount() const {
    cout << "Account No: " << accNo << endl;
    cout << "Name: " << name << endl;
    cout << "Type: " << type << endl;
    cout << "Balance: " << balance << endl;
}

void Account::modify() {
    cout << "Modify Name: ";
    cin.ignore();
    cin.getline(name, 50);
    cout << "Modify Type (C/S): ";
    cin >> type;
    type = toupper(type);
    cout << "Modify Balance: ";
    cin >> balance;
}

void Account::deposit(double amt) {
    balance += amt;
}

void Account::withdraw(double amt) {
    balance -= amt;
}

int Account::getAccNo() const {
    return accNo;
}

double Account::getBalance() const {
    return balance;
}

char Account::getType() const {
    return type;
}

void Account::report() const {
    cout << setw(10) << accNo
         << setw(20) << name
         << setw(8) << type
         << setw(12) << balance << endl;
}

// ---------------- File Functions ----------------

void writeAccount() {
    Account acc;
    ofstream outFile("bank.dat", ios::binary | ios::app);
    acc.createAccount();
    outFile.write(reinterpret_cast<char *>(&acc), sizeof(Account));
    outFile.close();
}

void displayAccount(int n) {
    Account acc;
    ifstream inFile("bank.dat", ios::binary);
    bool found = false;

    while (inFile.read(reinterpret_cast<char *>(&acc), sizeof(Account))) {
        if (acc.getAccNo() == n) {
            acc.showAccount();
            found = true;
            break;
        }
    }
    inFile.close();

    if (!found)
        cout << "Account not found.\n";
}

void modifyAccount(int n) {
    Account acc;
    fstream file("bank.dat", ios::binary | ios::in | ios::out);
    bool found = false;

    while (!file.eof() && !found) {
        streampos pos = file.tellg();
        file.read(reinterpret_cast<char *>(&acc), sizeof(Account));
        if (acc.getAccNo() == n) {
            acc.showAccount();
            cout << "\nEnter new details:\n";
            acc.modify();
            file.seekp(pos);
            file.write(reinterpret_cast<char *>(&acc), sizeof(Account));
            cout << "Account Updated.\n";
            found = true;
        }
    }
    file.close();

    if (!found)
        cout << "Account not found.\n";
}

void deleteAccount(int n) {
    Account acc;
    ifstream inFile("bank.dat", ios::binary);
    ofstream outFile("temp.dat", ios::binary);
    bool found = false;

    while (inFile.read(reinterpret_cast<char *>(&acc), sizeof(Account))) {
        if (acc.getAccNo() != n)
            outFile.write(reinterpret_cast<char *>(&acc), sizeof(Account));
        else
            found = true;
    }
    inFile.close();
    outFile.close();

    remove("bank.dat");
    rename("temp.dat", "bank.dat");

    if (found)
        cout << "Account Deleted.\n";
    else
        cout << "Account not found.\n";
}

void displayAll() {
    Account acc;
    ifstream inFile("bank.dat", ios::binary);
    cout << "\n\nACCOUNT LIST\n";
    cout << setw(10) << "Acc No" << setw(20) << "Name"
         << setw(8) << "Type" << setw(12) << "Balance\n";

    while (inFile.read(reinterpret_cast<char *>(&acc), sizeof(Account))) {
        acc.report();
    }
    inFile.close();
}

void depositWithdraw(int n, int option) {
    Account acc;
    fstream file("bank.dat", ios::binary | ios::in | ios::out);
    bool found = false;

    while (!file.eof() && !found) {
        streampos pos = file.tellg();
        file.read(reinterpret_cast<char *>(&acc), sizeof(Account));
        if (acc.getAccNo() == n) {
            acc.showAccount();
            double amt;
            if (option == 1) {
                cout << "Enter amount to deposit: ";
                cin >> amt;
                acc.deposit(amt);
            } else {
                cout << "Enter amount to withdraw: ";
                cin >> amt;
                if (acc.getBalance() - amt < (acc.getType() == 'S' ? 500 : 1000))
                    cout << "Insufficient balance.\n";
                else
                    acc.withdraw(amt);
            }
            file.seekp(pos);
            file.write(reinterpret_cast<char *>(&acc), sizeof(Account));
            cout << "Transaction Complete.\n";
            found = true;
        }
    }
    file.close();

    if (!found)
        cout << "Account not found.\n";
}

// ---------------- Main Menu ----------------

int main() {
    int choice, accNo;

    do {
        cout << "\n\n*** BANKING SYSTEM MENU ***\n";
        cout << "1. Create Account\n";
        cout << "2. Deposit Amount\n";
        cout << "3. Withdraw Amount\n";
        cout << "4. Balance Inquiry\n";
        cout << "5. All Account Holder List\n";
        cout << "6. Close Account\n";
        cout << "7. Modify Account\n";
        cout << "8. Exit\n";
        cout << "Select Option (1-8): ";
        cin >> choice;

        switch (choice) {
            case 1: writeAccount(); break;
            case 2:
                cout << "Enter Account No: "; cin >> accNo;
                depositWithdraw(accNo, 1); break;
            case 3:
                cout << "Enter Account No: "; cin >> accNo;
                depositWithdraw(accNo, 2); break;
            case 4:
                cout << "Enter Account No: "; cin >> accNo;
                displayAccount(accNo); break;
            case 5: displayAll(); break;
            case 6:
                cout << "Enter Account No: "; cin >> accNo;
                deleteAccount(accNo); break;
            case 7:
                cout << "Enter Account No: "; cin >> accNo;
                modifyAccount(accNo); break;
            case 8: cout << "Exiting..."; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 8);

    return 0;
}
