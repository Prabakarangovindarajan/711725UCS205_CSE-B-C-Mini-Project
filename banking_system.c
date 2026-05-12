// Complete Banking Management System
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Structure definitions
struct clientData {
    unsigned int acctNum;      // account number
    char lastName[15];         // last name
    char firstName[10];        // first name
    double balance;            // account balance
    char acctStatus;           // 'A' for active, 'D' for deleted
};

struct transaction {
    unsigned int acctNum;      // account number
    char transType[20];        // type of transaction (DEPOSIT, WITHDRAWAL, etc.)
    double amount;             // transaction amount
    double balanceAfter;       // balance after transaction
    char timestamp[30];        // timestamp of transaction
};

// Function prototypes
unsigned int enterChoice(void);
void displayMenu(void);
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);
void showTransactionHistory(void);
void logTransaction(const struct clientData *client, const char *action, double amount, double balanceAfter);
int compareAccounts(const void *a, const void *b);

// New features
void searchByLastName(FILE *fPtr);
void searchByAccountNumber(FILE *fPtr);
void exportAccountHistory(unsigned int accountNum);
void applyInterest(FILE *fPtr);
void undoLastTransaction(FILE *fPtr);
void displayAllAccounts(FILE *fPtr);
void depositMoney(FILE *fPtr);
void withdrawMoney(FILE *fPtr);
void getTimestamp(char *buffer, size_t size);

// Main program
int main(int argc, char *argv[]) {
    FILE *cfPtr;               // credit.dat file pointer
    unsigned int choice;       // user's choice

    // Create credit.dat file if it doesn't exist
    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL) {
        if ((cfPtr = fopen("credit.dat", "wb+")) == NULL) {
            printf("Error: Could not create credit.dat file\n");
            return -1;
        }
    }

    printf("\n========================================\n");
    printf("  WELCOME TO BANKING MANAGEMENT SYSTEM\n");
    printf("========================================\n");

    while ((choice = enterChoice()) != 11) {
        switch (choice) {
        case 1:
            textFile(cfPtr);
            break;
        case 2:
            updateRecord(cfPtr);
            break;
        case 3:
            newRecord(cfPtr);
            break;
        case 4:
            deleteRecord(cfPtr);
            break;
        case 5:
            showTransactionHistory();
            break;
        case 6:
            searchByLastName(cfPtr);
            break;
        case 7:
            searchByAccountNumber(cfPtr);
            break;
        case 8:
            displayAllAccounts(cfPtr);
            break;
        case 9:
            depositMoney(cfPtr);
            break;
        case 10:
            withdrawMoney(cfPtr);
            break;
        default:
            printf("Incorrect choice. Please try again.\n");
            break;
        }
    }

    printf("\nThank you for using Banking Management System!\n");
    fclose(cfPtr);
    return 0;
}

// Display menu options
void displayMenu(void) {
    printf("%s", "\n======== MAIN MENU ========\n"
                 "1 - Store sorted accounts to accounts.txt\n"
                 "2 - Update existing account\n"
                 "3 - Create new account\n"
                 "4 - Delete account\n"
                 "5 - Display all transaction history\n"
                 "6 - Search account by last name\n"
                 "7 - Search account by account number\n"
                 "8 - Display all accounts\n"
                 "9 - Deposit money\n"
                 "10 - Withdraw money\n"
                 "11 - Exit program\n"
                 "Enter choice (1-11): ");
}

// Get user's menu choice
unsigned int enterChoice(void) {
    unsigned int menuChoice;
    int valid;

    do {
        displayMenu();
        valid = scanf("%u", &menuChoice);

        if (valid != 1) {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n');
            valid = 0;
        } else if (menuChoice < 1 || menuChoice > 11) {
            printf("Invalid choice! Please enter 1-11.\n");
            valid = 0;
        } else {
            // Clear input buffer
            while (getchar() != '\n');
        }
    } while (valid != 1 || menuChoice < 1 || menuChoice > 11);

    return menuChoice;
}

// Get current timestamp
void getTimestamp(char *buffer, size_t size) {
    time_t now = time(NULL);
    struct tm *timeinfo = localtime(&now);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", timeinfo);
}

// Log transaction to file
void logTransaction(const struct clientData *client, const char *action, 
                    double amount, double balanceAfter) {
    FILE *tPtr = fopen("transactions.txt", "a");
    char timestamp[30];

    if (tPtr == NULL) {
        printf("Error: Could not open transactions.txt\n");
        return;
    }

    getTimestamp(timestamp, sizeof(timestamp));
    fprintf(tPtr, "[%s] Account: %u | Name: %s %s | Action: %s | Amount: %.2f | Balance: %.2f\n",
            timestamp, client->acctNum, client->firstName, client->lastName, 
            action, amount, balanceAfter);

    fclose(tPtr);
}

