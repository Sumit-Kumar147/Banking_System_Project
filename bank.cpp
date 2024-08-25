 #include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
using namespace std;

class BankAccount {
public:
    int accountNumber;
    string accountHolderName;
    double balance;

    BankAccount() : accountNumber(0), accountHolderName(""), balance(0.0) {
        // Ensure the files exist when the program starts
        createFileIfNotExists("accounts.txt");
        createFileIfNotExists("id.txt");
    }

    void createAccount() {
        cout << "Enter account holder's name: ";
        cin.ignore();
        getline(cin, accountHolderName);
        cout << "Enter initial balance: ";
        cin >> balance;
        accountNumber = generateAccountNumber();

        ofstream fout("accounts.txt", ios::app);
        if (!fout) {
            cerr << "Error opening file for writing. Check file permissions and path." << endl;
            throw runtime_error("Error opening file for writing.");
        }

        fout << accountNumber << "\n" << accountHolderName << "\n" << balance << "\n";
        fout.close();

        ofstream idFile("id.txt");
        if (!idFile) {
            cerr << "Error opening file for writing ID. Check file permissions and path." << endl;
            throw runtime_error("Error opening file for writing ID.");
        }
        idFile << accountNumber;
        idFile.close();

        cout << "Account created successfully with account number: " << accountNumber << endl;
    }

    void deposit() {
        int accNumber;
        double amount;
        cout << "Enter account number: ";
        cin >> accNumber;
        cout << "Enter amount to deposit: ";
        cin >> amount;

        ifstream fin("accounts.txt");
        if (!fin) {
            cerr << "Error opening file for reading. Check file permissions and path." << endl;
            throw runtime_error("Error opening file for reading.");
        }

        ofstream fout("temp.txt");
        if (!fout) {
            cerr << "Error opening file for writing. Check file permissions and path." << endl;
            throw runtime_error("Error opening file for writing.");
        }

        bool found = false;
        while (fin >> accountNumber) {
            fin.ignore();
            getline(fin, accountHolderName);
            fin >> balance;
            fin.ignore();

            if (accNumber == accountNumber) {
                balance += amount;
                found = true;
            }

            fout << accountNumber << "\n" << accountHolderName << "\n" << balance << "\n";
        }

        fin.close();
        fout.close();
        remove("accounts.txt");
        rename("temp.txt", "accounts.txt");

        if (found) {
            cout << "Amount deposited successfully." << endl;
        } else {
            cout << "Account not found." << endl;
        }
    }

    void withdraw() {
        int accNumber;
        double amount;
        cout << "Enter account number: ";
        cin >> accNumber;
        cout << "Enter amount to withdraw: ";
        cin >> amount;

        ifstream fin("accounts.txt");
        if (!fin) {
            cerr << "Error opening file for reading. Check file permissions and path." << endl;
            throw runtime_error("Error opening file for reading.");
        }

        ofstream fout("temp.txt");
        if (!fout) {
            cerr << "Error opening file for writing. Check file permissions and path." << endl;
            throw runtime_error("Error opening file for writing.");
        }

        bool found = false;
        while (fin >> accountNumber) {
            fin.ignore();
            getline(fin, accountHolderName);
            fin >> balance;
            fin.ignore();

            if (accNumber == accountNumber) {
                if (balance >= amount) {
                    balance -= amount;
                    found = true;
                } else {
                    cout << "Insufficient funds." << endl;
                    return;
                }
            }

            fout << accountNumber << "\n" << accountHolderName << "\n" << balance << "\n";
        }

        fin.close();
        fout.close();
        remove("accounts.txt");
        rename("temp.txt", "accounts.txt");

        if (found) {
            cout << "Amount withdrawn successfully." << endl;
        } else {
            cout << "Account not found." << endl;
        }
    }

    void displayAccount() {
        int accNumber;
        cout << "Enter account number: ";
        cin >> accNumber;

        ifstream fin("accounts.txt");
        if (!fin) {
            cerr << "Error opening file for reading. Check file permissions and path." << endl;
            throw runtime_error("Error opening file for reading.");
        }

        bool found = false;
        while (fin >> accountNumber) {
            fin.ignore();
            getline(fin, accountHolderName);
            fin >> balance;
            fin.ignore();

            if (accNumber == accountNumber) {
                cout << "Account Number: " << accountNumber << endl;
                cout << "Account Holder Name: " << accountHolderName << endl;
                cout << "Balance: " << balance << endl;
                found = true;
                break;
            }
        }

        fin.close();

        if (!found) {
            cout << "Account not found." << endl;
        }
    }

    void deleteAccount() {
        int accNumber;
        cout << "Enter account number to delete: ";
        cin >> accNumber;

        ifstream fin("accounts.txt");
        if (!fin) {
            cerr << "Error opening file for reading. Check file permissions and path." << endl;
            throw runtime_error("Error opening file for reading.");
        }

        ofstream fout("temp.txt");
        if (!fout) {
            cerr << "Error opening file for writing. Check file permissions and path." << endl;
            throw runtime_error("Error opening file for writing.");
        }

        bool found = false;
        while (fin >> accountNumber) {
            fin.ignore();
            getline(fin, accountHolderName);
            fin >> balance;
            fin.ignore();

            if (accNumber != accountNumber) {
                fout << accountNumber << "\n" << accountHolderName << "\n" << balance << "\n";
            } else {
                found = true;
            }
        }

        fin.close();
        fout.close();
        remove("accounts.txt");
        rename("temp.txt", "accounts.txt");

        if (found) {
            cout << "Account deleted successfully." << endl;
        } else {
            cout << "Account not found." << endl;
        }
    }

private:
    void createFileIfNotExists(const string& filename) {
        ifstream file(filename);
        if (!file) {
            ofstream outfile(filename);
            if (!outfile) {
                cerr << "Error creating file: " << filename << endl;
                throw runtime_error("Error creating file.");
            }
            outfile.close();
        }
    }

    int generateAccountNumber() {
        ifstream fin("accounts.txt");
        if (!fin) {
            cerr << "Error opening file for reading. Check file permissions and path." << endl;
            throw runtime_error("Error opening file for reading.");
        }

        int num = 0;
        while (fin >> accountNumber) {
            fin.ignore();
            getline(fin, accountHolderName);
            fin >> balance;
            fin.ignore();

            if (accountNumber > num) {
                num = accountNumber;
            }
        }

        fin.close();
        return num + 1;
    }
};

int main() {
    BankAccount account;
    int choice;

    while (true) {
        cout << "Banking System Menu:" << endl;
        cout << "1. Create Account" << endl;
        cout << "2. Deposit" << endl;
        cout << "3. Withdraw" << endl;
        cout << "4. Display Account" << endl;
        cout << "5. Delete Account" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        try {
            switch (choice) {
                case 1:
                    account.createAccount();
                    break;
                case 2:
                    account.deposit();
                    break;
                case 3:
                    account.withdraw();
                    break;
                case 4:
                    account.displayAccount();
                    break;
                case 5:
                    account.deleteAccount();
                    break;
                   case 6:
                    return 0;
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }
        } catch (const runtime_error& e) {
            cerr << "Exception: " << e.what() << endl;
        }
    }
}
