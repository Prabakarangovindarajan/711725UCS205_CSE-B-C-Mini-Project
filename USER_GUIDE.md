# Banking Management System - User Guide & Test Scenarios

## Quick Start

### Compilation
```bash
# Using GCC
gcc -o banking_system banking_system.c

# Or using Makefile (if available)
make
make run
```

### First Run
```bash
./banking_system
```

## Test Scenario 1: Basic Account Creation & Transactions

### Step 1: Create First Account
```
Menu Choice: 3 (Create new account)
Account Number: 1001
Last Name: Smith
First Name: John
Initial Balance: 10000.00
```

Expected Output:
```
New account created successfully!
Account Number: 1001
Name: John Smith
Initial Balance: $10000.00
```

### Step 2: Create Second Account
```
Menu Choice: 3 (Create new account)
Account Number: 1002
Last Name: Johnson
First Name: Mary
Initial Balance: 5000.00
```

### Step 3: Deposit Money
```
Menu Choice: 9 (Deposit money)
Account Number: 1001
Deposit Amount: 2500.00
```

Expected Output:
```
========== DEPOSIT CONFIRMATION ==========
Account#: 1001
Name: John Smith
Previous Balance: $10000.00
Deposit Amount: $2500.00
New Balance: $12500.00
=========================================
```

### Step 4: Withdraw Money
```
Menu Choice: 10 (Withdraw money)
Account Number: 1002
Withdrawal Amount: 1000.00
```

Expected Output:
```
========== WITHDRAWAL CONFIRMATION ==========
Account#: 1002
Name: Mary Johnson
Previous Balance: $5000.00
Withdrawal Amount: $1000.00
New Balance: $4000.00
=============================================
```

### Step 5: View All Accounts
```
Menu Choice: 8 (Display all accounts)
```

Expected Output:
```
========== ALL ACTIVE ACCOUNTS ==========
Account#    LastName        FirstName  Balance
----------  ---------------  ----------  ----------
1001        Smith           John       $12500.00
1002        Johnson         Mary       $4000.00
=========================================
Total active accounts: 2
```

### Step 6: Export Accounts to Text File
```
Menu Choice: 1 (Store sorted accounts to accounts.txt)
```

This creates an accounts.txt file with sorted account data.

### Step 7: View Transaction History
```
Menu Choice: 5 (Display all transaction history)
```

Expected Output:
```
========== TRANSACTION HISTORY ==========
[2026-05-12 14:30:45] Account: 1001 | Name: John Smith | Action: NEW_ACCOUNT | Amount: 10000.00 | Balance: 10000.00
[2026-05-12 14:30:50] Account: 1002 | Name: Mary Johnson | Action: NEW_ACCOUNT | Amount: 5000.00 | Balance: 5000.00
[2026-05-12 14:31:00] Account: 1001 | Name: John Smith | Action: DEPOSIT | Amount: 2500.00 | Balance: 12500.00
[2026-05-12 14:31:15] Account: 1002 | Name: Mary Johnson | Action: WITHDRAWAL | Amount: 1000.00 | Balance: 4000.00
=========================================
```

## Test Scenario 2: Search Operations

### Search by Last Name
```
Menu Choice: 6 (Search by last name)
Last Name: Smith
```

Expected Output:
```
========== SEARCH RESULTS ==========
Account#: 1001
Name: John Smith
Balance: $12500.00

====================================
```

### Search by Account Number
```
Menu Choice: 7 (Search by account number)
Account Number: 1002
```

Expected Output:
```
========== SEARCH RESULTS ==========
Account#: 1002
Name: Mary Johnson
Balance: $4000.00

====================================
```

## Test Scenario 3: Update Account

### Update Account Information
```
Menu Choice: 2 (Update existing account)
Account Number to Update: 1001
```

Expected Output:
```
Current account details:
Name: John Smith
Balance: $12500.00

Enter new last name (or press Enter to skip): [Press Enter]
Enter new first name (or press Enter to skip): [Press Enter]
Enter new balance (or enter -1 to skip): [Type 15000 and press Enter]

Account updated successfully!
```

## Test Scenario 4: Delete Account

### Delete an Account
```
Menu Choice: 4 (Delete account)
Account Number to Delete: 1002
Confirm deletion of account 1002 (Mary Johnson)?
Enter 'Y' to confirm or any other key to cancel: Y
```

