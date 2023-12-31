#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <cstring>
#include <vector>
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
using namespace std;
using std::vector;

const char adminPassword[50] = "admin";
const char fileLibrianAccount[50] = "fileaccountLibrian.txt";
const char fileUserAccount[50] = "fileaccountUser.txt";
const char libraryStored[50] = "libraryStored.txt";
char currentUser[500];
char bookBorrowedList[500]="bookBorrowedList.txt";
struct Account
{
    char accountName[50];
    char accountPassword[50];
};
enum TypeSearching
{
    BookName,
    BookType,
    BookLanguage,
    BookAuthor,
    BookPrice,
};
void clearScreen()
{
    system("cls");
}
void DeFaultColor()
{
    cout << GREEN;
}

class Book { //Node
public:
    string Book_name;
    string Book_type;
    string Book_language;
    double price;
    string author;
    Book* prev;
    Book* next;
    bool available;
};
class library { //linked list
public:
    Book* head;
    Book* tail;

    vector<pair<string, string>> borrowHistory;

    library()
    {
        ifstream fileAccount(libraryStored);

        if (!fileAccount.is_open())
        {
            head = NULL;
            tail = NULL;
            return;
        }
        Book loadBook;
        while (fileAccount >> loadBook.Book_type >> loadBook.Book_name >> loadBook.Book_language >> loadBook.author >> loadBook.price>>loadBook.available)
        {
            insert_last(loadBook.Book_name, loadBook.Book_type, loadBook.Book_language, loadBook.price, loadBook.author,loadBook.available);
        }
    }

    bool isempty() {  //check if the linked list is empty or not
        return(head == NULL);
    }
    bool IsFound(string Book_name) { //check if the book is found
        bool flag = false;
        int counter1 = 0;
        Book* book = head; //book=temp
        while (book != NULL) {
            if (book->Book_name == Book_name) {
                counter1++;
                flag = true;
            }
            book = book->next;
        }
        if (counter1 == 0) {
            flag = false;
        }
        return flag;
    }

    void insert_first(string Book_name, string Book_type, string Book_language, double price, string author, bool available) { //insert the book at first position
        transform(author.begin(), author.end(), author.begin(), ::tolower);
        transform(Book_name.begin(), Book_name.end(), Book_name.begin(), ::tolower);
        transform(Book_type.begin(), Book_type.end(), Book_type.begin(), ::tolower);
        transform(Book_language.begin(), Book_language.end(), Book_language.begin(), ::tolower);
        Book* newbook = new Book(); //new node
        newbook->Book_name = Book_name;
        newbook->Book_type = Book_type;
        newbook->Book_language = Book_language;
        newbook->price = price;
        newbook->author = author;
        newbook->available = available == 1 ? true : false;
        if (isempty())
        {
            head = newbook;
            tail = newbook;
            newbook->prev = NULL;
            newbook->next = NULL;
        }
        else
        {
            newbook->next = head;
            newbook->prev = NULL;
            head = newbook;
        }
        StoreBook();
    }

    void insert_last(string Book_name, string Book_type, string Book_language, double price, string author,bool available) //insert a book at the end of the list
    {
        transform(author.begin(), author.end(), author.begin(), ::tolower);
        transform(Book_name.begin(), Book_name.end(), Book_name.begin(), ::tolower);
        transform(Book_type.begin(), Book_type.end(), Book_type.begin(), ::tolower);
        transform(Book_language.begin(), Book_language.end(), Book_language.begin(), ::tolower);
        if (isempty())
            insert_first(Book_name, Book_type, Book_language, price, author,available);
        else
        {
            Book* LastBook = head;  //as temp
            Book* newBook = new Book();
            newBook->Book_name = Book_name;
            newBook->Book_type = Book_type;
            newBook->Book_language = Book_language;
            newBook->price = price;
            newBook->author = author;
            newBook->available =available== 1 ? true : false;
            while (LastBook->next != NULL) {
                LastBook = LastBook->next;
            }
            LastBook->next = newBook;
            newBook->next = NULL;
            newBook->prev = LastBook;
            StoreBook();
        }
    }

    bool insert_After(string Current_book, string Book_name1, string Book_type, string Book_language, double price, string author, bool available) { //inserts the book after the book required
        transform(author.begin(), author.end(), author.begin(), ::tolower);
        transform(Book_name1.begin(), Book_name1.end(), Book_name1.begin(), ::tolower);
        transform(Book_type.begin(), Book_type.end(), Book_type.begin(), ::tolower);
        transform(Current_book.begin(), Current_book.end(), Current_book.begin(), ::tolower);
        transform(Book_language.begin(), Book_language.end(), Book_language.begin(), ::tolower);
        if (isempty())
            insert_first(Book_name1, Book_type, Book_language, price, author,available);
        else  if (IsFound(Current_book)) {
            {
                Book* newBook = new Book();
                newBook->Book_name = Book_name1;
                newBook->Book_type = Book_type;
                newBook->Book_language = Book_language;
                newBook->price = price;
                newBook->author = author;
                newBook->available = available == 1 ? true : false;
                Book* current = head;
                while (current != NULL && current->Book_name != Current_book) {

                    current = current->next;
                }

                newBook->next = current->next;
                newBook->prev = current;
                current->next = newBook;
                StoreBook();
                return true;
            }
        }
        else
        {
            cout << RED << "          Book you're searching for isn't Found" << endl;
            DeFaultColor();
            return false;
        }
    }

