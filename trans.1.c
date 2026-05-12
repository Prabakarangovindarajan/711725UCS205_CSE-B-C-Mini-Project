// Bank-account program with extended features
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// clientData structure definition
struct clientData {
    unsigned int acctNum; // account number
    char lastName[15];    // account last name
    char firstName[10];   // account first name
    double balance;       // account balance
};

// prototypes
unsigned int enterChoice(void);
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);
void showTransactionHistory(void);
void logTransaction(const struct clientData *client, const char *action, double amount);
int compareAccounts(const void *a, const void *b);

// new features
void searchByLastName(FILE *fPtr);
void exportAccountHistory(unsigned int accountNum);
void applyInterest(FILE *fPtr);
void undoLastTransaction(FILE *fPtr);

int main(void) {
    FILE *cfPtr;         // credit.dat file pointer
    unsigned int choice; // user's choice

    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL) {
        printf("Error: File could not be opened.\n");
        exit(-1);
    }

    while ((choice = enterChoice()) != 6) {
        switch (choice) {
        case 1: textFile(cfPtr); break;
        case 2: updateRecord(cfPtr); break;
        case 3: newRecord(cfPtr); break;
        case 4: deleteRecord(cfPtr); break;
        case 5: showTransactionHistory(); break;
        case 7: searchByLastName(cfPtr); break;
        case 8: {
            unsigned int acct;
            printf("Enter account number: ");
            scanf("%u", &acct);
            exportAccountHistory(acct);
            break;
        }
        case 9: applyInterest(cfPtr); break;
        case 10: undoLastTransaction(cfPtr); break;
        default: puts("Incorrect choice"); break;
        }
    }

    fclose(cfPtr);
}

// Compare function for qsort
int compareAccounts(const void *a, const void *b) {
    const struct clientData *acctA = (const struct clientData *)a;
    const struct clientData *acctB = (const struct clientData *)b;
    return acctA->acctNum - acctB->acctNum;
}

// Log transaction to file
void logTransaction(const struct clientData *client, const char *action, double amount) {
    FILE *tPtr = fopen("transactions.txt", "a");
    if (tPtr == NULL) {
        printf("Error opening transactions.txt\n");
        return;
    }
    fprintf(tPtr, "%s %u %s %s amount %.2f\n", action, client->acctNum, 
            client->lastName, client->firstName, amount);
    fclose(tPtr);
}

