# Banking Management System - Technical Documentation

## Architecture Overview

### System Components

```
┌─────────────────────────────────────────┐
│     Banking Management System          │
├─────────────────────────────────────────┤
│                                         │
│  ┌──────────────────────────────────┐  │
│  │   User Interface (Menu System)   │  │
│  └──────────────────────────────────┘  │
│               ↓                         │
│  ┌──────────────────────────────────┐  │
│  │   Business Logic Layer           │  │
│  │  (Account & Transaction Mgmt)    │  │
│  └──────────────────────────────────┘  │
│               ↓                         │
│  ┌──────────────────────────────────┐  │
│  │   Data Access Layer              │  │
│  │  (File I/O Operations)           │  │
│  └──────────────────────────────────┘  │
│               ↓                         │
│  ┌──────────────────────────────────┐  │
│  │   Data Storage                   │  │
│  │  (Binary Files & Text Logs)      │  │
│  └──────────────────────────────────┘  │
│                                         │
└─────────────────────────────────────────┘
```

## Data Structures

### clientData Structure
```c
struct clientData {
    unsigned int acctNum;      // 4 bytes - Primary key
    char lastName[15];         // 15 bytes - Customer surname
    char firstName[10];        // 10 bytes - Customer first name
    double balance;            // 8 bytes - Account balance
    char acctStatus;           // 1 byte - 'A' or 'D'
};
// Total: 38 bytes per record
```

### transaction Structure
```c
struct transaction {
    unsigned int acctNum;      // Account number (reference)
    char transType[20];        // Type: DEPOSIT, WITHDRAWAL, UPDATE, DELETE, NEW_ACCOUNT
    double amount;             // Transaction amount
    double balanceAfter;       // Balance after transaction
    char timestamp[30];        // Formatted as YYYY-MM-DD HH:MM:SS
};
```

## File System Architecture

### credit.dat (Binary Database)
- **Type**: Binary file (random access)
- **Structure**: Sequential records of `clientData` structures
- **Access Mode**: r+b (read/write binary)
- **Record Size**: 38 bytes per account
- **Features**:
  - O(n) for sequential scan
  - O(1) for direct access if index maintained
  - Space-efficient for large datasets

```
┌─────────────────────────────────┐
│ Record 0: clientData[0]         │  0-37 bytes
├─────────────────────────────────┤
│ Record 1: clientData[1]         │  38-75 bytes
├─────────────────────────────────┤
│ Record 2: clientData[2]         │  76-113 bytes
├─────────────────────────────────┤
│ ...                             │
└─────────────────────────────────┘
```

### transactions.txt (Append-Only Log)
- **Type**: Text file (sequential)
- **Access Mode**: a+ (append mode)
- **Format**: Plain text with newline-separated records
- **Features**:
  - Immutable transaction history
  - Human-readable audit trail
  - No deletion or modification
  - Timestamp for traceability

### accounts.txt (Export File)
- **Type**: Text file (generated on demand)
- **Access Mode**: w (write mode)
- **Format**: Formatted table with headers
- **Features**:
  - Generated from credit.dat
  - Contains only active accounts
  - Sorted by account number
  - Used for reporting/printing

## Function Call Hierarchy