    void update_first(string new_Book_name, string new_Book_type, string new_Book_language, double new_price, string new_author) { //update information of the first position book
        transform(new_author.begin(), new_author.end(), new_author.begin(), ::tolower);
        transform(new_Book_name.begin(), new_Book_name.end(), new_Book_name.begin(), ::tolower);
        transform(new_Book_type.begin(), new_Book_type.end(), new_Book_type.begin(), ::tolower);
        transform(new_Book_language.begin(), new_Book_language.end(), new_Book_language.begin(), ::tolower);
        if (isempty()) {
            cout << "          No book found to update" << endl;
        }
        else {
            Book* book = head;
            book->Book_name = new_Book_name;
            book->Book_type = new_Book_type;
            book->Book_language = new_Book_language;
            book->price = new_price;
            book->author = new_author;
            cout << "Update completed" << endl;
            StoreBook();
        }
    }

    void update_last(string new_Book_name, string new_Book_type, string new_Book_language, double new_price, string new_author) { //update information of the last position book
        transform(new_author.begin(), new_author.end(), new_author.begin(), ::tolower);
        transform(new_Book_name.begin(), new_Book_name.end(), new_Book_name.begin(), ::tolower);
        transform(new_Book_type.begin(), new_Book_type.end(), new_Book_type.begin(), ::tolower);
        transform(new_Book_language.begin(), new_Book_language.end(), new_Book_language.begin(), ::tolower);
        if (isempty()) {
            cout << RED << "          No book found to update" << endl;
            DeFaultColor();
        }
        else {
            Book* book = head;
            while (book->next != NULL) {
                book = book->next;
            }
            book->Book_name = new_Book_name;
            book->Book_type = new_Book_type;
            book->Book_language = new_Book_language;
            book->price = new_price;
            book->author = new_author;
            cout << "Update completed" << endl;
            StoreBook();
        }
    }

    void update_at(string replaced_book, string new_Book_name, string new_Book_type, string new_Book_language, double new_price, string new_author) { //update information of  a certain book book
        transform(new_author.begin(), new_author.end(), new_author.begin(), ::tolower);
        transform(new_Book_name.begin(), new_Book_name.end(), new_Book_name.begin(), ::tolower);
        transform(new_Book_type.begin(), new_Book_type.end(), new_Book_type.begin(), ::tolower);
        transform(new_Book_language.begin(), new_Book_language.end(), new_Book_language.begin(), ::tolower);
        if (IsFound(replaced_book))
        {
            Book* book = head; //book=temp
            while (book != NULL && book->Book_name != replaced_book) {
                book = book->next;
            }
            book->Book_name = new_Book_name;
            book->Book_type = new_Book_type;
            book->Book_language = new_Book_language;
            book->price = new_price;
            book->author = new_author;
            cout << "Update completed" << endl;
            StoreBook();
        }
        else
        {
            cout << RED << "          No book found to update with this name" << endl;
            DeFaultColor();

        }

    }
    void StoreBook()
    {
        fstream file;
        file.open(libraryStored, ios::out);
        ofstream fileAccount(libraryStored, ios::app);
        if (!fileAccount.is_open())
        {
            cout << RED << "Error to Store Book" << endl;
            DeFaultColor();
        }
        Book* temp = head;
        while (temp != NULL)
        {
            fileAccount << temp->Book_type << " " << temp->Book_name << " " << temp->Book_language << " " << temp->author << " " << temp->price<<" " << temp->available << endl;
            temp = temp->next;
        }
        fileAccount.flush();
        fileAccount.close();

    }

    void SearchingBook(string FidingName, TypeSearching searchtype)
    {
        transform(FidingName.begin(), FidingName.end(), FidingName.begin(), ::tolower);

        cout << "-------------------------------------------\n"
            "       Searching.........................      \n"
            "-------------------------------------------\n";
        ifstream fileAccount(libraryStored);

        if (!fileAccount.is_open())
        {
            cout << RED << "           Library is empty" << endl << endl;
            DeFaultColor();
            return;
        }
        int count = 0;
        Book bookFind;
        char* bookname = &FidingName[0];
        while (fileAccount >> bookFind.Book_type >> bookFind.Book_name >> bookFind.Book_language >> bookFind.author >> bookFind.price)
        {
            char* bookFindname = &FidingName[0];
            string getPrice;
            switch (searchtype)
            {
            case BookName:
                bookFindname = &bookFind.Book_name[0];
                break;
            case BookType:
                bookFindname = &bookFind.Book_type[0];
                break;
            case BookLanguage:
                bookFindname = &bookFind.Book_language[0];
                break;
            case BookAuthor:
                bookFindname = &bookFind.author[0];
                break;
            case BookPrice:
                getPrice = to_string(bookFind.price);
                bookFindname = &getPrice[0];
                break;
            default:
                break;
            }
            if (strcmp(bookFindname, bookname) == 0)
            {
                cout << "        " << bookFind.Book_name << " Books Available:" << endl;
                cout << "        " << "Book type: " << bookFind.Book_type << endl;
                cout << "        " << "Book language: " << bookFind.Book_language << endl;
                cout << "        " << "Book price: " << bookFind.price << endl;
                cout << "        " << "Book author: " << bookFind.author << endl;
                cout << endl;
                count++;
            }
        }
        if (count == 0)
        {
            cout << RED << "          Book is not found" << endl << endl;
            DeFaultColor();
        }
        fileAccount.close();
    }

