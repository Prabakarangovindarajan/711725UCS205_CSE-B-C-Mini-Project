# Banking Management System - Quick Reference Guide

## Menu Options Quick Reference

| # | Operation | Steps | Best For |
|---|-----------|-------|----------|
| 1 | Store sorted accounts | Select 1 | Generate printable report |
| 2 | Update account | Select 2 → Account# → New info | Modify account details |
| 3 | Create account | Select 3 → Fill details | Add new customer |
| 4 | Delete account | Select 4 → Account# → Confirm Y | Remove customer account |
| 5 | View transactions | Select 5 | Review all transactions |
| 6 | Search by name | Select 6 → Last name | Find customer by name |
| 7 | Search by account# | Select 7 → Account# | Find customer by number |
| 8 | View all accounts | Select 8 | List all customers |
| 9 | Deposit money | Select 9 → Account# → Amount | Add funds |
| 10 | Withdraw money | Select 10 → Account# → Amount | Remove funds |
| 11 | Exit program | Select 11 | Quit application |

## Sample Menu Flow

```
START
  ↓
[1] Create Account (John Smith, $10,000)
  ↓
[8] View All Accounts
  ↓
[9] Deposit $2,500
  ↓
[5] View Transaction History
  ↓
[1] Export to accounts.txt
  ↓
[11] Exit
  END
```

## File Guide

| File | Type | Purpose | Created When |
|------|------|---------|--------------|
| banking_system.c | Source Code | Main program | Provided |
| credit.dat | Binary DB | Account data | First run |
| transactions.txt | Text Log | Transaction history | First transaction |
| accounts.txt | Report | Sorted account list | Option 1 used |
| README.md | Documentation | Full documentation | Reference |
| USER_GUIDE.md | Guide | Usage examples | Reference |
| TECHNICAL_DOCS.md | Technical | Implementation details | Reference |

## Common Tasks

### Setup & First Run
```bash
gcc -o banking_system banking_system.c
./banking_system
# Select option 3 to create first account
```

### Typical Daily Operations
1. View accounts (Option 8)
2. Process deposits (Option 9)
3. Process withdrawals (Option 10)
4. Check transactions (Option 5)
5. Export report (Option 1)

### End of Day
1. View all transactions (Option 5)
2. Export accounts (Option 1) - for verification
3. Exit (Option 11) - data saved automatically

## Input Format Reference

### Account Number
- **Format**: Positive integer
- **Range**: 1 to 2,147,483,647
- **Example**: 1001

### Name Fields
- **Format**: Text string
- **Max Length**: Last name 14 chars, First name 9 chars
- **Example**: Smith, John

### Amount (Deposit/Withdrawal)
- **Format**: Decimal number
- **Min**: $0.01
- **Max**: $9,999,999.99
- **Example**: 1500.50

### Interest Rate
- **Format**: Decimal (as percentage/100)
- **Example**: 0.05 for 5%

## Validation Rules

| Field | Rule | Error Message |
|-------|------|---------------|
| Account # | > 0 | Invalid account number |
| Account # | Unique | Account already exists |
| Amount | > 0 | Invalid amount |
| Withdrawal | ≤ Balance | Insufficient balance |
| Name | Not empty | Cannot be empty |

## Error Solutions

### "File could not be opened"
- Check file permissions
- Ensure write access to directory

### "Account already exists"
- Account number must be unique
- Choose different account number

### "Insufficient balance"
- Withdrawal exceeds current balance
- Check current balance first (Option 8)

### "No active accounts"
- Database is empty
- Create new account (Option 3)

### "Invalid input"
- Enter numeric value when required
- Enter text when required

## Data Backup

### Backup Important Files
```bash
# Windows
copy credit.dat credit.dat.backup
copy transactions.txt transactions.txt.backup

# Linux/Mac
cp credit.dat credit.dat.backup
cp transactions.txt transactions.txt.backup
```

