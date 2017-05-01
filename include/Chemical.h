//
// Created by tgifr on 01.05.17.
//

#ifndef HTTPSERVERLAB_CHEMICAL_H
#define HTTPSERVERLAB_CHEMICAL_H

#include <iostream>
#include <sstream>
#include <vector>
#include <iterator>

#include "Chemical.h"
using namespace std;
/**
 * @class - describes a chemical
 */
class Chemical{
public:
    string name;
    double weight;
    string influence;
    /**
     *
     * @brief - simple constructor
     */
    Chemical(string , double, string);
};


#endif //HTTPSERVERLAB_CHEMICAL_H
