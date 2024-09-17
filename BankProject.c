#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include<math.h>
#include <string.h>
#include <time.h>

typedef struct {
    char CustomerId[5];
    char aadharNumber[15];
    char panCard[11];
    char firstName[50];
    char lastName[50];
    char Address[100];
    int age;
    int balance;
    int AccountNo;
} bankAccount;

struct Account {
    char name[20];
    int accountNo;
    int amount;
};

int readAccounts(struct Account accounts[], const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error could not open file\n");
        return -1;
    }

    int count = 0;
    while (fscanf(file, "%s %d %d", accounts[count].name, &accounts[count].accountNo, &accounts[count].amount) != EOF) {
        count++;
    }

    fclose(file);

    return count;
}

int findAccount(struct Account accounts[], int count, int accountNo) {
    for (int i = 0; i < count; i++) {
        if (accounts[i].accountNo == accountNo) {
            return i;
        }
    }
    return -1;
}

void transferAccount(struct Account accounts[], int count, int fromAccount, int toAccount, int transferAmount, const char *filename) {
    int fromIndex = findAccount(accounts, count, fromAccount);
    int toIndex = findAccount(accounts, count, toAccount);

    if (fromIndex == -1) {
        printf("The Sender Account does not exist\n");
        return;
    }
    if (toIndex == -1) {
        printf("The Receiver Account does not exist\n");
        return;
    }
    if (accounts[fromIndex].amount < transferAmount) {
        printf("Error: Insufficient Funds\n");
        return;
    }

    accounts[fromIndex].amount -= transferAmount;
    accounts[toIndex].amount += transferAmount;

    printf("Transfer Successful\n");
    printf("Transferred %d from Account Number %d to Account Number %d\n", transferAmount, fromAccount, toAccount);

    writeAccounts(accounts, count, filename);
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // Consume remaining characters in the buffer
    }
}

int AadharCheck(const char* aadharVar) {
    size_t length = strlen(aadharVar);
    if (length != 14) {
        return 0;
    }
    for (size_t i = 0; i < length; i++) {
        if (i == 4 || i == 9) {
            if (aadharVar[i] != ' ') {
                return 0;
            }
        } else {
            if (!isdigit(aadharVar[i])) {
                return 0;
            }
        }
    }
    return 1;
}

int panCardCheck(const char* panCardVar) {
    if (strlen(panCardVar) != 10) {
        return 0;
    }
    for (int i = 0; i < 5; i++) {
        if (!isalpha(panCardVar[i])) {
            return 0;
        }
    }
    for (int i = 5; i < 9; i++) {
        if (!isdigit(panCardVar[i])) {
            return 0;
        }
    }
    if (!isalpha(panCardVar[9])) {
        return 0;
    }
    return 1;
}

void generateUniqueId(bankAccount *Customer, int counter) {
    sprintf(Customer->CustomerId, "C%03d", counter);
}

void generateBankAccount(bankAccount *Customer) {
    srand(time(NULL));
    int bankNumber = rand() % 100000 + 1;
    Customer->AccountNo = bankNumber;
}

int createAccount(bankAccount *Customer) {
    size_t length;
    int money;

    printf("Enter The Aadhar Number (format: XXXX XXXX XXXX): ");
    fgets(Customer->aadharNumber, sizeof(Customer->aadharNumber), stdin);
    length = strlen(Customer->aadharNumber);
    if (length > 0 && Customer->aadharNumber[length - 1] == '\n') {
        Customer->aadharNumber[length - 1] = '\0';
    }
    if (!AadharCheck(Customer->aadharNumber)) {
        printf("%s is an Invalid Aadhar Number.\n", Customer->aadharNumber);
        return 0;
    }

    clearInputBuffer();

    printf("Enter the Pan Card Number (format: XXXXX1234X): ");
    fgets(Customer->panCard, sizeof(Customer->panCard), stdin);
    length = strlen(Customer->panCard);
    if (length > 0 && Customer->panCard[length - 1] == '\n') {
        Customer->panCard[length - 1] = '\0';
    }
    if (!panCardCheck(Customer->panCard)) {
        printf("%s is an Invalid Pan Card Number.\n", Customer->panCard);
        return 0;
    }

    clearInputBuffer();

    printf("Enter First Name: ");
    fgets(Customer->firstName, sizeof(Customer->firstName), stdin);
    length = strlen(Customer->firstName);
    if (length > 0 && Customer->firstName[length - 1] == '\n') {
        Customer->firstName[length - 1] = '\0';
    }

    printf("Enter Last Name: ");
    fgets(Customer->lastName, sizeof(Customer->lastName), stdin);
    length = strlen(Customer->lastName);
    if (length > 0 && Customer->lastName[length - 1] == '\n') {
        Customer->lastName[length - 1] = '\0';
    }

    printf("Enter Address: ");
    fgets(Customer->Address, sizeof(Customer->Address), stdin);
    length = strlen(Customer->Address);
    if (length > 0 && Customer->Address[length - 1] == '\n') {
        Customer->Address[length - 1] = '\0';
    }

    clearInputBuffer();

    printf("Enter Age: ");
    scanf("%d", &Customer->age);
    if (Customer->age < 17) {
        printf("You are a Minor. Account cannot be created.\n");
        return 0;
    }

    clearInputBuffer();

    printf("Amount You want to Deposit: ");
    scanf("%d", &money);
    Customer->balance = money;

    clearInputBuffer();

    FILE *ptr = fopen("log.txt", "a");
    if (ptr == NULL) {
        printf("Error opening log file.\n");
        return 0;
    }

    generateBankAccount(Customer);
    fprintf(ptr, "\n%s\t%s\t%s\t%s\t%s\t%s\t%d\t%d\n",
            Customer->CustomerId, Customer->aadharNumber, Customer->panCard,
            Customer->firstName, Customer->lastName, Customer->Address,
            Customer->age, Customer->balance);
    fclose(ptr);

    FILE *ptr2 = fopen("Amount.txt", "a");
    if (ptr2 == NULL) {
        printf("Error opening Amount file.\n");
        return 0;
    }
    fprintf(ptr2, "%s\t%d\t%d\t\n", Customer->firstName, Customer->AccountNo, Customer->balance);
    fclose(ptr2);

    return 1;
}

