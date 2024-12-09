
// Super Shop Management

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure Definitions
typedef struct
{
    char productID[10];
    char name[50];
    float price;
    int stock;
} Product;

typedef struct
{
    char customerName[50];
    char productID[10];
    int quantity;
    float totalPrice;
} Bill;

// Function Prototypes
void login();
void exitSystem();
void mainMenu();
void adminMenu();
void employeeMenu();
void inventoryMenu();
void billingMenu();
void addProduct();
void viewProducts();
void updateStock();
void deleteProduct();
void generateBill();
void salesReport();
void lowStockAlert();

// Helper Functions
int authenticateUser(const char *username, const char *password, const char *role);
Product findProductByID(const char *productID);
void updateProductFile(Product updatedProduct);
void displayLowStock();

// File Names
#define PRODUCT_FILE "products.txt"
#define BILL_FILE "bills.txt"

int main()
{
    login();
    return 0;
}

// Function Definitions

// Exit System
void exitSystem()
{
    printf("\nExiting the system... Goodbye!\n");
    exit(0);
}
void clearScreen()
{
    system("cls");
}
// Login System
void login()
{
    char username[50], password[50];
    int role;

    printf("\n=== Welcome to Supershop Management System ===\n");
    printf("Enter Username: ");
    scanf("%s", username);
    printf("Enter Password: ");
    scanf("%s", password);
    clearScreen();
    if (authenticateUser(username, password, "admin"))
    {
        printf("\nLogin successful as Admin!\n");
        adminMenu();
    }
    else if (authenticateUser(username, password, "employee"))
    {
        printf("\nLogin successful as Employee!\n");
        employeeMenu();
    }
    else
    {
        printf("\nInvalid Credentials! Exiting...\n");
        exitSystem();
    }
}

