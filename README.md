# Banking Management System

## Overview
A complete C-based banking management system that provides comprehensive account and transaction management capabilities.

## Features

### Core Banking Operations
- **Create New Accounts**: Add new customer accounts with account number, name, and initial balance
- **Update Accounts**: Modify account information (name and balance)
- **Delete Accounts**: Soft delete accounts (marked as deleted, not removed)
- **View All Accounts**: Display all active accounts with sorted view
- **Deposit Money**: Add funds to accounts
- **Withdraw Money**: Withdraw funds with balance validation

### Search & Reporting
- **Search by Last Name**: Find accounts by customer's last name
- **Search by Account Number**: Find specific account
- **View All Accounts**: Sorted list of all active accounts
- **Export to Text File**: Save sorted account data to accounts.txt
- **Transaction History**: View complete transaction log with timestamps

### Data Management
- **Transaction Logging**: All transactions are timestamped and logged to transactions.txt
- **Binary Database**: Uses credit.dat file for secure data storage
- **Data Persistence**: All changes are permanently saved

## File Structure

### Data Files
- **credit.dat**: Binary database file containing all account information
- **accounts.txt**: Text export of sorted accounts (generated on demand)
- **transactions.txt**: Log file containing all transactions with timestamps

### Source Code
- **banking_system.c**: Complete implementation with all functions

## Compilation

### On Windows (using MinGW/GCC):
```bash
gcc -o banking_system banking_system.c
```

### On Linux/Mac:
```bash
gcc -o banking_system banking_system.c
```

## Running the Program

```bash
./banking_system
```

### Program Output
```
========================================
  WELCOME TO BANKING MANAGEMENT SYSTEM
========================================

======== MAIN MENU ========
1 - Store sorted accounts to accounts.txt
2 - Update existing account
3 - Create new account
4 - Delete account
5 - Display all transaction history
6 - Search account by last name
7 - Search account by account number
8 - Display all accounts
9 - Deposit money
10 - Withdraw money
11 - Exit program
Enter choice (1-11):
```

## Usage Examples

### Create a New Account
1. Choose option 3
2. Enter account number (e.g., 1001)
3. Enter last name (e.g., Smith)
4. Enter first name (e.g., John)
5. Enter initial balance (e.g., 5000.00)

### Deposit Money
1. Choose option 9
2. Enter account number
3. Enter deposit amount
4. Confirmation shows new balance

### Withdraw Money
1. Choose option 10
2. Enter account number
3. Enter withdrawal amount
4. System checks for sufficient balance
5. Confirmation shows new balance

### Search Account
1. Choose option 6 (by last name) or option 7 (by account number)
2. Enter search criteria
3. View account details

### Export Accounts
1. Choose option 1
2. Sorted account list is saved to accounts.txt
3. File can be printed or viewed

### View Transaction History
1. Choose option 5
2. All transactions with timestamps are displayed

## Data Structures

### clientData Structure
```c
struct clientData {
    unsigned int acctNum;      // account number (unique identifier)
    char lastName[15];         // customer's last name
    char firstName[10];        // customer's first name
    double balance;            // current account balance
    char acctStatus;           // 'A' for active, 'D' for deleted
};
```

### Transaction Log Format
```
[YYYY-MM-DD HH:MM:SS] Account: 1001 | Name: John Smith | Action: DEPOSIT | Amount: 500.00 | Balance: 5500.00
```

## Key Functions

| Function | Purpose |
|----------|---------|
| `newRecord()` | Create new account |
| `updateRecord()` | Modify existing account |
| `deleteRecord()` | Delete account (soft delete) |
| `depositMoney()` | Add funds to account |
| `withdrawMoney()` | Remove funds from account |
| `searchByLastName()` | Find account by last name |
| `searchByAccountNumber()` | Find account by number |
| `displayAllAccounts()` | Show all active accounts |
| `textFile()` | Export sorted accounts to file |
| `showTransactionHistory()` | Display all transactions |
| `logTransaction()` | Record transaction with timestamp |

## Input Validation

The system includes validation for:
- Account numbers (must be greater than 0)
- Duplicate account numbers (prevents duplicates)
- Deposit/withdrawal amounts (must be positive)
- Withdrawal amount (must not exceed balance)
- Menu choices (1-11 only)
- Invalid input types (non-numeric when numeric required)

## Data Persistence

- **All data is automatically saved** to credit.dat
- **Transactions are logged** with timestamps to transactions.txt
- **No data is lost** when the program exits
- **Soft delete** ensures data integrity (deleted records are marked but not removed)

## Security Features

- Binary file storage (not plain text)
- Soft delete implementation
- Transaction audit trail
- Timestamped transaction logging
- Account status tracking

## Example Workflow

1. **Start Program** → See main menu
2. **Create Account** → Add new customer account
3. **Deposit Money** → Add initial funds
4. **View Accounts** → See all active accounts
5. **Export Data** → Save to accounts.txt
6. **View History** → Check all transactions
7. **Withdraw Money** → Remove funds
8. **Update Account** → Modify account info
9. **Exit Program** → Data is saved

## Troubleshooting

### Program won't compile
- Ensure you have a C compiler installed (GCC, MinGW, or Clang)
- Check file path and permissions

### File access errors
- Ensure write permissions in the program directory
- Check disk space availability

### Data not persisting
- Verify credit.dat file exists in the program directory
- Check file permissions

## Future Enhancements

Possible additions:
- Interest calculation on accounts
- Account transfer between accounts
- Mini statement generation
- Pin/password protection
- Account freeze/unfreeze
- Overdraft facility
- Loan management

## License

Educational Project - Free to use and modify

## Notes

- This system uses binary files for efficient data storage
- All transactions are logged with precise timestamps
- The system handles up to millions of accounts (limited by disk space)
- Regular backups of credit.dat are recommended for production use