void display(bankAccount *Customer) {
    printf("Customer Name: %s %s\n", Customer->firstName, Customer->lastName);
    printf("Customer Age: %d\n", Customer->age);
    printf("Customer Address: %s\n", Customer->Address);
    printf("Customer Bank Balance: %d\n", Customer->balance);
    printf("Account Created Successfully\n");
}

void writeAccounts(struct Account accounts[], int count, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error could not open file\n");
        return;
    }
    for (int i = 0; i < count; i++) {
        fprintf(file, "%s %d %d\n", accounts[i].name, accounts[i].accountNo, accounts[i].amount);
    }
    fclose(file);
}

double valueOfMoney(double money,double rate,int years){
    double temp = money;

  printf("\nThe Current Value of Money :%.2lf\n",money);
  printf("\nThe Rate of Inflation :%.2lf % \n",rate);

  // double Change_in_value = money * (1-rate/100);
  double changeInValue;

  printf("--------------------------------------\n");

  for(int i = 0;i<years;i++){
    changeInValue = money * (1-rate/100);
    printf("\nThe Change of Value in Money after Year %d %.2lf\n",i+1,changeInValue);
    money = changeInValue;
  }
  printf("--------------------------------------\n");

  printf("\nYour Value of Money Today %.2lf\n",temp);
  printf("\nAfter %d years The value of money %.2lf\n",years,money);

  return money;
}

double costOfMoney(double money,double rate,int years){
  
  printf("\nThe Current Value of Money: %.2lf\n",money);
  printf("\nThe Rate of Inflation %.2lf % \n",rate );
  double temp = money;
  double changeInValue;

  printf("--------------------------------------\n");
  
  for(int i = 0;i<years;i++){
    changeInValue = money *(1+rate/100);
    printf("\nThe Cost after %d year %.2lf\n",i+1,changeInValue);
    money = changeInValue;
  }

  printf("--------------------------------------\n");

  printf("\nWhat costs today %.2lf\n",temp);
  printf("\nWill cost %.2lf after %d years\n",changeInValue,years);

  return changeInValue;

}

double emia(double interestRate, double principal, int months,double monthlyRate) {
    
    double EMI = (principal * monthlyRate * pow(1 + monthlyRate, months)) / 
                 (pow(1 + monthlyRate, months) - 1);
    
    return EMI;
}

double principal(double principal,double interestRate,int year){
    int months = year*12;
    double monthlyRate = interestRate/1200;
    double EMI;

    if(monthlyRate == 0){
        printf("EMI: %.2f\n",principal/months);
        return 0;
    }    
    double amount = 0;

    EMI = emia(interestRate,principal,months,monthlyRate);
    double totalAmountPayable = EMI * months;
    // if(amount>0){}
    double totalInterestPayable = totalAmountPayable - principal;
    for(int i = 0;i<months;i++){
        EMI = emia(interestRate,principal,months-i,monthlyRate);
        principal = principal - EMI;
        amount = amount + EMI;
        printf("--------------------------------------------\n");
        printf("The Total EMI for Month %d is %.2lf\n",i+1,EMI);
        if(principal<0){
            principal = 0;
        }
        printf("The Remaining Principal Amount would be %.2lf\n",principal);
        printf("The Total Amount Paid %.2lf\n",amount);
        printf("---------------------------------------------\n");
    }

    printf("totalAmountPayable %lf\n ",totalAmountPayable);
    printf("totalInterestPayable %lf\n",totalInterestPayable);

    return totalAmountPayable;
}

