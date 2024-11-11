#pragma once
#include <iostream>

//struct TableNode;
struct Table;

// Структура данных для узла связанного списка
struct Node {
    string data;
    Node* next;

    Node(const string& data) : data(data), next(nullptr) {}

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
            cout << current->data << "\t";
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
            current=current->nextRow;
        }

    }
};


struct  TablesNode {
    Table* table;
    TablesNode* nextTable;
    TablesNode(Table* table) : table(table), nextTable(nullptr) {}
};

struct Tables {
    TablesNode* head;

    Tables() : head(nullptr) {}
      void push_back(Table* data) {
        TablesNode* newNode = new TablesNode(data);
        if (head == nullptr) {
            head = newNode;
            return;
        }

        TablesNode* current = head;
        while (current->nextTable != nullptr) {
            current = current->nextTable;
        }
        current->nextTable = newNode;
    }

        void print(TablesNode* table,LinkedList* row){
        TablesNode* currentTable=head;

        while (currentTable!= nullptr) {
            row->print();
            LinkedList* currentRow = currentTable->table->head->row;
            print(currentTable->nextTable,currentRow);
        }

    }

    void print(Tables* tables){
        TablesNode* currentTable=tables->head;

        while (currentTable!= nullptr) {
            LinkedList* currentRow = currentTable->table->head->row;
            print(currentTable->nextTable,currentRow);
            currentRow=currentTable->table->head->nextRow->row;
        }

    }
};

struct Schema {
    string name;
    int tuples_limit;
   
};