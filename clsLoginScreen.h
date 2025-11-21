#pragma once

#include <iostream>
#include <iomanip>
#include "clsScreen.h"
#include "clsUser.h"
#include "clsMainScreen.h"
#include "Global.h"


class clsLoginScreen :protected clsScreen
{

private:

    static  bool _Login()
    {
        bool LoginFaild = false;
        int NumofTrials = 3;
        string Username, Password;
        do
        {

            if (LoginFaild)
            {
                NumofTrials--;
                cout << "\nInvlaid Username/Password!\n";
                cout << "You have " << NumofTrials << " Trials to login.\n";
                if (NumofTrials == 0)
                {
                    cout << "\nYou are Lock after 3 faild trails\n";
                    return false;
                }

            }

            cout << "Enter Username? ";
            cin >> Username;

            cout << "Enter Password? ";
            cin >> Password;

            CurrentUser = clsUser::Find(Username, Password);

            LoginFaild = CurrentUser.IsEmpty();

        } while (LoginFaild);

        CurrentUser.RegisterLogIn();
        clsMainScreen::ShowMainMenue();

        return true;
    }

public:


    static bool ShowLoginScreen()
    {
        system("cls");
        _DrawScreenHeader("\t  Login Screen");
        return _Login();

    }

};


