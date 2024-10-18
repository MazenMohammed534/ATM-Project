#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

const string ClientsFileName = "Clients.txt";
sClient CurrentClient;
void ShowATMMainMenue();
void ShowQuickWithDrawScreen();
void Login();

enum enATMMainMenueOptions
{
    QuickWithDraw = 1,
    NormalWithDraw = 2,
    Deposit = 3,
    CheckBalance = 4,
    Logout = 5,
};

vector<string> SplitString(string S1, string Delim)
{
    vector<string> vString;
    short pos = 0;
    string sWord;
    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos);
        if (sWord != "")
        {
            vString.push_back(sWord);
        }
        S1.erase(0, pos + Delim.length());
    }
    if (S1 != "")
    {
        vString.push_back(S1);
    }
    return vString;
}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{
    sClient Client;
    vector<string> vClientData;
    vClientData = SplitString(Line, Seperator);
    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);
    return Client;
}

vector<sClient> LoadClientsDataFromFile(string FileName)
{
    vector<sClient> vClientData;
    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        while (getline(MyFile, Line))
        {
            vClientData.push_back(ConvertLinetoRecord(Line));
        }
        MyFile.close();
    }
    return vClientData;
}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{
    string stClientRecord = "";
    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);
    return stClientRecord;
}

vector<sClient> SaveClientsDataToFile(string FileName, vector<sClient> vClients)
{

    fstream MyFile;
    MyFile.open(FileName, ios::out);

    string DataLine;

    if (MyFile.is_open())
    {

        for (sClient C : vClients)
        {

            if (C.MarkForDelete == false)
            {
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }
        }

        MyFile.close();
    }

    return vClients;
}

void GoBackToATMMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    ShowATMMainMenue();
}

bool FindCientByAccountNumberAndPinCode(string AccountNumber, string PinCode, sClient &Client)
{

    vector<sClient> vClient = LoadClientsDataFromFile(ClientsFileName);

    for (sClient C : vClient)
    {

        if (C.AccountNumber == AccountNumber && C.PinCode == PinCode)
        {
            Client = C;
            return true;
        }
    }
    return false;
}

short ReadQuickWithDrawOption()
{
    short QuickDOp = 0;
    cout << "Choose what do you want to do [1 to 9]...";
    cin >> QuickDOp;
    return QuickDOp;
}

short QuickDrawAmount(short QuickWithDrawOption)
{
    switch (QuickWithDrawOption)
    {
    case 1:
        return 20;
    case 2:
        return 50;
    case 3:
        return 100;
    case 4:
        return 200;
    case 5:
        return 400;
    case 6:
        return 600;
    case 7:
        return 800;
    case 8:
        return 1000;
    default:
        return 9;
    }
}

bool QuickWithDrawBalance(string AccountNumber, short Amount, vector<sClient> &vClients)
{
    char Answer = 'n';

    cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {

        for (sClient &C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount * -1;
                SaveClientsDataToFile(ClientsFileName, vClients);
                cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;

                return true;
            }
        }
        return false;
    }
}

bool NormalWithDrawBalance(string AccountNumber, short Amount, vector<sClient> &vClients)
{
    char Answer = 'n';

    cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {

        for (sClient &C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount * -1;
                SaveClientsDataToFile(ClientsFileName, vClients);
                cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;

                return true;
            }
        }
        return false;
    }
}

bool DepositeBalance(string AccountNumber, short Amount, vector<sClient> &vClients)
{
    char Answer = 'n';

    cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {

        for (sClient &C : vClients)
        {
            if (C.AccountNumber == AccountNumber)
            {
                C.AccountBalance += Amount;
                SaveClientsDataToFile(ClientsFileName, vClients);
                cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;

                return true;
            }
        }
        return false;
    }
}

void ShowBalance()
{
    system("cls");
    vector<sClient> vClient = LoadClientsDataFromFile(ClientsFileName);
    cout << "===========================================\n";
    cout << "\t\tCheck Balance \n";
    cout << "===========================================\n";
    for (sClient C : vClient)
    {

        if (C.AccountNumber == CurrentClient.AccountNumber && C.PinCode == CurrentClient.PinCode)
        {
            cout << "Your Balance is : " << C.AccountBalance << endl;
        }
    }
}

void ShowDepositeScreen()
{
    cout << "===========================================\n";
    cout << "\t\tDeposite Screen \n";
    cout << "===========================================\n";
    short Amount;
    cout << "\nEnter a positive Deposit Amount? ";
    cin >> Amount;
    while (Amount <= 0)
    {
        cout <<"Invalid Value!\n";
        cout << "Please, Enter a positive Deposit Amount? ";
        cin >> Amount;
    }
    vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    DepositeBalance(CurrentClient.AccountNumber, Amount, vClients);
}

