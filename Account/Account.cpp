#include "Account.h"
#include "Shipper.h"

Account::Account()
{
	ID = "";
	Name = "";
	Money.setBalance(0);
	YOB = 0;
	Address = "";
	Email = "";
	Phone = "";
}

Account::Account(const Account& p)
{
	// TODO: my money or your money?
	this->Money = p.Money;
	this->ID = p.ID;
	this->Name = p.Name;
	this->YOB = p.YOB;
	this->Address = p.Address;
	this->Email = p.Email;
	this->Phone = p.Phone;
}

Account::Account(string ID, string Name, int64_t Balance, int64_t YOB, string Address , string Email, string Phone) {
	Money.Deposit(Balance);
	this->ID = ID;
	this->Name = Name;
	this->YOB = YOB;
	this->Address = Address;
	this->Email = Email;
	this->Phone = Phone;
}

Account::Account(string id, string name, int64_t yob, string address, string email, string phone) {
	ID = id;
	Name = name;
	YOB = yob;
	Address = address;
	Email = email;
	Phone = phone;
}

Account::~Account()
{
	this->FillteredOrder.clear();
}

void Account::EditInfo() {
	string a;
	cout << " \t Account Information : \n";
	cout << "ID : \t" << ID << endl;
	cout << "NAME : \t" << Name << endl;
	cout << "YEAR of BIRTH :\t" << YOB << endl;
	cout << "ADDRESS : \t" << Address << endl;
	cout << "EMAIL : \t" << Email << endl;
	cout << "PHONE NUMBER : \t" << Phone << endl;
	cout << endl << endl << "Do you want to change your infomation ? (Y/N) :" << endl;
	while (getchar() != '\n');
	getline(cin, a);
	if (a == "Y") {
		cout << " \t Account Information : \n";
		cout << "ID : \t"; getline(cin, ID); cout << endl;
		cout << "NAME : \t"; getline(cin, Name); cout << endl;
		cout << "YEAR of BIRTH :\t"; cin >> YOB; cout << endl;
		cout << "ADDRESS : \t"; while (getchar() != '\n'); getline(cin, Address); cout << endl;
		cout << "EMAIL : \t"; getline(cin, Email); cout << endl;
		cout << "PHONE NUMBER : \t"; getline(cin, Phone); cout << endl;
	}
	return;
}

void Account::DeleteAccount()
{
	this->~Account();
}

void Account::OutputInfo(){
	cout << "ID            : " << ID << endl;
	cout << "Name          : " << Name << endl;
	cout << "Year of Birth : " << YOB << endl;
	cout << "Address       : " << Address << endl;
	cout << "Email         : " << Email << endl;
	cout << "Phone         : " << Phone << endl;
}