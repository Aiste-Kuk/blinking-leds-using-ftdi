// bandymas.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//#include "pch.h"
//#include <iostream>
//
//int main()
//{
//    std::cout << "Hello World!\n";
//}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

#include <windows.h>
#include <stdio.h>
#include "ftd2xx.h"

void delay();

int main()
{
    FT_HANDLE ftHandle;
    FT_STATUS ftStatus;

    UCHAR Mask;             // Selects which pins are input/output
    UCHAR Mode;             // Selects the mode of operation for the chip

    ftStatus = FT_Open(0, &ftHandle);
    ftStatus |= FT_SetUSBParameters(ftHandle, 4096, 4096); // Set USB transfer sizes
    ftStatus |= FT_SetChars(ftHandle, false, 0, false, 0); // Disable event characters
    ftStatus |= FT_SetTimeouts(ftHandle, 5000, 5000); // Set read/write timeouts to 5 sec
    ftStatus |= FT_SetLatencyTimer(ftHandle, 16); // Latency timer at default 16ms
    ftStatus |= FT_SetFlowControl(ftHandle, FT_FLOW_NONE, 0x11, 0x13); // No flow control
    ftStatus |= FT_SetBaudRate(ftHandle, 9600); // Baud rate = 9600
    ftStatus |= FT_SetDataCharacteristics(ftHandle, FT_BITS_8, FT_STOP_BITS_1, FT_PARITY_NONE);
    if (ftStatus != FT_OK) printf("ftStatus not ok %d\n", ftStatus); //check for error

    Mode = 0x01;              // Select Chip mode as Asynchronous Bit Bang
    Mask = 0xff;              // Direction Mask,8 bit port all output

    ftStatus = FT_SetBitMode(ftHandle, Mask, Mode); //sets Asynchronous BBM

    if (ftStatus == FT_OK)             // Error Checking
    {
        printf("\nMode = 0x%x, Asynchronous Bit Bang Mode Selected.", Mode);
        printf("\nMask = 0x%x, Direction = All 8 bits output.\n", Mask);
    }
    else
        printf("\nERROR! in setting Asynchronous Bit Bang Mode.\n");

    UCHAR buffer[] = { 0x01 , 0x02 , 0x04 , 0x08 };
    DWORD BytesWritten = 0; // No of bytes written by FT_Write()
    delay();
    UCHAR test = 0x0;
    for (int i = 0; i < 4; i++) {
        test = buffer[i];
        printf("For cycle no. %d. buffer value = %d\n", i+1, buffer[i]);
        ftStatus = FT_Write(ftHandle, &test, sizeof(test), &BytesWritten);
        if (ftStatus == FT_OK)             // Error Checking
        {
            //printf("\n\n\t  0x%x data written",Hexadecimal_Characters[i]);
            printf("\n Status Ok \n ");
        }
        else
            printf("\n\t  ERROR! in Writing to Port");
        delay();
    }

    delay();

    test = 0x0B; 
    while (1) {
        ftStatus = FT_Write(ftHandle, &test, sizeof(test), &BytesWritten);
        delay();
        test = 0x00;
        ftStatus = FT_Write(ftHandle, &test, sizeof(test), &BytesWritten);
        delay();
        test = 0x0B;
    }
    ftStatus = FT_Close(ftHandle);
    printf("Press Return To End Program");
    getchar();
    return 0;
}

void delay()
{
    int i, j;
    for (i = 0; i < 2999999; i++)
        for (j = 0; j < 240; j++);
}