### Restore from Backup
```bash
# Windows
copy credit.dat.backup credit.dat

# Linux/Mac
cp credit.dat.backup credit.dat
```

## Keyboard Shortcuts

| Key | Function |
|-----|----------|
| `Enter` | Confirm input |
| `Y` | Yes (for confirmations) |
| `N` | No/Cancel |
| `Ctrl+C` | Force exit (not recommended) |

## Account Status Reference

| Status | Meaning | Visible? |
|--------|---------|----------|
| A | Active | Yes |
| D | Deleted | No (soft delete) |

## Transaction Types

| Type | Description | Logged By |
|------|-------------|-----------|
| NEW_ACCOUNT | Account created | Option 3 |
| DEPOSIT | Money added | Option 9 |
| WITHDRAWAL | Money removed | Option 10 |
| UPDATE | Account modified | Option 2 |
| DELETE | Account removed | Option 4 |

## Output File Formats

### accounts.txt (Space-separated columns)
```
Account#    LastName        FirstName  Balance
----------  ---------------  ----------  ----------
1001        Smith           John       $12500.00
1002        Johnson         Mary       $4000.00
```

### transactions.txt (Pipe-separated fields)
```
[2026-05-12 14:30:45] Account: 1001 | Name: John Smith | Action: DEPOSIT | Amount: 2500.00 | Balance: 12500.00
```

## Performance Tips

1. **Faster Searches**: Keep account numbers sequential
2. **Better Organization**: Use logical account numbering (e.g., 1000-1999 for checking)
3. **Regular Backups**: Backup credit.dat daily
4. **Archive Logs**: Periodically archive transactions.txt

## Compilation Variants

### Standard Compilation
```bash
gcc -o banking_system banking_system.c
```

### With All Warnings
```bash
gcc -Wall -Wextra -std=c99 -o banking_system banking_system.c
```

### With Debug Info
```bash
gcc -g -o banking_system banking_system.c
```

### With Math Library (if needed)
```bash
gcc -lm -o banking_system banking_system.c
```

## Supported Platforms

- ✅ Windows (MinGW GCC)
- ✅ Linux (GCC)
- ✅ macOS (Clang)
- ✅ Unix systems (GCC)

## Requirements

- **C Compiler**: GCC, Clang, or MSVC
- **RAM**: 1 MB minimum
- **Disk Space**: 1 MB minimum
- **OS**: Any Unix-like or Windows

## Troubleshooting Quick Tips

1. **Compilation fails** → Install GCC
2. **Account not found** → Check account number is correct
3. **Balance wrong** → Review transaction history (Option 5)
4. **Can't create account** → Account number might already exist
5. **File errors** → Check directory permissions

## Additional Commands (if using Makefile)

```bash
make           # Compile
make run       # Compile and run
make clean     # Delete executable
make distclean # Delete all generated files
make help      # Show help
```

## Getting Help

1. **In Program**: Look at menu descriptions
2. **USER_GUIDE.md**: Complete usage examples
3. **TECHNICAL_DOCS.md**: Implementation details
4. **README.md**: General information

## Data Persistence

✅ **Automatic Save**: All changes saved automatically
✅ **Permanent Storage**: Data survives program exit
✅ **Audit Trail**: Complete transaction history
❌ **No Manual Save**: Save button not needed

## Security Notes

- No password protection (add if needed)
- No data encryption (add for production)
- Local file storage only
- No network/cloud access

## Example Session

```
$ ./banking_system
[Menu appears]
? 3
Enter account number: 1001
Enter last name: Smith
Enter first name: John
Enter initial balance: 5000
✓ Account created
? 9
Enter account number: 1001
Enter deposit amount: 1000
✓ Deposit confirmed
? 5
[All transactions shown]
? 1
✓ Accounts exported
? 11
Thank you!
```

---

**Version**: 1.0
**Last Updated**: 2026-05-12
**Status**: Production Ready
