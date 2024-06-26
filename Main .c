#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define DISCOUNT_MIN_AGE 18
#define DISCOUNT_MAX_AGE 28
#define DISCOUNT_PERCENTAGE 0.20f
#define MAX_SALES 10
#define MENU_OPTION_BUY_CAR 'a'
#define MENU_OPTION_VIEW_SALES 'b'
#define MENU_OPTION_EXIT 'x'
#define TRUE 1
#define FALSE 0
#define bool unsigned char


#define DATA_FILE "C:\\Users\\Public\\Documents\\carsales.csv"
#define FILE_OPENED 0
#define FILE_CLOSED 1
#define FILE_ERROR 2



unsigned short carsRequested = 0, userAge = 0, carType, giveDiscount = FALSE;
float totalPrice;
char userChoice;

unsigned short numberOfSales = 0;
unsigned short carAmountPerSale[MAX_SALES];
unsigned short typeOfCarPerSale[MAX_SALES];
unsigned short discountGivenPerSale[MAX_SALES];
char customerNames[MAX_SALES][201];

float carPrices[] = { 16000.00f, 12000.00f, 15000.00f };
char carTypes[][10] = { "Audi", "Ford", "BMW" };
unsigned short carsAvailable[] = { 20, 40, 25 };

FILE* file;
unsigned char fileStatus = FILE_CLOSED;


void clearScreen();
char getCharFromConsole(char message[201]);
unsigned short getUnsignedShortFromConsole(char message[201]);
void pauseExec(char userChoice);
void userIntro();
void displayMainMenu();
void displayCarsMenu();
float applyDiscount(float currentPrice);
unsigned short checkDiscount(unsigned short userAge);
void displayDiscountVerdict(unsigned short giveDiscount);
void doBuyCars();
void viewSalesStats();
void exitProgram();

FILE* createFile(char fileName[201]);
void openFile(char fileName[201], char mode[4]);
void closeFile();
void loadDataFile();
void writeDataFile();


void main() {

    loadDataFile();

    do {

        clearScreen();
        userIntro();
        displayMainMenu();

        userChoice = getCharFromConsole("Please enter choice: ");
        clearScreen();

        switch (userChoice) {
        case MENU_OPTION_BUY_CAR:
            displayCarsMenu();
            carType = getUnsignedShortFromConsole("\nWhat Car Model would you like to purchase? Choose one from the options above: ");
            carType = carType - 1;
            carsRequested = getUnsignedShortFromConsole("\nHow many Cars would you like to purchase? Enter number: ");

            doBuyCars();

            break;

        case MENU_OPTION_VIEW_SALES:
            viewSalesStats();
            break;

        case MENU_OPTION_EXIT:
            exitProgram();
            break;
        }

        pauseExec(userChoice);

    } while (userChoice != MENU_OPTION_EXIT);

    clearScreen();

    printf("\n\n Thank you for your purchase, Have a wonderful day...");

}


void clearScreen() {
    system("cls");
}

char getCharFromConsole(char message[201]) {
    char userInput;
    printf(message);
    scanf("\n%c", &userInput);
    return userInput;
}

unsigned short getUnsignedShortFromConsole(char message[201]) {
    unsigned short userInput;
    printf(message);
    scanf("%hd", &userInput);
    return userInput;
}

void pauseExec(char userChoice) {
    if (userChoice == MENU_OPTION_EXIT) {
        printf("\n\nPress Enter to Exit...");
    }
    else {
        printf("\n\nPress Enter to return to the Menu...");
    }
    getchar(); getchar();
}

void userIntro() {
    printf("Welcome to Edward's Car Sales Centre\n\n");
}

void displayMainMenu() {
    printf("Please select an option (a, b, x) from the menu below:\n");
    printf("Menu:\n");
    printf("\ta. Buy Car\n");
    printf("\tb. View Sales Statistics\n");
    printf("\tx. Exit\n\n");
}

void displayCarsMenu() {
    int numberOfCars = sizeof(carPrices) / sizeof(float);
    printf("\nAvailable Car Models:\n");

    printf(" Menu No. | Car Model\t| Cost Price     | Number Available\n");
    for (int i = 0; i < numberOfCars; i++) {
        printf("        %d | %s \t| %f    | %hd\n", (i + 1), carTypes[i], carPrices[i], carsAvailable[i]);
    }
}



float applyDiscount(float currentPrice) {
    return currentPrice * (1 - DISCOUNT_PERCENTAGE);
}

unsigned short checkDiscount(unsigned short userAge) {
    if (userAge >= DISCOUNT_MIN_AGE && userAge <= DISCOUNT_MAX_AGE) {
        return TRUE;
    }
    else {
        char nusCardResponse = getCharFromConsole("\nDo you have a NUS card ? Answer 'y' or 'n' : ");

        scanf("\n%c", &nusCardResponse);

        if (nusCardResponse == 'y') {
            return TRUE;
        }
    }

    return FALSE;
}

void displayDiscountVerdict(unsigned short giveDiscount) {
    switch (giveDiscount) {

    case TRUE:
        printf("\n\nCongratulations! You get a discount.");
        break;

    case FALSE:
        printf("\n\nNo discount given!");
        break;
    }
}

