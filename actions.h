#include "main.h"
#include "structures.h"
#include "parsers.h"
#include <fstream>


struct DBMS{

int get_primary_key(const string& table_path ){
    string file_path = table_path + "_pk_sequence";
    ifstream infile(file_path);
    int primary_key = 0;
    if (infile.is_open()) {
        infile >> primary_key;
        infile.close();
    }
    return primary_key;
}

void update_primary_key(const string& table_name, int new_primary_key) {
    string file_path = table_name + "_pk_sequence";
    ofstream outfile(file_path);
    if (outfile.is_open()) {
        outfile << new_primary_key;
        outfile.close();
    }
}

int get_row_count(const string& file_path) {
    ifstream infile(file_path);
    int count = 0;
    string line;
    while (getline(infile, line)) {
        count++;
    }
    infile.close();
    return count;
}

void insert_into_csv(const Schema& schema,const string& table_name, const SQLQuery& query ) {
        
        int primary_key = get_primary_key(schema.name+"/"+table_name+"/"+table_name);
        int count_file= 1;
        string file_path = schema.name+"/"+table_name+"/"+to_string(count_file)+".csv";
    ofstream outfile(file_path, ios::app); // Открываем файл для добавления
         int row_count = get_row_count(file_path);



        while (row_count >= schema.tuples_limit) {
        count_file++;
        row_count = 0; // Сбрасываем счетчик для нового файла
        file_path = schema.name+"/"+table_name+"/"+to_string(count_file)+".csv";
        row_count = get_row_count(file_path);
    }   



    if (outfile.is_open()) {

        // Добавляем первичный ключ как первую колонку
        outfile << primary_key << ",";
        // Добавляем остальные значения
        for (Node* current = query.values->head; current != nullptr; current = current->next) {
            outfile << current->data;
            if (current->next != nullptr) {
                outfile << ",";
            }
        }
        outfile << endl;
        outfile.close();

        // Обновляем первичный ключ в файле
        update_primary_key(schema.name+"/"+table_name+"/"+table_name, primary_key + 1);

        cout << "Строка добавлена в файл " << file_path << endl;
    } else {
        cout << "Ошибка открытия файла " << file_path << endl;
    }
}

string build_condition_string(Node* node) {
    stringstream ss;
    while (node != nullptr) {
        ss << node->data << " ";
        node = node->next;
    }
    return ss.str();
}

bool evaluate_condition(const string& line, const string& condition, const string& column_names) {
    stringstream ss(condition);
    string token;
    LinkedList* tokens = new LinkedList();
    while (getline(ss, token, ' ')) {
        tokens->push_back(token);
    }

    // Проверка на "WHERE"
    if (tokens->head->data == "WHERE") {
        // Ищем "AND" или "OR" в списке
        Node* and_or_node = nullptr;
        for (Node* current = tokens->head->next; current != nullptr; current = current->next) {
            if (current->data == "AND" || current->data == "OR") {
                and_or_node = current;
                break;
            }
        }

        if (and_or_node != nullptr) {
            // Найден оператор "AND" или "OR"
            string operator_token = and_or_node->data;

            // Разделяем условие на две части
            Node* left_part = tokens->head->next;
            Node* right_part = and_or_node->next;

            // Рекурсивно проверяем левую и правую части
            bool left_result = evaluate_condition(line, build_condition_string(left_part), column_names);
            bool right_result = evaluate_condition(line, build_condition_string(right_part), column_names);

            // Возвращаем результат в соответствии с оператором
            if (operator_token == "AND") {
                return left_result && right_result;
            } else if (operator_token == "OR") {
                return left_result || right_result;
            }
        }
    }
                if(tokens->head->data == "WHERE") tokens->remove("WHERE");
                if (tokens->head->next->data == "=") {
                string column = tokens->head->data;
                stringstream ss(column_names);
                string column_name;
                int count=0;
                while(getline(ss,column_name,',') && column_name!=column){
                    count++;
                }
                ss.str("");

                string value = tokens->head->next->next->data;
                value.erase(0,1);
                value.erase(value.size() - 1, 1);
                ss << line;
                

                string current_value;
                while (getline(ss, current_value,',') && count!=0){
                    count--;
                }
                if (current_value == value) {
                    return current_value == value;
                }
            }

    tokens->clear();
    delete tokens;
    return false;
}

void delete_from_csv(const Schema& schema, const SQLQuery& query) {
    int count_file=1;
    string file_path = schema.name+"/"+query.table_name+"/" +to_string(count_file)+ ".csv";


    ifstream infile(file_path);



    while(infile.is_open()){
    ofstream outfile(file_path + ".tmp", ios::out); // Временный файл

    if (infile.is_open() && outfile.is_open()) {
        string line, column_names;
        getline(infile,column_names);
        outfile << column_names << endl;
        while (getline(infile, line)) {
            if (!evaluate_condition(line, query.condition, column_names)) {
                // Если условие не выполняется, записываем строку в временный файл
                outfile << line << endl;
            }
        }
        infile.close();
        outfile.close();

        // Перемещаем временный файл обратно
        remove(file_path.c_str());
        rename((file_path + ".tmp").c_str(), file_path.c_str());

        cout << "Строки удалены из файла " << file_path << endl;
    } else {
        cout << "Ошибка открытия файла " << file_path << endl;
    }


    count_file++;
     file_path = schema.name+"/"+query.table_name+"/" +to_string(count_file)+ ".csv";
     infile.open(file_path);

}


}

LinkedList* select_data(const SQLQuery& query, const string& file_path, const string& column_names) {
    LinkedList* result = new LinkedList();
    ifstream infile(file_path);
    string line;

    // Читаем заголовок файла
    getline(infile, line);

    while (getline(infile, line)) {
        // Проверяем условие WHERE
        if (query.condition.empty() || evaluate_condition(line, query.condition, column_names)) {
            // Если условие не задано или выполняется
            stringstream ss(line);
            string value;
            while (getline(ss, value, ',')) {
                // Проверяем, нужно ли выбрать текущую колонку
                if (query.columns != nullptr && query.columns->find(value) != nullptr) {
                    result->push_back(value);
                }
            }
        }
    }
    infile.close();
    return result;
}

};