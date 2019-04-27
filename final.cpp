#include <iostream>
#include <iomanip>
#include <time.h>
#include <string>
#include <sstream>
#include <fstream>
#include <typeinfo>
#include "newrecordfile.h"
using namespace std;
int budget=10000, id = 0, auto_report = 1;
double monthly_expense = 0;

struct Record {
  string r_date;
  string r_type;
  string r_method;
  string r_category;
  string r_amount;
};
string getDate();
//gets date in YYYY-MM-DD format
void addRecord(string date,string type,string method,string category,double amount);
//appends record to record.txt file
void printrecords(Record rb[]);
//displays record to user
void grow_recordbook(Record * &rb, int &rb_size, int n);
//enlarge dynamic array "recordbook" by 3
void sortrecords(Record rb[], int function);
//bubble sorts records before displaying on screen
void viewRecord(char view_by); //1 date,2 method,3 category,4 amount
//extract data from records.txt and allocate them in a dynamic array for later use
void monthly_expense_calculator();
//recalculates monthly expense after deleting and editing records
void delete_or_edit_Record(int edit_id, string mode);
//copies the data from records.txt to a temp file, omitting the line to be editted
//or deleted, then renames the temp file to record.txt
void save_data(string todaydate);
//save global variables
void getData(string &last_login_date, string todaydate);
//load global variables and gets last_login_date
void Reset_Monthly_Expense();
//resets monthly expense counter to 0
char homepage(string todaydate);
//shows homepage
void last_month_report(int days);
//creates a report
void Auto_report_function(string todaydate);
//automatically generates a report at the start of each month
int main()
{
  nrf nrf1;
  string todaydate = getDate();
  string record_type, payment_method, category, last_login_date;
  double amount=0;
  int days;
  getData(last_login_date, todaydate);
  if(last_login_date.substr(0,7)!=todaydate.substr(0,7))
  {
    if (auto_report==1) Auto_report_function(last_login_date);
    cout << "New month reached, please rename the current records file"<<endl;
    nrf1.new_record_file();
    Reset_Monthly_Expense();
    id = 0;
  }
  stringstream(todaydate.substr(8,2)) >> days;
  char function = homepage(todaydate);
  while (function!='0')
  {
    switch (function)
    {
      case '1':
        cout << "income or expense? Please enter the type of record.\n";
        cin >> record_type;
        cout << "Enter payment method, category and amount"<< endl;
        cin >> payment_method >> category >> amount;
        if (amount>10000000)
        {
          cout << "Error!(amount<10,000,000)"<<endl;
          exit(1);
        }
        addRecord(todaydate,record_type,payment_method,category,amount);
        break;
      case '2':
        int delete_id;
        viewRecord('5');
        cout << "Enter the id of the record to be deleted"<<endl;
        cin >> delete_id;
        delete_or_edit_Record(delete_id,"DELETE");
        break;
      case '3':
        char view_by;
        cout << "ENTER 1 to view by date\n"
                "ENTER 2 to view by payment method\n"
                "ENTER 3 to view by category\n"
                "ENTER 4 to view by amount\n";
        cin >> view_by;
        viewRecord(view_by);
        break;
      case '4':
        int edit_id;
        viewRecord('5');
        cout << "Enter the id of the record to be edited"<<endl;
        cin >> edit_id;
        delete_or_edit_Record(edit_id,"EDIT");
        break;
      case '6':
        cout << "Here is the report for the last "<<days<<" days"<<endl;
        last_month_report(days);
        break;
      case '7':
        cout << "Set a budget: ";
        cin >> budget;
        break;
      case '8':
        nrf1.new_record_file();
        Reset_Monthly_Expense();
        id = 0;
        break;
      case '9':
        if (auto_report == 1) {
          auto_report = 0;
          cout << "Autoreport has been deactivated" <<endl;
        }
        else if (auto_report == 0) {
          auto_report = 1;
          cout << "Autoreport has been activated" <<endl;
        }
        break;
      case '0':
        break;
      default:
        cout << "Invalid entry or fatal error" << endl;
        cout << "The program will now be terminated." << endl;
        exit(1);
    }
    function = homepage(todaydate);
  }
  save_data(todaydate);
  cout << "Bye Bye" <<endl;
  return 0;
}
string getDate()
{
  char buffer [80];
  time_t rawtime;
  struct tm * timeinfo;
  time (&rawtime);
  timeinfo = localtime (&rawtime);
  strftime(buffer,80,"%F",timeinfo);
  string datenow;
  stringstream(buffer) >> datenow;
  return datenow;
}
void addRecord(string date,string type,string method,string category,double amount)
{
  id++;
  string records_file = "records.txt";
  ofstream fout;
  fout.open(records_file.c_str(), ios::app);
  if (fout.fail())
  {
    cout << "Error in opening " << records_file << endl;
    return;
  }
  if (type=="expense")
  {
    monthly_expense+=amount;
    amount *= -1;
  }
  fout <<date<<setw(8)<<type<<setw(10)<<method
  <<setw(10)<<category<<setw(13)<< amount << endl;
  fout.close();
  cout << "Entry successfully added!" << endl;
  return;
}
void printrecords(Record rb[])
{
  cout << right;
  cout <<"#   "<<setw(12)<<"date"<<setw(8)<<"type"<<setw(10)<<"method"
  <<setw(10)<<"category"<<setw(13)<<"amount"<< endl;
  for (int i = 0;i < id; i++)
  {
    cout <<left<<setw(4)<<i+1;
    cout <<right<<setw(12)<<rb[i].r_date<<setw(8)<<rb[i].r_type<<setw(10)
    <<rb[i].r_method<<setw(10)<<rb[i].r_category<<setw(13)<<rb[i].r_amount<< endl;
  }
}
void grow_recordbook(Record * &rb, int &rb_size, int n)
{
  Record * rb_new = new Record[rb_size+n];
  for (int i = 0; i < rb_size; i++)
  {
    rb_new[i]=rb[i];
  }
  delete [] rb;
  rb = rb_new;
  rb_size += n;
  cout << "---> recordbook enlarged to hold" << rb_size << "records" <<endl;
  return;
}
void sortrecords(Record rb[], int function)
{
  string s1,s2;
  if (function==1)
  {
    //bubble sort
    for (int i=0;i<id;i++){
      for (int j=i+1;j<id;j++){
        s1 = rb[i].r_date;
        s2 = rb[j].r_date;
        if (s1 > s2)
        {
          Record buffer = rb[j];
          rb[j]=rb[i];
          rb[i]=buffer;
        }
      }
    }
  }
  else if (function==2)
  {
    for (int i=0;i<id;i++){
      for (int j=i+1;j<id;j++){
        s1 = rb[i].r_method;
        s2 = rb[j].r_method;
        if (s1 > s2)
        {
          Record buffer = rb[j];
          rb[j]=rb[i];
          rb[i]=buffer;
        }
      }
    }
  }
  else if (function==3)
  {
    for (int i=0;i<id;i++){
      for (int j=i+1;j<id;j++){
        s1 = rb[i].r_category;
        s2 = rb[j].r_category;
        if (s1 > s2)
        {
          Record buffer = rb[j];
          rb[j]=rb[i];
          rb[i]=buffer;
        }
      }
    }
  }
  else if (function==4)
  {
    int n1,n2;
    for (int i=0;i<id;i++){
      for (int j=i+1;j<id;j++){
        stringstream(rb[i].r_amount) >> n1;
        stringstream(rb[j].r_amount) >> n2;
        if (n1 > n2)
        {
          Record buffer = rb[j];
          rb[j]=rb[i];
          rb[i]=buffer;
        }
      }
    }
  }
  return;
}
void viewRecord(char view_by) //1 date,2 method,3 category,4 amount,5 no order
{
  string records_file = "records.txt";
  string record_entry;
  ifstream fin;
  fin.open(records_file.c_str(), ios::binary);

  if (fin.fail())
  {
    cout << "Error in opening " << records_file << endl;
    return;
  }
  int recordbook_size = 3;
  Record * recordbook = new Record[recordbook_size];
  int i=0;
  while (getline(fin,record_entry))
  {
    if (i >= recordbook_size) grow_recordbook(recordbook, recordbook_size, 3);

    if (i < recordbook_size)
    {
      string date, type, method, category, amount;
      istringstream iss(record_entry);
      iss >> date >> type >> method >> category >> amount;
      recordbook[i].r_date = date;
      recordbook[i].r_type = type;
      recordbook[i].r_method = method;
      recordbook[i].r_category = category;
      recordbook[i].r_amount = amount;
      ++i;
    }
  }
  fin.close();
  switch (view_by)
  {
    case '1':
      sortrecords(recordbook,1);
      printrecords(recordbook);
      break;
    case '2':
      sortrecords(recordbook,2);
      printrecords(recordbook);
      break;
    case '3':
      sortrecords(recordbook,3);
      printrecords(recordbook);
      break;
    case '4':
      sortrecords(recordbook,4);
      printrecords(recordbook);
      break;
    case '5':
      printrecords(recordbook);
      break;
    default:
      cout << "Incorrect input!" << endl;
  }
  delete [] recordbook;
  return;
}
void monthly_expense_calculator()
{
  ifstream fin("records.txt");
  if (fin.fail())
  {
    cout << "Error in monthly_expense_calculator file opening!" <<endl;
    return;
  }
  string x;
  double monexp=0, y;
  int count=1;
  while (fin >> x)
  {
    if (count%5==0) {
      stringstream(x) >> y;
      if (y < 0) monexp-=y;
    }
    count++;
  }
  fin.close();
  monthly_expense = monexp;
  return;
}
void delete_or_edit_Record(int edit_id, string mode)
{
  if (edit_id > id) {
    cout << "No such entry" << endl;
    return;
  }
  ifstream fin("records.txt");
  if (fin.fail())
  {
    cout << "Error in opening records.txt" << endl;
    return;
  }
  ofstream fout("records2.txt", ios::ate);
  if (fout.fail())
  {
    cout << "Error in opening records2.txt" << endl;
    return;
  }
  string line;
  int count=1;
  while (getline(fin,line))
  {
    if (count==edit_id)
    {
      if (mode=="EDIT")
      {
        cout << "Please enter the edited values of the entry in the following format:\n";
        cout << "YYYY-MM-DD type payment method category amount(negative if expense)"<<endl;
        string date,type,method,category;
        double amount;
        while (amount>10000000||amount<-10000000)
        {
          cout << "Error!(amount range:+/-10,000,000)"<<endl;
          exit(1);
        }
        if (type=="expense") amount *= -1;
        cin >> date >> type >> method >> category >> amount;
        fout <<date<<setw(8)<<type<<setw(10)<<method
        <<setw(10)<<category<<setw(13)<< amount << endl;
      }
      count++;
      continue;
    }
    fout << line << endl;
    count++;
  }
  fin.close();
  fout.close();
  remove("records.txt");
  rename("records2.txt","records.txt");
  if (mode=="DELETE") {
    id--;
    cout <<"Record successfully deleted! There are "<<id<<" records now."<<endl;
  }
  else {
    cout << "Record successfully edited! There are "<<id<<" records now."<<endl;
  }
  monthly_expense_calculator();
  return;
}
void save_data(string todaydate)
{
  ofstream fout;
  fout.open("data.txt", ios::ate);
  if (fout.fail())
  {
    cout << "Failed to save data!" <<endl;
    return;
  }
  fout << budget <<" "<<auto_report<<" "<<todaydate;
  fout.close();
}
void getData(string &last_login_date,string todaydate)
{
  int z,w,count = 0;
  string s;
  ifstream fin,fdata;
  fin.open("data.txt");
  if (fin.fail()) {last_login_date = todaydate; return;}
  fin >> z >> w >> s;
  fin.close();
  fdata.open("records.txt");
  if (fin.fail()) return;
  string line;
  while (getline(fdata, line))
  {
    count++;
  }
  fdata.close();
  id = count;
  monthly_expense_calculator();
  budget = z;
  auto_report = w;
  last_login_date = s;
  return;
}
void Reset_Monthly_Expense()
{
  monthly_expense = 0;
  cout << "Monthly expense reset!" << endl;
  return;
}
char homepage(string todaydate)
{
  char function;
  cout << left;
  cout << "################################################\n"
          "#                                              #\n"
          "#  Program started on "<<setw(25)<<todaydate<<"#"<<'\n';
  if (monthly_expense>budget)
  {
    cout<<"#                                              #\n"
          "#  WARNING: MONTHLY EXPENSE HAS REACHED BUDGET #\n"
          "#  This alert will not be dismissed until the  #\n"
          "#    start of the next month or the budget     #\n"
          "#               has been changed               #\n";
  }
  cout << "#  Expense since last reset: "<<setw(18)<<monthly_expense<<"#"<<"\n"
          "#  Current budget: "<<setw(28)<<budget<<"#"<<"\n"
          "#                                              #\n"
          "#             Select a function                #\n"
          "#  ENTER 1 to add a record                     #\n"
          "#  ENTER 2 to delete a record                  #\n"
          "#  ENTER 3 to view records                     #\n"
          "#  ENTER 4 to edit an existing record          #\n"
          "#  ENTER 6 to generate a report                #\n"
          "#  ENTER 7 to change monthly budget            #\n"
          "#  ENTER 8 to use a new record file            #\n"
          "#  ENTER 9 to toggle autoreport                #\n"
          "#                                              #\n"
          "#  ENTER 0 to exit                             #\n"
          "################################################\n";
  cout << "Select a function: ";
  cin >> function;
  return function;
}
void last_month_report(int days)
{
  double monthly_income = 0, min=0, amount = 0;
  string line, line_in, date, type, method, category, amount_string, duration;
  string min_date = "nothing", min_category = "nothing";
  ifstream fin;
  fin.open("records.txt", ios::binary);
  if (fin.fail()) {
    cout << "error in opening records.txt" <<endl;
    return;
  }
  while (getline(fin,line))
  {
    istringstream line_in(line);
    line_in >> date >> type >> method >> category >> amount;
    stringstream(amount_string) >> amount;
    if (amount>0) monthly_income+=amount;
    if (amount<min) {
      min = amount;
      min_date = date;
      min_category = category;
    }
  }
  min*=-1;
  cout << "----------------------------------------------------"<<endl;
  cout << "Total expense: $"<< monthly_expense<<endl;
  cout << "Total income: $"<< monthly_income<<endl;
  cout << "Average expense: $" << monthly_expense/days << " per day" <<endl;
  cout << "Maximum expense: $"<<min<<" on "<<min_date<<" buying "<<min_category<<endl;
  cout << "Net Balance: $"<<monthly_income-monthly_expense<<endl;

}
void Auto_report_function(string last_login_date)
{
  int days=31;
  string month = last_login_date.substr(5,2);
  if (month=="04"||month=="06"||month=="09"||month=="11")
  {days = 30;}
  else if (month=="02") days = 28;
  cout << "Here is the monthly report for "<<last_login_date.substr(0,7)<<endl;
  last_month_report(days);
}