void doBuyCars() {
    if (carsAvailable[carType] < 0) {
        printf("Sorry, the Car Model you requested for is no longer available.");
        printf("\n\nPress Enter to return to the Menu...");
        getchar();
        return;
    }

    if (carsAvailable[carType] < carsRequested) {
        printf("Sorry, we do not have enough of this Car Model.");
        printf("\n\nPress Enter to return to the Menu...");
        getchar();
        return;
    }

    carAmountPerSale[numberOfSales] = carsRequested;

    typeOfCarPerSale[numberOfSales] = carType;

    carsAvailable[carType] = carsAvailable[carType] - carsRequested;

    totalPrice = carsRequested * carPrices[carType];


    printf("What is your name? Name: ");
    scanf("\n%[^\n]s", &customerNames[numberOfSales]);


    userAge = getUnsignedShortFromConsole("How old are you? Age: ");

    giveDiscount = checkDiscount(userAge);
    if (giveDiscount) {
        totalPrice = applyDiscount(totalPrice);
    }
    displayDiscountVerdict(giveDiscount);
    discountGivenPerSale[numberOfSales] = giveDiscount;


    numberOfSales++;

    printf("\n\nThank you for dealing with us.");
    printf("\nYou have purchased %hd Cars.\n", carsRequested);
    printf("Total cost of all purchases is %f GBP.\n", totalPrice);
    printf("\n\nThere are %hd Cars remaining for this Car Model.\n\n", carsAvailable[carType]);

}

void viewSalesStats() {
    int numberOfCars = sizeof(carPrices) / sizeof(float);
    printf("All Sales Data:\n\n");

    float totalSalesValue = 0;
    unsigned int carsSold = 0;


    printf(" S/n | Sale Amount\t| Car Model | Car Price\t| Quant.| "
        "Discount| Customer\n");
    for (int i = 0; i < numberOfSales; i++) {

        int typeOfCar = typeOfCarPerSale[i];

        float price = carAmountPerSale[i] * carPrices[typeOfCar];

        char discountGivenText[4];
        if (discountGivenPerSale[i] == TRUE) {
            strcpy(discountGivenText, "Yes");
            price *= (1 - DISCOUNT_PERCENTAGE);
        }
        else {
            strcpy(discountGivenText, "No");
        }

        printf("  %d  | %f\t| %s    |  %f\t| %hd\t| %s\t| %s\n",
            (i + 1), price, carTypes[typeOfCar],
            carPrices[typeOfCar], carAmountPerSale[i],
            discountGivenText, customerNames[i]);

        totalSalesValue += price;
        carsSold += carAmountPerSale[i];

    }

    printf("\n%hd Cars have been sold with a total value of %f GBP.\n", carsSold, totalSalesValue);

    printf("\nStock Data:\n");

    printf(" No. | Car Model| Number Available\n");
    for (int i = 0; i < numberOfCars; i++) {
        printf("   %d | %s\t| %hd\n", (i + 1), carTypes[i], carsAvailable[i]);
    }

}

void exitProgram() {
    printf("Thank you for using this Car Sales program. Have a lovely day!");
    writeDataFile();
}


FILE* createFile(char fileName[201]) {

    file = fopen(fileName, "w");
    if (file != NULL) {
        fclose(file);
    }
    return file;
}

void openFile(char fileName[201], char mode[4]) {

    file = fopen(fileName, mode);

    if (file == NULL) {

        if (createFile(fileName) == NULL) {
            fileStatus = FILE_ERROR;
        }
        else {
            openFile(fileName, mode);
        }

    }
    else {
        fileStatus = FILE_OPENED;
    }
}

void closeFile() {
    if (fileStatus == FILE_OPENED) {
        fclose(file);
        fileStatus = FILE_CLOSED;
    }
}


//
void loadDataFile() {
    openFile(DATA_FILE, "r");

    if (fileStatus == FILE_OPENED) {

        int lineCounter = 0;

        while (TRUE) {

            unsigned short currCarAmountPerSale = 0, currTypeOfCarPerSale = 0, currDiscountGivenPerSale = 0;
            char currCustomerNames[201] = "";

            int scanResult = fscanf(file, "%hd,%hd,%hd,%[^\n]s",
                &currCarAmountPerSale,
                &currTypeOfCarPerSale,
                &currDiscountGivenPerSale,
                &currCustomerNames
            );

            if (scanResult == EOF) {

                break;
            }

            carAmountPerSale[lineCounter] = currCarAmountPerSale;
            typeOfCarPerSale[lineCounter] = currTypeOfCarPerSale;
            discountGivenPerSale[lineCounter] = currDiscountGivenPerSale;
            strcpy(customerNames[lineCounter], currCustomerNames);

            carsAvailable[currTypeOfCarPerSale] -= currCarAmountPerSale;
            lineCounter++;
        }

        numberOfSales = lineCounter;

    }
    else if (fileStatus == FILE_ERROR) {
        printf("There was an error trying to read from the file %s.", DATA_FILE);
    }

    closeFile();
}


//
void writeDataFile() {

    openFile(DATA_FILE, "w");

    if (fileStatus == FILE_OPENED) {

        for (int i = 0; i < numberOfSales; i++) {

            char line[201];

            char data[50];
            _itoa((int)carAmountPerSale[i], data, 10);
            strcpy(line, data);
            strcat(line, ",");

            _itoa((int)typeOfCarPerSale[i], data, 10);
            strcat(line, data);
            strcat(line, ",");


            _itoa((int)discountGivenPerSale[i], data, 10);
            strcat(line, data);
            strcat(line, ",");

            strcat(line, customerNames[i]);

            fprintf(file, line);

            if (i < numberOfSales - 1) {
                fprintf(file, "\n");
            }
        }
    }
    else if (fileStatus == FILE_ERROR) {
        printf("\nThere was an error trying to write to the file %s.", DATA_FILE);
    }

    closeFile();
}