#include "main.h"
#include "jsonparse.cpp"
#include "actions.h"


void setConfig(Schema& schema) ;
int main()
{
    Schema schema;
    DBMS dmbs;
    setConfig(schema);
    while (true) {
    string query;
    cout << "Введите SQL-запрос: ";
    getline(cin, query);

    SQLQuery parsed_query;
    if (query.find("INSERT") != string::npos) {
        parsed_query = parse_insert_query(query);
       dmbs.insert_into_csv(schema,parsed_query.table_name,parsed_query);
    } else if (query.find("DELETE") != string::npos) {
        parsed_query = parse_delete_query(query);
        dmbs.delete_from_csv(schema,parsed_query);
    } else if (query.find("SELECT") != string::npos) {


        parsed_query = parse_select_query(query);

    } else {
        cout << "Некорректный запрос." << endl;
        return 1;
    }



    }




    return 0;
}
//DELETE FROM таблица1 WHERE колонка2 = '12345' AND колонка1 = 'somedata'
//INSERT INTO таблица1 VALUES ('somedata','12345','dsfsdg','dfgdfg')
//DELETE FROM таблица1 WHERE колонка2 = '12345'
//SELECT колонка1, колонка1 FROM таблица1, таблица2
//DELETE FROM WHERE колонка2 = '12345'


//Седживк, Корман ?, 