```
main()
├── enterChoice()
├── displayMenu()
│
├── Option 1: textFile()
│   ├── rewind()
│   ├── fread() - load records
│   ├── qsort() with compareAccounts()
│   └── fprintf() - write to accounts.txt
│
├── Option 2: updateRecord()
│   ├── fread() - find account
│   ├── fseek() - position pointer
│   └── fwrite() - save changes
│   └── logTransaction()
│
├── Option 3: newRecord()
│   ├── fseek(SEEK_END) - append
│   ├── fwrite() - add new account
│   └── logTransaction()
│
├── Option 4: deleteRecord()
│   ├── fread() - find account
│   ├── mark acctStatus = 'D'
│   ├── fseek() - reposition
│   ├── fwrite() - save changes
│   └── logTransaction()
│
├── Option 5: showTransactionHistory()
│   ├── fopen(transactions.txt)
│   └── fgets() - read lines
│
├── Option 6: searchByLastName()
│   ├── rewind()
│   └── fread() with strcmp()
│
├── Option 7: searchByAccountNumber()
│   ├── rewind()
│   └── fread() with direct comparison
│
├── Option 8: displayAllAccounts()
│   ├── rewind()
│   └── fread() with filtering
│
├── Option 9: depositMoney()
│   ├── fread() - find account
│   ├── update balance
│   ├── fwrite() - save
│   └── logTransaction()
│
├── Option 10: withdrawMoney()
│   ├── fread() - find account
│   ├── validate balance
│   ├── update balance
│   ├── fwrite() - save
│   └── logTransaction()
│
└── fclose()
```

## Key Algorithms

### 1. Account Search (Sequential)
```c
Algorithm SearchAccount(accountNum):
    rewind(file)
    while fread(record) do
        if record.acctNum == accountNum AND record.acctStatus != 'D'
            return record
    return NOT_FOUND
Time Complexity: O(n) - n = number of records
```

### 2. Record Update (Direct Access)
```c
Algorithm UpdateRecord(accountNum):
    rewind(file)
    while fread(record) do
        if record.acctNum == accountNum
            modify record
            fseek(file, -sizeof(record), SEEK_CUR)
            fwrite(record)
            return SUCCESS
    return NOT_FOUND
Time Complexity: O(n) for search, O(1) for update
```

### 3. Account Export & Sort
```c
Algorithm ExportAccounts():
    count = 0
    rewind(file)
    for each record
        if record.acctNum != 0 AND record.acctStatus != 'D'
            add to array
            count++
    
    qsort(array, count, compareAccounts)
    
    for i = 0 to count
        write array[i] to accounts.txt
Time Complexity: O(n log n) - due to qsort
```

### 4. Validation Flow
```
User Input
    ↓
Check Type (numeric/string)
    ↓
Check Range/Valid Values
    ↓
Check Business Rules
    (e.g., account exists, balance sufficient)
    ↓
Process Transaction
    ↓
Log Transaction
```

## Memory Management

### Stack Usage
- Local variables in functions: minimal
- Character buffers: 256-512 bytes per function

### Heap Allocation
- `textFile()`: Allocates array for sorting
  ```c
  clients = malloc(recordCount * sizeof(struct clientData))
  // Used during sort operation
  free(clients) // Freed after use
  ```

### File Handle Management
- `credit.dat`: Opened once at startup, closed at exit
- `transactions.txt`: Opened/closed for each transaction
- `accounts.txt`: Opened/closed during export

## Error Handling Strategy

### File Operations
```c
if ((file = fopen(filename, mode)) == NULL) {
    printf("Error opening file\n");
    return; // Or handle gracefully
}
// ... use file
fclose(file);
```

### Input Validation
```c
scanf("%u", &value)
if (valid != 1) {
    printf("Invalid input\n");
    while (getchar() != '\n'); // Clear buffer
}
```

### Business Logic Validation
```c
if (client.acctNum != 0 && client.acctStatus != 'D') {
    // Process valid, active account
}
```

## Transaction Logging System

### Log Entry Format
```
[TIMESTAMP] Account: NUM | Name: FIRST LAST | Action: TYPE | Amount: AMNT | Balance: BAL
```

### Supported Actions
- `NEW_ACCOUNT`: Account creation
- `DEPOSIT`: Money deposited
- `WITHDRAWAL`: Money withdrawn
- `UPDATE`: Account information updated
- `DELETE`: Account marked for deletion

### Timestamp Generation
```c
time_t now = time(NULL);
struct tm *timeinfo = localtime(&now);
strftime(buffer, size, "%Y-%m-%d %H:%M:%S", timeinfo);
// Example: 2026-05-12 14:30:45
```

## Soft Delete Implementation

