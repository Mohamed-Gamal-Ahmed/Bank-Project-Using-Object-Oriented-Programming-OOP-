#pragma once
#include <iostream>
#include <string>
#include "clsPerson.h"
#include "clsString.h"
#include <vector>
#include <fstream>
#include<iomanip>
using namespace std;
class clsBankClients : public clsPerson
{
private:
	enum enMode { EmtyMode = 0, UpdateMode = 1, AddNewMode = 2 };
	enMode _Mode;
	string _AccountNumber;
	string _PinCode;
	float _AccountBalance;
	bool _MarkForDelete = false;

	static clsBankClients _CnvertLinetoClientObject(string Line, string Seperator = "#//#")
	{
		vector<string>VClientData;
		VClientData = clsString::Split(Line, Seperator);

		return clsBankClients(enMode::UpdateMode, VClientData[0], VClientData[1], VClientData[2],
			VClientData[3], VClientData[4], VClientData[5], stod(VClientData[6]));

	}

	static string _ConvertClientObjecttoLine(clsBankClients Client, string Seperator = "#//#")
	{
		string stClientDataRecord = "";
		stClientDataRecord += Client.FirstName + Seperator;
		stClientDataRecord += Client.LastName + Seperator;
		stClientDataRecord += Client.FullName() + Seperator;
		stClientDataRecord += Client.Email + Seperator;
		stClientDataRecord += Client.Phone + Seperator;
		stClientDataRecord += Client.AccountNumber() + Seperator;
		stClientDataRecord += Client.FirstName + Seperator;
		stClientDataRecord += to_string(Client.AccountBalance);
		return stClientDataRecord;
	}

	static vector<clsBankClients> _LoadDataClientFromFile()
	{
		vector< clsBankClients>VClinetsData;
		fstream MyFile;
		MyFile.open("Clients.txt", ios::in);

		if (MyFile.is_open())
		{
			string Line;
			while (getline(MyFile, Line))
			{
				clsBankClients Clients = _CnvertLinetoClientObject(Line);
				VClinetsData.push_back(Clients);
			}
			MyFile.close();
		}
		return VClinetsData;
	}

	static void _SaveCleintsDataToFile(vector< clsBankClients>VClinetsData)
	{
		fstream MyFile;
		MyFile.open("Clients.txt", ios::out);

		if (MyFile.is_open())
		{
			string Line;
			for (clsBankClients C : VClinetsData)
			{
				if (C._MarkForDelete == false)
				{
					Line = _ConvertClientObjecttoLine(C);
					MyFile << Line << endl;
				}

			}
			MyFile.close();
		}
	}

	void _Update()
	{
		vector< clsBankClients> _VClinetsData;
		_VClinetsData = _LoadDataClientFromFile();
		for (clsBankClients& C : _VClinetsData)
		{
			if (C.AccountNumber() == AccountNumber())
			{
				C = *this;
				break;
			}
		}
		_SaveCleintsDataToFile(_VClinetsData);
	}

	static void _AddDataLinetoFile(string Line)
	{
		fstream MyFile;
		MyFile.open("Clients.txt", ios::out | ios::app);

		if (MyFile.is_open())
		{
			MyFile << Line << endl;
			MyFile.close();
		}
	}

	void _AddNew()
	{
		_AddDataLinetoFile(_ConvertClientObjecttoLine(*this));
	}

	static clsBankClients _GetEmtyClientObject()
	{
		return clsBankClients(enMode::EmtyMode, "", "", "", "", "", "", 0);
	}



public:

	
	clsBankClients(enMode Mode, string FirstName, string LastName,
		string Email, string Phone, string AccountNumber, string PinCode,
		float AccountBalance) : clsPerson(FirstName, LastName, Email, Phone)
	{
		_Mode = Mode;
		_AccountNumber = AccountNumber;
		_PinCode = PinCode;
		_AccountBalance = AccountBalance;
	}

	bool IsEmpty()
	{
		return (_Mode = enMode::EmtyMode);
	}

	string AccountNumber()
	{
		return _AccountNumber;
	}

	void setPinCode(string PinCode)
	{
		_PinCode = PinCode;
	}
	string getPinCode()
	{
		return _PinCode;
	}
	__declspec(property(get = getPinCode, put = setPinCode)) string PinCode;