// Compare function for qsort
int compareAccounts(const void *a, const void *b) {
    const struct clientData *acctA = (const struct clientData *)a;
    const struct clientData *acctB = (const struct clientData *)b;

    // Only compare active accounts
    if (acctA->acctNum == 0) return 1;
    if (acctB->acctNum == 0) return -1;

    return acctA->acctNum - acctB->acctNum;
}

// Create sorted text file of accounts
void textFile(FILE *readPtr) {
    FILE *writePtr;
    struct clientData client;
    int recordCount = 0;
    struct clientData *clients = NULL;

    // Count active records
    rewind(readPtr);
    while (fread(&client, sizeof(struct clientData), 1, readPtr) == 1) {
        if (client.acctNum != 0 && client.acctStatus != 'D') {
            recordCount++;
        }
    }

    if (recordCount == 0) {
        printf("No active accounts to export.\n");
        return;
    }

    // Allocate memory for accounts
    clients = (struct clientData *)malloc(recordCount * sizeof(struct clientData));
    if (clients == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    // Read accounts into memory
    rewind(readPtr);
    int index = 0;
    while (fread(&client, sizeof(struct clientData), 1, readPtr) == 1) {
        if (client.acctNum != 0 && client.acctStatus != 'D') {
            clients[index++] = client;
        }
    }

    // Sort accounts by account number
    qsort(clients, recordCount, sizeof(struct clientData), compareAccounts);

    // Write to text file
    writePtr = fopen("accounts.txt", "w");
    if (writePtr == NULL) {
        printf("Error: Could not open accounts.txt\n");
        free(clients);
        return;
    }

    fprintf(writePtr, "%-10s %-15s %-10s %s\n", "Account#", "LastName", "FirstName", "Balance");
    fprintf(writePtr, "%-10s %-15s %-10s %s\n", "----------", "---------------", "----------", "----------");

    for (int i = 0; i < recordCount; i++) {
        fprintf(writePtr, "%-10u %-15s %-10s $%.2f\n",
                clients[i].acctNum, clients[i].lastName, clients[i].firstName, clients[i].balance);
    }

    fclose(writePtr);
    printf("Sorted account data exported to accounts.txt successfully!\n");
    printf("Total active accounts: %d\n", recordCount);

    free(clients);
}

// Display all accounts
void displayAllAccounts(FILE *fPtr) {
    struct clientData client;
    int count = 0;

    rewind(fPtr);
    printf("\n========== ALL ACTIVE ACCOUNTS ==========\n");
    printf("%-10s %-15s %-10s %s\n", "Account#", "LastName", "FirstName", "Balance");
    printf("%-10s %-15s %-10s %s\n", "----------", "---------------", "----------", "----------");

    while (fread(&client, sizeof(struct clientData), 1, fPtr) == 1) {
        if (client.acctNum != 0 && client.acctStatus != 'D') {
            printf("%-10u %-15s %-10s $%.2f\n",
                   client.acctNum, client.lastName, client.firstName, client.balance);
            count++;
        }
    }

    if (count == 0) {
        printf("No active accounts found.\n");
    } else {
        printf("=========================================\n");
        printf("Total active accounts: %d\n", count);
    }
}

// Update existing record
void updateRecord(FILE *fPtr) {
    unsigned int accountNum;
    struct clientData client;
    int found = 0;

    printf("\nEnter account number to update: ");
    scanf("%u", &accountNum);
    while (getchar() != '\n');

    rewind(fPtr);
    while (fread(&client, sizeof(struct clientData), 1, fPtr) == 1) {
        if (client.acctNum == accountNum && client.acctStatus != 'D') {
            printf("\nCurrent account details:\n");
            printf("Name: %s %s\n", client.firstName, client.lastName);
            printf("Balance: $%.2f\n", client.balance);

            printf("\nEnter new last name (or press Enter to skip): ");
            char tempLastName[15];
            if (fgets(tempLastName, sizeof(tempLastName), stdin) != NULL) {
                if (tempLastName[0] != '\n') {
                    // Remove newline
                    tempLastName[strcspn(tempLastName, "\n")] = '\0';
                    strcpy(client.lastName, tempLastName);
                }
            }

            printf("Enter new first name (or press Enter to skip): ");
            char tempFirstName[10];
            if (fgets(tempFirstName, sizeof(tempFirstName), stdin) != NULL) {
                if (tempFirstName[0] != '\n') {
                    // Remove newline
                    tempFirstName[strcspn(tempFirstName, "\n")] = '\0';
                    strcpy(client.firstName, tempFirstName);
                }
            }

            printf("Enter new balance (or enter -1 to skip): ");
            double newBalance;
            scanf("%lf", &newBalance);
            while (getchar() != '\n');

            if (newBalance >= 0) {
                double oldBalance = client.balance;
                client.balance = newBalance;
                logTransaction(&client, "UPDATE", newBalance - oldBalance, client.balance);
            }

            // Write updated record back
            fseek(fPtr, -sizeof(struct clientData), SEEK_CUR);
            fwrite(&client, sizeof(struct clientData), 1, fPtr);
            printf("\nAccount updated successfully!\n");
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Account not found or has been deleted.\n");
    }
}

// Add new account
void newRecord(FILE *fPtr) {
    struct clientData client = {0};

    printf("\nEnter account number: ");
    scanf("%u", &client.acctNum);
    while (getchar() != '\n');

    if (client.acctNum == 0) {
        printf("Invalid account number. Account numbers must be greater than 0.\n");
        return;
    }

    // Check if account already exists
    struct clientData temp;
    rewind(fPtr);
    while (fread(&temp, sizeof(struct clientData), 1, fPtr) == 1) {
        if (temp.acctNum == client.acctNum && temp.acctStatus != 'D') {
            printf("Account already exists!\n");
            return;
        }
    }

    printf("Enter last name: ");
    fgets(client.lastName, sizeof(client.lastName), stdin);
    client.lastName[strcspn(client.lastName, "\n")] = '\0';

    printf("Enter first name: ");
    fgets(client.firstName, sizeof(client.firstName), stdin);
    client.firstName[strcspn(client.firstName, "\n")] = '\0';

    printf("Enter initial balance: ");
    scanf("%lf", &client.balance);
    while (getchar() != '\n');

    client.acctStatus = 'A';  // Mark as active

    fseek(fPtr, 0, SEEK_END);
    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("\nNew account created successfully!\n");
    printf("Account Number: %u\n", client.acctNum);
    printf("Name: %s %s\n", client.firstName, client.lastName);
    printf("Initial Balance: $%.2f\n", client.balance);

    logTransaction(&client, "NEW_ACCOUNT", client.balance, client.balance);
}

// Delete account (mark as deleted)
void deleteRecord(FILE *fPtr) {
    unsigned int accountNum;
    struct clientData client;
    int found = 0;

    printf("\nEnter account number to delete: ");
    scanf("%u", &accountNum);
    while (getchar() != '\n');

    rewind(fPtr);
    while (fread(&client, sizeof(struct clientData), 1, fPtr) == 1) {
        if (client.acctNum == accountNum && client.acctStatus != 'D') {
            printf("\nConfirm deletion of account %u (%s %s)?\n", 
                   accountNum, client.firstName, client.lastName);
            printf("Enter 'Y' to confirm or any other key to cancel: ");
            
            char confirm;
            scanf("%c", &confirm);
            while (getchar() != '\n');

            if (confirm == 'Y' || confirm == 'y') {
                client.acctStatus = 'D';  // Mark as deleted
                fseek(fPtr, -sizeof(struct clientData), SEEK_CUR);
                fwrite(&client, sizeof(struct clientData), 1, fPtr);
                printf("Account deleted successfully!\n");
                logTransaction(&client, "DELETE", 0, client.balance);
            } else {
                printf("Deletion cancelled.\n");
            }
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Account not found or already deleted.\n");
    }
}

// Display transaction history
void showTransactionHistory(void) {
    FILE *historyPtr;
    char line[256];

    historyPtr = fopen("transactions.txt", "r");
    if (historyPtr == NULL) {
        printf("No transaction history available yet.\n");
        return;
    }

    printf("\n========== TRANSACTION HISTORY ==========\n");
    while (fgets(line, sizeof(line), historyPtr) != NULL) {
        printf("%s", line);
    }
    printf("=========================================\n");

    fclose(historyPtr);
}

// Search by last name
void searchByLastName(FILE *fPtr) {
    char searchName[15];
    struct clientData client;
    int found = 0;

    printf("\nEnter last name to search: ");
    fgets(searchName, sizeof(searchName), stdin);
    searchName[strcspn(searchName, "\n")] = '\0';

    printf("\n========== SEARCH RESULTS ==========\n");
    rewind(fPtr);
    while (fread(&client, sizeof(struct clientData), 1, fPtr) == 1) {
        if (client.acctNum != 0 && client.acctStatus != 'D' && 
            strcmp(client.lastName, searchName) == 0) {
            printf("Account#: %u\n", client.acctNum);
            printf("Name: %s %s\n", client.firstName, client.lastName);
            printf("Balance: $%.2f\n\n", client.balance);
            found = 1;
        }
    }

    if (!found) {
        printf("No active accounts found with last name '%s'.\n", searchName);
    }
    printf("====================================\n");
}

// Search by account number
void searchByAccountNumber(FILE *fPtr) {
    unsigned int accountNum;
    struct clientData client;
    int found = 0;

    printf("\nEnter account number to search: ");
    scanf("%u", &accountNum);
    while (getchar() != '\n');

    printf("\n========== SEARCH RESULTS ==========\n");
    rewind(fPtr);
    while (fread(&client, sizeof(struct clientData), 1, fPtr) == 1) {
        if (client.acctNum == accountNum && client.acctStatus != 'D') {
            printf("Account#: %u\n", client.acctNum);
            printf("Name: %s %s\n", client.firstName, client.lastName);
            printf("Balance: $%.2f\n\n", client.balance);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Account not found or has been deleted.\n");
    }
    printf("====================================\n");
}

// Deposit money into account
void depositMoney(FILE *fPtr) {
    unsigned int accountNum;
    double depositAmount;
    struct clientData client;
    int found = 0;

    printf("\nEnter account number: ");
    scanf("%u", &accountNum);
    while (getchar() != '\n');

    printf("Enter deposit amount: $");
    scanf("%lf", &depositAmount);
    while (getchar() != '\n');

    if (depositAmount <= 0) {
        printf("Invalid deposit amount!\n");
        return;
    }

    rewind(fPtr);
    while (fread(&client, sizeof(struct clientData), 1, fPtr) == 1) {
        if (client.acctNum == accountNum && client.acctStatus != 'D') {
            double oldBalance = client.balance;
            client.balance += depositAmount;

            fseek(fPtr, -sizeof(struct clientData), SEEK_CUR);
            fwrite(&client, sizeof(struct clientData), 1, fPtr);

            printf("\n========== DEPOSIT CONFIRMATION ==========\n");
            printf("Account#: %u\n", client.acctNum);
            printf("Name: %s %s\n", client.firstName, client.lastName);
            printf("Previous Balance: $%.2f\n", oldBalance);
            printf("Deposit Amount: $%.2f\n", depositAmount);
            printf("New Balance: $%.2f\n", client.balance);
            printf("=========================================\n");

            logTransaction(&client, "DEPOSIT", depositAmount, client.balance);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Account not found or has been deleted.\n");
    }
}

// Withdraw money from account
void withdrawMoney(FILE *fPtr) {
    unsigned int accountNum;
    double withdrawAmount;
    struct clientData client;
    int found = 0;

    printf("\nEnter account number: ");
    scanf("%u", &accountNum);
    while (getchar() != '\n');

    printf("Enter withdrawal amount: $");
    scanf("%lf", &withdrawAmount);
    while (getchar() != '\n');

    if (withdrawAmount <= 0) {
        printf("Invalid withdrawal amount!\n");
        return;
    }

    rewind(fPtr);
    while (fread(&client, sizeof(struct clientData), 1, fPtr) == 1) {
        if (client.acctNum == accountNum && client.acctStatus != 'D') {
            if (client.balance < withdrawAmount) {
                printf("\nInsufficient balance!\n");
                printf("Current Balance: $%.2f\n", client.balance);
                printf("Requested Withdrawal: $%.2f\n", withdrawAmount);
                found = 1;
                break;
            }

            double oldBalance = client.balance;
            client.balance -= withdrawAmount;

            fseek(fPtr, -sizeof(struct clientData), SEEK_CUR);
            fwrite(&client, sizeof(struct clientData), 1, fPtr);

            printf("\n========== WITHDRAWAL CONFIRMATION ==========\n");
            printf("Account#: %u\n", client.acctNum);
            printf("Name: %s %s\n", client.firstName, client.lastName);
            printf("Previous Balance: $%.2f\n", oldBalance);
            printf("Withdrawal Amount: $%.2f\n", withdrawAmount);
            printf("New Balance: $%.2f\n", client.balance);
            printf("=============================================\n");

            logTransaction(&client, "WITHDRAWAL", withdrawAmount, client.balance);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Account not found or has been deleted.\n");
    }
}
