#include <iostream>
#include <limits>
#include "bank_customer.h"
#include "buyer.h"

enum PrimaryPrompt{LOGIN, REGISTER, EXIT, ADMIN_LOGIN};
enum RegisterPrompt{CREATE_BUYER, CREATE_SELLER, BACK};
using namespace std;

#include "seller.h"
#include <vector>
#include <algorithm>

// Data structures to store users
vector<BankCustomer> bankCustomers;
vector<Buyer> buyers;
vector<seller> sellers;

// Function to find a bank customer by ID
BankCustomer* findBankCustomer(int id) {
    auto it = find_if(bankCustomers.begin(), bankCustomers.end(),
        [id](const BankCustomer& customer) { return customer.getId() == id; });
    return it != bankCustomers.end() ? &(*it) : nullptr;
}

// Function to find a buyer by ID
Buyer* findBuyer(int id) {
    auto it = find_if(buyers.begin(), buyers.end(),
        [id](const Buyer& buyer) { return buyer.getId() == id; });
    return it != buyers.end() ? &(*it) : nullptr;
}

// Function to find a seller by ID
seller* findSeller(int id) {
    auto it = find_if(sellers.begin(), sellers.end(),
        [id](const seller& s) { return s.getId() == id; });
    return it != sellers.end() ? &(*it) : nullptr;
}

// Function to find the lowest available ID
int findLowestAvailableId(const vector<int>& usedIds) {
    if (usedIds.empty()) return 1;
    
    // Sort the IDs
    vector<int> sortedIds = usedIds;
    sort(sortedIds.begin(), sortedIds.end());
    
    // Find first gap in sequence or return next number after last
    int expectedId = 1;
    for (int id : sortedIds) {
        if (id > expectedId) {
            return expectedId;
        }
        expectedId = id + 1;
    }
    return expectedId;
}

// Function to generate new unique ID for bank customers
int generateUniqueBankId() {
    vector<int> usedIds;
    for (const auto& customer : bankCustomers) {
        usedIds.push_back(customer.getId());
    }
    return findLowestAvailableId(usedIds);
}

// Function to generate new unique ID for buyers/sellers
int generateUniqueBuyerId() {
    vector<int> usedIds;
    for (const auto& buyer : buyers) {
        usedIds.push_back(buyer.getId());
    }
    return findLowestAvailableId(usedIds);
}

// Function to display all buyers
void displayAllBuyers() {
    cout << "\n=== All Buyers ===\n";
    if (buyers.empty()) {
        cout << "No buyers registered.\n";
        return;
    }
    for (const auto& buyer : buyers) {
        cout << "ID: " << buyer.getId() << ", Name: " << buyer.getName() << endl;
    }
}

// Function to display all sellers
void displayAllSellers() {
    cout << "\n=== All Sellers ===\n";
    if (sellers.empty()) {
        cout << "No sellers registered.\n";
        return;
    }
    for (const auto& seller : sellers) {
        cout << "ID: " << seller.getId() << ", Name: " << seller.getName() << endl;
    }
}

// Search functions
void searchUsersByName(const string& name) {
    cout << "\n=== Search Results for Name: " << name << " ===\n";
    bool found = false;
    
    for (const auto& buyer : buyers) {
        if (buyer.getName().find(name) != string::npos) {
            cout << "Buyer - ID: " << buyer.getId() 
                 << ", Name: " << buyer.getName()
                 << ", Address: " << buyer.getAddress()
                 << ", Phone: " << buyer.getPhoneNumber() << endl;
            found = true;
        }
    }
    
    for (const auto& s : sellers) {
        if (s.getName().find(name) != string::npos) {
            cout << "Seller - ID: " << s.getId() 
                 << ", Name: " << s.getName()
                 << ", Address: " << s.getAddress()
                 << ", Phone: " << s.getPhoneNumber() << endl;
            found = true;
        }
    }
    
    if (!found) {
        cout << "No users found with this name.\n";
    }
}

void searchUsersByAddress(const string& address) {
    cout << "\n=== Search Results for Address: " << address << " ===\n";
    bool found = false;
    
    for (const auto& buyer : buyers) {
        if (buyer.getAddress().find(address) != string::npos) {
            cout << "Buyer - ID: " << buyer.getId() 
                 << ", Name: " << buyer.getName()
                 << ", Address: " << buyer.getAddress() << endl;
            found = true;
        }
    }
    
    for (const auto& s : sellers) {
        if (s.getAddress().find(address) != string::npos) {
            cout << "Seller - ID: " << s.getId() 
                 << ", Name: " << s.getName()
                 << ", Address: " << s.getAddress() << endl;
            found = true;
        }
    }
    
    if (!found) {
        cout << "No users found with this address.\n";
    }
}

