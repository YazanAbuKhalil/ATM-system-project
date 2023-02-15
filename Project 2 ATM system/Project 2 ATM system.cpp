#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <iomanip>
#include <windows.h>


using namespace std;

struct stClient {
    string AccountNumber;
    string PinCode;
    string Name;
    string PhoneNumber;
    double AccountBalance = 0;
    bool IsBlocked = false;
};

enum enMainMenueOption {
    eQuickWithdraw = 1, eNormalWithdraw = 2, eDeposit = 3, eCheckBalance = 4, eLogout = 5
};

enum enQuickWithdrawAmount {
    e20 = 1, e50 = 2, e100 = 3, e200 = 4, e400 = 5, e600 = 6, e800 = 7, e1000 = 8, eExit = 9
};

stClient CurrentClient;
const string ClientsFileName = "Clients.txt";

void Login();
void ShowATMMainMenueScreen();
void ShowQuickWithdrawScreen();
void ShowNormalWithdrawScreen();


vector <string> SplitString(string S1, string Delim) {

    vector <string> Words;
    short pos = 0;
    string sWord;

    while ((pos = S1.find(Delim)) != std::string::npos) {
        sWord = S1.substr(0, pos);
        if (sWord != "") {
            Words.push_back(sWord);

        }

        S1.erase(0, pos + Delim.length());
    }

    if (S1 != "") {
        Words.push_back(S1);
    }

    return Words;
}

stClient ConvertClientLineToRecord(string LineRecord, string Seperator = "#//#") {

    stClient Client;
    vector <string> vCLientData = SplitString(LineRecord, Seperator);

   
    Client.AccountNumber = vCLientData[0];
    Client.PinCode = vCLientData[1];
    Client.Name = vCLientData[2];
    Client.PhoneNumber = vCLientData[3];
    Client.AccountBalance = stod(vCLientData[4]);

    return Client;

}

string ConvertClientRecordToLine(stClient Client, string Seperator = "#//#") {

    string LineRecord = "";

    LineRecord += Client.AccountNumber + Seperator;
    LineRecord += Client.PinCode + Seperator;
    LineRecord += Client.Name + Seperator;
    LineRecord += Client.PhoneNumber + Seperator;
    LineRecord += to_string(Client.AccountBalance);

    return LineRecord;
}
vector <stClient> LoadClientsDateFromFile(string FileName) {

    vector <stClient> vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open()) {
        string Line;
        stClient Client;

        while (getline(MyFile, Line)) {
            Client = ConvertClientLineToRecord(Line);
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return vClients;

}

bool FindClientByAccountNumberAndPinCode(string AccountNumber, string PinCode, stClient& CurrentClient) {

    vector <stClient> vClients = LoadClientsDateFromFile(ClientsFileName);

    for (stClient C : vClients) {
        if (C.AccountNumber == AccountNumber && C.PinCode == PinCode) {
            CurrentClient = C;
            return true;
        }
    }
    return false;
}

bool LoadClientInfo(string AccountNumber, string PinCode) {

    if (FindClientByAccountNumberAndPinCode(AccountNumber, PinCode, CurrentClient)) {
        return true;
    }
    return false;
}

short ReadMainMenueOption() {
    short Choice = 0;
    cout << "Choose what do you want to do? [1 to 5]?";
    cin >> Choice;

    return Choice;
}

void GoBackToMainMenue() {
    cout << "\npress any key to go back to main menue...";
    system("pause>0");
    ShowATMMainMenueScreen();
}

short ReadQuickWithdrawOption() {

    short Choice = 0;
    while (Choice < 1 || Choice > 9) {
        cout << "Choose what to withdraw from [1] to [9] ? ";
        cin >> Choice;
    }
 

    return Choice;
}

short GetQuickWithdrawAmount(short QuickWithDrawOption) {

    switch (QuickWithDrawOption) {

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
        return 0;
    }
}
void SaveClientDataToFile(string FileName, vector <stClient> vClients) {

    fstream MyFile;
    MyFile.open(FileName, ios::out);

    if (MyFile.is_open()) {

        for (stClient C : vClients) {
            MyFile << ConvertClientRecordToLine(C) << endl;
        }

        MyFile.close();
    }
}
bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <stClient>& vClients) {

    char Answer = 'n';
    cout << "\n\nAre you sure you want perform this transaction? y/n ? ";
    cin >> Answer;

    if (Answer == 'Y' || Answer == 'y') {

        for (stClient& C : vClients) {
            if (C.AccountNumber == AccountNumber) {
                C.AccountBalance += Amount;
                SaveClientDataToFile(ClientsFileName, vClients);
                cout << "\nDone successfully, New balance is: " << C.AccountBalance;
                return true;
            }
        }
        return false;
    }

}

void PerformQuickWithdrawOption(short QuickWithdrawOption) {

    if (QuickWithdrawOption == 9) { // exit
        return;
    }

    short WithdrawAmount = GetQuickWithdrawAmount(QuickWithdrawOption);

    if (WithdrawAmount > CurrentClient.AccountBalance) {
        cout << "\nThe amount exceeds balance, make another choice.\n";
        cout << "Press any keu to continue...";
        system("pause>0");
        ShowQuickWithdrawScreen();
        return;
    }

    vector <stClient> vClients = LoadClientsDateFromFile(ClientsFileName);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithdrawAmount * -1, vClients);
    CurrentClient.AccountBalance -= WithdrawAmount;

}


