#include "mydatastore.h"
#include "util.h"

using namespace std;

MyDataStore::MyDataStore() {

}

MyDataStore::~MyDataStore(){
  for(vector<Product*>::iterator it=products_.begin(); it!=products_.end(); ++it){
    delete *it;
  }
  for(map<string, User*>::iterator it=users_.begin(); it!=users_.end(); ++it){
    delete it->second;
  }
}

void MyDataStore::addProduct(Product* p){
  products_.push_back(p);
  set<string> words = p->keywords();
  for(set<string>::iterator it = words.begin(); it != words.end(); ++it)
    keywordMap_[*it].insert(p);
}

void MyDataStore::addUser(User* u){
  users_[convToLower(u->getName())] = u;
}

vector<Product*> MyDataStore::search(vector<string>& terms, int type){
  vector<Product*> results;
  if(terms.size() == 0){
    return results;
  }

  set<Product*> found;
  if(type == 0){
    string term = convToLower(terms[0]);
    if(keywordMap_.find(term) == keywordMap_.end()){
      return results;
    }
    found = keywordMap_[term];

    for(unsigned int i = 1; i < terms.size(); i++){
      term = convToLower(terms[i]);
      if(keywordMap_.find(term) == keywordMap_.end()){
        found.clear();
        break;
      }
      found = setIntersection(found, keywordMap_[term]);
    }
  }
  else {
    for(unsigned int i = 0; i < terms.size(); i++){
      string term = convToLower(terms[i]);
      if(keywordMap_.find(term) != keywordMap_.end()){
        found = setUnion(found, keywordMap_[term]);
      }
    }
  }

  for(set<Product*>::iterator it = found.begin(); it != found.end(); ++it){
    results.push_back(*it);
  }
  return results;
}

bool MyDataStore::userExists(string username){
  return users_.find(convToLower(username)) != users_.end();
}

bool MyDataStore::addToCart(string username, Product* p){
  username = convToLower(username);
  if(!userExists(username)){
    return false;
  }
  carts_[username].push(p);
  return true;
}

vector<Product*> MyDataStore::viewCart(string username){
  vector<Product*> result;
  username = convToLower(username);

  if(!userExists(username)){
    return result;
  }

  queue<Product*> temp = carts_[username];
  while(!temp.empty()){
    result.push_back(temp.front());
    temp.pop();
  }
  return result;
}

bool MyDataStore::buyCart(string username){
  username = convToLower(username);
  if(!userExists(username)){
    return false;
  }

  User* user = users_[username];
  queue<Product*> remaining;

  while(!carts_[username].empty()){
    Product* p = carts_[username].front();
    carts_[username].pop();

    if(p->getQty() > 0 && user->getBalance() >= p->getPrice()){
      p->subtractQty(1);
      user->deductAmount(p->getPrice());
    }
    else {
      remaining.push(p);
    }
  }
  carts_[username] = remaining;
  return true;
}

void MyDataStore::dump(ostream& ofile){
  ofile << "<products>" << endl;

  for(vector<Product*>::iterator it = products_.begin(); it != products_.end(); ++it){
    (*it)->dump(ofile);
  }
  ofile << "</products>" << endl;
  ofile << "<users>" << endl;
  for(map<string, User*>::iterator it = users_.begin(); it != users_.end(); ++it){
    it->second->dump(ofile);
  }
  ofile << "</users>" << endl;
}