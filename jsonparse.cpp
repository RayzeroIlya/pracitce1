#include "main.h"
#include <fstream>
#include <sys/stat.h>

struct Schema {
    string name;
    int tuples_limit;
};

bool createDir(string name);
void setConfig() {

    ifstream file("schema.json");
    json j;
    file >>j;

    Schema schema;
    for ( const auto& schema_json:j){
    schema.name = schema_json["name"].get<string>();
    createDir(schema.name);
    schema.tuples_limit=schema_json["tuples_limit"].get<int>();
    for (const auto& [table_name, columns] : schema_json["structure"].items()){
        createDir(schema.name+"/"+table_name);
        ifstream fin(schema.name+"/"+table_name+"/"+"1.csv");
        if (fin.good()){
            cout << "1.csv already exist!" << endl;
        }
        else {
       ofstream fout;
       fout.open (schema.name+"/"+table_name+"/"+"1.csv"); 
        fout << "\""+table_name+"_" <<"pk\"" <<",";
       int i=0;
    
        for( const auto& column : columns) {
            fout << column;
            if (i<columns.size()-1) fout << ",";
            i++;
        }
        fout << endl;
        fout.close();
        cout << "Created "+schema.name+"/"+table_name+"/"+table_name+"1.csv"<<endl;
        }
        fin.close();
        fin.open(schema.name+"/"+table_name+"/"+table_name+"_pk_sequence");
        if (fin.good()) {
            cout << schema.name+"/"+table_name+"/"+table_name+"_pk_sequence is already exist!" << endl;
        } else{
            ofstream fout;
            fout.open(schema.name+"/"+table_name+"/"+table_name+"_pk_sequence");
            fout << "\"0\"" << endl;
            fout.close();
            cout <<  "Created "+schema.name+"/"+table_name+"/"+table_name+"_pk_sequence"<< endl;
        }
        fin.close();
        fin.open(schema.name+"/"+table_name+"/"+table_name+"_lock");
        if (fin.good()){
            cout << schema.name+"/"+table_name+"/"+table_name+"_lock is already exist!"<< endl;
        }else{
            ofstream fout;
            fout.open(schema.name+"/"+table_name+"/"+table_name+"_lock");
            fout << "\"0\""<<endl;
            fout.close();
            cout << "Created "+schema.name+"/"+table_name+"/"+table_name+"_lock" <<endl;
        }
        fin.close();
    
    }
}
}   

bool createDir (string name) {
 if (mkdir(name.c_str(), 0755) == -1) {
    cout << "Error creating directory: Directory exists " << endl;
    return 0;
  } else {
    cout << "Directory " << name << " created successfully!" << endl;
  return 1;
  }

}