void ShowQuickWithdrawScreen() {

    char Answer = 'y';
    vector <stClient> vClients = LoadClientsDateFromFile(ClientsFileName);

    cout << "\n=======================================\n";
    cout << "\t\tQuick withdraw";
    cout << "\n=======================================\n";
    cout << "\t[1] 20\t\t[2] 50\n";
    cout << "\t[3] 100\t\t[4] 200\n";
    cout << "\t[5] 400\t\t[6] 600\n";
    cout << "\t[7] 800\t\t[8] 1000\n";
    cout << "\t[9] Exit";
    cout << "\n=======================================\n";

    cout << "Your Balance is " << CurrentClient.AccountBalance << endl;

    PerformQuickWithdrawOption(ReadQuickWithdrawOption());

}

void ShowCheckBalanceScreen() {

    cout << "\n=======================================\n";
    cout << "\t\tCheck Balance Screen";
    cout << "\n=======================================\n";


    cout << "Your Balance is: " << CurrentClient.AccountBalance << endl;

}

int ReadNormatWithdrawAmount() {

    int WithdrawAmount = 0;

    do {
        cout << "\nEnter an amount multiple of 5's ? ";
        cin >> WithdrawAmount;
    } while (WithdrawAmount % 5 != 0);

    return WithdrawAmount;
 
}

void PerdormNormalWithdrawOption() {

    int WithDrawAmount = ReadNormatWithdrawAmount();

    if (WithDrawAmount > CurrentClient.AccountBalance) {
        cout << "\nThe amount exceeds balance, make another choice.\n";
        cout << "Press any keu to continue...";
        system("pause>0");
        ShowNormalWithdrawScreen();
    }

    vector <stClient> vClients = LoadClientsDateFromFile(ClientsFileName);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, WithDrawAmount * -1, vClients);
    CurrentClient.AccountBalance -= WithDrawAmount;

}

void ShowNormalWithdrawScreen() {

    char Answer = 'y';
    vector <stClient> vClients = LoadClientsDateFromFile(ClientsFileName);

    cout << "\n=====================================\n";
    cout << "\t\tNormal withdraw Screen";
    cout << "\n=====================================\n";
    PerdormNormalWithdrawOption();
}

double ReadPositiveDepositAmount() {
    double DepositAMount = 0;

    do {
        cout << "\nEnter a positive Deposit Amount? ";
        cin >> DepositAMount;
    } while (DepositAMount < 0);

    return DepositAMount;
}

void PerformDepositOption() {

    double DepositAmount = ReadPositiveDepositAmount();

    vector <stClient> vClients = LoadClientsDateFromFile(ClientsFileName);
    DepositBalanceToClientByAccountNumber(CurrentClient.AccountNumber, DepositAmount, vClients);
    CurrentClient.AccountBalance += DepositAmount;
}

void ShowDepositScreen() {
    cout << "\n=======================================\n";
    cout << "\t\tDeposit Screen";
    cout << "\n=======================================\n";
    PerformDepositOption();

}

void PerformMainMenueOption(enMainMenueOption MainMenueOption) {

    switch (MainMenueOption) {

    case enMainMenueOption::eQuickWithdraw:
        system("cls");
        ShowQuickWithdrawScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOption::eNormalWithdraw:
        system("cls");
        ShowNormalWithdrawScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOption::eDeposit:
        system("cls");
        ShowDepositScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOption::eCheckBalance:
        system("cls");
        ShowCheckBalanceScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOption::eLogout:
        system("cls");
        Login();
    }
}

void ShowATMMainMenueScreen() {

    system("cls");
    cout << "\n" << "\033[94m" << "Welcome " << "\033[97m" << CurrentClient.Name  << endl << endl;
    cout << "\n==================================\n";
    cout << "\tATM Main Menue Screen";
    cout << "\n==================================\n";

    cout << "\t\033[94m[1]\033[97m Quick Withdraw.\n";
    cout << "\t\033[94m[2]\033[97m Normal Withdraw.\n";
    cout << "\t\033[94m[3]\033[97m Deposit.\n";
    cout << "\t\033[94m[4]\033[97m Check Balance.\n";
    cout << "\t\033[94m[5]\033[97m Logout.";
    cout << "\n==================================\n";
    
    PerformMainMenueOption((enMainMenueOption)ReadMainMenueOption());
}

void Login() {
    
    bool LoginFailed = false;

    string AccountNumber;
    string PinCode;

    do {
        system("cls");

        cout << "\n\t\t\t\t-------------------------------------------\n";
        cout << "\t\t\t\t\t\tLogin Screen";
        cout << "\n\t\t\t\t-------------------------------------------\n";

        if (LoginFailed) {
            cout << "\033[91m" << "\n\t\t\t\tInvalid Account Number/PinCode!\n";
        }

        cout << "\033[97m";
        cout << "\n\t\t\t\tEnter Account Number? ";
        cin >> AccountNumber;

        cout << "\n\t\t\t\tEnter Pin Code? ";
        cin >> PinCode;


        LoginFailed = !LoadClientInfo(AccountNumber, PinCode);
       

    } while (LoginFailed);

    int Seconds = 60;

    while (Seconds != 0) {
        Sleep(5);
        Seconds--;
    }

            



    ShowATMMainMenueScreen();
}

int main()
{
    Login();
}

