//=============================================================
// Name         : main.cpp
// Author       : Albin Engström
// Created      : 2014-10-23
// Modified     : 2014-10-24
// Description  : main for project gomoku_game
// Purpose      : Start the program
//=============================================================
#include "GomokuClient.h"

#include <iostream>

using namespace std;

int main()
{
    //Creates a GomokuClient object
    GomokuClient gomokuclient;

    //Calls it's Run() method
    gomokuclient.Run();


    return 0;
}