    void delete_first() {
        if (isempty()) {
            return;
        }
        Book* book;
        book = head;
        if (head != NULL) {
            head = head->next;
            if (head)
                head->prev = NULL;
            delete book;
            cout << "Delete completed" << endl;
            StoreBook();
        }
    }

    void delete_end() {  //delete the last book
        if (!isempty()) {
            if (head->next == NULL) {
                head = NULL;
            }
            else {
                Book* book = head;
                while (book->next->next != NULL)
                    book = book->next;
                Book* lastbook = book->next;
                book->next = NULL;
                delete lastbook;
                cout << "Delete completed" << endl;
                StoreBook();
            }
        }
    }

    void delete_by_name(string Book_name) {
        if (isempty())
            return;
        if (head->Book_name == Book_name)
            delete_first();
        else
        {
            Book* temp = head->next;
            while (temp != NULL) {
                if (temp->Book_name == Book_name)
                    break;
                temp = temp->next;
            }
            if (temp == NULL)
            {
                cout << RED << "Book not found." << endl;
                DeFaultColor();
            }
            else if (temp->next == NULL)
                delete_end();
            else
            {
                temp->prev->next = temp->next;
                temp->next->prev = temp->prev;
                delete temp;
                cout << "Delete completed" << endl;
                StoreBook();
            }
        }
    }
    void sortListWithPrice() {  //sort all book by price
        Book* current;
        Book* q;
        current = head;
        q = NULL;
        string temp_Book_name;
        string temp_Book_type;
        string temp_Book_language;
        double temp_price;
        string temp_author;
        if (head == NULL) {
            return;
        }
        else {
            while (current != NULL)
            {
                q = current->next;
                while (q != NULL)
                {
                    if (current->price > q->price)
                    {
                        temp_price = current->price;
                        current->price = q->price;
                        q->price = temp_price;

                        temp_Book_name = current->Book_name;
                        current->Book_name = q->Book_name;
                        q->Book_name = temp_Book_name;

                        temp_Book_type = current->Book_type;
                        current->Book_type = q->Book_type;
                        q->Book_type = temp_Book_type;

                        temp_Book_language = current->Book_language;
                        current->Book_language = q->Book_language;
                        q->Book_language = temp_Book_language;

                        temp_author = current->author;
                        current->author = q->author;
                        q->author = temp_author;
                    }
                    q = q->next;
                }
                current = current->next;
            }
        }
    }

    void sortListWithBookName() {  //sort all books by name
        Book* current;
        Book* q;
        current = head;
        q = NULL;
        string temp_Book_name;
        string temp_Book_type;
        string temp_Book_language;
        double temp_price;
        string temp_author;
        if (head == NULL) {
            return;
        }
        else {
            while (current != NULL)
            {
                q = current->next;
                while (q != NULL)
                {
                    if (int(tolower(current->Book_name[0])) > int(tolower(q->Book_name[0])))
                    {
                        temp_price = current->price;
                        current->price = q->price;
                        q->price = temp_price;

                        temp_Book_name = current->Book_name;
                        current->Book_name = q->Book_name;
                        q->Book_name = temp_Book_name;

                        temp_Book_type = current->Book_type;
                        current->Book_type = q->Book_type;
                        q->Book_type = temp_Book_type;

                        temp_Book_language = current->Book_language;
                        current->Book_language = q->Book_language;
                        q->Book_language = temp_Book_language;

                        temp_author = current->author;
                        current->author = q->author;
                        q->author = temp_author;
                    }
                    q = q->next;
                }
                current = current->next;
            }
        }
    }

    void count() { // counts number of books
        int counter = 0;
        Book* nextBook = head;
        while (nextBook != NULL) {
            counter++;
            nextBook = nextBook->next;
        }
        cout << "--------------------------------\n";
        cout << "    NUMBER OF BOOKS IN LIBRARY  " << counter << endl;
        cout << "--------------------------------\n" << endl << endl;
    }

    void traversal() //displays the books in the library
    {
        Book* nextBook = head;
        if (nextBook == NULL) {
            cout << RED << "No Books are available";
            DeFaultColor();
        }
        else
        {
            // Until p is not NULL
            cout << "--------------------------------\n"
                "       Books In Library         \n"
                "--------------------------------\n" << endl << endl;
            while (nextBook != NULL)
            {
                cout << "      Book Name:" << nextBook->Book_name << "       " << endl;
                cout << "       Book Type:" << nextBook->Book_type << "       " << endl;
                cout << "       Book Language:" << nextBook->Book_language << "       " << endl;
                cout << "       Author:" << nextBook->author << "       " << endl;
                cout << "       Price:" << nextBook->price << "       " << endl << endl;
                if (nextBook->available==0)
                {
                    cout << "       Not Available" <<endl << endl;
                }
                else
                {
                    cout << "        Available" << endl << endl;
                }
                nextBook = nextBook->next;
            }
        }
    }

