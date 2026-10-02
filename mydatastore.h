#ifndef MYDATASTORE_H
#define MYDATASTORE_H

#include "datastore.h"
#include <map>
#include <queue>
#include <set>
#include <vector>
#include <string>

class MyDataStore : public DataStore {
public:
  MyDataStore();
  virtual ~MyDataStore();
  void addProduct(Product* p);
  void addUser(User* u);

  std::vector<Product*> search(std::vector<std::string>& terms, int type);

  void dump(std::ostream& ofile);

  bool addToCart(std::string username, Product* p);
  std::vector<Product*> viewCart(std::string username);
  bool buyCart(std::string username);    
  bool userExists(std::string username);
  
private:
  std::vector<Product*> products_;
  std::map<std::string, User*> users_;
  std::map<std::string, std::set<Product*> > keywordMap_;
  std::map<std::string, std::queue<Product*> > carts_;
};

#endif