	void setAccountBalance(float AccountBalance)
	{
		_AccountBalance = AccountBalance;
	}
	float getAccountBalance()
	{
		return _AccountBalance;
	}
	__declspec(property(get = getAccountBalance, put = setAccountBalance)) float AccountBalance;

	static clsBankClients Find(string AccountNumber)
	{
		fstream MyFile;
		MyFile.open("Clients.txt", ios::in);

		if (MyFile.is_open())
		{
			string Line;
			while (getline(MyFile, Line))
			{
				clsBankClients Client = _CnvertLinetoClientObject(Line);
				if (Client.AccountNumber() == AccountNumber)
				{
					MyFile.close();
					return Client;
				}
			}

			MyFile.close();

		}
		return _GetEmtyClientObject();
	}

	static clsBankClients Find(string AccountNumber, string PinCode)
	{
		fstream MyFile;
		MyFile.open("Clients.txt", ios::in);

		if (MyFile.is_open())
		{
			string Line;
			while (getline(MyFile, Line))
			{
				clsBankClients Client = _CnvertLinetoClientObject(Line);
				if (Client.AccountNumber() == AccountNumber && Client.PinCode == PinCode)
				{
					MyFile.close();
					return Client;
				}
			}

		}
		return _GetEmtyClientObject();
	}

	static bool IsClientExist(string AccountNumber)
	{
		clsBankClients Client = clsBankClients::Find(AccountNumber);
		return (!Client.IsEmpty());
	}

	bool Delete()
	{
		vector< clsBankClients>_VClientData;
		_VClientData = _LoadDataClientFromFile();
		for (clsBankClients& C : _VClientData)
		{
			if (C.AccountNumber() == _AccountNumber)
			{
				C._MarkForDelete = true;
				break;
			}
		}
		_SaveCleintsDataToFile(_VClientData);

		*this = _GetEmtyClientObject();

		return true;

	}

	enum enSaveResult { svFaildEmptyObject = 0, svSucceeded = 1, svFaildAccountNumberExists = 2 };

	enSaveResult Save()
	{
		switch (_Mode)
		{
		case clsBankClients::EmtyMode:

			return enSaveResult::svFaildEmptyObject;

			break;
		case clsBankClients::UpdateMode:

			_Update();

			return enSaveResult::svSucceeded;

			break;
		case clsBankClients::AddNewMode:
			if (clsBankClients::IsClientExist(_AccountNumber))
			{
				return enSaveResult::svFaildAccountNumberExists;
			}
			else
			{
				_AddNew();
				_Mode = enMode::AddNewMode;
				return enSaveResult::svSucceeded;
			}
		}



	}
	static clsBankClients GetAddNewClientObject(string AccountNumber)
	{
		return clsBankClients(enMode::AddNewMode, "", "", "", "", AccountNumber, "", 0);
	}

	static vector<clsBankClients> GetClientsList()
	{
		return _LoadDataClientFromFile();
	}
	static double GetTotalBalances()
	{
		vector <clsBankClients> vClients = clsBankClients::GetClientsList();

		double TotaleBalances = 0;
		for (clsBankClients C : vClients)
		{
			TotaleBalances += C.AccountBalance;
		}
		return TotaleBalances;
	}
	static void PrintClientRecordLine(clsBankClients Clients)
	{
		cout << "| " << setw(15) << left << Clients.AccountNumber();
		cout << "| " << setw(20) << left << Clients.FullName();
		cout << "| " << setw(12) << left << Clients.Phone;
		cout << "| " << setw(20) << left << Clients.Email;
		cout << "| " << setw(10) << left << Clients.PinCode;
		cout << "| " << setw(12) << left << Clients.AccountBalance;
	}

	static void PrintClientRecordBalanceLine(clsBankClients Client)
	{

		cout << "| " << setw(15) << left << Client.AccountNumber();
		cout << "| " << setw(40) << left << Client.FullName();
		cout << "| " << setw(12) << left << Client.AccountBalance;

	}

	void Print()
	{
		cout << "\nClient Card:";
		cout << "\n___________________";
		cout << "\nFirstName   : " << FirstName;
		cout << "\nLastName    : " << LastName;
		cout << "\nFull Name   : " << FullName();
		cout << "\nEmail       : " << Email;
		cout << "\nPhone       : " << Phone;
		cout << "\nAcc. Number : " << _AccountNumber;
		cout << "\nPassword    : " << _PinCode;
		cout << "\nBalance     : " << _AccountBalance;
		cout << "\n___________________\n";

	}
};

