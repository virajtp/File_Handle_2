#include<iostream>
#include <fstream>
#include <string>
#include <math.h>
#include<iomanip>
using namespace std;

double bankBal = 1000000;

//function to round the float value to two decimal places
float roundVal(float x)
{
	float val = (int)(x * 100 + 0.5);
	return (float)val / 100;
}

//function to detect the last line of the file
string getLastline(string accNo) {
	string lastLine;

	ifstream read("Account_" + accNo + ".txt", std::ios_base::ate);//open file
	string tmp;
	int length;

	char c = '\0';

	if (read)
	{
		length = read.tellg();//Get file size
		int i;
		for (i = length - 5; i >= 0; i--)
		{
			read.seekg(i);
			c = read.get();
			if (c == '\r' || c == '\n')//new line?
				break;
		}
		getline(read, tmp);//read last line
		lastLine = tmp;
	}
	return lastLine;
}

//function to add interest and reduce charges in days which hasn't any transactions
void balanceAccounts(fstream& file) {

	if (file.is_open()) { //checking the file open or not
		
		string initial = "";
		int c = 0;

		while (getline(file, initial)) { //read data from the file

			string date = "";
			string lastDate = "";
			string accNo = "";
			string lastBal;
			double accBal;

			int n = initial.length();	//getting the num of characters of the line
			if (n != 0) {				//check the line is empty or not

				for (int i = 0; i < n;) {
					while (initial[i] != ',') {
						i++;
					}
					i++;
					while (initial[i] != ',') {
						accNo += initial[i];
						i++;
					}
					break;
				}

				if (accNo != "111111") {	//check is it the bank vault account

					string lastLine = getLastline(accNo);	//detect the last line of the file

					int m = lastLine.length();	//getting the num of characters of the line
					for (int i = 0; i < 8; i++)
						lastDate += lastLine[i];

					int j = 0, a = 0;
					while (a < 3) {
						if (lastLine[j] == ',')
							a++;
						j++;
					}

					while (j < m)
					{
						lastBal += lastLine[j];
						j++;
					}
					accBal = stod(lastBal);	//convert last mentioned balance into double

					int intlastDate = stoi(lastDate);	//convert last mentioned date into integer

					ofstream acc, accBank;

					if (lastDate != "20190112"&&accNo != "111111") {
						while (intlastDate < 20190112) {

							if (accBal < 0)
							{
								accBal -= 50;
								bankBal += 50;

								acc.open("Account_" + accNo + ".txt", ios::app);
								acc << intlastDate << "," << '4' << "," << "50" << "," << fixed << setprecision(2) << accBal << endl << endl;
								acc.close();
								
								accBank.open("Account Bank's vault_111111.txt", ios::app);
								accBank << intlastDate  << "," << '4' << "," << "50" << "," << fixed << setprecision(2) << bankBal << endl << endl;
								accBank.close();
							}

							else if (accBal < 1000)
							{
								accBal -= 10;
								bankBal += 10;

								acc.open("Account_" + accNo + ".txt", ios::app);
								acc << intlastDate << "," << '4' << "," << "10" << "," << fixed << setprecision(2) << accBal << endl << endl;
								accBank.close();

								accBank.open("Account Bank's vault_111111.txt", ios::app);
								accBank << intlastDate  << "," << '4' << "," << "10" << "," << fixed << setprecision(2) << bankBal << endl << endl;
								accBank.close();
							}

							if (accBal >= 0)
							{
								double interest = roundVal(accBal*0.02 / 100);
								accBal += interest;
								bankBal -= interest;

								acc.open("Account_" + accNo + ".txt", ios::app);
								acc << intlastDate + 1 << "," << '3' << "," << interest << "," << fixed << setprecision(2) << accBal << endl << endl;
								acc.close();

								accBank.open("Account Bank's vault_111111.txt", ios::app);
								accBank << intlastDate + 1 << "," << '3' << "," << interest << "," << fixed << setprecision(2) << bankBal << endl << endl;
								accBank.close();
							}
							intlastDate++;
						}
					}
				}
			}
		}
	}
}

