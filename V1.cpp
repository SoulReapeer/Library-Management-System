#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

// Book structure
struct Book {
    int id;
    string title;
    string author;
    bool isBorrowed;
};

// Member structure
struct Member {
    int id;
    string name;
};

// Borrow record structure
struct Borrow {
    int bookId;
    int memberId;
    string borrowDate;
    string returnDate;
};

// Function prototypes
void addBook();
void listBooks();
void addMember();
void listMembers();
void borrowBook();
void returnBook();
int generateBookId();
int generateMemberId();

int main() {
    int choice;
    do {
        cout << "\n=== Library Management System ===\n";
        cout << "1. Add Book\n2. List Books\n3. Add Member\n4. List Members\n";
        cout << "5. Borrow Book\n6. Return Book\n0. Exit\nEnter choice: ";
        cin >> choice;
        cin.ignore(); // Clear newline

        switch (choice) {
            case 1: addBook(); break;
            case 2: listBooks(); break;
            case 3: addMember(); break;
            case 4: listMembers(); break;
            case 5: borrowBook(); break;
            case 6: returnBook(); break;
            case 0: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice!\n";
        }
    } while (choice != 0);

    return 0;
}

// Add a new book
void addBook() {
    Book b;
    b.id = generateBookId();
    cout << "Enter book title: ";
    getline(cin, b.title);
    cout << "Enter book author: ";
    getline(cin, b.author);
    b.isBorrowed = false;

    ofstream file("books.dat", ios::binary | ios::app);
    file.write(reinterpret_cast<char*>(&b), sizeof(Book));
    file.close();

    cout << "Book added successfully with ID: " << b.id << endl;
}

// List all books
void listBooks() {
    Book b;
    ifstream file("books.dat", ios::binary);
    cout << "\n--- Book List ---\n";
    cout << left << setw(5) << "ID" << setw(30) << "Title" << setw(20) << "Author" << "Status\n";
    while(file.read(reinterpret_cast<char*>(&b), sizeof(Book))) {
        cout << setw(5) << b.id << setw(30) << b.title << setw(20) << b.author 
             << (b.isBorrowed ? "Borrowed" : "Available") << endl;
    }
    file.close();
}

// Add a new member
void addMember() {
    Member m;
    m.id = generateMemberId();
    cout << "Enter member name: ";
    getline(cin, m.name);

    ofstream file("members.dat", ios::binary | ios::app);
    file.write(reinterpret_cast<char*>(&m), sizeof(Member));
    file.close();

    cout << "Member added successfully with ID: " << m.id << endl;
}

// List all members
void listMembers() {
    Member m;
    ifstream file("members.dat", ios::binary);
    cout << "\n--- Member List ---\n";
    cout << left << setw(5) << "ID" << "Name\n";
    while(file.read(reinterpret_cast<char*>(&m), sizeof(Member))) {
        cout << setw(5) << m.id << m.name << endl;
    }
    file.close();
}

// Borrow a book
void borrowBook() {
    int bookId, memberId;
    string date;
    cout << "Enter Book ID to borrow: ";
    cin >> bookId;
    cout << "Enter Member ID: ";
    cin >> memberId;
    cin.ignore();
    cout << "Enter borrow date (YYYY-MM-DD): ";
    getline(cin, date);

    // Update book status
    fstream bookFile("books.dat", ios::binary | ios::in | ios::out);
    Book b;
    bool found = false;
    while(bookFile.read(reinterpret_cast<char*>(&b), sizeof(Book))) {
        if(b.id == bookId) {
            if(b.isBorrowed) {
                cout << "Book is already borrowed!\n";
                bookFile.close();
                return;
            }
            b.isBorrowed = true;
            bookFile.seekp(-static_cast<int>(sizeof(Book)), ios::cur);
            bookFile.write(reinterpret_cast<char*>(&b), sizeof(Book));
            found = true;
            break;
        }
    }
    bookFile.close();
    if(!found) {
        cout << "Book not found!\n";
        return;
    }

    // Add borrow record
    Borrow br;
    br.bookId = bookId;
    br.memberId = memberId;
    br.borrowDate = date;
    br.returnDate = "";
    ofstream borrowFile("borrow.dat", ios::binary | ios::app);
    borrowFile.write(reinterpret_cast<char*>(&br), sizeof(Borrow));
    borrowFile.close();

    cout << "Book borrowed successfully.\n";
}

// Return a book
void returnBook() {
    int bookId;
    string date;
    cout << "Enter Book ID to return: ";
    cin >> bookId;
    cin.ignore();
    cout << "Enter return date (YYYY-MM-DD): ";
    getline(cin, date);

    // Update book status
    fstream bookFile("books.dat", ios::binary | ios::in | ios::out);
    Book b;
    bool found = false;
    while(bookFile.read(reinterpret_cast<char*>(&b), sizeof(Book))) {
        if(b.id == bookId) {
            if(!b.isBorrowed) {
                cout << "Book was not borrowed!\n";
                bookFile.close();
                return;
            }
            b.isBorrowed = false;
            bookFile.seekp(-static_cast<int>(sizeof(Book)), ios::cur);
            bookFile.write(reinterpret_cast<char*>(&b), sizeof(Book));
            found = true;
            break;
        }
    }
    bookFile.close();
    if(!found) {
        cout << "Book not found!\n";
        return;
    }

    // Update borrow record
    fstream borrowFile("borrow.dat", ios::binary | ios::in | ios::out);
    Borrow br;
    while(borrowFile.read(reinterpret_cast<char*>(&br), sizeof(Borrow))) {
        if(br.bookId == bookId && br.returnDate == "") {
            br.returnDate = date;
            borrowFile.seekp(-static_cast<int>(sizeof(Borrow)), ios::cur);
            borrowFile.write(reinterpret_cast<char*>(&br), sizeof(Borrow));
            break;
        }
    }
    borrowFile.close();

    cout << "Book returned successfully.\n";
}

// Generate unique Book ID
int generateBookId() {
    ifstream file("books.dat", ios::binary);
    Book b;
    int maxId = 0;
    while(file.read(reinterpret_cast<char*>(&b), sizeof(Book))) {
        if(b.id > maxId) maxId = b.id;
    }
    file.close();
    return maxId + 1;
}

// Generate unique Member ID
int generateMemberId() {
    ifstream file("members.dat", ios::binary);
    Member m;
    int maxId = 0;
    while(file.read(reinterpret_cast<char*>(&m), sizeof(Member))) {
        if(m.id > maxId) maxId = m.id;
    }
    file.close();
    return maxId + 1;
}