// Create sorted text file of accounts
void textFile(FILE *readPtr) {
    FILE *writePtr;
    struct clientData client;
    int recordCount = 0;
    struct clientData *clients = NULL;

    // Count records
    rewind(readPtr);
    while (fread(&client, sizeof(struct clientData), 1, readPtr) == 1) {
        if (client.acctNum != 0) {
            recordCount++;
        }
    }

    if (recordCount == 0) {
        printf("No accounts to export.\n");
        return;
    }

    // Allocate memory
    clients = (struct clientData *)malloc(recordCount * sizeof(struct clientData));
    if (clients == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    // Read into array
    rewind(readPtr);
    int index = 0;
    while (fread(&client, sizeof(struct clientData), 1, readPtr) == 1) {
        if (client.acctNum != 0) {
            clients[index++] = client;
        }
    }

    // Sort accounts
    qsort(clients, recordCount, sizeof(struct clientData), compareAccounts);

    // Write to file
    writePtr = fopen("accounts.txt", "w");
    if (writePtr == NULL) {
        printf("Error opening accounts.txt\n");
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
    printf("Sorted account data exported to accounts.txt\n");
    free(clients);
}

// Update existing record
void updateRecord(FILE *fPtr) {
    unsigned int accountNum;
    struct clientData client;
    int found = 0;

    printf("Enter account number to update: ");
    scanf("%u", &accountNum);

    rewind(fPtr);
    while (fread(&client, sizeof(struct clientData), 1, fPtr) == 1) {
        if (client.acctNum == accountNum) {
            printf("Current info: %s %s Balance: $%.2f\n", client.firstName, 
                   client.lastName, client.balance);
            printf("Enter new last name: ");
            scanf("%14s", client.lastName);
            printf("Enter new first name: ");
            scanf("%9s", client.firstName);
            printf("Enter new balance: ");
            scanf("%lf", &client.balance);

            fseek(fPtr, -(long)sizeof(struct clientData), SEEK_CUR);
            fwrite(&client, sizeof(struct clientData), 1, fPtr);
            printf("Account updated successfully.\n");
            logTransaction(&client, "UPDATE", client.balance);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Account not found.\n");
    }
}

// Add new record
void newRecord(FILE *fPtr) {
    struct clientData client = {0};

    printf("Enter account number: ");
    scanf("%u", &client.acctNum);

    printf("Enter last name: ");
    scanf("%14s", client.lastName);

    printf("Enter first name: ");
    scanf("%9s", client.firstName);

    printf("Enter balance: ");
    scanf("%lf", &client.balance);

    fseek(fPtr, 0, SEEK_END);
    fwrite(&client, sizeof(struct clientData), 1, fPtr);
    printf("New account created successfully.\n");
    logTransaction(&client, "NEW_ACCOUNT", client.balance);
}

// Delete record (set acctNum to 0)
void deleteRecord(FILE *fPtr) {
    unsigned int accountNum;
    struct clientData client;
    int found = 0;

    printf("Enter account number to delete: ");
    scanf("%u", &accountNum);

    rewind(fPtr);
    while (fread(&client, sizeof(struct clientData), 1, fPtr) == 1) {
        if (client.acctNum == accountNum) {
            printf("Delete %s %s? (Y/N): ", client.firstName, client.lastName);
            char response;
            scanf(" %c", &response);

            if (response == 'Y' || response == 'y') {
                client.acctNum = 0;
                fseek(fPtr, -(long)sizeof(struct clientData), SEEK_CUR);
                fwrite(&client, sizeof(struct clientData), 1, fPtr);
                printf("Account deleted.\n");
                logTransaction(&client, "DELETE", 0);
            }
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Account not found.\n");
    }
}

// Display all transactions
void showTransactionHistory(void) {
    FILE *historyPtr;
    char line[256];

    historyPtr = fopen("transactions.txt", "r");
    if (historyPtr == NULL) {
        printf("No transaction history available.\n");
        return;
    }

    printf("\n========== TRANSACTION HISTORY ==========\n");
    while (fgets(line, sizeof(line), historyPtr) != NULL) {
        printf("%s", line);
    }
    printf("=========================================\n");

    fclose(historyPtr);
}

// new feature 1: search by last name
void searchByLastName(FILE *fPtr) {
    char searchName[15];
    struct clientData client;
    int found = 0;

    printf("Enter last name to search: ");
    scanf("%14s", searchName);

    rewind(fPtr);
    while (fread(&client, sizeof(struct clientData), 1, fPtr) == 1) {
        if (client.acctNum != 0 && strcmp(client.lastName, searchName) == 0) {
            printf("Found: %u %s %s Balance: %.2f\n",
                   client.acctNum, client.lastName, client.firstName, client.balance);
            found = 1;
        }
    }

    if (!found) {
        printf("No accounts found with last name '%s'.\n", searchName);
    }
}

// new feature 2: export transaction history for one account
void exportAccountHistory(unsigned int accountNum) {
    FILE *historyPtr, *exportPtr;
    char line[256];
    char filename[30];

    sprintf(filename, "history_%u.txt", accountNum);
    historyPtr = fopen("transactions.txt", "r");
    exportPtr = fopen(filename, "w");

    if (historyPtr == NULL || exportPtr == NULL) {
        puts("Error opening files.");
        return;
    }

    while (fgets(line, sizeof(line), historyPtr) != NULL) {
        char acctStr[10];
        sprintf(acctStr, "%u", accountNum);
        if (strstr(line, acctStr)) {
            fputs(line, exportPtr);
        }
    }

    fclose(historyPtr);
    fclose(exportPtr);
    printf("Exported history for account %u to %s\n", accountNum, filename);
}

// new feature 3: apply interest to all accounts
void applyInterest(FILE *fPtr) {
    double rate;
    struct clientData client;

    printf("Enter interest rate (e.g., 0.05 for 5%%): ");
    scanf("%lf", &rate);

    rewind(fPtr);
    while (fread(&client, sizeof(struct clientData), 1, fPtr) == 1) {
        if (client.acctNum != 0) {
            double interest = client.balance * rate;
            client.balance += interest;
            fseek(fPtr, -(long)sizeof(struct clientData), SEEK_CUR);
            fwrite(&client, sizeof(struct clientData), 1, fPtr);
            logTransaction(&client, "UPDATE", interest);
        }
    }
    printf("Applied %.2f%% interest to all accounts.\n", rate * 100);
}

// new feature 4: undo last transaction
void undoLastTransaction(FILE *fPtr) {
    FILE *historyPtr;
    char line[256], lastLine[256] = "";
    struct clientData client;

    historyPtr = fopen("transactions.txt", "r");
    if (historyPtr == NULL) {
        puts("No transaction history available.");
        return;
    }

    while (fgets(line, sizeof(line), historyPtr) != NULL) {
        strcpy(lastLine, line);
    }
    fclose(historyPtr);

    if (strlen(lastLine) == 0) {
        puts("No transactions to undo.");
        return;
    }

    unsigned int acct;
    double amount;
    char action[10], lname[15], fname[10];

    if (sscanf(lastLine, "%s %u %s %s amount %lf", action, &acct, lname, fname, &amount) >= 5) {
        fseek(fPtr, (acct - 1) * sizeof(struct clientData), SEEK_SET);
        fread(&client, sizeof(struct clientData), 1, fPtr);
        client.balance -= amount;
        fseek(fPtr, -(long)sizeof(struct clientData), SEEK_CUR);
        fwrite(&client, sizeof(struct clientData), 1, fPtr);
        printf("Undid last transaction for account %u.\n", acct);
    } else {
        puts("Undo not supported for this transaction type.");
    }
}

// updated menu
unsigned int enterChoice(void) {
    unsigned int menuChoice;
    int valid;
    do {
        printf("%s", "\nEnter your choice\n"
                     "1 - store a sorted text file of accounts called\n"
                     "    \"accounts.txt\" for printing\n"
                     "2 - update an account\n"
                     "3 - add a new account\n"
                     "4 - delete an account\n"
                     "5 - display all transaction history\n"
                     "6 - end program\n"
                     "7 - search account by last name\n"
                     "8 - export transaction history for one account\n"
                     "9 - apply interest to all accounts\n"
                     "10 - undo last transaction\n? ");

        valid = scanf("%u", &menuChoice);
        if (valid != 1) {
            puts("Invalid input. Please enter a number.");
            while (getchar() != '\n');
        } else if (menuChoice < 1 || menuChoice > 10) {
            puts("Incorrect choice");
        }
    } while (valid != 1 || menuChoice < 1 || menuChoice > 10);
    return menuChoice;
}