void transaction(fstream& file) {

	int count = 0;
	char transType;	//transaction type

	if (file.is_open()) { //checking the file open or not
		string initial = " ";

		while (getline(file, initial)) { //read data from the file
			
			string date = "";		//variable for date
			string lastDate = "";	//variable for last mentioned date	
			string accNo = "";		//variable for account number
			string iniBal = "";		//initial balance
			string strAmount = "";	//string variable for transaction amount
			string lastBal;			//variable for last balance of the account
			double amount;			//variable for transaction amount
			double accBal = 0;		//variable for account balance
			int n = initial.length();	//getting the num of characters of the line

			if (n != 0) {

				for (int i = 0; i < n;) {
				while (initial[i] != ',') {
					date += initial[i];
					i++;
				}
				i++;
				while (initial[i] != ',') {
					accNo += initial[i];
					i++;
				}
				i++;

				if (initial[i + 1] != ',') {
					transType = '0';
					while (i < n)
					{
						iniBal += initial[i];
						i++;
					}
					strAmount = iniBal;
					break;
				}
				else {
					transType = initial[i];
					i += 2;
					while (i < n) {
						strAmount += initial[i];
						i++;
					}
					break;
				}
			}

				ofstream acc, accBank;
				//Getting the last line
				if (transType != '0') {		//check is it initial balance

					string lastLine = getLastline(accNo);	//detect the last line of the file

					int m = lastLine.length();				//getting the num of characters of the line
					for (int i = 0; i < 8;i++)
						lastDate += lastLine[i];

					int i = 0, a = 0;
					while (a < 3) {
						if (lastLine[i] == ',')
							a++;
						i++;
					}
					
					while (i < m)
					{
						lastBal += lastLine[i];
						i++;
					}
					accBal = stod(lastBal);

		
					if (lastDate != date) {
						int intlastDate = stoi(lastDate);	//convert last mentioned date into integer
						int intDate = stoi(date);			//convert current date into integer
						
						while (intlastDate < intDate) {

						if (accBal < 0)
						{
							accBal -= 50;
							bankBal += 50;

							acc.open("Account_" + accNo + ".txt", ios::app);
							acc << intlastDate << "," << '4' << "," << "50" << "," << fixed << setprecision(2) << accBal << endl << endl;
							acc.close();

							accBank.open("Account Bank's vault_111111.txt", ios::app);
							accBank << intlastDate << "," << '4' << "," << "50" << "," << fixed << setprecision(2) << bankBal << endl << endl;
							accBank.close();
						}
						
						else if (accBal < 1000)
						{
							accBal -= 10;
							bankBal += 10;

							acc.open("Account_" + accNo + ".txt", ios::app);
							acc << intlastDate << "," << '4' << "," << "10" << "," << fixed << setprecision(2) << accBal << endl << endl;
							acc.close();

							accBank.open("Account Bank's vault_111111.txt", ios::app);
							accBank << intlastDate  << "," << '4' << "," << "10" << "," << fixed << setprecision(2) << bankBal << endl << endl;
							accBank.close();
						}

						if (accBal >= 0)
						{
							double interest = roundVal(accBal*0.02 / 100);
							accBal += interest;
							bankBal -= interest;

							acc.open("Account_" + accNo + ".txt", ios::app);
							acc << intlastDate+1 << "," << '3' << "," << interest << "," << fixed << setprecision(2) << accBal << endl << endl;
							acc.close();

							accBank.open("Account Bank's vault_111111.txt", ios::app);
							accBank << intlastDate+1 << "," << '3' << "," << interest << "," << fixed << setprecision(2) << bankBal << endl << endl;
							accBank.close();
						}
						intlastDate++;
						}

					}
				}

				else {
					accBal += stod(iniBal);
					if(accNo=="111111")
						acc.open("Account Bank's vault_" + accNo + ".txt");
					else
						acc.open("Account_" + accNo + ".txt");
					acc <<" "<< date << "," << transType << "," << strAmount << "," << fixed << setprecision(2) << accBal << endl << endl;
					acc.close();
				}
					
					switch (transType) {
					case '0': {	//initial balance
						break;	//initial balance is also printed
					}
					
					case '1': {	//deposit
						accBal += stod(strAmount);
						acc.open("Account_" + accNo + ".txt", ios::app);
						acc << date << "," << transType << "," << strAmount << "," << fixed << setprecision(2) << accBal << endl << endl;
						acc.close();
						break;
					}
					case '2': {	//withdraw
						accBal -= stod(strAmount);
						acc.open("Account_" + accNo + ".txt", ios::app);
						acc << date << "," << transType << "," << strAmount << "," << fixed << setprecision(2) << accBal << endl << endl;
						acc.close();
						break;
					}
				}
			}
		}
	}
}


void showCurrentBal() {

	fstream file;
	file.open("balance.txt", ios::in); //open the balance file
	if (file.is_open()) { //checking the file open or not

		string initial = "";
		int c = 0;

		while (getline(file, initial)) { //read data from the file

			string accNo = "";
			string accountBal;

			int n = initial.length();	//getting the num of characters of the line
			if (n != 0) {				//check the line is empty or not

				for (int i = 0; i < n;) {
					while (initial[i] != ',') {
						i++;
					}
					i++;
					while (initial[i] != ',') {
						accNo += initial[i];
						i++;
					}
					break;
				}
				if (accNo != "111111") {
					string lastLine = getLastline(accNo);	//detect the last line of the file
					int m = lastLine.length();				//getting the num of characters of the line
					int i = 0, a = 0;
					while (a < 3) {
						if (lastLine[i] == ',')
							a++;
						i++;
					}
					while (i < m)
					{
						accountBal += lastLine[i];
						i++;
					}
					//showing customer's account current balance
					cout << "Account No." << accNo << " Current Balance : " << fixed << setprecision(2) << stod(accountBal)<< endl;
				}
			}
		}
		
	}
	file.close();
}



int main()
{
	fstream inFile;
	inFile.open("balance.txt", ios::in); //open the balance file
	
	if (!inFile)
	{
		cout << "The balance file cannot be loaded!!!!" << endl;
		system("pause");
		return 0;
	}

	transaction(inFile);	//explpore the balance file

	inFile.close();			//close the balance file

	inFile.open("transaction.txt", ios::in);	//open the transaction file

	if (!inFile)
	{
		cout << "The transaction file cannot be loaded!!!!" << endl;
		system("pause");
		return 0;
	}

	transaction(inFile);	//explore the transaction file

	inFile.close();			//close the transaction file

	inFile.open("balance.txt", ios::in); //open the balance file

	balanceAccounts(inFile);	//explpore the balance file to get the acc numbers

	inFile.close();			//close the balance file

	//declare variables
	int selection;
	string accNum,accountBal;
	cout << "-----------Select your option-----------\n\n";	//Display options to the user
	cout << "\t1 - Bank Vault Balance\n";
	cout << "\t2 - Customer Accounts' Balances\n";
	cout << "\n\tEnter your choice : ";
	cin >> selection;		//getting user input
	cout << endl;

	if(selection==1) {	//show Current Bank vault Balance
		cout << "Current Bank vault Balance : " << fixed << setprecision(2) << bankBal << endl << endl;
	}
	else if (selection == 2) {	//to show a customer account balance
		showCurrentBal();
	}
	else {
		cout << "\tInvalid Input !!!\n";
	}
	cout << endl;
	system("pause");
	return 0;
}