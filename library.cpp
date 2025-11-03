/*
------------------------------------------------------------
📚 Library Management System (C++)
Author: Krrish Kumar Singh
Date: November 2025
------------------------------------------------------------
Description:
A simple object-oriented Library Management System using C++.
Demonstrates:
- Classes and Inheritance
- Dynamic Memory Allocation
- Friend Functions and Classes
- Virtual Functions and Polymorphism
- Admin Authentication System
------------------------------------------------------------
*/

#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
using namespace std;

// 🔐 Admin credentials
const string admin = "Krrish";
const string pass = "12345";

// ============================================================
// 📘 Base Class: Book
// ============================================================
class Book
{
protected:
    string title;
    string author;
    float price;

public:
    // 📦 Constructor
    Book(string title, string author, float price) {
        this->title = title;
        this->author = author;
        this->price = price;
    }

    // 💣 Virtual Destructor (ensures derived destructor is called)
    virtual ~Book() {
        cout << "Book " << title << " destroyed. ";
    }

    // 📄 Display Book Details
    virtual void display_details() {
        cout << "\nTitle: " << title << ", Author: " << author << ", Price: " << price;
    }

    // 🧩 Friend declarations
    friend void compare_price(Book &b1, Book &b2);
    friend class Library;
};

// ============================================================
// 💻 Derived Class: EBook (inherits from Book)
// ============================================================
class EBook : public Book
{
private:
    float filesize;

public:
    // 📦 Constructor
    EBook(string name, string author, float price, float filesize)
        : Book(name, author, price) {
        this->filesize = filesize;
    }

    // 📄 Overridden Display Function
    void display_details() override {
        Book::display_details();
        cout << ", File Size: " << filesize << " MB.";
    }

    // 💣 Destructor
    ~EBook() {
        cout << "EBook " << title << " destroyed. ";
    }

    friend void compare_price(Book &b1, Book &b2);
    friend class Library;
};

// ============================================================
// 🏛️ Class: Library (manages Books and EBooks)
// ============================================================
class Library
{
    Book **books; // Pointer to array of Book pointers
    int size;     // Maximum capacity
    int count;    // Current number of books

public:
    // 🏗️ Constructor
    Library(int size) {
        count = 0;
        this->size = size;
        books = new Book *[size];
    }

    // ➕ Add Book
    void add_book(string title, string author, float price) {
        books[count++] = new Book(title, author, price);
    }

    // ➕ Add EBook
    void add_Ebook(string title, string author, float price, float filesize) {
        books[count++] = new EBook(title, author, price, filesize);
    }

    // 🔍 Find a Book by title and author
    Book *find_book(string title, string author) const {
        transform(title.begin(), title.end(), title.begin(), ::tolower);
        transform(author.begin(), author.end(), author.begin(), ::tolower);
        string new_title, new_author;
        for (int i = 0; i < count; i++) {
            new_title = books[i]->title;
            new_author = books[i]->author;
            transform(new_title.begin(), new_title.end(), new_title.begin(), ::tolower);
            transform(new_author.begin(), new_author.end(), new_author.begin(), ::tolower);
            if (new_title == title && new_author == author) {
                return books[i];
            }
        }
        return nullptr;
    }

    // 🔁 Update Book details
    void update_book(string title, string author) {
        Book *bptr = find_book(title, author);
        if (bptr == nullptr) {
            cout << "Book not found!\n";
            return;
        }

        int opt;
        cout << "\nWhat do you want to update?\n1. Title\n2. Author\n3. Price\nEnter choice: ";
        cin >> opt;
        cin.ignore();

        if (opt == 1) {
            cout << "Enter new title: ";
            getline(cin, bptr->title);
        } else if (opt == 2) {
            cout << "Enter new author: ";
            getline(cin, bptr->author);
        } else if (opt == 3) {
            cout << "Enter new price: ";
            cin >> bptr->price;
        } else {
            cout << "Invalid choice!\n";
            return;
        }

        cout << "\nBook details updated successfully!\n";
    }

    // ❌ Delete a Book
    void delete_Book(string title, string author) {
        Book *bptr = find_book(title, author);
        if (bptr == nullptr) {
            cout << "Book not found!\n";
            return;
        }

        int index = -1;
        for (int i = 0; i < count; i++) {
            if (books[i] == bptr) {
                index = i;
                break;
            }
        }

        if (index == -1) {
            cout << "Error locating book in array!\n";
            return;
        }

        cout << "Deleting book: ";
        bptr->display_details();
        cout << endl;

        delete books[index];           // Calls the appropriate destructor
        books[index] = books[count-1]; // Replace deleted element with last
        count--;
        books[count] = nullptr;

        cout << "Book deleted successfully.\n";
    }

    // 📋 Show all books
    void show_all() const {
        cout << "\nAll Book Details:- ";
        for (int i = 0; i < count; i++) {
            books[i]->display_details();
        }
        cout << endl;
    }

    // 🧮 Utility functions
    bool is_empty() const { return count == 0; }
    bool is_full() const { return count == size; }

    // 💣 Destructor
    ~Library() {
        cout << "\nDestroying Library...\n";
        for (int i = 0; i < count; i++) {
            delete books[i];
        }
        delete[] books;
        cout << "Library Destroyed." << endl;
    }
};

