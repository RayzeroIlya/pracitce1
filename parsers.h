#include "structures.h"
#include "main.h"

struct SQLQuery {
    

    string table_name;
    LinkedList* values;
    LinkedList* columns;
    LinkedList* tablesName;
    string condition;
};

 bool is_service_word(string token) {
    string verbs = "INSERT INTO FROM WHERE AND OR SELECT DELETE VALUES";
    if (verbs.find(token)!=string::npos) return true;
    return false;

 }

SQLQuery parse_insert_query(const string& query) {
    SQLQuery result;
    result.values = new LinkedList(); // Инициализация LinkedList
    stringstream ss(query);
    string token;

    // Пропускаем ключевые слова INSERT INTO
    getline(ss, token, ' ');
    if(token!="INSERT") {
        result.table_name="-1";
        return result;
    };  
    getline(ss, token, ' ');
    if (token!="INTO") {    
        result.table_name="-1";
        return result;
    }
    getline(ss, token, ' ');

    // Получаем имя таблицы
    
    if(is_service_word(token)) {
        result.table_name="-1";
        return result;
    }
    result.table_name = token;
    // Пропускаем ключевое слово VALUES
    getline(ss, token, ' ');
    if(token!="VALUES"){
        result.table_name="-1";
        return result;
    }

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
    if (token!= "DELETE") {
        result.table_name="-1";
        return result;
    }

    // Пропускаем ключевое слово FROM
    getline(ss, token, ' ');
    if (token!="FROM") {
    result.table_name="-1";
    return result;
    }
    getline(ss, token, ' ');

    // Получаем имя таблицы
    result.table_name = token;
   if(is_service_word(token)) {
        result.table_name="-1";
        return result;
    }
    // Проверяем наличие условия WHERE
    getline(ss, token, ' ');
    if(token!="WHERE")
    {
        result.table_name="-1";
        return result;
    }
    result.condition=token;
    getline(ss,token,';');
    result.condition+=" "+token;

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
    string token, cur;

    // Пропускаем ключевое слово SELECT
    getline(ss, token, ' ');
    if (token!="SELECT"){
        result.table_name="-1";
        return result;
    }
    // Получаем список колонок
    getline(ss, token, ' ');
    stringstream cur_ss(token);

    while(getline(cur_ss,token,',')){
        if (is_service_word(token)) {
            result.table_name="-1";
            return result;
        };
        result.columns->push_back(token);
    };
    


    // Пропускаем ключевое слово FROM
    getline(ss, token, ' ');
    if (token!="FROM"){
        result.table_name="-1";
        return result;
    }

    // Получаем имя таблицы
    getline(ss,token,' ');
    result.tablesName = new LinkedList();
    stringstream cur_ss1(token);
    //cur_ss.clear();
  //  cur_ss<<(token);
    while(getline(cur_ss1,token,',')) {
        if (is_service_word(token)) {
        result.table_name="-1";
        return result;
        };

        result.tablesName->push_back(token);
     }

    // Проверяем наличие условия WHERE
    getline(ss, token, ' ');
    if (token == "WHERE") {
        getline(ss, result.condition, ';'); // Извлекаем условие WHERE
    }
    return result;
}