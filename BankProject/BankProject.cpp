#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
enum ChoiceMenue { ListClient = 1, Add = 2, Delete = 3, Update = 4,Find=5,Transaction=6,ManageUser=7,Logout=8};
enum ChoiceTrans { Deposite = 1, Withdraw = 2, MainMenu = 3};
enum PermessionOption{FullAcces=-1,ShowClient=1,AddClient=2,Del=4,Upd=8,FindClient=16,Trans=32,ManageUsers=64};
enum ChoiceManageUsers{ShowUsers=1,AddUser=2,FindUser=3,DelteUser=4,MainMenue=5};
using namespace std;
const string FileName1 = "Clients.txt";
const string FileName2 = "Users.txt";
struct Client {
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance = 0;
	bool MarkForDelete = false;
};
struct User
{
	string UserName;
	string Password;
	int    Permession = 0;
	bool MarkForDelete = false;
};
User CurrentUser;
void PrintUserCard(User);
string ReadAccountNumber();
void ShowMainMenue();
void ShowTransactionMenue();
void ShowManageUsersMenue();
void Login();
double ReadWithdrawAmount(Client);
vector<string> SplitString(string s1, string delim)
{
	vector<string> vString;
	short pos = 0;
	string word = "";
	while ((pos = s1.find(delim)) != string::npos)
	{
		word = s1.substr(0, pos);
		if (word != "")
			vString.push_back(word);
		s1.erase(0, pos + delim.length());
	}
	if (s1 != "")
		vString.push_back(s1);
	return vString;
}
Client ConvertLinetoRecordClient(string Line, string seperator = "#//#")
{
	Client client;
	vector<string> vString = SplitString(Line,seperator);
	client.AccountNumber = vString[0];
	client.PinCode = vString[1];
	client.Name = vString[2];
	client.Phone = vString[3];
	client.AccountBalance = stod(vString[4]);
	return client;
}
User ConvertLinetoRecodUser(string Line, string sep = "#//#")
{
	User user;
	vector<string> vString = SplitString(Line, sep);
	user.UserName = vString[0];
	user.Password = vString[1];
	user.Permession = stoi(vString[2]);
	return user;
}
string ConvertRecordtoLine(Client client, string seperator = "#//#")
{
	string ClientRecord = "";
	ClientRecord += client.AccountNumber+ seperator;
	ClientRecord += client.PinCode + seperator;
	ClientRecord += client.Name + seperator;
	ClientRecord += client.Phone + seperator;
	ClientRecord += to_string(client.AccountBalance);
	return ClientRecord;
}
string ConvertRecordtoLine(User user, string seperator = "#//#")
{
	string Line = "";
	Line += user.UserName + seperator;
	Line += user.Password + seperator;
	Line += to_string(user.Permession);
	return Line;
}
bool ClinetExistByAccountNumber(string AccountNumber, string FileName)
{
	fstream Myfile;
	Myfile.open(FileName, ios::in);
	if (Myfile.is_open())
	{
		string Line;
		Client client;
		while (getline(Myfile, Line))
		{
			client = ConvertLinetoRecordClient(Line);
			if (client.AccountNumber == AccountNumber)
			{
				Myfile.close();
				return true;
			}
		}
		Myfile.close();
	}
	return false;
}
bool UserExistByName(string UserName,string FileName)
{
	fstream Myfile;
	Myfile.open(FileName, ios::in);
	if (Myfile.is_open())
	{
		string Line;
		User user;
		while (getline(Myfile, Line))
		{
			user = ConvertLinetoRecodUser(Line);
			if (user.UserName == UserName)
			{
				Myfile.close();
				return true;
			}
		}
		Myfile.close();
	}
	return false;
}
Client ReadNewClient()
{
	Client client;
	cout << "Enter Account Number?";
	getline(cin >> ws, client.AccountNumber);
	while (ClinetExistByAccountNumber(client.AccountNumber,FileName1))
	{
		cout << "\nClient with [" << client.AccountNumber << "] already exists\n";
		cout<<"\nEnter another Account Number ? ";
		getline(cin >> ws, client.AccountNumber);
	}
	cout << "Enter PinCode? ";
	getline(cin >> ws, client.PinCode);
	cout << "Enter Name? ";
	getline(cin >> ws, client.Name);
	cout << "Enter Phone? ";
	getline(cin >> ws, client.Phone);
	cout << "Enter Account Balance? ";
	cin >> client.AccountBalance;
	return client;
}
int ReadPermession()
{
	int Permession = 0;
	char Answer = 'n';
	cout << "\nDo you want to give Full access? y/n? ";
	cin >> Answer;
	if (tolower(Answer) == 'y')
	{
		return -1;
	}
	cout << "\nDo you wnat to give acces to :\n";
	cout << "\nShow Client List? y/n? ";
	cin >> Answer;
	if (tolower(Answer) == 'y')
	{
		Permession += PermessionOption::ShowClient;
	}
	cout << "\nAdd New Client? y/n? ";
	cin >> Answer;
	if (tolower(Answer) == 'y')
		Permession += PermessionOption::AddClient;
	cout << "\nDelete Client? y/n? ";
	cin >> Answer;
	if (tolower(Answer) == 'y')
		Permession += PermessionOption::Del;
	cout << "\nUpdate Client? y/n? ";
	cin >> Answer;
	if (tolower(Answer) == 'y')
		Permession += PermessionOption::Upd;
	cout << "\nFind Client y/n? ";
	cin >> Answer;
	if (tolower(Answer) == 'y')
		Permession += PermessionOption::FindClient;
	cout << "\nTransaction? y/n? ";
	cin >> Answer;
	if (tolower(Answer) == 'y')
		Permession += PermessionOption::Trans;
	cout << "\nManage Users? y/n? ";
	cin >> Answer;
	if (tolower(Answer) == 'y')
		Permession += PermessionOption::ManageUsers;
	return Permession;
}
User ReadNewUser()
{
	User user;
	cout << "Please Enter a UserName? ";
	getline(cin >> ws, user.UserName);
	while (UserExistByName(user.UserName,FileName2))
	{
		cout << "User [" << user.UserName << "] Already exist ,Enter another UserName? ";
		cin >> user.UserName;
	}
	cout << "Please Enter a Password? ";
	getline(cin >> ws, user.Password);
	user.Permession = ReadPermession();
	return user;
}
string ReadUserName()
{
	string UserName;
	cout << "\nPlease enter Username? ";
	cin >> UserName;
	return UserName;
}
vector<Client> LoadClientsDataFromFile(string FileName)
{
	fstream Myfile;
	vector<Client> Clients;
	Myfile.open(FileName, ios::in);
	if (Myfile.is_open())
	{
		string Line;
		while (getline(Myfile, Line))
		{
			Clients.push_back(ConvertLinetoRecordClient(Line));
		}
		Myfile.close();
	}
	return Clients;
}
vector<User> LoadUsersDataFromFile(string FileName)
{
	fstream Myfile;
	vector<User> Users;
	Myfile.open(FileName, ios::in);
	if (Myfile.is_open())
	{
		string Line;
		while (getline(Myfile, Line))
		{
			Users.push_back(ConvertLinetoRecodUser(Line));
		}
		Myfile.close();
	}
	return Users;
}
void PrintClient(Client client)
{
	cout << "| " << setw(25) << left << client.AccountNumber << "| " << setw(10) << left << client.PinCode << "| " << setw(40) << left << client.Name << "| " << setw(12) << left << client.Phone << "| " << setw(12) << left << client.AccountBalance << endl;
}
void PrintUser(User user)
{
	cout << "| " << setw(15) << left << user.UserName;
	cout << "| " << setw(10) << left << user.Password;
	cout << "| " << setw(40) << left << user.Permession;
}
void PrintClientCard(Client client)
{
	cout << "\n\t\tClient Detail\n";
	cout << "*****************************************************\n";
	cout << "Account Number  :" << client.AccountNumber << endl;
	cout << "Pin Code        :" << client.PinCode << endl;
	cout << "Name            :" << client.Name << endl;
	cout << "Phone           :" << client.Phone << endl;
	cout << "Account Balance :" << client.AccountBalance << endl;
	cout << "*****************************************************\n";
}
bool FindClientByAccountNumber(string AccountNumber, vector<Client> Clients, Client& client1)
{
	for (Client client : Clients)
	{
		if (client.AccountNumber == AccountNumber)
		{
			client1 = client;
			return true;
		}
	}
	return false;
}
bool FindClientByAccountNumber(string AccountNumber, vector<Client> Clients)
{
	for (Client client : Clients)
	{
		if (client.AccountNumber == AccountNumber)
		{
			return true;
		}
	}
	return false;
}
bool FindUserByUsername(string Username, vector<User> Users, User& user)
{
	for (User u : Users)
	{
		if (u.UserName == Username)
		{
			user = u;
			return true;
		}
	}
	return false;
}
bool FindUserByName_Password(string UserName, string Password, User& user)
{
	vector<User> Users = LoadUsersDataFromFile(FileName2);
	for (User user1 : Users)
	{
		if (user1.UserName == UserName && user1.Password == Password)
		{
			user = user1;
			return true;
		}
	}
	return false;
}
Client ChangeClientRecord(string AccountNumber)
{
	Client client;
	client.AccountNumber = AccountNumber;
	cout << "\n Enter the PinCode? ";
	getline(cin >> ws, client.PinCode);
	cout << "Enter Name? ";
	getline(cin >> ws, client.Name);
	cout << "Enter Phone? ";
	getline(cin >> ws, client.Phone);
	cout << "Enter Account Balance? ";
	cin >> client.AccountBalance;
	return client;
}
bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector<Client>& Clients)
{
	for (Client &client : Clients)
	{
		if (client.AccountNumber == AccountNumber)
		{
			client.MarkForDelete = true;
			return true;
		}
	}
	return false;
}
bool MarkUserForDeleteByName(string Username, vector<User> &Users)
{
	for (User &user : Users)
	{
		if (user.UserName == Username)
		{
			user.MarkForDelete = true;
			return true;
		}
	}
	return false;
}
void SaveClientsDatatoFile(string FileName, vector<Client> Clients)
{
	fstream Myfile;
	Myfile.open(FileName, ios::out);
	if (Myfile.is_open())
	{
		for (Client client : Clients)
		{
			if (!client.MarkForDelete)
			{
				Myfile << ConvertRecordtoLine(client) << endl;
			}
		}
	}
}
void SaveUsersDatatoFile(string FileName, vector<User> Users)
{
	fstream Myfile;
	Myfile.open(FileName, ios::out);
	if (Myfile.is_open())
	{
		for (User user : Users)
		{
			if(!user.MarkForDelete)
			Myfile << ConvertRecordtoLine(user) << endl;
		}
		Myfile.close();
	}
}
void AddDataLinetoFile(string FileName, string DataLine)
{
	fstream Myfile;
	Myfile.open(FileName, ios::out |ios::app);
	if (Myfile.is_open())
	{
		Myfile << DataLine << endl;
		Myfile.close();
	}
}
void AddNewClient()
{
	Client client;
	client = ReadNewClient();
	AddDataLinetoFile(FileName1, ConvertRecordtoLine(client));
}
void AddNewUser()
{
	User user = ReadNewUser();
	AddDataLinetoFile(FileName2, ConvertRecordtoLine(user));
}
void AddNewClients()
{
	char Answer = 'n';
	do {
		cout << "\n\nAdding New Client:\n";
		AddNewClient();
		cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";
		cin.ignore();
		cin >> Answer;
	} while (toupper(Answer) == 'Y');
}
void AddNewUsers()
{
	char Answer = 'n';
	do {
		cout << "\t\tAdding New User:\n\n";
		AddNewUser();
		cout << "User Added Successfully, do you want to add more Users? Y/N ";
		cin >> Answer;

	} while (tolower(Answer) == 'y');
}
bool DeleteClientByAccountNumber(string AccountNumber, vector<Client> &Clients)
{
	Client client;
	char Answer = 'y';
	if (FindClientByAccountNumber(AccountNumber,Clients, client))
	{
		PrintClientCard(client);
		cout << "\n\nAre you sure you want delete this clinet? y/n? ";
		cin >> Answer;
		if (tolower(Answer) == 'y')
		{
			MarkClientForDeleteByAccountNumber(AccountNumber, Clients);
			SaveClientsDatatoFile(FileName1, Clients);
			Clients = LoadClientsDataFromFile(FileName1);
			cout << "\n\nClient Deleted Successfully ";
			return true;
		}
	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
		return false;
	}
}
bool DeleteUserByUsername(string Username, vector<User>& Users)
{
	if (Username == "Admin")
	{
		cout << "\n\nYou cannot Delete This User.";
		return false;
	}
	User user;
	char Answer = 'n';
	if (FindUserByUsername(Username, Users, user))
	{
		PrintUserCard(user);
		cout << "\n\nAre you sure you want delete this User? y/N ?";
		cin >> Answer;
		if (tolower(Answer) == 'y')
		{
			MarkUserForDeleteByName(Username, Users);
			SaveUsersDatatoFile(FileName2, Users);
			cout << "\n\nUser Deleted Successfully. ";
			return true;
		}
	}
	else
	{
		cout << "\nUser with Username (" << Username << ") is Not Found!";
		return false;
	}
}
bool UpdateClientByAccountNumber(string AccountNumber, vector<Client>& Clients)
{
	Client client;
	char Answer = 'n';
	if (FindClientByAccountNumber(AccountNumber, Clients, client))
	{
		PrintClientCard(client);
		cout << "\n\nAre you sure you want update this client? y/n?";
		cin >> Answer;
		if (tolower(Answer) == 'y')
		{
			for (Client& C : Clients)
			{
				if (C.AccountNumber == AccountNumber)
				{
					C = ChangeClientRecord(AccountNumber);
					break;
				}
			}
			SaveClientsDatatoFile(FileName1, Clients);
			cout << "\n\nClient Updated Successfully.";
			return true;
		}
	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
		return false;
	}
}
bool DepositeToAccount(string AccountNumber, vector<Client>& Clients)
{
	Client client;
	char Answer = 'n';
	double DepositeAmount;
	FindClientByAccountNumber(AccountNumber, Clients, client);
	PrintClientCard(client);
	cout << "\nPlease Enter a Deposite Amount? ";
	cin >> DepositeAmount;
	cout << "\nAre you sure you want perform this transaction? Y/N? ";
	cin >> Answer;
	if (tolower(Answer) == 'y')
	{
		for (Client& client : Clients)
		{
			if (client.AccountNumber == AccountNumber)
			{
				client.AccountBalance += DepositeAmount;
				SaveClientsDatatoFile(FileName1, Clients);
				cout << "\nTransaction Done Succesfully\n";
				return true;
			}
		}
	}
	return false;
}
void WithdrawFromAccount(string AccountNumber, vector<Client>& Clients)
{
	Client client;
	char Answer = 'n';
	double WithdrawAmount=0;
	FindClientByAccountNumber(AccountNumber, Clients, client);
	PrintClientCard(client);
	WithdrawAmount = ReadWithdrawAmount(client);
	cout << "\nAre you sure you want perform this transaction? Y/N? ";
	cin >> Answer;
	if (tolower(Answer) == 'y')
	{
		for (Client& client : Clients)
		{
			if (client.AccountNumber == AccountNumber)
			{
				client.AccountBalance -= WithdrawAmount;
				SaveClientsDatatoFile(FileName1, Clients);
			}
		}

	}
}
void ShowAccessDenied()
{
	system("cls");
	cout << "_______________________________________________________\n";
	cout << "Access Denied ,";
	cout << "\nYou Dont Have Permession To Do This,";
	cout << "\nPlease Contact You Admin\n";
	cout << "_______________________________________________________\n";
}
bool CheckAccessPermession(PermessionOption Permession)
{
	if (CurrentUser.Permession == PermessionOption::FullAcces)
		return true;
	if (CurrentUser.Permession & Permession)
		return true;
	else
		return false;
}
void ShowAllClinetsScreen()
{
	if (!CheckAccessPermession(PermessionOption::ShowClient))
	{
		ShowAccessDenied();
		return;
	}
	vector<Client> Clients = LoadClientsDataFromFile(FileName1);
	cout << "\n\t\t\t\t\t    Clients List (" << Clients.size() << ") Client(s)." << endl;
	cout << "______________________________________________________________________________________________________________________________\n" << endl;
	cout << "| " << setw(25) << left << "Account Number" << "| " << setw(10) << left << "Pin Code" << "| " << setw(40) << left << "Client Name" << "| " << setw(12) << left << "Phone" << "| " << setw(12) << left << "Balance" << endl;
	cout << "______________________________________________________________________________________________________________________________\n" << endl;
	if (Clients.empty())
		cout << "\t\t\tNo Clients Available in the System!";
	else
	{
		for (Client client : Clients)
		{
			PrintClient(client);
		}
	}
	cout << "\n______________________________________________________________________________________________________________________________\n" << endl;
}
void ShowAllUsersScreen()
{
	vector<User> Users = LoadUsersDataFromFile(FileName2);
	cout << "\t\t\t\t\tUsers List (" << Users.size() << ") User(s).";
	cout << "\n________________________________________________________________________________________________\n";
	cout << "|                |          "<<" | "<< endl;
	cout << "| " << left << setw(15) << "User Name";
	cout << "| " << left << setw(10) << "Password";
	cout << "| " << left << setw(40) << "Permession" << endl;
	cout << "|                |          " << " |";
	cout << "\n________________________________________________________________________________________________\n";
	if (Users.empty())
		cout << "n\t\t\tNo Users Availabel In the System!";
	else
	{
		for (User user : Users)
		{
			PrintUser(user);
			cout << endl;
		}
	}
	cout << "\n_________________________________________________________________________________________________\n" << endl;

}
void PrintUserCard(User user)
{
	cout << "\n\t\tUser Details:\n";
	cout << "_________________________________________________________\n";
	cout << "Username     :" << user.UserName << endl;
	cout << "Password     :" << user.Password << endl;
	cout << "Permession   :" << user.Permession << endl;
	cout << "_________________________________________________________\n";
}
void ShowDeleteClientScreen()
{
	if (!CheckAccessPermession(PermessionOption::Del))
	{
		ShowAccessDenied();
		return;
	}
	cout << "\n______________________________________________________________\n";
	cout << "\n\t\tDelete Client Screen";
	cout << "\n_______________________________________________________________\n";
	vector<Client> Clients = LoadClientsDataFromFile(FileName1);
	string AccountNumber = ReadAccountNumber();
	DeleteClientByAccountNumber(AccountNumber, Clients);
}
void ShowDeleteUserScreen()
{
	cout << "\n______________________________________________________________\n";
	cout << "\n\t\tDelete Users Screen" << endl;
	cout << "\n______________________________________________________________\n";
	vector<User> Users = LoadUsersDataFromFile(FileName2);
	string Username = ReadUserName();
	DeleteUserByUsername(Username, Users);
}
void ShowUpdateClientScreen()
{
	if (!CheckAccessPermession(PermessionOption::Upd))
	{
		ShowAccessDenied();
		return;
	}
	cout << "\n______________________________________________________________\n";
	cout << "\n\tUpdate Client Info Screen";
	cout << "\n______________________________________________________________\n";
	vector<Client> Clients = LoadClientsDataFromFile(FileName1);
	string AccountNumber = ReadAccountNumber();
	UpdateClientByAccountNumber(AccountNumber, Clients);
}
void ShowAddNewClientScreen()
{
	if (!CheckAccessPermession(PermessionOption::AddClient))
	{
		ShowAccessDenied();
		return;
	}
	cout << "\n_____________________________________________\n";
	cout << "\n\tAdd New Clients Screen "; 
	cout << "\n_____________________________________________\n";
	AddNewClients();
}
void ShowAddNewUserScreen()
{

	cout << "\n_______________________________________________________________\n";
	cout << "\n\t\tAdd New User Screen " << endl;
	cout << "\n_______________________________________________________________\n";
	AddNewUser();
}
void ShowFindClienScreen()
{
	if (!CheckAccessPermession(PermessionOption::FindClient))
	{
		ShowAccessDenied();
		return;
	}
	cout << "\n_______________________________________________\n";
	cout << "\n\tFind Client Screen";
	cout << "\n________________________________________________\n";
	vector<Client> Clients = LoadClientsDataFromFile(FileName1);
	Client client;
	string AccountNumber = ReadAccountNumber();
	if (FindClientByAccountNumber(AccountNumber, Clients, client))
	{
		PrintClientCard(client);
	}
	else
		cout << "\nClient with Account Number [" << AccountNumber << "] is not found!";
}
void ShowFindUserScreen()
{
	cout << "\n_______________________________________________________\n";
	cout << "\n\t\tFind User Screen" << endl;
	cout << "\n_______________________________________________________\n";
	vector<User> Users = LoadUsersDataFromFile(FileName2);
	User user;
	string Username = ReadUserName();
	if (FindUserByUsername(Username, Users, user))
	{
		PrintUserCard(user);
	}
	else
		cout << "\nUser with Username [" << Username << "] is not found!";
}
void ShowDepositeScreen()
{
	cout << "\n________________________________________________\n";
	cout << "\n\tDeposite Screen";
	cout << "\n________________________________________________\n";
	vector<Client> Clients = LoadClientsDataFromFile(FileName1);
	string AccountNumber= ReadAccountNumber();
	while (!FindClientByAccountNumber(AccountNumber, Clients))
	{
		cout << "\nClient with [" << AccountNumber<<"] does not exist\n";
		AccountNumber = ReadAccountNumber();
	}
	DepositeToAccount(AccountNumber, Clients);
}
void ShowWithdrawScreen()
{
	cout << "\n------------------------------------------------\n";
	cout << "\n\tWithdraw Screen";
	cout << "\n~-----------------------------------------------\n";
	vector<Client> Clients = LoadClientsDataFromFile(FileName1);
	string AccountNumber = ReadAccountNumber();
	while (!FindClientByAccountNumber(AccountNumber, Clients))
	{
		cout << "\nClient with [" << AccountNumber << "] does not exist\n";
		AccountNumber = ReadAccountNumber();
	}
	WithdrawFromAccount(AccountNumber, Clients);
}
void ShowEndScreen()
{
	cout << "\n_________________________________________________\n";
	cout << "\n\tProgram Ends :)";
	cout << "\n_________________________________________________\n";
}
void GoBacktoMainMenue()
{
	cout << "\n\nPress any key to go back to Main Menue...."; 
	system("pause>0");
	system("cls");
	ShowMainMenue();
}
void GoBacktoTransactionMenue()
{
	cout << "\n\nPress any key to go back to transaction Menue ......";
	system("pause>0");
	system("cls");
	ShowTransactionMenue();
}
void GoBacktoManageUsersMenue()
{
	printf("\n\nPress any key to go back to Manage Users Menue ......");
	system("pause>0");
	system("cls");
	ShowManageUsersMenue();
}
string ReadAccountNumber()
{
	string AccountNumber = "";
	cout << "\nPlease enter AccountNumber? ";
	cin >> AccountNumber;
	return AccountNumber;
}
double ReadWithdrawAmount(Client client)
{
	double WithdrawAmount = 0;
	cout << "\nPlease Enter a Withdraw Amount? ";
	cin >> WithdrawAmount;
	while (WithdrawAmount > client.AccountBalance)
	{
		cout << "\nYou Don't have this Ammount!" << endl;
		cout << "\nPlease Entar a Withdraw Amount? ";
		cin >> WithdrawAmount;
	}
	return WithdrawAmount;
}
short ReadMainMenueOptions()
{
	cout << "Please Enter your Choice? ";
	short Choice = 0;
	cin >> Choice;
	while (Choice > 8 || Choice < 1 || cin.fail() )
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Invalid Choice ,Please Enter you Choice: ";
		cin >> Choice;
	}
	return Choice;
}
short ReadTrasactionMeneu()
{
	cout << "Please Choose an option? ";
	short Choice = 0;
	cin >> Choice;
	while (Choice > 3 || Choice < 1 || cin.fail())
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Invalid Choice , Please Choose an option? ";
		cin >> Choice;
	}
	return Choice;
}
short ReadManageUsersMeneu()
{
	short Choice=0;
	cout << "Please Choose an option? ";
	cin >> Choice;
	while (Choice < 1 || Choice>5 || cin.fail())
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Invalid Choice , Please Choose an option? ";
		cin >> Choice;
	}
	return Choice;

}
void OptionMenue(ChoiceMenue choice)
{
	switch (choice)
	{
	case (ChoiceMenue::ListClient):
		system("cls");
		ShowAllClinetsScreen();
		GoBacktoMainMenue();
		break;
	case (ChoiceMenue::Add):
		system("cls");
		ShowAddNewClientScreen();
		GoBacktoMainMenue();
		break;
	case (ChoiceMenue::Delete):
		system("cls");
		ShowDeleteClientScreen();
		GoBacktoMainMenue();
		break;
	case (ChoiceMenue::Update):
		system("cls");
		ShowUpdateClientScreen();
		GoBacktoMainMenue();
		break;
	case (ChoiceMenue::Find):
		system("cls");
		ShowFindClienScreen();
		GoBacktoMainMenue();
		break;
	case (ChoiceMenue::Transaction):
		system("cls");
		ShowTransactionMenue();
		break;
	case (ChoiceMenue::ManageUser):
		system("cls");
		ShowManageUsersMenue();
		break;
	case (ChoiceMenue::Logout):
		system("cls");
		Login();
		break;
	}
}
void OptionTransaction(ChoiceTrans choice)
{
	switch (choice)
	{
	case ChoiceTrans::Deposite:
		system("cls");
		ShowDepositeScreen();
		GoBacktoTransactionMenue();
		break;
	case ChoiceTrans::Withdraw:
		system("cls");
		ShowWithdrawScreen();
		GoBacktoTransactionMenue();
		break;
	case ChoiceTrans::MainMenu:
		system("cls");
		ShowMainMenue();
		break;
	}
}
void OptionManageUsers(ChoiceManageUsers choice)
{
	switch (choice)
	{
	case (ChoiceManageUsers::ShowUsers):
		system("cls");
		ShowAllUsersScreen();
		GoBacktoManageUsersMenue();
		break;
	case (ChoiceManageUsers::FindUser):
		system("cls");
		ShowFindUserScreen();
		GoBacktoManageUsersMenue();
		break;
	case (ChoiceManageUsers::DelteUser):
		system("cls");
		ShowDeleteUserScreen();
		GoBacktoManageUsersMenue();
		break;
	case (ChoiceManageUsers::AddUser):
		system("cls");
		ShowAddNewUserScreen();
		GoBacktoManageUsersMenue();
	case (ChoiceManageUsers::MainMenue):
		system("cls");
		ShowMainMenue();
	}
}
void ShowMainMenue()
{
	cout << "===============================================================\n";
	cout << "\n\t\tMain Menue Screen\n" << endl;
	cout << "===============================================================\n";
	cout << "\t[1] Show Client List\n";
	cout << "\t[2] Add New Client\n";
	cout << "\t[3] Delete Client\n";
	cout << "\t[4] Update Client\n";
	cout << "\t[5] Find Client Info\n";
	cout << "\t[6] Transaction" << endl;
	cout << "\t[7] Manage Users\n";
	cout << "\t[8] logout" << endl;
	cout << "===============================================================\n";
	OptionMenue(ChoiceMenue(ReadMainMenueOptions()));
}
void ShowTransactionMenue()
{
	if (!CheckAccessPermession(PermessionOption::Trans))
	{
		ShowAccessDenied();
		GoBacktoMainMenue();
		return;
	}
	cout << "===============================================================\n";
	cout << "\n\t\tTransaction Menue \n\n";
	cout << "===============================================================\n"; 
	cout << "\t[1] Deposite\n";
	cout << "\t[2] Withdraw\n";
	cout << "\t[3] MainMenue\n";
	cout << "================================================================\n";
	OptionTransaction(ChoiceTrans(ReadTrasactionMeneu()));
}
void ShowManageUsersMenue()
{
	if (!CheckAccessPermession(PermessionOption::ManageUsers))
	{
		ShowAccessDenied();
		GoBacktoMainMenue();
		return;
	}
	cout << "============================================================\n";
	cout << "\n\t\tManage Users Menue\n\n";
	cout << "============================================================\n";
	cout << "\t[1] Show User List.\n";
	cout << "\t[2] Add User.\n";
	cout << "\t[3] Find User.\n"; 
	cout << "\t[4] Delete User.\n";
	cout << "\t[5] Main Menue.\n";
	cout << "============================================================\n";
	OptionManageUsers(ChoiceManageUsers(ReadManageUsersMeneu()));
}
bool LoadUserInfo(string Username, string Password)
{
	if (FindUserByName_Password(Username, Password,CurrentUser))
	{
		return true;
	}
	else
		return false;
}
void Login()
{
	bool LoginFailed = false;
	string Username, Password;
	do {
		system("cls");
		cout << "_______________________________________________________\n";
		cout << "\n\t\tLogin Screen\n";
		cout << "_______________________________________________________\n";
		cout << endl;
		if (LoginFailed)
		{
			cout << "Invalid Username/Password!\n";
		}
		cout << "Enter Username? ";
		cin >> Username;
		cout << "Enter Password? ";
		cin >> Password;
		LoginFailed = !LoadUserInfo(Username, Password);
	} while (LoginFailed);
	system("cls");
	ShowMainMenue();
}
int main()
{
	Login();
	system("pause>0");
	return 0;
}