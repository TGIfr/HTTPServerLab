//
// Created by tgifr on 01.05.17.
//

#include "../include/processor.h"
#include <time.h>
#include <jansson.h>
#include <cstring>

const string filepath = "data/text.txt";


vector<std::string> getTokens(std::string str){
    istringstream iss(str);
    vector<std::string> tokens{istream_iterator<std::string>{iss},
                               istream_iterator<std::string>{}};
    return tokens;
}
int analyzeTokens(vector<std::string> tokens){
    int size = tokens.size();
    if(size > 1){
        std::string first = tokens.at(0);
        std::string second = tokens.at(1);
        if(first.compare("GET")) return UNKNOWN;
        if(!second.compare("/file") ) return FILE_INF;
        if(!second.compare("/") ) return ROOT;
        if(!second.compare("/file/data")) return FILE_DATA;
        if(!second.compare("/favorites")) return FAVORITES;

        std::size_t found = second.find("/favorites?");
        if (found!=std::string::npos) return FAVORITES_BY_VALUE;

        found = second.find("/favorites/");
        if (found!=std::string::npos) return FAVORITES_BY_ID;


    }
    return UNKNOWN;
}
string makeResponse(vector<std::string> tokens, int analyzingStatus, vector<Chemical> chemicals) {

    switch(analyzingStatus){
        case UNKNOWN:
            return  make404Response();
        case ROOT:
            return makeRootResponse();
        case FAVORITES:
            return makeFavoritesResponse(chemicals);
        case FAVORITES_BY_ID:
            return makeFavoritesResponseById(chemicals, tokens);
        case FAVORITES_BY_VALUE:
            return makeFavoritesResponseByValue(chemicals, tokens);
        case FILE_DATA:
            return makeResponseFileData();
        case FILE_INF:
            return  makeResponseFileInf();

    }
}

string makeResponseFileInf(){
    string fileDataResponse = "HTTP/1.1 200 OK\n";
    fileDataResponse += "Connection: Closed\n"
            "Content-Type: text/html; charset=iso-8859-1\r\n\r\n";

    string data = readFileContents(filepath.c_str());
    json_t * json = json_object();
    json_object_set_new(json, "name", json_string( "text.txt"));
    json_object_set_new(json, "size", json_integer(getFileSize(filepath.c_str())));
    json_object_set_new(json, "content", json_string(data.c_str()));

    char * jsonString = json_dumps(json, JSON_INDENT(2));
    fileDataResponse += jsonString;
    return fileDataResponse;
}
string makeResponseFileData(){
    string fileDataResponse = "HTTP/1.1 200 OK\n";
    fileDataResponse += "Connection: Closed\n"
            "Content-Type: text/html; charset=iso-8859-1\r\n\r\n";

    string data = readFileContents(filepath.c_str());
    auto tokens = getTokens(data);
    json_t * json = json_object();
    json_t * arr = json_array();
    for (auto str : tokens) // access by reference to avoid copying
    {
        json_t * wordJ = json_object();
        json_object_set_new(wordJ,  "word", json_string(str.c_str()));
        json_array_append(arr, wordJ);
    }

    json_object_set_new(json, "words", arr);
    char * jsonString = json_dumps(json, JSON_INDENT(2));
    fileDataResponse += jsonString;
    return fileDataResponse;
}
char * readFileContents(const char * fileName) {
    FILE * textFile = fopen(fileName, "r");
    if (textFile == NULL) {
        return NULL;
    }
    fseek(textFile, 0, SEEK_END);
    long size = ftell(textFile);
    fseek(textFile, 0, SEEK_SET);
    char * fcontent = (char * ) malloc(size);
    fread(fcontent, 1, size, textFile);
    fclose(textFile);
    return fcontent;
}
int getFileSize(const char * fileName) {
    FILE * textFile = fopen(fileName, "r");
    if (textFile == NULL) {
        return 0;
    }
    fseek(textFile, 0, SEEK_END);
    return ftell(textFile);

}
string makeFavoritesResponseByValue(vector<Chemical> chemicals, vector<std::string> tokens){
    string favoritesResponse = "HTTP/1.1 200 OK\n"
            "Connection: Closed\r\n\r\n";

    auto str = tokens.at(1);
    size_t last_index = str.find("?");
    string result = str.substr(last_index + 1);

    cout << "SUBSTR\n\n\n"<< result.substr(0,10);
    if(result.substr(0,9).compare("influence"))
        return  make404Response();

    last_index = str.find("=");
    result = str.substr(last_index + 1);


    json_t * json = json_object();
    json_t * arr = json_array();
    for (int i = 0; i < chemicals.size(); i++) {
        Chemical  chem = chemicals.at(i);

        if(chem.influence.compare(result)) continue;

        json_t * chemJ = json_object();
        json_object_set_new(chemJ, "name", json_string(chem.name.c_str()));
        json_object_set_new(chemJ, "weight", json_real(chem.weight));
        json_object_set_new(chemJ, "influence", json_string(chem.influence.c_str()));
        json_array_append(arr, chemJ);
    }
    json_object_set_new(json, "chemicals", arr);


    char * jsonString = json_dumps(json, JSON_INDENT(2));

    favoritesResponse += jsonString;


    return favoritesResponse;
}

