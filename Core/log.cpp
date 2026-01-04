#include"log.h"
#include<string>
#include<iostream>
  //classe pour la gestion des messages et des erruers en console
using namespace std;
void Log::message(const string& message){
    cout<< "message :" <<message <<endl;
}
void Log::erreur(const std::string& erreur){
    cout<<"erreur de l'application :"<< erreur <<endl;
}