//Bank Management System
#include<iostream>
#include<fstream>
#include<iomanip>
#include<string>
#include<algorithm>

using namespace std;
class CustomerAccount{
 int accNo;
 string name;
 string type;
 double balance;
 
 public: void createAccount(){
  string accStr,balStr;
  while(true) {
    cout<<"Enter Account Number: ";
    cin>>accStr;
    bool isNumeric = true;
    for(char c : accStr) {
      if(!isdigit(c)) {
        isNumeric = false;
        break;
      }
    }
    if(isNumeric && !accStr.empty()) {
      accNo = stoi(accStr);
      break;
    }
    else {
      cout<<"Invalid Account Number. Please enter a numeric value."<<endl;
    }
  }
  
  cin.ignore();
  
 while(true) {
  cout<<"Enter Account Holder Name: ";
  getline(cin,name);
  bool valid = true;
  for(char c : name) {
    if(!isalpha(c) && !isspace(c)) {
      valid = false;
      break;
    }
  }
  if(valid && !name.empty()) {
    break;
  }
  else {
    cout<<"Invalid Name. Please enter alphabetic characters only."<<endl;
  }
 }
 
  while(true) {
    cout<<"Enter Account Type(Savings/Current): ";
    getline(cin,type);
    transform(type.begin(),type.end(),type.begin(),::tolower);
    if(type=="savings"||type=="current"){
      break;
    }
  else{
    cout<<"Invalid Account Type. Please enter either 'Savings' or 'Current'."<<endl;
  }
  }  
  
  while(true){
  cout<<"Enter Initial Deposit: ";
  cin>>balStr;
  bool isNumeric = true;
  int dotCount = 0;
  for(char c: balStr) {
    if(!isdigit(c)) {
      if(c == '.' && dotCount == 0) {
        dotCount++;
      }
      else{
        isNumeric = false;
        break;
      }
    }
  }
  if(isNumeric && !balStr.empty()) {
    balance = stod(balStr);
    if(balance>=0) {
      break;
    }
    else{
      cout<<"Balance cannot be Negative!"<<endl;
    }
  }
  else{
    cout<<"Invalid Amount! Enter numeric value only"<<endl;
  }
  } 
 
  cout<<"Account Created Successfully!"<<endl;
 }
 
 void showAccount() const{
  cout<<left<<setw(10)<<accNo
  <<left<<setw(20)<<name
  <<left<<setw(15)<<type
  <<right<<setw(10)<<fixed
  <<setprecision(2)<<balance<<endl;
 }
  int getAccNo() const{
    return accNo;
  }
  string getName() const{
    return name;
  }
  string getType() const{
    return type;
  }
  double getBalance() const {
 return balance;
  }
  
  void deposit(double amount) {
  balance+= amount;
   }
  
  bool withdraw(double amount) {
  if(amount<=balance) {
    balance -= amount;
    return true;
  }
  return false;
  }
  
  void modify() {
    cin.ignore();
    cout<<"Modify Name: "; 
    getline(cin,name);
    cout<<"Modify Type: ";
    getline(cin,type);
    cout<<"Modify Balance: ";
    cin>>balance;
  }
  
  //Save account data to file
  void writetoFile(ofstream &out) const {
 out<<accNo<<" "<<name<<" "<<type<<" "<<balance<<endl;
   }
   
   //Read accoount data from file
   bool readfromFile(ifstream &in) {
    in>>accNo>>name>>type>>balance;
    return !in.fail();
   }
};

void addAccount(){
  CustomerAccount acc;
  acc.createAccount();
  ofstream outfile("bank.txt",ios::app);
  acc.writetoFile(outfile);
  outfile.close();
}

void displayAll() {
  ifstream infile("bank.txt");
  if(!infile) {
    cout<<"No accounts found"<<endl;
    return;
  }
  CustomerAccount acc;
  cout<<left<<setw(10)<<"AccNO"
  <<left<<setw(20)<<"Name"
  <<left<<setw(15)<<"Type"
  <<right<<setw(10)<<"Balance"<<endl;
  
  while(acc.readfromFile(infile)) {
    acc.showAccount();
  }
  infile.close();
}

