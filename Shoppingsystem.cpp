#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>

using namespace std;

class SHOP {
private:
    int code;
    char name[30];
    int quantity;
    double price, tax;

public:
    void getinfo();
    void showinfo();
    void modifyinfo();
    void showallinfo();
    double taxation();
    int retcode();
    const char* retname();
};

void SHOP::getinfo() {
    cout << "Enter product code: ";
    cin >> code;
    cin.ignore(); // Clear the newline character from the buffer
    cout << "Enter product name: ";
    cin.getline(name, 30);
    cout << "Enter quantity of product: ";
    cin >> quantity;
    cout << "Enter price of product: ";
    cin >> price;
    taxation();
}

void SHOP::showinfo() {
    cout << "\nProduct Code: " << code;
    cout << "\nProduct Name: " << name;
    cout << "\nProduct Quantity: " << quantity;
    cout << "\nProduct Price: " << price << endl;
}

void SHOP::modifyinfo() {
    cout << "\nEnter new product name: ";
    cin.ignore(); // Clear the newline character from the buffer
    cin.getline(name, 30);
    cout << "Enter new quantity of product: ";
    cin >> quantity;
    cout << "Enter new price of product: ";
    cin >> price;
    taxation();
}

void SHOP::showallinfo() {
    cout << setw(19) << code << setw(19) << name << setw(24) << quantity << setw(20) << price << endl;
}

double SHOP::taxation() {
    if (price < 1000) {
        tax = price * 0.05;
    } else if (price >= 1000 && price < 10000) {
        tax = price * 0.15;
    } else if (price >= 10000 && price < 50000) {
        tax = price * 0.25;
    } else {
        tax = price * 0.4;
    }
    price = price + tax;
    return price;
}

int SHOP::retcode() {
    return code;
}

const char* SHOP::retname() {
    return name;
}

void welcome() {
    cout << "Welcome to the shop!" << endl;
}

void addProduct() {
    SHOP shop;
    ofstream outFile("SHOP.dat", ios::binary | ios::app);
    shop.getinfo();
    outFile.write(reinterpret_cast<char*>(&shop), sizeof(SHOP));
    outFile.close();
    cout << "\nProduct successfully added!" << endl;
}

void showAllProducts() {
    ifstream inFile("SHOP.dat", ios::binary);
    SHOP shop;
    cout << "\n================================= PRODUCT LIST =================================" << endl;
    cout << setw(19) << "Product Code" << setw(19) << "Product Name" << setw(24) << "Product Quantity" << setw(20) << "Product Price" << endl;
    cout << "================================================================================" << endl;
    while (inFile.read(reinterpret_cast<char*>(&shop), sizeof(SHOP))) {
        shop.showallinfo();
    }
    inFile.close();
}

void searchProduct() {
    ifstream inFile("SHOP.dat", ios::binary);
    SHOP shop;
    int code;
    char name[30];
    cout << "Enter the product code to search: ";
    cin >> code;
    bool found = false;
    while (inFile.read(reinterpret_cast<char*>(&shop), sizeof(SHOP))) {
        if (shop.retcode() == code) {
            shop.showinfo();
            found = true;
            break;
        }
    }
    inFile.close();
    if (!found) {
        cout << "Product not found!" << endl;
    }
}

void modifyProduct() {
    fstream file("SHOP.dat", ios::binary | ios::in | ios::out);
    SHOP shop;
    int code;
    cout << "Enter the product code to modify: ";
    cin >> code;
    bool found = false;
    while (file.read(reinterpret_cast<char*>(&shop), sizeof(SHOP))) {
        if (shop.retcode() == code) {
            shop.showinfo();
            int pos = (-1) * static_cast<int>(sizeof(shop));
            file.seekp(pos, ios::cur);
            shop.modifyinfo();
            file.write(reinterpret_cast<char*>(&shop), sizeof(SHOP));
            cout << "\nProduct modified successfully!" << endl;
            found = true;
            break;
        }
    }
    file.close();
    if (!found) {
        cout << "Product not found!" << endl;
    }
}

void deleteProduct() {
    ifstream inFile("SHOP.dat", ios::binary);
    ofstream outFile("Temp.dat", ios::binary);
    SHOP shop;
    int code;
    cout << "Enter the product code to delete: ";
    cin >> code;
    bool found = false;
    while (inFile.read(reinterpret_cast<char*>(&shop), sizeof(SHOP))) {
        if (shop.retcode() != code) {
            outFile.write(reinterpret_cast<char*>(&shop), sizeof(SHOP));
        } else {
            found = true;
        }
    }
    inFile.close();
    outFile.close();
    remove("SHOP.dat");
    rename("Temp.dat", "SHOP.dat");
    if (found) {
        cout << "Product deleted successfully!" << endl;
    } else {
        cout << "Product not found!" << endl;
    }
}

bool authenticate() {
    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    // Replace this simple authentication logic with your own secure method
    return (username == "Admin" && password == "Admin");
}

void login() {
    cout << "Login required to access the system." << endl;
    bool loggedIn = false;
    int attempts = 3;
    while (attempts > 0) {
        loggedIn = authenticate();
        if (loggedIn) {
            cout << "Login successful!" << endl;
            break;
        } else {
            attempts--;
            cout << "Invalid username or password. Attempts left: " << attempts << endl;
        }
    }
    if (!loggedIn) {
        cout << "Too many unsuccessful attempts. Exiting..." << endl;
        exit(0);
    }
}

void generateBill() {
    ifstream inFile("SHOP.dat", ios::binary);
    SHOP shop;
    int code, quantity;
    double total = 0;
    cout << "Enter the product code to purchase (0 to finish): ";
    while (cin >> code && code != 0) {
        bool found = false;
        while (inFile.read(reinterpret_cast<char*>(&shop), sizeof(SHOP))) {
            if (shop.retcode() == code) {
                shop.showinfo();
                found = true;
                cout << "Enter quantity to purchase: ";
                cin >> quantity;
                total += shop.taxation() * quantity;
                break;
            }
        }
        if (!found) {
            cout << "Product not found!" << endl;
        }
        inFile.clear();
        inFile.seekg(0, ios::beg);
        cout << "Enter the next product code (0 to finish): ";
    }
    inFile.close();
    cout << "\nTotal amount to be paid: $" << fixed << setprecision(2) << total << endl;
}


int main() {
    welcome();
    login();
    char choice;
    do {
        cout << "\nMENU:";
        cout << "\n1. Add a new product";
        cout << "\n2. Show all products";
        cout << "\n3. Search for a product";
        cout << "\n4. Modify a product";
        cout << "\n5. Delete a product";
        cout << "\n6. Generate Bill";
        cout << "\n7.Exit";
        cout << "\nEnter your choice (1-7): ";
        cin >> choice;

        switch (choice) {
            case '1':
                addProduct();
                break;
            case '2':
                showAllProducts();
                break;
            case '3':
                searchProduct();
                break;
            case '4':
                modifyProduct();
                break;
            case '5':
                deleteProduct();
                break;
            case '6':
                generateBill();
                
            case '7':
                cout << "Thank you for using the shop management system!" << endl;
                break;
            default:
                cout << "Invalid choice! Please enter a number between 1 and 6." << endl;
        }
    } while (choice != '7');

    return 0;
}
