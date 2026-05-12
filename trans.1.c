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

int main(int argc, char *argv[]) {
    FILE *cfPtr;         // credit.dat file pointer
    unsigned int choice; // user's choice

    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL) {
        printf("%s: File could not be opened.\n", argv[0]);
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

// --- existing functions unchanged (textFile, updateRecord, deleteRecord, logTransaction, compareAccounts, showTransactionHistory, newRecord) ---

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
            fseek(fPtr, -sizeof(struct clientData), SEEK_CUR);
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
        fseek(fPtr, -sizeof(struct clientData), SEEK_CUR);
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