string makeFavoritesResponseById(vector<Chemical> chemicals, vector<std::string> tokens){
    string favoritesResponse = "HTTP/1.1 200 OK\n"
            "Connection: Closed\r\n\r\n";

    auto str = tokens.at(1);
    size_t last_index = str.find_last_not_of("0123456789");
    string result = str.substr(last_index + 1);
    int index = stoi(result);
    if(index < 0 || index >= chemicals.size()){
        return make404Response();
    }

    json_t * json = json_object();
    json_t * arr = json_array();

        Chemical  chem = chemicals.at(index);
        json_t * chemJ = json_object();
        json_object_set_new(chemJ, "name", json_string(chem.name.c_str()));
        json_object_set_new(chemJ, "weight", json_real(chem.weight));
        json_object_set_new(chemJ, "influence", json_string(chem.influence.c_str()));
        json_array_append(arr, chemJ);

    json_object_set_new(json, "chemicals", arr);


    char * jsonString = json_dumps(json, JSON_INDENT(2));

    favoritesResponse += jsonString;


    return favoritesResponse;
}
string makeFavoritesResponse(vector<Chemical> chemicals){
    string favoritesResponse = "HTTP/1.1 200 OK\n"
                            "Connection: Closed\r\n\r\n";
    json_t * json = json_object();
    json_t * arr = json_array();
    for (int i = 0; i < chemicals.size(); i++) {
        Chemical  chem = chemicals.at(i);
        json_t * chemJ = json_object();
        json_object_set_new(chemJ, "name", json_string(chem.name.c_str()));
        json_object_set_new(chemJ, "weight", json_real(chem.weight));
        json_object_set_new(chemJ, "influence", json_string(chem.influence.c_str()));
        json_array_append(arr, chemJ);
    }
    json_object_set_new(json, "chemicals", arr);


    char * jsonString = json_dumps(json, JSON_INDENT(2));

    favoritesResponse += jsonString;


    return favoritesResponse;
}
string makeRootResponse(){
    string rootResponse = "HTTP/1.1 200 OK\n";
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    rootResponse += "Connection: Closed\n"
            "Content-Type: text/html; charset=iso-8859-1\r\n\r\n";


    json_t * json = json_object();
    json_object_set_new(json, "title", json_string( "RuntimeJunkie 3"));
    json_object_set_new(json, "developer", json_string("TGIfr"));
    json_object_set_new(json, "time", json_string(asctime(timeinfo)));

    char * jsonString = json_dumps(json, JSON_INDENT(2));

    rootResponse += jsonString;


    return rootResponse;
}
string make404Response(){
    string response404 = "HTTP/1.1 404 Not Found\n";
//            "Date: ";
//    time_t rawtime;
//    struct tm * timeinfo;
//    time ( &rawtime );
//    timeinfo = localtime ( &rawtime );
//    response404 += asctime(timeinfo);
//    response404 += "Server: Apache/2.2.14 (Win32)\n"
//            "Content-Length: 230\n"
//            "Connection: Closed\n"
//            "Content-Type: text/html; charset=iso-8859-1\r\n\r\n"
//            "<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\n"
//            "<html>\n"
//            "<head>\n"
//            "   <title>404 Not Found</title>\n"
//            "</head>\n"
//            "<body>\n"
//            "   <h1>Not Found</h1>\n"
//            "   <p>The requested URL /t.html was not found on this server.</p>\n"
//            "</body>\n"
//            "</html>";
    return response404;
}