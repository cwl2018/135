###Simple Accounting System

***Assumptions & Remarks:***

1. Considering practicality and to avoid overflow, each transaction will be capped at 10 million dollars (<10M).
2. All transactions are assumed to be of the same currency.
3. Resets take place monthly, the monthly expense counter will be zeroed. Users will be prompted to rename the current "records.txt" and a new "records.txt" file will be created afterwards.
4. Program is intended for normal use,unreasonable usage and usage without following the instructions may result in crashing of the program.
5. All values of transactions are corrected to 4 decimal places.
6. The transaction amount of expenses is regarded as negative, whereas positive for income.

**Program features:**
1. **Income and Expense Records:** Payment method, amount, date, category and type of transaction will be recorded.
2. **Record Editing and Deleting:** The records could be deleted and edited at any time.
3. **Sorting:** Users could view their records by date, category, payment method and amount.
4. **Statistical Report:** It will provide users' monthly statistical data dating from the start of the month(i.e.all records in the current "records.txt" file). Statistics include income and expense records, net balance and average expenses etc, showing the financial state.
5. **Budget Setting:** There will be an alert from the system when expenses reach users' budget. Default setting is 10,000.
6. **Auto-report** A function that automatically generates a statistical report on the first start up of the program each month, showing the statistics for the previous month. Can be toggled on/off in the menu.

**Instructions:**
1. Initialization:
   - There are 4 files in this program. The "run" target in the makefile will generate a final executable "main".
   - Users should see the homepage of the function upon initializing the program.
   - After adding a record entry (Enter 1), "records.txt" will be created.
   - After terminating the program(Enter 0), "data.txt"  will be created
   - Starting from the next time, use "./main" to run the program.
2. Functions and inputs:
   - Enter 1 to ADD A RECORD, then enter type(income/expense), payment method, category and amount (**MUST be a NUMERICAL VALUE <     10M**).
   - Enter 2 to DELETE A RECORD, all records in the "records.txt" file will be shown, enter the id of the entry to be deleted.
   - Enter 3 to enter VIEW RECORDS page, then enter 1 to view by DATE, 2 (PAYMENT METHOD), 3 (CATEGORY), 4 (AMOUNT).Records will then be               shown.
   - Enter 4 to EDIT AN EXISTING RECORD, all records in the "records.txt" file will be shown, enter the id of the entry to be deleted.
            Then enter the updated information of the entry in the following format.
            YYYY-MM-DD type(income/expense) payment-method category and amount(**MUST be a NUMERICAL VALUE < 10M, add a negative sign                for expenses**)
   -  Enter 6 to GENERATE a REPORT. A report will be shown. 
   -  Enter 7 to change the BUDGET. Enter a **Positive Integer < 100M**
   -  Enter 8 to rename and replace the current "records.txt" with a new one. Enter a new name for the file.
   -  Enter 9 to TOGGLE autoreport.