// Function to create a new bank account
BankCustomer createBankAccount() {
    string name;
    double initialBalance;
    
    cout << "Enter account holder name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter initial balance: ";
    cin >> initialBalance;
    
    int newId = generateUniqueBankId();
    bankCustomers.emplace_back(newId, name, initialBalance);
    return bankCustomers.back();
}

// Function to create a new buyer account
void createBuyerAccount() {
    string name, address, phone;
    
    cout << "Enter buyer name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter address: ";
    getline(cin, address);
    cout << "Enter phone number: ";
    getline(cin, phone);
    
    // Create associated bank account
    cout << "\nCreating associated bank account:\n";
    cout << "Enter initial balance: ";
    double initialBalance;
    cin >> initialBalance;
    cin.ignore(); // Clear the newline from the input buffer
    
    bankCustomers.emplace_back(generateUniqueBankId(), name, initialBalance);
    
    int newId = generateUniqueBuyerId();
    buyers.emplace_back(newId, name, address, phone, &bankCustomers.back());
    
    cout << "\nBuyer account created successfully!\n";
}

// Function to create a new seller account
// Function to remove a user account and related information
bool removeUser(int userId) {
    // First check if the user exists as a seller
    auto sellerIt = find_if(sellers.begin(), sellers.end(),
        [userId](const seller& s) { return s.getId() == userId; });
    
    if (sellerIt != sellers.end()) {
        // Remove from sellers first
        sellers.erase(sellerIt);
    }
    
    // Then find and remove the buyer account
    auto buyerIt = find_if(buyers.begin(), buyers.end(),
        [userId](const Buyer& b) { return b.getId() == userId; });
    
    if (buyerIt != buyers.end()) {
        // Get the bank account ID before removing the buyer
        int bankId = buyerIt->getAccount()->getId();
        
        // Remove from buyers
        buyers.erase(buyerIt);
        
        // Remove associated bank account
        auto bankIt = find_if(bankCustomers.begin(), bankCustomers.end(),
            [bankId](const BankCustomer& b) { return b.getId() == bankId; });
        
        if (bankIt != bankCustomers.end()) {
            bankCustomers.erase(bankIt);
        }
        
        return true;
    }
    
    return false;
}

void upgradeBuyerToSeller(int buyerId) {
    // Find the buyer
    auto buyerIt = find_if(buyers.begin(), buyers.end(),
        [buyerId](const Buyer& b) { return b.getId() == buyerId; });
        
    if (buyerIt == buyers.end()) {
        cout << "Buyer not found!" << endl;
        return;
    }
    
    // Check if buyer is already a seller
    auto existingSeller = find_if(sellers.begin(), sellers.end(),
        [buyerId](const seller& s) { return s.getId() == buyerId; });
        
    if (existingSeller != sellers.end()) {
        cout << "This buyer is already a seller!" << endl;
        return;
    }
    
    // Check if buyer has a bank account
    if (buyerIt->getAccount() == nullptr) {
        cout << "Buyer must have a bank account to become a seller!" << endl;
        return;
    }
    
    // Get seller business name
    string sellerName;
    cout << "\nEnter seller business name: ";
    cin.ignore();
    getline(cin, sellerName);
    
    // Create seller account from buyer
    sellers.push_back(seller(*buyerIt, buyerIt->getId(), sellerName));
    cout << "\nBuyer successfully upgraded to seller!\n";
}

void searchUsersByPhone(const string& phone) {
    cout << "\n=== Search Results for Phone: " << phone << " ===\n";
    bool found = false;
    
    for (const auto& buyer : buyers) {
        if (buyer.getPhoneNumber().find(phone) != string::npos) {
            cout << "Buyer - ID: " << buyer.getId() 
                 << ", Name: " << buyer.getName()
                 << ", Phone: " << buyer.getPhoneNumber() << endl;
            found = true;
        }
    }
    
    for (const auto& s : sellers) {
        if (s.getPhoneNumber().find(phone) != string::npos) {
            cout << "Seller - ID: " << s.getId() 
                 << ", Name: " << s.getName()
                 << ", Phone: " << s.getPhoneNumber() << endl;
            found = true;
        }
    }
    
    if (!found) {
        cout << "No users found with this phone number.\n";
    }
}

// Function to display detailed user information
void displayUserDetails(int userId) {
    Buyer* buyer = findBuyer(userId);
    if (buyer) {
        cout << "\n=== Buyer Details ===\n";
        cout << "ID: " << buyer->getId() << endl;
        cout << "Name: " << buyer->getName() << endl;
        BankCustomer* bankAcc = buyer->getAccount();
        cout << "Bank Account Details:\n";
        cout << "  Account ID: " << bankAcc->getId() << endl;
        cout << "  Balance: $" << bankAcc->getBalance() << endl;
    }

    seller* sellerAcc = findSeller(userId);
    if (sellerAcc) {
        cout << "\n=== Seller Details ===\n";
        cout << "ID: " << sellerAcc->getId() << endl;
        cout << "Name: " << sellerAcc->getName() << endl;
        // Additional seller-specific information can be added here
    }

    if (!buyer && !sellerAcc) {
        cout << "User not found.\n";
    }
}

