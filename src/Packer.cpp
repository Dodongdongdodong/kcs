#include <stdio.h>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <map>
#include <sstream>

#include "Packer.h"
#include "Painter.h"

namespace myPacker
{

void
Macro::move(int dx, int dy)
{
  lx_ += dx;
  ly_ += dy;

  for(auto& pin : pins_)
  {
    pin->setCx(pin->cx() + dx);
    pin->setCy(pin->cy() + dy);
  }
}

void
Macro::setLx(int newLx)
{
  int dx = newLx - lx_;
  lx_ = newLx;
  for(auto& pin : pins_)
    pin->setCx(pin->cx() + dx);
}

void
Macro::setLy(int newLy)
{
  int dy = newLy - ly_;
  ly_ = newLy;
  for(auto& pin : pins_)
    pin->setCy(pin->cy() + dy);
}

Packer::Packer()
  : painter_  (nullptr),
    coreLx_   (      0),
    coreLy_   (      0),
    coreUx_   (      0),
    coreUy_   (      0),
    totalWL_  (      0)
{}

void 
Packer::readFile(const std::filesystem::path& file)
{
  //std::cout << "Read " << file.string() << "..." << std::endl;
  
  std::string filename = file.string();
  size_t dotPos = filename.find_last_of('.');
  std::string suffix = filename.substr(dotPos + 1); 

  if(suffix != "macros")
  {
    std::cerr << "This file is not .macros file..." << std::endl;
    exit(1);
  }

  std::ifstream ifs;
  ifs.open(file);

  std::string line;
  std::string tempStr1;
  std::string tempStr2;

  int numMacro = 0;
  int numNet = 0;
  int numPin = 0;
  int lx, ly, w, h = 0;

  // READ DIE (CORE)
  std::getline(ifs, line);
  assert(line == "DIE");

  std::getline(ifs, line);
  std::stringstream dieInfo(line);

  dieInfo >> coreLx_;
  dieInfo >> coreLy_;
  dieInfo >> coreUx_;
  dieInfo >> coreUy_;
  
  // READ MACROS
  std::getline(ifs, line);
  std::stringstream macroInfo(line);

  macroInfo >> tempStr1;
  macroInfo >> tempStr2;

  assert(tempStr1 == "MACROS");
  numMacro = std::stoi(tempStr2);

  macroInsts_.reserve(numMacro);
  macroPtrs_.reserve(numMacro);

  std::map<std::string, Pin*> pinTable;

  while(1)
  {
    std::getline(ifs, line);
    //std::cout << "Line1: " << line << std::endl;

    if(line.empty()) 
      continue;

    if(line == "END MACROS")
      break;

    std::stringstream ss1(line);
    std::string macroName;
    ss1 >> macroName;

    //std::cout << "Macro Name: " << macroName << std::endl;
      
    std::getline(ifs, line);
    std::stringstream ss2(line);
    ss2 >> lx;
    ss2 >> ly;
    ss2 >> w;
    ss2 >> h;

    macroInsts_.push_back( Macro(macroName, lx, ly, w, h) );
    Macro* macroPtr = &(macroInsts_.back());
    macroPtrs_.push_back( macroPtr );

    std::getline(ifs, line);
    assert(line == "PINS");

    while(1)
    {
      std::getline(ifs, line);
      //std::cout << "Line2: " << line << std::endl;

      if(line.empty()) 
        continue;

      if(line == "END PINS")
        break;

      std::stringstream ss3(line);

      std::string pinName;
      int pinCx, pinCy = 0;
      ss3 >> pinName;
      ss3 >> pinCx;
      ss3 >> pinCy;

      //std::cout << "Pin Name : " << pinName << " Cx : " << pinCx << " Cy : " << pinCy << std::endl; 

      pinInsts_.push_back( Pin(pinName, numPin++, pinCx, pinCy, macroPtr) );
    }
  }

  for(auto& pin : pinInsts_)
  {
    Pin* pinPtr = &pin;
    pinPtrs_.push_back( pinPtr );
    pinPtr->macro()->addPin( pinPtr );
    pinTable[ pinPtr->name() ] = pinPtr;
    //std::cout << "Pin Name : " << pinPtr->name() << " MacroName from Pin   : " << pinPtr->macro()->name() << std::endl;
  }

  // READ NETS

  std::getline(ifs, line);
  std::stringstream netInfo(line);

  netInfo >> tempStr1;
  netInfo >> tempStr2;

  assert(tempStr1 == "NETS");
  numNet = std::stoi(tempStr2);

  netInsts_.reserve(numNet);
  netPtrs_.reserve(numNet);

  int numNetRead = 0;
  while(1)
  {
    std::getline(ifs, line);

    if(line.empty()) 
      continue;

    if(line == "END NETS")
      break;

    std::stringstream pinInfo(line);

    std::string pinName1, pinName2;

    pinInfo >> pinName1;
    pinInfo >> pinName2;

    Pin* pinPtr1;
    Pin* pinPtr2;

    if( pinTable.find(pinName1) != pinTable.end() )
      pinPtr1 = pinTable[pinName1];
    else
    {
      std::cout << pinName1 << " does not exist in the pinTable.\n";
      exit(0);
    }

    if( pinTable.find(pinName2) != pinTable.end() )
      pinPtr2 = pinTable[pinName2];
    else
    {
      std::cout << pinName2 << " does not exist in the pinTable.\n";
      exit(0);
    }

    netInsts_.push_back( Net(numNetRead++, pinPtr1, pinPtr2) );
    Net* netPtr = &(netInsts_.back());
    netPtrs_.push_back( netPtr );
    pinPtr1->setNet( netPtr );
    pinPtr2->setNet( netPtr );
  }

  updateWL();

  printf("=====================================\n");
  printf("DB Info\n");
  printf("-------------------------------------\n");
  printf("NumMacro: %zu\n", macroPtrs_.size());
  printf("NumNet:   %zu\n",   netPtrs_.size());
  printf("NumPin:   %zu\n",   pinPtrs_.size());
  printf("Core (%d, %d) - (%d, %d)\n", coreLx_, coreLy_, coreUx_, coreUy_);
  printf("Initial HPWL: %lld\n", totalWL_);
  printf("=====================================\n");
}

void
Packer::updateWL()
{
  totalWL_ = 0;
  for(auto& net : netPtrs_)
  {
    net->updateWL();
    totalWL_ += static_cast<int64_t>( net->wl() );
  }
}

void
Packer::naivePacking()
{
  //std::sort( macroPtrs_.begin(), macroPtrs_.end(), sortByHeight() );
  std::sort( macroPtrs_.begin(), macroPtrs_.end(), [](Macro* a, Macro* b){ return a->h() > b->h(); } );

  int xPos = coreLx_;
  int yPos = coreLy_;
  int largestHeightThisRow = 0;

  for(auto& macro : macroPtrs_)
  {
    printf("w: %d", macro->w());
    if( ( xPos - coreLx_ + macro->w() ) > coreUx_)
    {
      yPos += largestHeightThisRow;
      xPos = coreLx_;
      largestHeightThisRow = 0;
    }

    if( ( yPos - coreLy_ + macro->h() ) > coreUy_)
    {
      printf("Naive packing failed\n");
      break;
    }

    macro->setLx(xPos);
    macro->setLy(yPos);

    xPos += macro->w();

    if( macro->h() > largestHeightThisRow )
      largestHeightThisRow = macro->h();

    macro->setPacked();
  }

  updateWL();
  printf("WL: %lld\n", totalWL_);
}

void 
Packer::categoriacedPacking(){
  std::vector<Macro*> smallMacros;
  std::vector<Macro*> mediumMacros;
  std::vector<Macro*> largeMacros;
  smallMacroNum = 0;
  mediumMacroNum = 0;
  largeMacroNum = 0;
  smallMacroTree.treeclear();
  mediumMacroTree.treeclear();
  largeMacroTree.treeclear();

  for (auto& macro :macroPtrs_){
    if ((macro->h() + macro->w())  < 200000){
      smallMacros.push_back(macro);
      smallMacroNum += 1;
      //printf("small \n");
    }
    else if((macro->h() + macro->w())  < 400000){
      mediumMacros.push_back(macro);
      mediumMacroNum += 1;
      //printf("medium \n");
    }
    else{
      largeMacros.push_back(macro);
      largeMacroNum += 1;
      //printf("large \n");
    }
  }

  packsmallMacros(smallMacros);
  packmediumMacros(mediumMacros);
  packlargeMacros(largeMacros);

}

void Packer::simulatedAnnealing() {
  smallTreeSimulatedAnnealing(smallMacroTree);
  mediumTreeSimulatedAnnealing(mediumMacroTree);
  largeTreeSimulatedAnnealing(largeMacroTree);
}

void
Packer::packsmallMacros(std::vector<Macro*>& macros){
  int xPos = coreLx_;
  int yPos = coreLy_;
  int smallMacroNum_ = smallMacroNum;
  int largestHeightThisRow = 0;

  for(auto& macro : macros)
  {
    if(( ( xPos - coreLx_ + macro->w() ) > coreUx_))
    {
      yPos += macro->h();
      xPos = coreLx_;
      largestHeightThisRow = 0;
    }
    else if (!(isSpaceFree(xPos, yPos, macro->w(), macro->h(), placedMacros_))){
        xPos += macro->w();
        yPos = coreLy_;
    }
    /*else if (smallMacroNum_ <= (smallMacroNum/2)){
        xPos = coreLx_;
        yPos += macro->h();
        smallMacroNum_ = smallMacroNum;
    }*/

    if( ( yPos - coreLy_ + macro->h() ) > coreUy_)
    {
      printf("Naive packing failed\n");
      break;
    }

    macro->setLx(xPos);
    macro->setLy(yPos);
    placedMacros_.push_back(macro);
    smallMacroTree.insertSmallMacros(new Macro (*macro));
    smallMacroNum_ -= 1;

    xPos += macro->w();

    if( macro->h() > largestHeightThisRow )
      largestHeightThisRow = macro->h();

    macro->setPacked();
  }

  updateWL();
  printf("WL: %lld\n", totalWL_);
  smallMacroTree.printTree();
}

void
Packer::packmediumMacros(std::vector<Macro*>& macros){
  int xPos = coreLx_;
  int yPos = coreUy_;
  int mediumMacroNum_ = mediumMacroNum;
  int largestHeightThisRow = 0;

  for(auto& macro : macros) {
      if((yPos - macro->h() < coreLy_)) {
          xPos += macro->w();
          yPos = coreUy_;
          largestHeightThisRow = 0;
      }
      else if (!(isSpaceFree(xPos, yPos - macro->h(), macro->w(), macro->h(), placedMacros_))){
          xPos += macro->w();
          yPos = coreUy_;
      }
      else if (mediumMacroNum_ <= (mediumMacroNum/2)){
          xPos += macro->w();
          yPos = coreUy_;
          mediumMacroNum_ = mediumMacroNum;
      }

      if(xPos > coreUx_) { 
          printf("Packing failed for macro %s\n", macro->name().c_str());
          break;
      }

      macro->setLx(xPos);
      macro->setLy(yPos - macro->h());
      placedMacros_.push_back(macro);
      mediumMacroTree.insertMediumMacros(new Macro (*macro));
      mediumMacroNum_ -= 1;

      yPos -= macro->h();

      if(macro->h() > largestHeightThisRow)
          largestHeightThisRow = macro->h();

      macro->setPacked();
  }

  updateWL();
  printf("WL: %lld\n", totalWL_);
  mediumMacroTree.printTree();

}

void
Packer::packlargeMacros(std::vector<Macro*>& macros){
    int xPos = coreUx_;
    int yPos = coreUy_;
    int largeMacroNum_ = largeMacroNum;
    int largestHeightThisRow = 0;

    for(auto& macro : macros) {
        if((xPos - macro->w() < coreLx_)){
            yPos -= macro->h();
            xPos = coreUx_;
            largestHeightThisRow = 0;
        }
        
        else if (!(isSpaceFree(xPos - macro->w(), yPos - macro->h(), macro->w(), macro->h(), placedMacros_))){
            yPos -= macro->h();
            xPos = coreUx_;
        }

        else if (largeMacroNum_ <= (largeMacroNum/2)){
            yPos -= macro->h();
            xPos = coreUx_;
            largeMacroNum_ = largeMacroNum;
        }        
         

        /*
        if((yPos - macro->h() < coreLy_)) {
            xPos -= macro->w();
            yPos = coreUy_;
            largestHeightThisRow = 0;
        }

        if(yPos - macro->h() < coreLy_) { 
            printf("Packing failed for largemacro %s\n", macro->name().c_str());
            break;
        }
        */

        

        macro->setLx(xPos - macro->w());
        macro->setLy(yPos - macro->h());
        placedMacros_.push_back(macro);
        largeMacroTree.insertLargeMacros(new Macro (*macro));
        largeMacroNum -= 1;
        
        xPos -= macro->w();

        if(macro->h() > largestHeightThisRow)
            largestHeightThisRow = macro->h();

        macro->setPacked();
    }

  updateWL();
  printf("WL: %lld\n", totalWL_);
  largeMacroTree.printTree();
}

bool Packer::isSpaceFree(int x, int y, int w, int h, std::vector<Macro*>& placedMacros_) {
    for (auto& placedMacro : placedMacros_) {
        if ( (x >= placedMacro->lx()) && (x < placedMacro->lx() + placedMacro->w()) && (y >= placedMacro->ly()) && (y < placedMacro->ly() + placedMacro->h()) ){
          return false;
        }
        else if ( (x + w > placedMacro->lx()) && (x + w <= placedMacro->lx() + placedMacro->w()) && (y >= placedMacro->ly()) && (y < placedMacro->ly() + placedMacro->h()) ){
          return false;
        }
        else if ( (x >= placedMacro->lx()) && (x < placedMacro->lx() + placedMacro->w()) && (y + h > placedMacro->ly()) && (y + h <= placedMacro->ly() + placedMacro->h()) ){
          return false;
        }
        else if ( (x + w > placedMacro->lx()) && (x + w <= placedMacro->lx() + placedMacro->w()) && (y + h > placedMacro->ly()) && (y + h <= placedMacro->ly() + placedMacro->h()) ){
          return false;
        }

        else if ( (x > placedMacro->lx()) && (x < placedMacro->lx() + placedMacro->w()) && (y < placedMacro->ly()) && (y > placedMacro->ly() + placedMacro->h()) ){
          return false;
        }
        else if ( (x + w > placedMacro->lx()) && (x + w < placedMacro->lx() + placedMacro->w()) && (y < placedMacro->ly()) && (y > placedMacro->ly() + placedMacro->h()) ){
          return false;
        }
        else if ( (x < placedMacro->lx()) && (x > placedMacro->lx() + placedMacro->w()) && (y > placedMacro->ly()) && (y < placedMacro->ly() + placedMacro->h()) ){
          return false;
        }
        else if ( (x < placedMacro->lx()) && (x > placedMacro->lx() + placedMacro->w()) && (y + h > placedMacro->ly()) && (y + h < placedMacro->ly() + placedMacro->h()) ){
          return false;
        }
      
        /*
        if ( x + w > placedMacro->lx() && x + w < placedMacro->lx() + placedMacro->w()){
          return false;
        } 
        else if ( x > placedMacro->lx() && x < placedMacro->lx() + placedMacro->w() ){
          return false;
        } 
        else if ( y + h > placedMacro->ly() && y + h < placedMacro->ly() + placedMacro->h() ){
          return false;
        } 
        else if ( y > placedMacro->ly() && y < placedMacro->ly() + placedMacro->h() ){
          return false; // 겹치는 경우
        }
        */
    }
    return true; // 겹치지 않는 경우
}

void Packer::smallTreeSimulatedAnnealing(MacroBinaryTree& tree) {
  double temperature = 10000;
  double coolingRate = 0.9;
  double minTemperature = 100;
  int64_t WL1, WL2;

  std::srand(static_cast<unsigned>(std::time(0)));

  while (temperature > minTemperature) {
    auto nodes = tree.randomNode();
    MacroNode* node1 = nodes.first;
    MacroNode* node2 = nodes.second;

    if (node1 == node2) {
      continue;
    }

    WL1 = totalWL_;

    tree.swapNodes(node1, node2);
    tree.smallTree2Macro();
    updateWL();

    WL2 = totalWL_;

    if (WL2 < WL1) {
      temperature *= coolingRate;
    }
    else {
      double p = exp((WL2 - WL1) / temperature);
      if (p > static_cast<double>(std::rand()) / RAND_MAX) {
      tree.swapNodes(node1, node2);
      tree.smallTree2Macro();
      updateWL();
      temperature *= coolingRate;
      }
      else {
        temperature *= coolingRate;
      }

    printf("temperature: %f ", temperature);
    printf("WL: %lld\n", totalWL_);
    }

  }
}

void Packer::mediumTreeSimulatedAnnealing(MacroBinaryTree& tree) {
  double temperature = 10000;
  double coolingRate = 0.9;
  double minTemperature = 1;
  int64_t WL1, WL2;

  std::srand(static_cast<unsigned>(std::time(0)));

  while (temperature > minTemperature) {
    auto nodes = tree.randomNode();
    MacroNode* node1 = nodes.first;
    MacroNode* node2 = nodes.second;

    if (node1 == node2) {
      continue;
    }

    WL1 = totalWL_;

    tree.swapNodes(node1, node2);
    tree.mediumTree2Macro();
    updateWL();

    WL2 = totalWL_;

    
    if (WL2 < WL1) {
      temperature *= coolingRate;
    }
    else {
      double p = exp((WL2 - WL1) / temperature);
      if (p > static_cast<double>(std::rand()) / RAND_MAX) {
      tree.swapNodes(node1, node2);
      tree.mediumTree2Macro();
      updateWL();
      temperature *= coolingRate;
      }
      else {
        temperature *= coolingRate;
      }

    printf("temperature: %f ", temperature);
    printf("WL: %lld\n", totalWL_);
    }
  }
}

void Packer::largeTreeSimulatedAnnealing(MacroBinaryTree& tree) {
  double temperature = 10000;
  double coolingRate = 0.9;
  double minTemperature = 100;
  int64_t WL1, WL2;

  std::srand(static_cast<unsigned>(std::time(0)));

  while (temperature > minTemperature) {
    auto nodes = tree.randomNode();
    MacroNode* node1 = nodes.first;
    MacroNode* node2 = nodes.second;

    if (node1 == node2) {
      continue;
    }

    WL1 = totalWL_;

    tree.swapNodes(node1, node2);
    tree.largeTree2Macro();
    updateWL();

    WL2 = totalWL_;

    if (WL2 < WL1) {
      temperature *= coolingRate;
    }
    else {
      double p = exp((WL2 - WL1) / temperature);
      if (p > static_cast<double>(std::rand()) / RAND_MAX) {
      tree.swapNodes(node1, node2);
      tree.mediumTree2Macro();
      updateWL();
      temperature *= coolingRate;
      }
      else {
        temperature *= coolingRate;
      }

    printf("temperature: %f ", temperature);
    printf("WL: %lld\n", totalWL_);
    }

  }
}

MacroBinaryTree::MacroBinaryTree() : root(nullptr) {}

MacroBinaryTree::~MacroBinaryTree() {
  deleteNode(root);
}

void MacroBinaryTree::insertSmallMacros(Macro* macro){
    if (!root){
      root = new MacroNode(macro);
    }
    else{
      insertSmallRecursive(root, root, macro);
    }
}

void MacroBinaryTree::insertSmallRecursive(MacroNode*& node, MacroNode* parent, Macro* macro) {

    if (node == nullptr) {
        node = new MacroNode(macro, parent);
    }

    if (macro->ly() > node->macro->ly()) {
      if (!node->left){
        node->left = new MacroNode(macro, node);
      }
      else{
        insertSmallRecursive(node->left, node, macro);
      }
        
    }
    else{
      if (!node->right){
        node->right = new MacroNode(macro, node);
      }
      else{
        insertSmallRecursive(node->right, node, macro);
      }
    } 

}

void MacroBinaryTree::insertMediumMacros(Macro* macro){
    if (!root){
      root = new MacroNode(macro);
    }
    else{
      insertMediumRecursive(root, root, macro);
    }
}

void MacroBinaryTree::insertMediumRecursive(MacroNode*& node, MacroNode* parent, Macro* macro) {

    if (node == nullptr) {
        node = new MacroNode(macro, parent);
    }

    if (macro->lx() > node->macro->lx()) {
      if (!node->left){
        node->left = new MacroNode(macro, node);
      }
      else{
        insertMediumRecursive(node->left, node, macro);
      }
        
    }
    else{
      if (!node->right){
        node->right = new MacroNode(macro, node);
      }
      else{
        insertMediumRecursive(node->right, node, macro);
      }
    } 

}


void MacroBinaryTree::insertLargeMacros(Macro* macro){
    if (!root){
      root = new MacroNode(macro);
    }
    else{
      insertLargeRecursive(root, root, macro);
    }
}

void MacroBinaryTree::insertLargeRecursive(MacroNode*& node, MacroNode* parent, Macro* macro) {

    if (node == nullptr) {
        node = new MacroNode(macro, parent);
    }

    if (macro->ly() < node->macro->ly()) {
      if (!node->left){
        node->left = new MacroNode(macro, node);
      }
      else{
        insertLargeRecursive(node->left, node,  macro);
      }
        
    }
    else{
      if (!node->right){
        node->right = new MacroNode(macro, node);
      }
      else{
        insertLargeRecursive(node->right, node, macro);
      }
    } 

}


void MacroBinaryTree::printTree() {
    printTreeRecursive(root, 0, "");
}

void MacroBinaryTree::printTreeRecursive(MacroNode* node, int depth, const std::string& path) {
  if (node){
    std::cout << std::string(depth * 4, ' ') << node->macro->lx() << ' ' << node->macro->ly() << " (Path: " << path << ")" << std::endl;
    printTreeRecursive(node->left, depth + 1, path + "L");
    printTreeRecursive(node->right, depth + 1, path + "R");
  }
}

void MacroBinaryTree::smallTree2Macro() {
  if (root != nullptr) {
    smallTree2MacroRecursive(root);
  }
}

void MacroBinaryTree::smallTree2MacroRecursive(MacroNode* node) {
  if (!node){
    return;
  }

  if (node == root) {
    node->macro->setLx(20140);
    node->macro->setLy(22400);
  }

  if (node->left != nullptr) {
    node->left->macro->setLx(node->macro->lx());
    node->left->macro->setLy(node->macro->ly() + node->macro->h());
  }

  if (node->right != nullptr) {
    node->right->macro->setLx(node->macro->lx() + node->macro->w());
    node->right->macro->setLy(node->macro->ly());
  }
  
  smallTree2MacroRecursive(node->left);
  smallTree2MacroRecursive(node->right);
}

void MacroBinaryTree::mediumTree2Macro() {
  if (root != nullptr) {
    mediumTree2MacroRecursive(root);
  }
}

void MacroBinaryTree::mediumTree2MacroRecursive(MacroNode* node) {
  if (!node){
    return;
  }

  if (node == root) {
    node->macro->setLx(20140);
    node->macro->setLy(1979600 - node->macro->h());
  }

  if (node->left != nullptr) {
    node->left->macro->setLx(node->macro->lx() + node->macro->w());
    node->left->macro->setLy(node->macro->ly());
  }

  if (node->right != nullptr) {
    node->right->macro->setLx(node->macro->lx());
    node->right->macro->setLy(node->macro->ly() - node->macro->h());
  }
  
  mediumTree2MacroRecursive(node->left);
  mediumTree2MacroRecursive(node->right);
}

void MacroBinaryTree::largeTree2Macro() {
  if (root != nullptr) {
    largeTree2MacroRecursive(root);
  }
}

void MacroBinaryTree::largeTree2MacroRecursive(MacroNode* node) {
  if (!node){
    return;
  }

  if (node == root) {
    node->macro->setLx(2179680 - node->macro->w());
    node->macro->setLy(1979600 - node->macro->h());
  }

  if (node->left != nullptr) {
    node->left->macro->setLx(node->macro->lx());
    node->left->macro->setLy(node->macro->ly() - node->macro->h());
  }

  if (node->right != nullptr) {
    node->right->macro->setLx(node->macro->lx() - node->macro->w());
    node->right->macro->setLy(node->macro->ly());
  }
  
  largeTree2MacroRecursive(node->left);
  largeTree2MacroRecursive(node->right);
}

bool MacroBinaryTree::deleteMacro(Macro* macro){
  return deleteRecursive(root, macro);
}

bool MacroBinaryTree::deleteRecursive(MacroNode*& node, Macro* macro){
    if (!node) {
      return false;  // 노드를 찾지 못한 경우
    }

    if (macro->lx() == node->macro->lx() && macro->ly() == node->macro->ly()) {
      // 노드를 찾은 경우: 삭제 로직
      if (!node->left && !node->right) {
        delete node;
        node = nullptr;
      }
      else if (node->left == nullptr) {
        MacroNode* temp = node;
        node = node->right;
        node->parent = temp->parent;
        delete temp;
      }
      else if (node->right == nullptr) {
        MacroNode* temp = node;
        node = node->left;
        node->parent = temp->parent;
        delete temp;
      } 
      else {
          MacroNode* temp = findMinNode(node->right);
          node->macro = temp->macro;
          deleteRecursive(node->right, temp->macro);
      }
      return true;
    } 
    else if (macro->lx() < node->macro->lx()) {
        return deleteRecursive(node->left, macro);
    } 
    else {
        return deleteRecursive(node->right, macro);
    }
}

MacroNode* MacroBinaryTree::findMinNode(MacroNode* node){
    while (node && node->left) {
        node = node->left;
    }
    return node;
}

void MacroBinaryTree::deleteNode(MacroNode* node) {
  if (node){
    deleteNode(node->left);
    deleteNode(node->right);
    delete node;
  }
}

void MacroBinaryTree::treeclear() {
  deleteNode(root);
  root = nullptr;
}


void MacroBinaryTree::swapNodes(MacroNode* node1, MacroNode* node2){
  if (node1 == nullptr || node2 == nullptr){
    return;
  }

  Macro* temp = node1->macro;
  node1->macro = node2->macro;
  node2->macro = temp;

}

void MacroBinaryTree::moveNode(MacroNode* node, MacroNode* afterparent) {
  if (node == nullptr || afterparent == nullptr || node == afterparent || node->parent == afterparent) {
    return;
  }

  if (node->parent == afterparent) {
    if (node->parent->left == node) {
      node->parent->left = nullptr;
    }
    else {
      node->parent->right = nullptr;
    }
  }

  if (afterparent->parent && afterparent->parent->left == afterparent) {
    if (!afterparent->parent->right) {
      afterparent->parent->right = node;
    }
    else {
      MacroNode* node4 = afterparent->parent;

      int rightChildCountLeft = 0;
      int rightChildCountRight = 0;

      MacroNode* currentNode = node4->left;
      while (currentNode && currentNode->right) {
        rightChildCountLeft++;
        currentNode = currentNode->right;
      }

      currentNode = node4->right;
      while (currentNode && currentNode->right) {
        rightChildCountRight++;
        currentNode = currentNode->right;
      }

      if (rightChildCountLeft < rightChildCountRight) {
        std::srand(std::time(nullptr));
        if (std::rand() % 2 == 0){
          afterparent->left = node;
        }
        else {
          afterparent->right = node;
        }
      }
      else {
        afterparent->left = node;
      }
    }
  }

  else if (afterparent->parent && afterparent->parent->right == afterparent) {
    MacroNode* node4 = afterparent;

    while (node4->parent && (!node4->parent->left || !node->parent->right)) {
      node4 = node4->parent;
    }

    int rightChildCountLeft = 0;
    int rightChildCountRight = 0;

    MacroNode* currentNode = node4->left;
    while (currentNode && currentNode->right) {
      rightChildCountLeft++;
      currentNode = currentNode->right;
    }

    currentNode = node4->right;
    while (currentNode && currentNode->right) {
      rightChildCountRight++;
      currentNode = currentNode->right;
    }

    if (rightChildCountLeft < rightChildCountRight) {
      afterparent->right = node;
      node->parent = afterparent;
    }
    else {
      return;
    }

  }

}

void MacroBinaryTree::getAllNodes(MacroNode* node, std::vector<MacroNode*>& nodes) {
  int n  = 0;
  if (node) {
    nodes.push_back(node);
    getAllNodes(node->left, nodes);
    getAllNodes(node->right, nodes);
    n++;
  }
}

std::pair<MacroNode*, MacroNode*> MacroBinaryTree::randomNode() {
  std::vector<MacroNode*> nodes;
  getAllNodes(root, nodes);
  int n = nodes.size();

  if (n < 2) {
    return {nullptr, nullptr};
  }

  int idx1 = std::rand() % n;
  int idx2 = idx1;
  
  while(idx2 == idx1) {
    idx2 = std::rand() % n;
  }
  
  return {nodes[idx1], nodes[idx2]};
}


int
Packer::show(int& argc, char* argv[])
{
  QApplication app(argc, argv);
  QSize size = app.screens()[0]->size();
  painter_ = std::make_unique<Painter>(size, Qt::darkGray, coreUx_, coreUy_, coreLx_, coreLy_, totalWL_);
  painter_->setQRect( macroPtrs_ );
  //painter_->setNetlist( netPtrs_ );
  painter_->show();
  return app.exec();
}

}
