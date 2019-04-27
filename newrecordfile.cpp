#include <iostream>
#include <string>
#include <fstream>
#include "newrecordfile.h"

void nrf::new_record_file()
{
  string filename;
  ifstream fin("records.txt");
  if (fin.fail()) {
    cout << "error in opening records.txt" <<endl;
    return;
  }
  ofstream fout;
  cout << "Enter a new name the current records.txt file" << endl;
  cin >> filename;
  rename("records.txt",filename.c_str());
  fout.open("records.txt");
  if(fout.fail())
  {
    cout << "error in creating a new records.txt file" << endl;
    return;
  }
  fin.close();
  fout.close();
  cout << "New file successfully created!" << endl;
  return;
}