// ============================================================
// ⚖️ Friend Function: Compare Prices
// ============================================================
void compare_price(Book &b1, Book &b2) {
    if (b1.price > b2.price)
        cout << b1.title << " is more expensive." << endl;
    else if (b1.price < b2.price)
        cout << b2.title << " is more expensive." << endl;
    else
        cout << "Both have same price." << endl;
}

// ============================================================
// 🧭 Menu Function
// ============================================================
void menu() {
    cout << "\n---------- Welcome to Library Management System ----------\n";
    cout << "1. Add Book/EBook\n";
    cout << "2. Display All Books\n";
    cout << "3. Search Book\n";
    cout << "4. Compare Book Prices\n";
    cout << "5. Update Book Details\n";
    cout << "6. Delete Book\n";
    cout << "7. Exit\n";
    cout << "Enter Your Choice: ";
}

// ============================================================
// 🏁 MAIN FUNCTION
// ============================================================
int main() {
    string username, password;

    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    if (username != admin || password != pass) {
        cout << "Access denied!";
        return 0;
    }

    int size;
    cout << "Enter library capacity: ";
    cin >> size;

    {
        Library lib(size); // Scoped block ensures destructor runs before exit

        while (true) {
            menu();
            int choice;
            cin >> choice;
            cin.ignore();

            if (choice == 1) {
                if (lib.is_full()) {
                    cout << "\nLibrary is Full." << endl;
                    continue;
                }

                start:
                cout << "\n1. Book\n2. EBook\nEnter Your Choice: ";
                cin >> choice;

                if (choice == 1) {
                    string title, author;
                    float price;
                    cout << "Enter Title of Book: ";
                    cin.ignore();
                    getline(cin, title);
                    cout << "Enter Author of Book: ";
                    getline(cin, author);
                    cout << "Enter Price of Book: ";
                    cin >> price;
                    if (price < 0) {
                        cout << "Price can't be negative!\n";
                        continue;
                    }
                    lib.add_book(title, author, price);
                } else if (choice == 2) {
                    string title, author;
                    float price, filesize;
                    cout << "Enter Title of EBook: ";
                    cin.ignore();
                    getline(cin, title);
                    cout << "Enter Author of EBook: ";
                    getline(cin, author);
                    cout << "Enter Price of EBook: ";
                    cin >> price;
                    if (price < 0) {
                        cout << "Price can't be negative!\n";
                        continue;
                    }
                    cout << "Enter FileSize of Ebook: ";
                    cin >> filesize;
                    if (filesize < 0) {
                        cout << "File Size can't be negative!\n";
                        continue;
                    }
                    lib.add_Ebook(title, author, price, filesize);
                } else {
                    cout << "Invalid Choice! Try again.\n";
                    goto start;
                }

            } else if (choice == 2) {
                if (lib.is_empty()) {
                    cout << "\nLibrary is empty!\n";
                    continue;
                }
                lib.show_all();

            } else if (choice == 3) {
                if (lib.is_empty()) {
                    cout << "\nNo Books to display!\n";
                    continue;
                }
                string title, author;
                cout << "Enter Book Name: ";
                cin.ignore();
                getline(cin, title);
                cout << "Enter Book Author: ";
                getline(cin, author);
                Book *bptr = lib.find_book(title, author);
                if (bptr != nullptr)
                    bptr->display_details();
                else
                    cout << "Book " << title << " not found!\n";

            } else if (choice == 4) {
                if (lib.is_empty()) {
                    cout << "\nNo Books to display!\n";
                    continue;
                }
                string title1, title2, author1, author2;
                cout << "Enter First Book Name: ";
                cin.ignore();
                getline(cin, title1);
                cout << "Enter First Book Author: ";
                getline(cin, author1);
                Book *bptr1 = lib.find_book(title1, author1);
                if (bptr1 == nullptr) {
                    cout << "This book is not available in our Library\n";
                    continue;
                }
                cout << "Enter Second Book Name: ";
                cin.ignore();
                getline(cin, title2);
                cout << "Enter Second Book Author: ";
                getline(cin, author2);
                Book *bptr2 = lib.find_book(title2, author2);
                if (bptr2 == nullptr) {
                    cout << "This book is not available in our Library\n";
                    continue;
                }
                compare_price(*bptr1, *bptr2);

            } else if (choice == 5) {
                if (lib.is_empty()) {
                    cout << "\nNo Books to update!\n";
                    continue;
                }
                string title, author;
                cout << "Enter Book Title: ";
                cin.ignore();
                getline(cin, title);
                cout << "Enter Author: ";
                getline(cin, author);
                lib.update_book(title, author);

            } else if (choice == 6) {
                if (lib.is_empty()) {
                    cout << "\nNo Books to delete!\n";
                    continue;
                }
                string title, author;
                cout << "Enter Book Title: ";
                cin.ignore();
                getline(cin, title);
                cout << "Enter Book Author: ";
                getline(cin, author);
                lib.delete_Book(title, author);

            } else if (choice == 7) {
                break;
            } else {
                cout << "Invalid choice!\n";
            }
        }
    }

    cout << "\n----- Exiting Library System... -----\n";
    return 0;
}