// Main Menus
void adminMenu()
{
    int choice;
    do
    {
        printf("\n=== Admin Menu ===\n");
        printf("1. Inventory Management\n");
        printf("2. View Sales Report\n");
        printf("3. Low Stock Alerts\n");
        printf("4. Logout\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearScreen();
        switch (choice)
        {
        case 1:
            inventoryMenu();
            break;
        case 2:
            salesReport();
            break;
        case 3:
            lowStockAlert();
            break;
        case 4:
            printf("Logging out...\n");
            login();
            return; // Return to login screen
        case 5:
            exitSystem();
            break;
        default:
            printf("Invalid choice! Try again.\n");
        }
    }
    while (1); // Keeps running until the user logs out or exits
}

void employeeMenu()
{
    int choice;
    do
    {
        printf("\n=== Employee Menu ===\n");
        printf("1. Billing\n");
        printf("2. Logout\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearScreen();
        switch (choice)
        {
        case 1:
            billingMenu();
            break;
        case 2:
            printf("Logging out...\n");
            login();
            return; // Return to login screen
        case 3:
            exitSystem();
            break;
        default:
            printf("Invalid choice! Try again.\n");
        }
    }
    while (1); // Keeps running until the user logs out or exits
}

// Inventory Management
void inventoryMenu()
{
    int choice;
    do
    {
        printf("\n=== Inventory Management ===\n");
        printf("1. Add Product\n");
        printf("2. View Products\n");
        printf("3. Update Stock\n");
        printf("4. Delete Product\n");
        printf("5. Back to Admin Menu\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearScreen();
        switch (choice)
        {
        case 1:
            addProduct();
            break;
        case 2:
            viewProducts();
            break;
        case 3:
            updateStock();
            break;
        case 4:
            deleteProduct();
            break;
        case 5:
            printf("Returning to Admin Menu...\n");
            return; // Return to admin menu
        case 6:
            exitSystem();
            break;
        default:
            printf("Invalid choice! Try again.\n");
        }
    }
    while (1); // Keeps running until the user goes back or exits
}

// Implement the rest of your functions as is...


void addProduct()
{
    Product product;
    FILE *file = fopen(PRODUCT_FILE, "a");

    printf("Enter Product ID: ");
    scanf("%s", product.productID);
    printf("Enter Product Name: ");
    scanf("%s", product.name);
    printf("Enter Product Price: ");
    scanf("%f", &product.price);
    printf("Enter Stock Quantity: ");
    scanf("%d", &product.stock);

    fprintf(file, "%s %s %.2f %d\n", product.productID, product.name, product.price, product.stock);
    fclose(file);
    printf("Product added successfully!\n");
}

void viewProducts()
{
    FILE *file = fopen(PRODUCT_FILE, "r");
    Product product;

    printf("\n=== Product List ===\n");
    printf("ID\tName\t\tPrice\tStock\n");
    printf("------------------------------------\n");
    while (fscanf(file, "%s %s %f %d", product.productID, product.name, &product.price, &product.stock) != EOF)
    {
        printf("%s\t%s\t\t%.2f\t%d\n", product.productID, product.name, product.price, product.stock);
    }
    fclose(file);
}

void updateStock()
{
    char id[10];
    int newStock;
    Product product;

    printf("Enter Product ID to Update Stock: ");
    scanf("%s", id);
    product = findProductByID(id);

    if (strlen(product.productID) == 0)
    {
        printf("Product not found!\n");
        return;
    }

    printf("Current Stock: %d\n", product.stock);
    printf("Enter New Stock Quantity: ");
    scanf("%d", &newStock);
    product.stock = newStock;

    updateProductFile(product);
    printf("Stock updated successfully!\n");
}

void deleteProduct()
{
    char id[10];
    FILE *file = fopen(PRODUCT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    Product product;

    printf("Enter Product ID to Delete: ");
    scanf("%s", id);

    while (fscanf(file, "%s %s %f %d", product.productID, product.name, &product.price, &product.stock) != EOF)
    {
        if (strcmp(product.productID, id) != 0)
        {
            fprintf(temp, "%s %s %.2f %d\n", product.productID, product.name, product.price, product.stock);
        }
    }

    fclose(file);
    fclose(temp);
    remove(PRODUCT_FILE);
    rename("temp.txt", PRODUCT_FILE);

    printf("Product deleted successfully!\n");
}

// Billing System
void billingMenu()
{
    generateBill();
}

void generateBill()
{
    FILE *file = fopen(BILL_FILE, "a");
    char productID[10];
    int quantity;
    Product product;
    Bill bill;
    float grandTotal = 0;

    printf("Enter Customer Name: ");
    scanf("%s", bill.customerName);

    printf("\n=== Bill Details ===\n");
    printf("Customer Name: %s\n", bill.customerName);
    printf("-------------------------------------------\n");
    printf("Product ID\tName\t\tPrice\tQty\tTotal\n");
    printf("-------------------------------------------\n");
    clearScreen();
    while (1)
    {
        printf("Enter Product ID (or 'done' to finish): ");
        scanf("%s", productID);
        if (strcmp(productID, "done") == 0) break;

        product = findProductByID(productID);

        if (strlen(product.productID) == 0)
        {
            printf("Product not found!\n");
            continue;
        }

        printf("Enter Quantity: ");
        scanf("%d", &quantity);

        if (quantity > product.stock)
        {
            printf("Insufficient stock!\n");
            continue;
        }

        bill.totalPrice = product.price * quantity;
        grandTotal += bill.totalPrice;

        printf("%s\t\t%s\t%.2f\t%d\t%.2f\n", product.productID, product.name, product.price, quantity, bill.totalPrice);
        fprintf(file, "%s %s %s %.2f\n", bill.customerName, product.productID, product.name, bill.totalPrice);

        product.stock -= quantity;
        updateProductFile(product);
    }

    printf("-------------------------------------------\n");
    printf("Grand Total: %.2f\n", grandTotal);
    printf("-------------------------------------------\n");

    fclose(file);
    printf("\nBill generated successfully!\n");
}


// Reports and Alerts
void salesReport()
{
    FILE *file = fopen(BILL_FILE, "r");
    Bill bill;
    float totalSales = 0;

    if (file == NULL)
    {
        printf("\nNo sales records found.\n");
        return;
    }

    printf("\n=== Sales Report ===\n");
    printf("Customer Name\tProduct ID\tQuantity\tTotal Price\n");
    printf("---------------------------------------------------------\n");
    
    while (fscanf(file, "%s %s %d %f", bill.customerName, bill.productID, &bill.quantity, &bill.totalPrice) != EOF)
    {
        printf("%s\t\t%s\t\t%d\t\t%.2f\n", bill.customerName, bill.productID, bill.quantity, bill.totalPrice);
        totalSales += bill.totalPrice;
    }

    fclose(file);

    printf("---------------------------------------------------------\n");
    printf("Total Sales: %.2f\n", totalSales);
}


void lowStockAlert()
{
    displayLowStock();
}

// Helper Functions
int authenticateUser(const char *username, const char *password, const char *role)
{
    // For simplicity, we hardcode credentials here. Replace with file-based storage for real use.
    if (strcmp(username, "admin") == 0 && strcmp(password, "admin123") == 0 && strcmp(role, "admin") == 0) return 1;
    if (strcmp(username, "employee") == 0 && strcmp(password, "emp123") == 0 && strcmp(role, "employee") == 0) return 1;
    return 0;
}

Product findProductByID(const char *productID)
{
    FILE *file = fopen(PRODUCT_FILE, "r");
    Product product = {"", "", 0.0, 0};

    while (fscanf(file, "%s %s %f %d", product.productID, product.name, &product.price, &product.stock) != EOF)
    {
        if (strcmp(product.productID, productID) == 0)
        {
            fclose(file);
            return product;
        }
    }

    fclose(file);
    return product;
}

void updateProductFile(Product updatedProduct)
{
    FILE *file = fopen(PRODUCT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    Product product;

    while (fscanf(file, "%s %s %f %d", product.productID, product.name, &product.price, &product.stock) != EOF)
    {
        if (strcmp(product.productID, updatedProduct.productID) == 0)
        {
            fprintf(temp, "%s %s %.2f %d\n", updatedProduct.productID, updatedProduct.name, updatedProduct.price, updatedProduct.stock);
        }
        else
        {
            fprintf(temp, "%s %s %.2f %d\n", product.productID, product.name, product.price, product.stock);
        }
    }

    fclose(file);
    fclose(temp);
    remove(PRODUCT_FILE);
    rename("temp.txt", PRODUCT_FILE);
}

void displayLowStock()
{
    FILE *file = fopen(PRODUCT_FILE, "r");
    Product product;

    printf("\n=== Low Stock Products ===\n");
    printf("ID\tName\t\tStock\n");
    printf("---------------------------------\n");
    while (fscanf(file, "%s %s %f %d", product.productID, product.name, &product.price, &product.stock) != EOF)
    {
        if (product.stock < 5)    // Threshold for low stock
        {
            printf("%s\t%s\t\t%d\n", product.productID, product.name, product.stock);
        }
    }
    fclose(file);
}
