#include "structures.h"
#include "main.h"

struct SQLQuery {
    string action;
    string table_name;
    LinkedList* values;
    LinkedList* columns; // Для SELECT
    LinkedList* tables;
    string condition;
};


SQLQuery parse_insert_query(const string& query) {
    SQLQuery result;
    result.values = new LinkedList(); // Инициализация LinkedList
    stringstream ss(query);
    string token;

    // Пропускаем ключевые слова INSERT INTO
    getline(ss, token, ' ');
    getline(ss, token, ' ');
    getline(ss, token, ' ');

    // Получаем имя таблицы
    result.table_name = token;

    // Пропускаем ключевое слово VALUES
    getline(ss, token, ' ');


    // Получаем значения
    getline(ss, token, '(');
    getline(ss, token, ')');
    stringstream values_ss(token);
    string value;
    while (getline(values_ss, value, ',')) {
        value.erase(0, 1); // Удаляем кавычки
        value.erase(value.size() - 1, 1);
        result.values->push_back(value);

    }



    return result;
}

SQLQuery parse_delete_query(const string& query) {
    SQLQuery result;
    stringstream ss(query);
    string token;

    // Пропускаем ключевое слово DELETE
    getline(ss, token, ' ');

    // Пропускаем ключевое слово FROM
    getline(ss, token, ' ');
    getline(ss, token, ' ');

    // Получаем имя таблицы
    result.table_name = token;

    // Проверяем наличие условия WHERE
    getline(ss, token, ';');
    
    result.condition=token;

    return result;
}

string removeSpaces(std::string str) {
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
    return str;
}

SQLQuery parse_select_query(const string& query) {
    SQLQuery result;
    result.columns = new LinkedList();
    stringstream ss(query);
    string token;

    // Пропускаем ключевое слово SELECT
    getline(ss, token, ' ');

    // Получаем список колонок
    getline(ss, token, ' ');
    //stringstream columns_ss(token);
    do {
        if (token=="FROM") break;
        stringstream column_ss(token);
        getline(column_ss,token,',');
        result.columns->push_back(token);
     }
    while(getline(ss,token,' '));
    


    // Пропускаем ключевое слово FROM
    getline(ss, token, ' ');


    // Получаем имя таблицы
    result.tables = new LinkedList();
    do {
        if (token=="WHERE") break;
        stringstream table_ss(token);
        getline(table_ss,token,',');
        result.tables->push_back(token);
     }
    while(getline(ss,token,' '));

    // Проверяем наличие условия WHERE
    getline(ss, token, ' ');
    if (token == "WHERE") {
        getline(ss, result.condition, ';'); // Извлекаем условие WHERE
    }

   

    return result;
}