Expected Output:
```
Account deleted successfully!
```

Verify deletion:
```
Menu Choice: 7 (Search by account number)
Account Number: 1002

Result: Account not found or already deleted.
```

## Test Scenario 5: Error Handling

### Insufficient Balance Withdrawal
```
Menu Choice: 10 (Withdraw money)
Account Number: 1001
Withdrawal Amount: 50000.00
```

Expected Output:
```
Insufficient balance!
Current Balance: $15000.00
Requested Withdrawal: $50000.00
```

### Invalid Menu Choice
```
Menu Choice: 15
```

Expected Output:
```
Invalid choice! Please enter 1-11.
```

### Non-numeric Input
```
Menu Choice: abc
```

Expected Output:
```
Invalid input! Please enter a number.
```

### Duplicate Account Number
```
Menu Choice: 3 (Create new account)
Account Number: 1001
```

Expected Output:
```
Account already exists!
```

### Invalid Withdrawal Amount
```
Menu Choice: 10 (Withdraw money)
Account Number: 1001
Withdrawal Amount: -100
```

Expected Output:
```
Invalid withdrawal amount!
```

## File Outputs After Tests

### accounts.txt
```
Account#    LastName        FirstName  Balance
----------  ---------------  ----------  ----------
1001        Smith           John       $15000.00
```

### transactions.txt
```
[2026-05-12 14:30:45] Account: 1001 | Name: John Smith | Action: NEW_ACCOUNT | Amount: 10000.00 | Balance: 10000.00
[2026-05-12 14:30:50] Account: 1002 | Name: Mary Johnson | Action: NEW_ACCOUNT | Amount: 5000.00 | Balance: 5000.00
[2026-05-12 14:31:00] Account: 1001 | Name: John Smith | Action: DEPOSIT | Amount: 2500.00 | Balance: 12500.00
[2026-05-12 14:31:15] Account: 1002 | Name: Mary Johnson | Action: WITHDRAWAL | Amount: 1000.00 | Balance: 4000.00
[2026-05-12 14:32:00] Account: 1001 | Name: John Smith | Action: UPDATE | Amount: 2500.00 | Balance: 15000.00
[2026-05-12 14:32:30] Account: 1002 | Name: Mary Johnson | Action: DELETE | Amount: 0.00 | Balance: 4000.00
```

## Important Notes

### Data Persistence
- All data is saved to binary file `credit.dat`
- Transactions are logged to `accounts.txt`
- Once you exit and run the program again, all data is preserved
- You can delete accounts but they remain in the database (marked as deleted)

### Account Status
- Active accounts show in searches and reports
- Deleted accounts are hidden but records are preserved
- Account numbers cannot be reused (even after deletion)

### Transaction Logging
- Every operation is timestamped
- Timestamps follow format: YYYY-MM-DD HH:MM:SS
- Transactions cannot be deleted (audit trail)

### Soft Delete
- When you delete an account, it's marked as 'D' (deleted)
- Original balance is preserved in the record
- This ensures data integrity and audit compliance

## Common Tasks

### Backup Your Data
```bash
# Copy the data file
cp credit.dat credit.dat.backup
cp transactions.txt transactions.txt.backup
```

### Start Fresh
```bash
# Delete all data files
rm credit.dat transactions.txt accounts.txt

# Or use make command
make distclean
```

### View Binary Database
The credit.dat file is binary and cannot be viewed as text.
Use the program's export feature (Option 1) to view accounts.

## Performance Tips

1. **First Time Setup**: Create a few test accounts first
2. **Regular Exports**: Use option 1 to regularly export to accounts.txt
3. **View History**: Check transactions.txt to audit activity
4. **Backup Often**: Regularly backup credit.dat
5. **Data Cleanup**: Remove deleted accounts periodically (if needed)

## Troubleshooting

### Program crashes on startup
- Check if credit.dat is corrupted
- Delete credit.dat and restart (will create new database)

### Missing transaction history
- Check if transactions.txt file exists
- Run some transactions to create the file

### Accounts not saving
- Verify write permissions in program directory
- Check available disk space

### Slow performance with many accounts
- This is normal for large databases
- Consider archiving old transactions

## End Program

```
Menu Choice: 11 (Exit program)
```

Expected Output:
```
Thank you for using Banking Management System!
```

Your data is automatically saved and will be available the next time you run the program.
