//
// Created by tgifr on 01.05.17.
//

#ifndef HTTPSERVERLAB_PROCESSOR_H
#define HTTPSERVERLAB_PROCESSOR_H

#include <iostream>
#include <sstream>
#include <vector>
#include <iterator>

#include "Chemical.h"
using namespace std;
/**
 * @brief enum of responses from analyzeTokens
 */
enum {
    OK,
    UNKNOWN,
    WRONG_INDEX,
    ROOT,
    FAVORITES,
    FILE_INF,
    FILE_DATA,
    FAVORITES_BY_ID,
    FAVORITES_BY_VALUE
};

/**
 *
 * @param str - string to divide
 * @return vector of separated words
 */
vector<std::string> getTokens(std::string str);
/**
 *
 * @param tokens - vector of separated words
 * @return int representing command
 */
int analyzeTokens(vector<std::string> tokens);

/**
 *
 * @param chemicals vector of chemicals to be turned into response
 * @return response on /favorites
 */
string makeFavoritesResponse(vector<Chemical> chemicals);
/**
 * @brief decides what response to do
 * @param tokens - words of http-get
 * @param analyzingStatus - value returned by analyzeTokens
 * @param chemicals - vector of chemicals
 * @return final response
 */
string makeResponse(vector<std::string> tokens, int analyzingStatus, vector<Chemical> chemicals);

/**
 *
 * @param chemicals vector of chemicals to be turned into response
 * @return response on /favorites/{id}
 */
string makeFavoritesResponseById(vector<Chemical> chemicals, vector<std::string> tokens);
/**
 *
 * @param chemicals vector of chemicals to be turned into response
 * @return response on /favorites?influence={value}
 */
string makeFavoritesResponseByValue(vector<Chemical> chemicals, vector<std::string> tokens);

/**
 *
 * @param fileName
 * @return all data from file in str
 */
char * readFileContents(const char * fileName);
/**
 *
 * @param fileName
 * @return size of file
 */
int getFileSize(const char * fileName);

/**
 *
 * @return response to /file
 */
string makeResponseFileInf();
/**
 *
 * @return response to /file/data
 */
string makeResponseFileData();

/**
 *
 * @return 404 statused empty response
 */
string make404Response();

/**
 *
 * @return response to /
 */
string makeRootResponse();
#endif //HTTPSERVERLAB_PROCESSOR_H