void PerfromQuickWithDrawOption(short QuickWithDrawOption)
{
    short Amount = QuickDrawAmount(QuickWithDrawOption);
    if (Amount == 9)
    {
        ShowATMMainMenue();
    }
    if (Amount > CurrentClient.AccountBalance)
    {
        cout << "The amount exceeds your balance, make another choice.\n";
        GoBackToATMMainMenue();
    }
    vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    QuickWithDrawBalance(CurrentClient.AccountNumber, Amount, vClients);
}

void ShowNormalWithDrawScreen()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tNormal WithDraw \n";
    cout << "===========================================\n";
    short Amount;
    cout << "\nEnter an amount for WithDraw? ";
    cin >> Amount;
    while (Amount % 5 != 0)
    {
        cout <<"\nInvalid Value!\n";
        cout << "Please, Enter an amount multiple of 5`s ? ";
        cin >> Amount;
    }
    if (Amount > CurrentClient.AccountBalance)
    {
        cout << "The amount exceeds your balance, make another choice.\n";
        GoBackToATMMainMenue();
    }
    vector<sClient> vClients = LoadClientsDataFromFile(ClientsFileName);
    NormalWithDrawBalance(CurrentClient.AccountNumber, Amount, vClients);
}

void ShowQuickWithDrawScreen()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tQuick WithDraw \n";
    cout << "===========================================\n";
    cout << "\t[1] 20     [2] 50\n";
    cout << "\t[3] 100    [4] 200\n";
    cout << "\t[5] 400    [6] 600\n";
    cout << "\t[7] 800    [8] 1000\n";
    cout << "\t[9] Exit\n";
    cout << "===========================================\n";
    cout << "Your Balance is : " << CurrentClient.AccountBalance << endl;
    PerfromQuickWithDrawOption(ReadQuickWithDrawOption());
}

void PerfromATMMainMenueOption(enATMMainMenueOptions ATMMainMenueOptions)
{

    switch (ATMMainMenueOptions)
    {
    case enATMMainMenueOptions::QuickWithDraw:
    {
        system("cls");
        ShowQuickWithDrawScreen();
        GoBackToATMMainMenue();
        break;
    }
    case enATMMainMenueOptions::NormalWithDraw:
        system("cls");
        ShowNormalWithDrawScreen();
        GoBackToATMMainMenue();
        break;
    case enATMMainMenueOptions::Deposit:
        system("cls");
        ShowDepositeScreen();
        GoBackToATMMainMenue();
        break;
    case enATMMainMenueOptions::CheckBalance:
        system("cls");
        ShowBalance();
        GoBackToATMMainMenue();
        break;
    case enATMMainMenueOptions::Logout:
        system("cls");
        Login();
        break;
    }
}

short ReadATMMainMenueOption()
{
    short ATMOption = 0;
    cout << "Choose what do you want to do [1 to 5]...";
    cin >> ATMOption;
    return ATMOption;
}

void ShowATMMainMenue()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t    ATM Main Menue \n";
    cout << "\t Client[" << CurrentClient.Name << "]";
    cout << "\n===========================================\n";
    cout << "\t[1] QuickWithDraw.\n";
    cout << "\t[2] NormalWithDraw.\n";
    cout << "\t[3] Deposit.\n";
    cout << "\t[4] CheckBalance.\n";
    cout << "\t[5] Logout.\n";
    cout << "===========================================\n";
    PerfromATMMainMenueOption((enATMMainMenueOptions)ReadATMMainMenueOption());
}

bool LoadClientInfo(string AccountNumber, string PinCode)
{
    if (FindCientByAccountNumberAndPinCode(AccountNumber, PinCode, CurrentClient))
        return true;
    else
        return false;
}

void Login()
{
    bool LoginFaild = false;

    string AccountNumber, PinCode;
    do
    {
        system("cls");
        cout << "===================================\n";
        cout << "\tWelcom to ATM";
        cout << "\n===================================\n";

        if (LoginFaild)
        {
            cout << "Invlaid Account Number/PIN Code!\n";
        }

        cout << "Enter Account Number? ";
        cin >> AccountNumber;

        cout << "Enter PIN Code? ";
        cin >> PinCode;

        LoginFaild = !LoadClientInfo(AccountNumber, PinCode);

        ShowATMMainMenue();
    } while (LoginFaild);
}

int main()
{
    Login();
    system("pause>0");
    return 0;
}