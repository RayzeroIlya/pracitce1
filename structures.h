#pragma once
#include <iostream>

//struct TableNode;
struct Table;

// Структура данных для узла связанного списка

struct Node {
    string data;
    Node* next;

    Node(const string& data) : data(data), next(nullptr) {}
    Node(Table* table) : data(data), next(nullptr) {}
};

// Структура данных для связанного списка

struct LinkedList {
    Node* head;

    LinkedList() : head(nullptr) {}
    // Вставка элемента в начало списка
    void insert(const string& data) {
        Node* newNode = new Node(data);
        newNode->next = head;
        head = newNode;
    }

    // Поиск элемента в списке
    Node* find(const string& data) {
        Node* current = head;
        while (current != nullptr) {
            if (current->data == data) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    // Удаление элемента из списка
    void remove(const string& data) {
        if (head == nullptr) {
            return;
        }

        if (head->data == data) {
            Node* temp = head;
            head = head->next;
            delete temp;
            return;
        }

        Node* current = head;
        while (current->next != nullptr) {
            if (current->next->data == data) {
                Node* temp = current->next;
                current->next = current->next->next;
                delete temp;
                return;
            }
            current = current->next;
        }
    }

    // Вывод всех элементов списка
    void print() {
        Node* current = head;
        while (current != nullptr) {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }

    // Освобождение памяти
    void clear() {
        Node* current = head;
        while (current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
        head = nullptr;
    }

       void push_back(const string& data) {
        Node* newNode = new Node(data);
        if (head == nullptr) {
            head = newNode;
            return;
        }

        Node* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }

       void push_back(Table* data) {
        Node* newNode = new Node(data);
        if (head == nullptr) {
            head = newNode;
            return;
        }

        Node* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }

};





struct TableNode {
    LinkedList* row;
    TableNode* nextRow;
    TableNode() : row(new LinkedList()), nextRow(nullptr) {}
    TableNode(LinkedList* row) : row(row), nextRow(nullptr) {}
};

struct Table{
    TableNode* head;
    Table() : head(new TableNode()){}
      void push_back(LinkedList* data) {
        TableNode* newNode = new TableNode(data);
        if (head == nullptr) {
            head = newNode;
            return;
        }

        TableNode* current = head;
        while (current->nextRow != nullptr) {
            current = current->nextRow;
        }
        current->nextRow = newNode;
    }
    void print(){
        TableNode* current=head;
        while(current!= nullptr) {
            current->row->print();
            cout << endl;
            current=current->nextRow;
        }

    }
};

struct Schema {
    string name;
    int tuples_limit;
   LinkedList* tables;
};
/*#pragma once
#include <iostream>
#include <string>

struct Table;
std::ostream& operator<<(std::ostream& os, const Table& table); 
// Структура данных для узла связанного списка
template <typename T>
struct Node {
    T data;
    Node<T>* next;

    // Конструктор для базового типа T
    Node(const T& data) : data(data), next(nullptr) {}

    // Конструктор для типа Table (с явным преобразованием типов)
    Node(Table* data) : data(static_cast<T>(data)), next(nullptr) {} 
};

// Структура данных для связанного списка
template <typename T>
struct LinkedList {
    Node<T>* head;

    LinkedList() : head(nullptr) {}

    // Вставка элемента в начало списка
    void insert(const T& data) {
        Node<T>* newNode = new Node<T>(data);
        newNode->next = head;
        head = newNode;
    }

    // Поиск элемента в списке
    Node<T>* find(const T& data) {
        Node<T>* current = head;
        while (current != nullptr) {
            if (current->data == data) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    // Удаление элемента из списка
    void remove(const T& data) {
        if (head == nullptr) {
            return;
        }

        if (head->data == data) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
            return;
        }

        Node<T>* current = head;
        while (current->next != nullptr) {
            if (current->next->data == data) {
                Node<T>* temp = current->next;
                current->next = current->next->next;
                delete temp;
                return;
            }
            current = current->next;
        }
    }

    // Вывод всех элементов списка
    void print() {
        Node<T>* current = head;
        while (current != nullptr) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }

    // Освобождение памяти
    void clear() {
        Node<T>* current = head;
        while (current != nullptr) {
            Node<T>* temp = current;
            current = current->next;
            delete temp;
        }
        head = nullptr;
    }

    // Добавление элемента в конец списка
    void push_back(const T& data) {
        Node<T>* newNode = new Node<T>(data);
        if (head == nullptr) {
            head = newNode;
            return;
        }

        Node<T>* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
        // Добавление элемента в конец списка
    void push_back(const T& data) {
        Node<Table>* newNode = new Node<Table>(data);
        if (head == nullptr) {
            head = newNode;
            return;
        }

        Node<Table>* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
};

// Структура данных для узла таблицы
struct TableNode {
    LinkedList<std::string>* row;
    TableNode* nextRow;
    TableNode() : row(new LinkedList<std::string>()), nextRow(nullptr) {}
    TableNode(LinkedList<std::string>* row) : row(row), nextRow(nullptr) {}
};

struct Table{
    TableNode* head;
    Table() : head(new TableNode()){}

    // Добавление строки в таблицу
    void push_back(LinkedList<std::string>* data) {
        TableNode* newNode = new TableNode(data);
        if (head == nullptr) {
            head = newNode;
            return;
        }

        TableNode* current = head;
        while (current->nextRow != nullptr) {
            current = current->nextRow;
        }
        current->nextRow = newNode;
    }

    // Вывод таблицы
    void print(){
        TableNode* current=head;
        while(current!= nullptr) {
            current->row->print();
            std::cout << std::endl;
            current=current->nextRow;
        }
    }
};
std::ostream& operator<<(std::ostream& os,Table* table) {
    TableNode* current = table->head;
    while (current != nullptr) {
        current->row->print();
        current = current->nextRow;
    }
    return os;
}

// Структура данных для схемы
struct Schema {
    std::string name;
    int tuples_limit;
    LinkedList<Table>* tables;
};

*/