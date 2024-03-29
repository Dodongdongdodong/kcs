#include <iostream>

#include "Packer.h"

using namespace myPacker;

int main(int argc, char **argv)
{
  if(argc < 2)
  {
    std::cout << "No input file. Please give .macros file...\n";
    exit(0);
  }

  Packer pack;

  std::filesystem::path txtfile = argv[1];

  pack.readFile(txtfile);
  pack.show(argc, argv);
  
  //////////////////////////////////////////////////
  // Make your own macro placement algorithm!!!
  //pack.naivePacking();
  pack.categoriacedPacking();
  pack.show(argc, argv);
  pack.simulatedAnnealing();
  //////////////////////////////////////////////////
  
  pack.show(argc, argv);

  return 0;
}