void searchAccount(){
  int accNo;
  cout<<"Enter Account Number to Search: ";
  cin>>accNo;
  ifstream infile("bank.txt");
  if(!infile) {
    cout<<"File not Found"<<endl;
    return;
  }
  CustomerAccount acc;
  bool found = false;
  while(acc.readfromFile(infile)) {
    if(acc.getAccNo() == accNo) {
      cout<<"Account Found!"<<endl;
      acc.showAccount();
      found = true;
      break;
    }
  }
  if(!found) {
    cout<<"Account not found"<<endl;
  }
  infile.close();
}

void depositWithdraw(bool isDeposit) {
  int accNo;
  double amount;
  cout<<"Enter Account Number: ";
  cin>>accNo;
  ifstream infile("bank.txt");
  ofstream tempfile("temp.txt");
  if(!infile) {
    cout<<"File not found"<<endl;
    return;
  }
 CustomerAccount acc;
 bool found = false;
 while(acc.readfromFile(infile)) {
  if(acc.getAccNo() == accNo) {
    found = true;
    cout<<"Current Details: ";
    acc.showAccount();
    if(isDeposit){
      cout<<"Enter amount to deposit: ";
      cin>>amount;
      acc.deposit(amount);
    }
    else{
      cout<<"Enter amount to withdraw: ";
      cin>>amount;
      if(!acc.withdraw(amount)){
        cout<<"Insufficient Balance"<<endl;
      }
    }
    cout<<"Updated Balance: "<<acc.getBalance()<<endl;
  }
  acc.writetoFile(tempfile);
 }
 infile.close();
 tempfile.close();
 remove("bank.txt");
 rename("temp.txt","bank.txt");
 if(!found) {
  cout<<"Account not found"<<endl;
 }
}

void deleteAccount() {
  int accNo;
  cout<<"Enter account number to delete: ";
  cin>>accNo;
  ifstream infile("bank.txt");
  ofstream tempfile("temp.txt");
  if(!infile) {
    cout<<"File not found"<<endl;
    return;
  }
  CustomerAccount acc;
  bool deleted = false;
  while(acc.readfromFile(infile)) {
    if(acc.getAccNo() == accNo) {
      deleted = true;
      cout<<"Account deleted successfully"<<endl;
      continue;
    }
    acc.writetoFile(tempfile);
  }
  infile.close();
  tempfile.close();
  remove("bank.txt");
  rename("temp.txt","bank.txt");
  if(!deleted) {
    cout<<"Account not found"<<endl;
  }
}

void modifyAccount() {
  int accNo;
  cout<<"Enter account number to modify: ";
  cin>>accNo;
  ifstream infile("bank.txt");
  ofstream tempfile("temp.txt");
  if(!infile) {
    cout<<"File not found"<<endl;
    return;
  }
 CustomerAccount acc;
 bool found = false;
 while(acc.readfromFile(infile)) {
  if(acc.getAccNo() == accNo) {
    cout<<"Existing Details: ";
    acc.showAccount();
    cout<<"Enter new details"<<endl;
    acc.modify();
    found = true;
  }
  acc.writetoFile(tempfile);
 }
 infile.close();
 tempfile.close();
 remove("bank.txt");
 rename("temp.txt","bank.txt");
 if(found) {
  cout<<"Account modified successfully"<<endl;
 }
 else{
  cout<<"Account not found"<<endl;
 }
}
 int main(){
    int choice;
  do{
  
    cout<<"----------Bank Management System----------"<<endl;
    cout<<"1-Create Account"<<endl;
    cout<<"2-Display All Accounts"<<endl;
    cout<<"3-Search Account"<<endl;
    cout<<"4-Deposit Amount"<<endl;
    cout<<"5-Withdraw Amount"<<endl;
    cout<<"6-Modify Account "<<endl;
    cout<<"7-Delete Account"<<endl;
    cout<<"8-Exit"<<endl;
    cout<<"Enter your choice"<<endl;
    cin>>choice;
        system("cls");

  
    switch(choice) {
     
      case 1: 
      addAccount();
      break;
      
      case 2: 
      displayAll();
      break;

      case 3:
      searchAccount();
      break;

      case 4:
      depositWithdraw(true);
      break;

      case 5:
      depositWithdraw(false);
      break;

      case 6:
      modifyAccount();
      break;

      case 7:
      deleteAccount();
      break;

      case 8:
      cout<<"Exiting program"<<endl;
      break;

      default:
      cout<<"Invalid Choice";
      break;
    }
  } 
  while(choice != 8);
  return 0;
}