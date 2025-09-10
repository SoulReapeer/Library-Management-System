import pickle
import os

# File names
BOOK_FILE = "books.dat"
MEMBER_FILE = "members.dat"
BORROW_FILE = "borrow.dat"

# Data structures
class Book:
    def __init__(self, id, title, author):
        self.id = id
        self.title = title
        self.author = author
        self.is_borrowed = False

class Member:
    def __init__(self, id, name):
        self.id = id
        self.name = name

class Borrow:
    def __init__(self, book_id, member_id, borrow_date):
        self.book_id = book_id
        self.member_id = member_id
        self.borrow_date = borrow_date
        self.return_date = None

# Utility functions
def load_data(filename):
    if os.path.exists(filename):
        with open(filename, "rb") as f:
            return pickle.load(f)
    return []

def save_data(filename, data):
    with open(filename, "wb") as f:
        pickle.dump(data, f)

def generate_id(data):
    if not data:
        return 1
    return max(item.id for item in data) + 1

# Core functionality
def add_book():
    books = load_data(BOOK_FILE)
    id = generate_id(books)
    title = input("Enter book title: ")
    author = input("Enter book author: ")
    book = Book(id, title, author)
    books.append(book)
    save_data(BOOK_FILE, books)
    print(f"Book added successfully with ID: {id}")

def list_books():
    books = load_data(BOOK_FILE)
    print("\n--- Book List ---")
    print(f"{'ID':<5}{'Title':<30}{'Author':<20}{'Status'}")
    for b in books:
        status = "Borrowed" if b.is_borrowed else "Available"
        print(f"{b.id:<5}{b.title:<30}{b.author:<20}{status}")

def add_member():
    members = load_data(MEMBER_FILE)
    id = generate_id(members)
    name = input("Enter member name: ")
    member = Member(id, name)
    members.append(member)
    save_data(MEMBER_FILE, members)
    print(f"Member added successfully with ID: {id}")

def list_members():
    members = load_data(MEMBER_FILE)
    print("\n--- Member List ---")
    print(f"{'ID':<5}Name")
    for m in members:
        print(f"{m.id:<5}{m.name}")

def borrow_book():
    books = load_data(BOOK_FILE)
    members = load_data(MEMBER_FILE)
    borrows = load_data(BORROW_FILE)

    book_id = int(input("Enter Book ID to borrow: "))
    member_id = int(input("Enter Member ID: "))
    date = input("Enter borrow date (YYYY-MM-DD): ")

    book = next((b for b in books if b.id == book_id), None)
    if not book:
        print("Book not found!")
        return
    if book.is_borrowed:
        print("Book is already borrowed!")
        return

    member = next((m for m in members if m.id == member_id), None)
    if not member:
        print("Member not found!")
        return

    book.is_borrowed = True
    borrow = Borrow(book_id, member_id, date)
    borrows.append(borrow)

    save_data(BOOK_FILE, books)
    save_data(BORROW_FILE, borrows)
    print("Book borrowed successfully.")

def return_book():
    books = load_data(BOOK_FILE)
    borrows = load_data(BORROW_FILE)

    book_id = int(input("Enter Book ID to return: "))
    date = input("Enter return date (YYYY-MM-DD): ")

    book = next((b for b in books if b.id == book_id), None)
    if not book:
        print("Book not found!")
        return
    if not book.is_borrowed:
        print("Book was not borrowed!")
        return

    borrow = next((br for br in borrows if br.book_id == book_id and br.return_date is None), None)
    if borrow:
        borrow.return_date = date
        book.is_borrowed = False
        save_data(BOOK_FILE, books)
        save_data(BORROW_FILE, borrows)
        print("Book returned successfully.")
    else:
        print("Borrow record not found!")

def list_borrowed_books():
    books = load_data(BOOK_FILE)
    members = load_data(MEMBER_FILE)
    borrows = load_data(BORROW_FILE)

    print("\n--- Borrowed Books ---")
    print(f"{'BID':<5}{'Title':<30}{'Author':<20}{'MID':<5}{'Member':<20}{'Borrow Date'}")
    for br in borrows:
        if br.return_date is None:
            book = next(b for b in books if b.id == br.book_id)
            member = next(m for m in members if m.id == br.member_id)
            print(f"{book.id:<5}{book.title:<30}{book.author:<20}{member.id:<5}{member.name:<20}{br.borrow_date}")

# Menu
def main():
    while True:
        print("\n=== Library Management System ===")
        print("1. Add Book\n2. List Books\n3. Add Member\n4. List Members")
        print("5. Borrow Book\n6. Return Book\n7. List Borrowed Books\n0. Exit")
        choice = input("Enter choice: ")

        if choice == "1":
            add_book()
        elif choice == "2":
            list_books()
        elif choice == "3":
            add_member()
        elif choice == "4":
            list_members()
        elif choice == "5":
            borrow_book()
        elif choice == "6":
            return_book()
        elif choice == "7":
            list_borrowed_books()
        elif choice == "0":
            print("Exiting...")
            break
        else:
            print("Invalid choice!")

if __name__ == "__main__":
    main()