int main() {
    //create a loop prompt 
    PrimaryPrompt prompt = LOGIN;
    RegisterPrompt regPrompt = CREATE_BUYER;
    const string ADMIN_USERNAME = "root";
    const string ADMIN_PASSWORD = "toor";
    string username, password;

    while (prompt != EXIT) {
        cout << "Select an option: " << endl;
        cout << "1. Login" << endl;
        cout << "2. Register" << endl;
        cout << "3. Exit" << endl;
        cout << "4. Admin Login" << endl;
        cout << "> ";
        
        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter a number between 1 and 4." << endl;
            continue;
        }
        
        if (choice < 1 || choice > 4) {
            cout << "Please enter a number between 1 and 4." << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        
        prompt = static_cast<PrimaryPrompt>(choice - 1);
        switch (prompt) {
            case LOGIN:
                cout << "Login selected." << endl;
                /* if Login is selected, based on authority then provide options:
                assume user is logged in as Buyer for now
                1. Chek Account Status (will display if user is Buyer or Seller or both and linked banking account status)
                Will display Buyer, Seller and Banking Account details
                2. Upgrade Account to Seller
                Will prompt user to enter Seller details and create a Seller account linked to Buyer account
                Will reject if a user dont have a banking account linked
                3. Create Banking Account (if not already linked), will be replaced with banking functions
                Must provides: initial deposit amount, Address, Phone number, Email
                Banking functions will provides: Balance checking, Transaction History, Deposit, Withdraw
                4. Browse Store Functionality
                Will display all stores initially
                Need to select a store to browse each store inventory
                Will display all items in the store inventory
                After selecting an item, will display item details and option to add to cart
                After adding to cart, will notify user item is added to cart
                5. Order Functionality
                Will display all items in cart
                Will provide option to remove item from cart
                Will provide option to checkout
                After checkout invoide will be generated (will go to payment functionality)
                6. Payment Functionality
                Will display all listed invoices
                Pick an invoice to pay
                Will display invoice details and total amount
                Will provide option to pay invoice
                Payment is done through confirmation dialogue
                In confirmation dialogue, will display account balance as precursor
                User will need to manually enter invoice id to pay
                After paying balance will be redacted from buyer and added to the responding seller account
                After payment, order status will be changed to paid
                7. Logout (return to main menu)
                Display confirmation dialogue
                If confirmed, return to main menu
                If not, return to Buyer menu
                8. Delete Account (remove both Buyer and Seller account and relevant banking account)
                Display confirmation dialogue
                If confirmed, delete account and return to main menu
                If not, return to Buyer menu

                assume user is logged in as Seller for now
                9. Check Inventory
                10. Add Item to Inventory
                11. Remove Item from Inventory
                12. View Orders (will display all orders placed to this seller
                Only orders with paid status will be listed
                Order details will listing items, quantity, total amount, buyer details, order status (paid, cancelled, completed)
                extra functions
                9. Exit to main Menu
                10. Exit Program
                **/
                break;
            case REGISTER:
                regPrompt = CREATE_BUYER; // reset regPrompt to CREATE_BUYER when entering register menu
                while (regPrompt != BACK){
                    cout << "Register selected. " << endl;
                    cout << "Select an option: " << endl;
                    cout << "1. Create Buyer Account" << endl;
                    cout << "2. Create Seller Account" << endl;
                    cout << "3. Back" << endl;
                    cout << "> ";
                    
                    int regChoice;
                    if (!(cin >> regChoice)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Please enter a number between 1 and 3." << endl;
                        continue;
                    }
                    
                    if (regChoice < 1 || regChoice > 3) {
                        cout << "Please enter a number between 1 and 3." << endl;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        continue;
                    }
                    
                    regPrompt = static_cast<RegisterPrompt>(regChoice - 1);
                    switch (regPrompt) {
                        case CREATE_BUYER:
                            cout << "Create Buyer Account selected." << endl;
                            break;
                        case CREATE_SELLER:
                            cout << "Create Seller Account selected." << endl;
                            break;
                        case BACK:
                            cout << "Back selected." << endl;
                            break;
                        default:
                            cout << "Invalid option." << endl;
                            break;
                    }
                }
                /* if register is selected then went throuhh registration process:
                1. Create a new Buyer Account
                Must provides: Name, Home Address, Phone number, Email
                2. Option to create a Seller Account (will be linked to Buyer account)
                Must Provides 1: Home Address, Phone number, Email
                Must provides 2: Store Name, Store Address, Store Phone number, Store Email
                Must provides 3: initial deposit amount
                After finished immediately logged in as Buyer/Seller
                */
                break;
            case EXIT:
                cout << "Exiting." << endl;
                break;
            case ADMIN_LOGIN:
                cout << "Username: ";
                cin >> username;
                cout << "Password: ";
                cin >> password;

                if (username == ADMIN_USERNAME && password == ADMIN_PASSWORD) {
                    cout << "\nAdmin login successful!\n" << endl;
                    bool adminLoggedIn = true;
                    while (adminLoggedIn) {
                        cout << "Admin Menu:\n";
                        cout << "1. View All Buyers and Sellers\n";
                        cout << "2. View Detailed User Information\n";
                        cout << "3. Search Users\n";
                        cout << "4. Create New Account\n";
                        cout << "5. Remove User\n";
                        cout << "6. Upgrade Buyer to Seller\n";
                        cout << "7. Logout\n";
                        cout << "Enter your choice: ";
                        
                        int adminChoice;
                        cin >> adminChoice;
                        
                        switch (adminChoice) {
                            case 1:
                                cout << "Viewing all users...\n";
                                displayAllBuyers();
                                displayAllSellers();
                                break;
                            case 2:
                                cout << "Enter user ID to view details: ";
                                int userIdToView;
                                cin >> userIdToView;
                                displayUserDetails(userIdToView);
                                break;
                            case 3:
                                {
                                    cout << "Search by:\n";
                                    cout << "1. Name\n";
                                    cout << "2. Account ID\n";
                                    cout << "3. Address\n";
                                    cout << "4. Phone Number\n";
                                    cout << "Enter search type: ";
                                    int searchType;
                                    cin >> searchType;
                                    
                                    string searchTerm;
                                    switch(searchType) {
                                        case 1:
                                            cout << "Enter name to search: ";
                                            cin.ignore();
                                            getline(cin, searchTerm);
                                            searchUsersByName(searchTerm);
                                            break;
                                        case 2:
                                            cout << "Enter ID to search: ";
                                            int searchId;
                                            cin >> searchId;
                                            displayUserDetails(searchId);
                                            break;
                                        case 3:
                                            cout << "Enter address to search: ";
                                            cin.ignore();
                                            getline(cin, searchTerm);
                                            searchUsersByAddress(searchTerm);
                                            break;
                                        case 4:
                                            cout << "Enter phone number to search: ";
                                            cin.ignore();
                                            getline(cin, searchTerm);
                                            searchUsersByPhone(searchTerm);
                                            break;
                                        default:
                                            cout << "Invalid search type.\n";
                                    }
                                }
                                break;
                            case 4:
                                {
                                    cout << "Create new account:\n";
                                    cout << "1. Buyer Account\n";
                                    cout << "2. Bank Account\n";
                                    cout << "Enter account type: ";
                                    int accountType;
                                    cin >> accountType;
                                    
                                    switch(accountType) {
                                        case 1:
                                            createBuyerAccount();
                                            break;
                                        case 2:
                                            createBankAccount();
                                            cout << "Bank account created successfully!\n";
                                            break;
                                        default:
                                            cout << "Invalid account type.\n";
                                    }
                                }
                                break;
                            case 5:
                                {
                                    cout << "Enter user ID to remove: ";
                                    int userIdToRemove;
                                    cin >> userIdToRemove;
                                    
                                    cout << "Are you sure you want to remove this user? This will delete all related accounts (y/n): ";
                                    char confirm;
                                    cin >> confirm;
                                    
                                    if (confirm == 'y' || confirm == 'Y') {
                                        if (removeUser(userIdToRemove)) {
                                            cout << "User and all related accounts successfully removed.\n";
                                        } else {
                                            cout << "User not found.\n";
                                        }
                                    } else {
                                        cout << "Operation cancelled.\n";
                                    }
                                }
                                break;
                            case 6:
                                {
                                    cout << "Enter buyer ID to upgrade to seller: ";
                                    int buyerId;
                                    cin >> buyerId;
                                    upgradeBuyerToSeller(buyerId);
                                }
                                break;
                            case 7:
                                cout << "Logging out from admin account.\n";
                                adminLoggedIn = false;
                                break;
                            default:
                                cout << "Invalid option.\n";
                                break;
                        }
                        cout << endl;
                    }
                } else {
                    cout << "Invalid username or password!\n";
                }
                break;
            default:
                cout << "Invalid option." << endl;
                break;
        }
        cout << endl;
    }

    //BankCustomer customer1(1, "Alice", 1000.0);
    //Buyer buyer1(1, customer1.getName(), customer1);
    return 1;
}