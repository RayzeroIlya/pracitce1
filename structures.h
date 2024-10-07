#pragma once
#include <iostream>


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
};



struct Schema {
    string name;
    int tuples_limit;
};