double FixedDepositInterest(double principal,double rate,int years){

  printf("------------------------------------\n");
  printf("The principal Amount %.2lf\n",principal);
  double maturityAmount = principal * pow(1 + rate/100,years);
  double interestEarned = maturityAmount - principal;
  printf("Total Interest Earned %.2lf\n",interestEarned);
  printf("Total Maturity Amount %.2lf\n",maturityAmount);
  printf("------------------------------------\n");

  // double interestEarned = maturityAmount - principal;

  return maturityAmount;
}

void menu() {
    printf("\nBank Account Menu\n");
    printf("1. Create an Account\n");
    printf("2. Display All Accounts\n");
    printf("3. Display Single Account\n");
    printf("4. Transfer Money\n");
    printf("5. Cost of Money After N Years\n");
    printf("6. Value of Money After N Years\n");
    printf("7. Loan Detailling");
    printf("8. FD Interest\n");
    printf("9. Exit\n");
    printf("Enter your choice: ");
}

int main() {

    double money,rate_of_interest;
    double princpalamount,interestRate;
    double SavingsRate,FDrate;
    int years,compoundsPerYear;


    struct Account accounts[100];
    bankAccount customer;
    int count = readAccounts(accounts, "Amount.txt");
    int account;
    int choice, fromAccount, toAccount, transferAmount;
    const char *filename = "Amount.txt";

    while (1) {
        menu();
        scanf("%d", &choice);
        clearInputBuffer();  // Clear the input buffer

        switch (choice) {
            case 1:
                generateUniqueId(&customer, count + 1);
                if (createAccount(&customer)) {
                    display(&customer);
                    count = readAccounts(accounts, filename);
                }
                break;
            case 2:
                for (int i = 0; i < count; i++) {
                    printf("---------------------------------\n");
                    printf("Account Number: %d\n", accounts[i].accountNo);
                    printf("Account Name: %s\n", accounts[i].name);
                    printf("Amount: %d\n", accounts[i].amount);
                    printf("---------------------------------\n");
                }
                break;
            case 3:
                printf("Enter the bank account No");
                scanf("%d",&account);
                for(int i = 0;i<count;i++){
                    if(accounts[i].accountNo == account){
                        printf("Account Number %d\n",accounts[i].accountNo);
                        printf("Account name %s\n",accounts[i].name);
                        printf("Amount in the Account %d\n",accounts[i].amount);
                        return 0;
                    }
                }
                printf("Account Not Found \n");
                break;

            case 4:
                printf("Enter Sender Account Number: ");
                scanf("%d", &fromAccount);
                printf("Enter Receiver Account Number: ");
                scanf("%d", &toAccount);
                printf("Enter Transfer Amount: ");
                scanf("%d", &transferAmount);
                transferAccount(accounts, count, fromAccount, toAccount, transferAmount, filename);
                break;
            case 5:
                printf("\nEnter the value of money\n");
                scanf("%lf", &money);
                    // int a = money;
                if(money<0){
                    printf("\nEnter the correct value");
                    return 1;
                }
        
                printf("\nEnter the number of years\n");
                scanf("%d", &years);
                if(years<0){
                    printf("\nEnter the correct value");
                    return 1;
                }
                printf("\nEnter the rate of Interest");
                scanf("%lf",&rate_of_interest);
        
                if((rate_of_interest<0)){
                    printf("\nEnter the correct value");
                    return 1;
                }
                costOfMoney(money,rate_of_interest,years);
                break;

            case 6:
                printf("\nEnter the value of money\n");
                scanf("%lf", &money);
                if(money<0){
                    printf("\nEnter the correct value");
                    return 1;
                }
      
                printf("\nEnter the number of years\n");
                scanf("%d", &years);
      
                if(years<0){
                    printf("Enter the correct value");
                    return 1;
                }
      
                printf("\nEnter the rate of Interest");
                scanf("%lf",&rate_of_interest);
                
                if(rate_of_interest<0){
                    printf("\nEnter the correct value");
                    return 1;
                }
                
                valueOfMoney(money,rate_of_interest,years);
                break;
            case 7:
                printf("\nEnter Your Principal Amount\n");
                scanf("%lf",&princpalamount);
                if(princpalamount<0)
                    return 1;
      
                printf("\nEnter The Interest Rate\n");
                scanf("%lf",&interestRate);
                if(interestRate<0)
                    return 1;

                printf("\nEnter The Number of Years\n");
                scanf("%d", &years);
                if(years<1)
                    return 1;                
                
                principal(princpalamount,interestRate,years);
                break; 

            case 8:
                printf("\nEnter the Principal Amount\n");
                scanf("%lf",&princpalamount);
                if(princpalamount<0){
                    printf("\nEnter the Positive Number");
                    return 1;
                }

                printf("\nEnter the annual interest rate(in percentage) :");
                scanf("%lf",&rate_of_interest);

                if(rate_of_interest<0){
                    printf("\nEnter the Positive Number");
                    return 1;
                }

                printf("\nEnter the number of years for FD");
                scanf("%d",&years);
                if(years<0){
                    printf("Enter the positive Number");
                    return 1;
                }
                FixedDepositInterest(princpalamount,rate_of_interest,years);
                break;
            

            case 9:
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");

        }
    }

    return 0;
}