    void updateListBookBorrowed(Book* getBook)
    {
        
        ofstream fileAccount(bookBorrowedList, ios::app);
        if (!fileAccount.is_open())
        {
            fstream file;
            file.open(bookBorrowedList, ios::out);
            ofstream fileAccount(bookBorrowedList, ios::app);
        }
        fileAccount << getBook->Book_name << " " << getBook->Book_type << " " << getBook->Book_language << " " << getBook->author << " " << currentUser << endl;
        fileAccount.flush();
        fileAccount.close();
    }


    void borrowBook( string Book_name)
    {
        if (IsFound(Book_name))
        {
            Book* book = head;
            bool found;
            while (book != NULL)
            {
                if (book->Book_name==Book_name&& book->available!=0)
                {
                    found = true;
                    break;
                }
                else
                {
                    found = false;
                }
                book = book->next;
            }
            if (found)
            {
                string fileUserName = currentUser;
                fileUserName += ".txt";
                ofstream fileAccount(fileUserName, ios::app);
                if (!fileAccount.is_open())
                {
                    fstream file;
                    file.open(fileUserName, ios::out);
                    ofstream fileAccount(fileUserName, ios::app);
                }
                fileAccount << book->Book_name << " " << book->Book_type << " " << book->Book_language << " " << book->author << endl;
                book->available = false;
                StoreBook();
                fileAccount.flush();
                fileAccount.close();
                updateListBookBorrowed(book);
            }
            else
            {
                cout << RED<< " No book available to borrow " << endl;
                DeFaultColor();
            }
        }
        else 
{
            cout << RED << "The book " << Book_name << " is not found." << endl;
            DeFaultColor();
            cin.clear();
        }
    }


    void giveBackBook(bool isLibrarian)
    {
        string Book_name, accountInfor;
        if (isLibrarian)
        {
            cout << "Enter Name of Book to GiveBack: " << endl;
            cin >> Book_name;
            cout << " Enter accountBorrow: " << endl;
            cin >> accountInfor;
            Book checkBook;
            Book getBookUser;
            Book getBookLib;
            string checkAccount;
            ifstream fileAccountUser(accountInfor + ".txt", ios::app);
            if (!fileAccountUser.is_open())
            {
                cout << RED << "No book was borrowed " << endl;
                DeFaultColor();
            }
            else
            {
                while (fileAccountUser >> checkBook.Book_name >> checkBook.Book_type >> checkBook.Book_language >> checkBook.author)
                {
                    if (checkBook.Book_name == Book_name)
                    {
                        getBookUser = checkBook;
                        break;
                    }
                
                }

            }
            fileAccountUser.close();
            DeleteLineofFile(accountInfor + ".txt", getBookUser.Book_name + " " + getBookUser.Book_type + " " + getBookUser.Book_language + " " + getBookUser.author);
            ifstream fileAccountLibrarian(bookBorrowedList, ios::app);
            if (!fileAccountLibrarian.is_open())
            {
                cout << RED << "No book was borrowed " << endl;
                DeFaultColor();
                return;
            }
            while (fileAccountLibrarian >> checkBook.Book_name >> checkBook.Book_type >> checkBook.Book_language >> checkBook.author >> checkAccount)
            {
                if (checkBook.Book_name == Book_name && checkAccount == accountInfor)
                {
                    getBookLib = checkBook;
                    break;
                }
            }
            fileAccountLibrarian.close();
            if (DeleteLineofFile(bookBorrowedList, getBookLib.Book_name + " " + getBookLib.Book_type + " " + getBookLib.Book_language + " " + getBookLib.author + " " + checkAccount))
            {
                Book* book = head;
                while (book->Book_name != Book_name)
                {
                    book = book->next;
                }
                book->available = true;
                StoreBook();
                cout << "          giveBack Successfully" << endl;
            }
            else
            {
                cout << RED << "      No book to give back " << endl;
                DeFaultColor();
            }

        }
        else
        {
            cout << "Enter Name of Book to GiveBack: " << endl;
            cin >> Book_name;
            string fileUser = currentUser;
            fileUser += ".txt";
            Book checkBook;
            Book getBookUser;
            Book getBookLib;
            string checkAccount;
            ifstream fileAccountUser(fileUser, ios::app);
            if (!fileAccountUser.is_open())
            {
                cout << RED << "No book was borrowed " << endl;
                DeFaultColor();
            }
            else
            {
                while (fileAccountUser >> checkBook.Book_name >> checkBook.Book_type >> checkBook.Book_language >> checkBook.author)
                {
                    if (checkBook.Book_name == Book_name)
                    {
                        getBookUser = checkBook;
                        break;
                    }
                    fileAccountUser.close();
                }

            }
            DeleteLineofFile(fileUser, getBookUser.Book_name + " " + getBookUser.Book_type + " " + getBookUser.Book_language + " " + getBookUser.author);
            ifstream fileAccountLibrarian(bookBorrowedList, ios::app);
            if (!fileAccountLibrarian.is_open())
            {
                cout << RED << "No book was borrowed " << endl;
                DeFaultColor();
                return;
            }
            while (fileAccountLibrarian >> checkBook.Book_name >> checkBook.Book_type >> checkBook.Book_language >> checkBook.author >> checkAccount)
            {
                if (checkBook.Book_name == Book_name && checkAccount == currentUser)
                {
                    getBookLib = checkBook;
                    break;
                }
            }
            fileAccountLibrarian.close();
            if (DeleteLineofFile(bookBorrowedList, getBookLib.Book_name + " " + getBookLib.Book_type + " " + getBookLib.Book_language + " " + getBookLib.author + " " + checkAccount))
            {
                Book* book = head;
                while (book->Book_name != Book_name)
                {
                    book = book->next;
                }
                book->available = true;
                StoreBook();
                cout << "          giveBack Successfully" << endl;
            }
            else
            {
                cout << RED << "      No book to give back " << endl;
                DeFaultColor();
            }
        }
    }
    bool DeleteLineofFile(string getfileName, string conentToDelete)
    {
        string lineToDelete = conentToDelete;
        string fileName = getfileName;
        ifstream inputFile(fileName);
        if (!inputFile.is_open())
        {
            return false;
        }
        string templine;
        int flag = 0;
        string checkline;
        fstream termfile;
        termfile.open("temp.txt", ios::out);
        ofstream settermfile("temp.txt", ios::app);
        while (getline(inputFile, templine))
        {
            settermfile << templine<<endl;
        }
        ifstream readtermfile("temp.txt");
        fstream file;
        file.open(fileName, ios::out);
        ofstream fileAccount(fileName, ios::app);
        while (getline(readtermfile, checkline))
        {
            if (checkline!= lineToDelete)
            {
                    fileAccount << checkline<<endl;
            }
            else
            {
                    flag++;
             }
        }
        inputFile.close();
        fileAccount.close();
        if (flag == 0)
        {
            return false;
        }
        else
        {
            return true;
        }
    }


