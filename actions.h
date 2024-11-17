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

        ifstream fin(file_path);
        if(!fin.is_open()) {
            cout << "Файл "+file_path+" не найден!" <<endl;
            return;
        };
        int count_column =0;
        string column_name;
        getline(fin,column_name);
        stringstream ss(column_name);
        while(getline(ss,column_name,',')) {
            count_column++;
        }
        for (Node* current = query.values->head;current!=nullptr;current=current->next,count_column--);
        if (count_column!=1) {
                    cout << "Ошибка добавления: неверное количество аргументов"  << endl;
            return;}


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

Tables* select_data(const SQLQuery& query, const string& file_path,Schema& schema) {
    LinkedList* columns = new LinkedList(); 
    Tables* tables=new Tables();
    int file_count=1;
    int current_row=1;
    ifstream fin;
    Node* table_name=query.tablesName->head;
    tables->head->table=new Table(table_name->data);
    TablesNode* currentTable=tables->head;
    while (currentTable!=nullptr) {
    fin.open(file_path+ currentTable->table->name+"/"+to_string(file_count)+".csv");
    string row,value;
    getline(fin,row);
    stringstream ss(row);
    while (getline(ss,value,',')){
        columns->push_back(value);
    } // Название колонок
    LinkedList* query_columns=query.columns;

    Node* currentColumn=columns->head;

    TableNode* currentTableRow = currentTable->table->head;
    while(currentTableRow!=nullptr && currentColumn!=nullptr) {
        if (query_columns->find(currentColumn->data) || currentColumn->data==currentTable->table->name+"_pk"){
            currentTableRow->row->push_back(currentColumn->data);
                if (currentColumn->data==currentTable->table->name+"_pk") {
                currentColumn=currentColumn->next;
                continue;
            };
        }
            currentColumn=currentColumn->next;

    }
    currentTableRow=currentTableRow->nextRow;
    currentTableRow=new TableNode();
    currentTable->table->head->nextRow=currentTableRow;

    currentColumn=columns->head;
    while(fin.is_open() && getline(fin,row)){
    
    
        stringstream ss(row);


        while(getline(ss,value,',') && currentColumn!= nullptr) {
        if (query_columns->find(currentColumn->data) || currentColumn->data==currentTable->table->name+"_pk"){
            currentTableRow->row->push_back(value);

            if (currentColumn->data==currentTable->table->name+"_pk") {
                currentColumn=currentColumn->next;
                continue;
            };

        }
        currentColumn=currentColumn->next;


        }
        currentTableRow->nextRow=new TableNode();
        currentTableRow=currentTableRow->nextRow;

        currentColumn=columns->head;
     
     
     }
    columns->clear();
  //  delete columns;

    table_name=table_name->next;
    if (table_name==nullptr) break;

    currentTable->nextTable=new TablesNode();
    currentTable=currentTable->nextTable;
    currentTable->table=new Table(table_name->data);
    fin.close();
}
    fin.close();

    if (!query.condition.empty()) {
        string cond = query.condition;
        currentTable=tables->head;
        while (currentTable){
        TableNode* currentRow=currentTable->table->head;
        string column_names=tables->buildRow(currentRow->row);
        TableNode* prevRow=currentRow;
        currentRow=currentRow->nextRow;
            while (currentRow->row->head){
                TableNode* temp=currentRow;    
                if (!evaluate_condition(tables->buildRow(currentRow->row),cond,column_names)){
                    prevRow->nextRow=currentRow->nextRow;
                    temp->row->clear();
                    delete temp;
                    currentRow=prevRow->nextRow;
                    continue;
        }
            prevRow=prevRow->nextRow;
            currentRow=currentRow->nextRow;
            }
            currentTable=currentTable->nextTable;
    }

}
     return tables;
}
};