### Approach
Instead of physically deleting records:
1. Mark record with `acctStatus = 'D'`
2. Keep original data intact
3. Filter out 'D' status in searches and displays

### Benefits
- Data recovery possible
- Audit trail maintained
- Account numbers cannot be reused
- Database integrity preserved

### Implementation
```c
// Deletion
client.acctStatus = 'D';
fseek(fPtr, -sizeof(struct clientData), SEEK_CUR);
fwrite(&client, sizeof(struct clientData), 1, fPtr);

// Display filter
if (client.acctNum != 0 && client.acctStatus != 'D') {
    // Display this account
}
```

## Performance Characteristics

### Operation Complexity

| Operation | Complexity | Notes |
|-----------|-----------|-------|
| Create Account | O(1) | Direct append |
| Read Account | O(n) | Sequential scan |
| Update Account | O(n) | Find + O(1) update |
| Delete Account | O(n) | Find + O(1) mark delete |
| Search by Name | O(n) | Full scan required |
| Search by ID | O(n) | Linear search |
| Export & Sort | O(n log n) | Loading + qsort |
| View History | O(m) | Read m log entries |

### Optimization Opportunities

1. **Indexing**: Create index file for account numbers
2. **Hashing**: Use hash table for name search
3. **Caching**: Cache frequently accessed accounts
4. **Database**: Consider SQLite for better performance

## Scalability Analysis

### Current Limitations
- File size: System-dependent (typically GB range)
- Search time: Linear (O(n))
- Memory: Minimal, doesn't scale with data

### Scaling Considerations
For 1,000,000 accounts:
- Database size: ~38 MB
- Sequential scan: ~seconds
- Export: ~minutes

### Recommendations
- 1,000-10,000 accounts: Current system adequate
- 10,000-100,000 accounts: Consider indexing
- 100,000+ accounts: Consider database migration

## Security Considerations

### Current Implementation
- No encryption
- No authentication
- No authorization levels
- Plain text transaction logs

### Future Enhancements
1. **User Authentication**: PIN/Password protection
2. **Encryption**: AES encryption for sensitive data
3. **Access Control**: Role-based permissions
4. **Audit Logging**: Enhanced logging with user info
5. **Data Backup**: Automated backup system

## Development Notes

### Code Quality
- ANSI C89/C99 compatible
- Clear function separation
- Consistent naming conventions
- Input validation on all user inputs

### Testing Recommendations
1. Boundary testing (empty database, full database)
2. Invalid input testing
3. Concurrent access testing
4. Data consistency verification
5. Performance profiling

### Known Limitations
1. Not thread-safe
2. No network access
3. No GUI interface
4. Limited to local file system
5. No encryption
6. No backup/recovery mechanisms

## Future Enhancement Roadmap

### Phase 1: Core Improvements
- Add data validation constraints
- Implement transaction rollback
- Add data export formats (CSV, XML)

### Phase 2: Features
- Interest calculation
- Account types (Savings/Checking)
- Multi-user support
- Transaction scheduling

### Phase 3: Infrastructure
- Database migration (SQLite/MySQL)
- Network architecture
- Web/Mobile interface
- Cloud backup

## Compilation & Linking

### Standard Compilation
```bash
gcc -Wall -Wextra -std=c99 -o banking_system banking_system.c -lm
```

### With Debug Information
```bash
gcc -Wall -Wextra -std=c99 -g -o banking_system banking_system.c -lm
```

### Libraries Used
- `stdio.h`: File I/O operations
- `stdlib.h`: Memory allocation, qsort
- `string.h`: String operations
- `time.h`: Timestamp generation

### Platform Compatibility
- **Linux**: Full support
- **Windows**: Full support with MinGW
- **macOS**: Full support with Clang

## References & Standards

- C99 Standard Compliance
- POSIX File Operations
- IEEE 1003.1 (POSIX.1)
- Binary file format (proprietary)
- Timestamp format: ISO 8601