    void viewBorrowHistory(bool islibrabian)
    {
        if (islibrabian)
        {
            ifstream inputFile(bookBorrowedList);
            if (!inputFile.is_open())
            {
                cout << RED << " No book was borrowed " << endl;
                DeFaultColor();
            }
            else
            {
                string line;
                cout << "       BorrowList:         " << endl;
                while (getline(inputFile,line))
                {
                    cout << line << endl;
                }
            }
        }
        else
        {
            string link = currentUser;
            link += ".txt";
            ifstream inputFile(link);
            if (!inputFile.is_open())
            {
                cout << RED << " No book was borrowed " << endl;
                DeFaultColor();
            }
            else
            {
                string line;
                cout << "       BorrowList:         " << endl;
                while (getline(inputFile, line))
                {
                    cout << line << endl;
                }
            }
        }
    }



};
class MainMenu
{
private:
    bool libiran;
public:

    MainMenu(bool isLibrian)
    {
        libiran = isLibrian;
    }
    void ShowMenu()
    {
        library Library;
        string book_name, author_name, book_type, book_language;
        double price;
        string current_book;
        //clearScreen();
        /*Library.insert_last("c++", "science", "english", 18, "james");
        Library.insert_last("100_day", "horror", "arabic", 25, "emilia");
        Library.insert_last("learn_spanish_easily", "language", "spanish", 10, "bill");
        Library.insert_last("data_structure", "science", "english", 30, "khaled");
        Library.insert_last("1980", "historical", "latin", 14, "pon");*/
        if (libiran)
        {
            cout << "---------------------------------------------" << endl;
            cout << "           Welcome to Our Library            " << endl;
            cout << "---------------------------------------------" << endl;
            cout << "       Select one of the following           " << endl;
            cout << "   1: Add a new book                         " << endl;
            cout << "   2: Search for a book                      " << endl;
            cout << "   3: Edit a book                            " << endl;
            cout << "   4: Delete a book                          " << endl;
            cout << "   5: Sort all books                         " << endl;
            cout << "   6: Display all books                      " << endl;
            cout << "   7: Retrieve Books                         " << endl;
            cout << "   8: View borrow history                    " << endl;
            cout << "   9: Exit                                  " << endl;

            cout << "\nEnter your Choice:\n";
            int choice;
            while (!(cin >> choice && choice <= 9 && choice >= 1))
            {
                cin.clear();
                cout << RED << "please Enter a Valid Choice" << endl;
                DeFaultColor();
            }

            if (choice == 1)
            {
                cout << "---------------------------------------------" << endl;
                cout << "       Select one of the following           " << endl;
                cout << "---------------------------------------------" << endl;
                cout << "   1: Add First                              " << endl;
                cout << "   2: Add last                               " << endl;
                cout << "   3: Add at position                        " << endl;
                cout << "\nEnter your Choice:\n";
                int choice1;
                while (!(cin >> choice1 && choice1 <= 3 && choice1 >= 1)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << RED << "please Enter a Valid Choice" << endl;
                    DeFaultColor();
                }
                cout << "Enter Book name: " << endl;
                cin >> book_name;
                cout << "Enter Book type: " << endl;
                cin >> book_type;
                cout << "Enter Book language: " << endl;
                cin >> book_language;
                cout << "Enter Price: " << endl;
                while (!(cin >> price && price >= 0)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << RED << "please Enter a Valid Price" << endl;
                    DeFaultColor();
                }
                cout << "Enter author name: " << endl;
                cin >> author_name;
                if (choice1 == 1) {
                    Library.insert_first(book_name, book_type, book_language, price, author_name,true);
                    cout << " Add completed.. " << endl;
                }
                else if (choice1 == 2) {
                    Library.insert_last(book_name, book_type, book_language, price, author_name,true);
                    cout << " Add completed.. " << endl;
                }
                else if (choice1 == 3) {
                    cout << "Enter current book: " << endl;
                    cin >> current_book;
                    if (Library.insert_After(current_book, book_name, book_type, book_language, price, author_name,true))
                    {
                        cout << " Add completed.. " << endl;
                    }

                }
            }

            else if (choice == 2) {
                cout << "---------------------------------------------" << endl;
                cout << "       Select one of the following           " << endl;
                cout << "---------------------------------------------" << endl;
                cout << "   1: Search by book name                    " << endl;
                cout << "   2: Search by book type                    " << endl;
                cout << "   3: Search by book language                " << endl;
                cout << "   4: Search by book price                   " << endl;
                cout << "   5: Search by book author                  " << endl;
                cout << "\nEnter your Choice:\n";
                int choice2;
                while (!(cin >> choice2 && choice2 <= 5 && choice2 >= 1)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << RED << "please Enter a Valid Choice" << endl;
                    DeFaultColor();
                }
                if (choice2 == 1) {
                    cout << "Enter book name: " << endl;
                    cin >> book_name;
                    Library.SearchingBook(book_name, BookName);
                }
                else if (choice2 == 2) {
                    cout << "Enter book type: " << endl;
                    cin >> book_type;
                    Library.SearchingBook(book_type, BookType);
                }
                else if (choice2 == 3) {
                    cout << "Enter book language: " << endl;
                    cin >> book_language;
                    Library.SearchingBook(book_language, BookLanguage);
                }
                else if (choice2 == 4) {
                    cout << "Enter book price: " << endl;
                    while (!(cin >> price && price >= 0)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << RED << "please Enter a Valid Price" << endl;
                        DeFaultColor();
                    }
                    Library.SearchingBook(to_string(price), BookPrice);
                }
                else if (choice2 == 5) {
                    cout << "Enter author name: " << endl;
                    cin >> author_name;
                    Library.SearchingBook(author_name, BookAuthor);
                }
            }

            else if (choice == 3) { //modifying a book list
                cout << "---------------------------------------------" << endl;
                cout << "       Select one of the following           " << endl;
                cout << "---------------------------------------------" << endl;
                cout << "   1: Edit first book                        " << endl;
                cout << "   2: Edit last book                         " << endl;
                cout << "   3: Edit a certain book                    " << endl;
                cout << "\nEnter your Choice:\n";
                int choice3;
                while (!(cin >> choice3 && choice3 <= 3 && choice3 >= 1)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << RED << "please Enter a Valid Choice" << endl;
                    DeFaultColor();
                }
                cout << "Enter Book name: " << endl;
                cin >> book_name;
                cout << "Enter Book type: " << endl;
                cin >> book_type;
                cout << "Enter Book language: " << endl;
                cin >> book_language;
                cout << "Enter Price: " << endl;
                while (!(cin >> price && price >= 0)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << RED << "please Enter a Valid Price" << endl;
                    DeFaultColor();
                }
                cout << "Enter author name: " << endl;
                cin >> author_name;
                if (choice3 == 1) {
                    Library.update_first(book_name, book_type, book_language, price, author_name);
                }
                else if (choice3 == 2) {
                    Library.update_last(book_name, book_type, book_language, price, author_name);
                }
                else if (choice3 == 3) {
                    cout << "Enter current book: " << endl;
                    cin >> current_book;
                    Library.update_at(current_book, book_name, book_type, book_language, price, author_name);
                }
            }

            else if (choice == 4) { //deleting a book list
                cout << "---------------------------------------------" << endl;
                cout << "       Select one of the following           " << endl;
                cout << "---------------------------------------------" << endl;
                cout << "   1: Delete first book                      " << endl;
                cout << "   2: Delete last book                       " << endl;
                cout << "   3: Delete a certain book                  " << endl;
                cout << "\nEnter your Choice:\n";
                int choice4;
                while (!(cin >> choice4 && choice4 <= 3 && choice4 >= 1)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << RED << "please Enter a Valid Choice" << endl;
                    DeFaultColor();
                }
                if (choice4 == 1) {
                    Library.delete_first();
                }
                else if (choice4 == 2) {
                    Library.delete_end();
                }
                else if (choice4 == 3) {
                    cout << "Enter book name: " << endl;
                    cin >> book_name;
                    Library.delete_by_name(book_name);
                }
            }

            else if (choice == 5) { //sorting list
                cout << "---------------------------------------------" << endl;
                cout << "       Select one of the following           " << endl;
                cout << "---------------------------------------------" << endl;
                cout << "   1: Sort by price                          " << endl;
                cout << "   2: Sort by book name                      " << endl;
                cout << "\nEnter your Choice:\n";
                int choice5;
                while (!(cin >> choice5 && choice5 <= 2 && choice5 >= 1)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << RED << "please Enter a Valid Choice" << endl;
                    DeFaultColor();
                }
                if (choice5 == 1) {
                    Library.sortListWithPrice();
                    Library.traversal();
                }
                else if (choice5 == 2) {
                    Library.sortListWithBookName();
                    Library.traversal();
                }

            }

            else if (choice == 6) { //display list
                Library.traversal();
            }

            else if (choice == 7) { // retrieve books
                Library.giveBackBook(true);
            }
            else if (choice == 8) { // view borrow history
                Library.viewBorrowHistory(true);
            }
            else if (choice == 9) { //quit the program
                exit(0);
            }
        }
        else
        {

            cout << "---------------------------------------------" << endl;
            cout << "           Welcome to Our Library            " << endl;
            cout << "---------------------------------------------" << endl;
            cout << "       Select one of the following           " << endl;
            cout << "   1: Search for a book                      " << endl;
            cout << "   2: Sort all books                         " << endl;
            cout << "   3: Display all books                      " << endl;
            cout << "   4: Borrow books                           " << endl;
            cout << "   5: Return books                           " << endl;
            cout << "   6: View borrow history                    " << endl;
            cout << "   7: Exit                                   " << endl;

            cout << "\nEnter your Choice:\n";
            int choice;
            while (!(cin >> choice && choice <= 6 && choice >= 1)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << RED << "please Enter a Valid Choice" << endl;
                DeFaultColor();
            }

            if (choice == 1) { //search_by_book list
                cout << "---------------------------------------------" << endl;
                cout << "       Select one of the following           " << endl;
                cout << "---------------------------------------------" << endl;
                cout << "   1: Search by book name                    " << endl;
                cout << "   2: Search by book type                    " << endl;
                cout << "   3: Search by book language                " << endl;
                cout << "   4: Search by book price                   " << endl;
                cout << "   5: Search by book author                  " << endl;
                cout << "\nEnter your Choice:\n";
                int choice;
                while (!(cin >> choice && choice <= 5 && choice >= 1))
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << RED << "please Enter a Valid Choice" << endl;
                    DeFaultColor();
                }
                if (choice == 1) {
                    cout << "Enter book name: " << endl;
                    cin >> book_name;
                    Library.SearchingBook(book_name, BookName);
                }
                else if (choice == 2) {
                    cout << "Enter book type: " << endl;
                    cin >> book_type;
                    Library.SearchingBook(book_type, BookType);
                }
                else if (choice == 3) {
                    cout << "Enter book language: " << endl;
                    cin >> book_language;
                    Library.SearchingBook(book_language, BookLanguage);
                }
                else if (choice == 4) {
                    cout << "Enter book price: " << endl;
                    while (!(cin >> price && price >= 0)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << RED << "please Enter a Valid Price" << endl;
                        DeFaultColor();
                    }
                    Library.SearchingBook(to_string(price), BookPrice);
                }
                else if (choice == 5) {
                    cout << "Enter author name: " << endl;
                    cin >> author_name;
                    Library.SearchingBook(author_name, BookAuthor);
                }
            }

            else if (choice == 2) { //sort list
                cout << "---------------------------------------------" << endl;
                cout << "       Select one of the following           " << endl;
                cout << "---------------------------------------------" << endl;
                cout << "   1: Sort by price                          " << endl;
                cout << "   2: Sort by book name                      " << endl;
                cout << "\nEnter your Choice:\n";
                int choice2;
                while (!(cin >> choice2 && choice2 <= 2 && choice2 >= 1)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << RED << "please Enter a Valid Choice" << endl;
                    DeFaultColor();
                }
                if (choice2 == 1) {
                    Library.sortListWithPrice();
                    Library.traversal();
                }
                else if (choice2 == 2) {
                    Library.sortListWithBookName();
                    Library.traversal();
                }
            }

            else if (choice == 3) { //displays books
                Library.traversal();
            }

            else if (choice == 4) { // borrow books
                cout << "Enter the name of the book you want to borrow: ";
                cin >> book_name;
                Library.borrowBook( book_name);
            }

            else if (choice == 5)
            { // return books
                Library.giveBackBook(false);
            }

            else if (choice == 6) 
            { 
                Library.viewBorrowHistory(false);
            }
            else if (choice == 7) { //quit the program
                exit(0);
            }
        }

    }
};
class Login
{
public:
    void DoLogin()
    {
        cout << "Login as Librian or user ?" << endl;
        string name;
        do
        {
            getline(cin, name);
            transform(name.begin(), name.end(), name.begin(), ::tolower);
            if (name != "librian" && name != "user")
            {
                cout << "Please enter a valid Choice" << endl;
            }
        } while (name != "librian" && name != "user");
        if (name == "librian")
        {
            LibrianLogin();
        }
        else
        {
            UserLogin();
        }
    }
    void LibrianLogin()
    {
        isSuccess = StartLogin(fileLibrianAccount,true);
        if (isSuccess)
        {
            while (true)
            {
                MainMenu menu(true);
                menu.ShowMenu();
            }

        }
        else
        {
            //clearScreen();
            cout << RED << "Account is not found. Please try again." << endl;
            DeFaultColor();
        }
    }
    void UserLogin()
    {
        isSuccess = StartLogin(fileUserAccount,false);
        if (isSuccess)
        {
            while (true)
            {
                MainMenu menu(false);
                menu.ShowMenu();
            }
        }
        else
        {
            //clearScreen();
            cout << RED << "Account is not found. Please try again." << endl;
            DeFaultColor();
            cin.clear();

        }
    }
    bool StartLogin(const char* fileName, bool isLibrarian)
    {

        char userName[50], passWord[50];
        cout << "Name: ";
        cin >> userName;
        cout << "passWord: ";
        cin >> passWord;
        ifstream fileAccount(fileName);

        if (!fileAccount.is_open())
        {
            return false;
        }
        Account A;
        bool found = false;

        while (fileAccount >> A.accountName >> A.accountPassword)
        {
            if (strcmp(A.accountName, userName) == 0 && strcmp(A.accountPassword, passWord) == 0)
            {
                cout << "\nlogin successfully. ";
                if (!isLibrarian)
                {
                    strncpy_s(currentUser, sizeof(currentUser), A.accountName, _TRUNCATE);
                    strncat_s(currentUser, sizeof(currentUser), A.accountPassword, _TRUNCATE);
                }
                found = true;
                break;
            }
        }

        fileAccount.close();

        if (!found)
        {
            return false;
        }
        return true;

    }
private:
    char getchoice;
    bool isSuccess;
};
class SignUp
{
public:
    void DoSignUp()
    {
        cout << "Sign Up as Librian or user ?" << endl;
        string name;
        do
        {
            getline(cin, name);
            transform(name.begin(), name.end(), name.begin(), ::tolower);
            if (name != "librian" && name != "user")
            {
                cout << RED << "Please enter a valid Choice" << endl;
                DeFaultColor();
            }
        } while (name != "librian" && name != "user");
        if (name == "librian")
        {
            LibrianSignUp();
        }
        else
        {
            UserSignUp();
        }
    }
private:
    void  LibrianSignUp()
    {
        if (StartSignUp(fileLibrianAccount, true))
        {
            cout << "Create Librian account successfully";
        }
        else
        {
            cout << RED << " Sign Up Failed. Sign Up Again" << endl;
            DeFaultColor();
        }
    }
    void UserSignUp()
    {
        if (StartSignUp(fileUserAccount, false))
        {
            cout << "Create User account successfully";
        }
        else
        {
            cout << RED << " Sign Up Failed. Sign Up Again " << endl;
            DeFaultColor();
        }
    }
    bool StartSignUp(const char* fileName, bool isLibrian)
    {
        char adminPasss[50];
        Account A;
        cout << "New Name: ";
        cin >> A.accountName;
        cout << "New passWord: ";
        cin >> A.accountPassword;
        bool success;
        int count = 3;
        if (isLibrian)
        {
            do
            {
                cout << "Enter adminCode: ";
                cin >> adminPasss;
                if (strcmp(adminPasss, adminPassword) == 0)
                {
                    success = true;
                    count = 0;
                }
                else
                {
                    success = false;
                    count--;
                }
            } while (count > 0);
            if (success)
            {
                return GetAccountNameAndPassword(A, fileName);
            }
            else
            {
                cout << RED << "          <<Access Denied>>         " << endl;
                DeFaultColor();
                return false;
            }
        }
        else
        {
            return GetAccountNameAndPassword(A, fileName);
        }
    }
    bool GetAccountNameAndPassword(Account getAccount, const char* filename)
    {
        bool allowSignup;
        ifstream checkfileAccount(filename);
        if (!checkfileAccount.is_open())
        {
            allowSignup = true;
        }
        else
        {
            Account A;
            bool found = false;

            while (checkfileAccount >> A.accountName >> A.accountPassword)
            {
                if (strcmp(A.accountName, getAccount.accountName) == 0 && strcmp(A.accountPassword, getAccount.accountPassword) == 0)
                {
                    cout << RED << "       Account already exists       ";
                    DeFaultColor();
                    found = true;
                }
            }
            allowSignup = !found;
        }
        if (allowSignup)
        {

            ofstream fileAccount(filename, ios::app);
            if (!fileAccount.is_open())
            {
                fstream file;
                file.open(filename, ios::out);
                ofstream fileAccount(filename, ios::app);
            }
            fileAccount << getAccount.accountName << " " << getAccount.accountPassword << endl;
            fileAccount.flush();
            fileAccount.close();
            return true;
        }
        else
        {
            return false;
        }

    }
};

int main()
{
    Login login;
    SignUp signup;
    clearScreen();
    DeFaultColor();
    while (true)
    {
        string option;
        cout << "             <<Login or SignUp>>             " << endl;
        getline(cin, option);
        transform(option.begin(), option.end(), option.begin(), ::tolower);
        if (option != "login" && option != "signup")
        {
            cout << RED << "Please enter a valid Choice" << endl;
            DeFaultColor();
        }
        else if (option == "login")
        {
            login.DoLogin();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else
        {
            signup.DoSignUp();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

    }
}