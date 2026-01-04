#pragma once
#include<string.h>
#include<iostream>
class Log{
    
    public:
    Log(){};
    void message(const std::string& message);
    void erreur(const std::string& erreur);

};