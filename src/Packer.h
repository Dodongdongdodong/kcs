#pragma once

#include <filesystem>
#include <fstream>
#include <memory>
#include <vector>

#include "Painter.h"


namespace myPacker
{

class Painter;

class Pin;
class Net;
class Macro;

class Pin
{
  public:

    Pin(std::string& name, int id, int cx, int cy, Macro* macro)
      : name_    (   name),
        id_      (     id), 
        cx_      (     cx), 
        cy_      (     cy), 
        net_     (nullptr), 
        macro_   (  macro)
    {}

    // Getters
    std::string name() const { return name_; }
    int           id() const { return id_;    }    
    int           cx() const { return cx_;    }
    int           cy() const { return cy_;    }
    Net*         net()       { return net_;   }
    Macro*     macro()       { return macro_; }

    // Setters
    void setNet(Net* net) { net_ = net; }
    void setCx(int newCx) { cx_ = newCx; }
    void setCy(int newCy) { cy_ = newCy; }

    void setMacro(Macro* newMacro) { macro_ = newMacro; }

  private:

    std::string name_;
    int           id_;
    int           cx_;
    int           cy_;
    Net*         net_;
    Macro*     macro_;
};

class Net
{
  public:

    Net(int id, Pin* pin1, Pin* pin2)
      : id_     (  id),
        pin1_   (pin1),
        pin2_   (pin2),
        wl_     (   0)
    {}

    int id() const { return   id_; }
    int wl() const { return   wl_; } 
    // NOTE: wl() has to be called after updateWL()

          Pin* pin1()       { return pin1_; }
          Pin* pin2()       { return pin2_; }
    const Pin* pin1() const { return pin1_; }
    const Pin* pin2() const { return pin2_; }

    void updateWL()
    {
      wl_ = std::abs(pin1_->cx() - pin2_->cx()) 
          + std::abs(pin1_->cy() - pin2_->cy());
    }

  private:

    int    id_;
    int    wl_;
    Pin* pin1_;
    Pin* pin2_;
};

class Macro
{
  public:

    Macro(std::string& name, int lx, int ly, int w, int h)
      : lx_       (lx   ),
        ly_       (ly   ),
        w_        (w    ),
        h_        (h    ),
        name_     (name ),
        isPacked_ (false)
    {}

    // Getters
    int           lx() const { return lx_;       }
    int           ly() const { return ly_;       }
    int            w() const { return w_;        }
    int            h() const { return h_;        }
    bool    isPacked() const { return isPacked_; }
    std::string name() const { return name_;     }

    // Setters
    // NOTE: If you want to change the coordinates of macro location, 
    //       then you also have to change the coordinates of its pins.
    //       This will be done by setLx(), setLy(), move() automatically.
    void setLx(int x);
    void setLy(int y);
    void setPacked()      { isPacked_ = true;     }  
    void addPin(Pin* pin) { pins_.push_back(pin); }
    void move(int dx, int dy); // x -> x + dx, y -> y + dy

    friend void swapMacros(std::shared_ptr<Macro> macro1, std::shared_ptr<Macro> macro2);

  private:

    std::string name_;

    int  lx_;
    int  ly_;
    int  w_;
    int  h_;
    bool isPacked_;

    std::vector<Pin*> pins_;
};


struct MacroNode{
    Macro* macro;
    MacroNode* left;
    MacroNode* right;
    MacroNode* parent;

    MacroNode(Macro* macro, MacroNode* parent = nullptr) : macro(macro), left(nullptr), right(nullptr), parent(parent) {}
};

class MacroBinaryTree{
public:
    MacroBinaryTree();
    ~MacroBinaryTree();

    void insertSmallMacros(Macro* macro);
    void insertMediumMacros(Macro* macro);
    void insertLargeMacros(Macro* macro);
    void printTree();
    bool deleteMacro(Macro* macro);
    void deleteNode(MacroNode* node);

    void smallTree2Macro();
    void mediumTree2Macro();
    void largeTree2Macro();
    void treeclear();

    void swapNodes(MacroNode* node1, MacroNode* node2);
    void moveNode(MacroNode* node, MacroNode* afterparent);
    
    
    std::pair<MacroNode*, MacroNode*> randomNode();

private:
    MacroNode* root;
    
    void insertSmallRecursive(MacroNode*& node, MacroNode* parent, Macro* macro);
    void insertMediumRecursive(MacroNode*& node, MacroNode* parent, Macro* macro);
    void insertLargeRecursive(MacroNode*& node, MacroNode* parent, Macro* macro);
    void printTreeRecursive(MacroNode* node, int depth, const std::string& path);
    bool deleteRecursive(MacroNode*& node, Macro* macro);

    MacroNode* findMinNode(MacroNode* node);

    void smallTree2MacroRecursive(MacroNode* node);
    void mediumTree2MacroRecursive(MacroNode* node);
    void largeTree2MacroRecursive(MacroNode* node);
    void getAllNodes(MacroNode* node, std:: vector<MacroNode*>& nodes);


};

class Packer
{
  public:
    
    // Constructor
    Packer();

    // APIs
    void readFile(const std::filesystem::path& file);
    void naivePacking();

    void categoriacedPacking();
    void packsmallMacros(std::vector<Macro*>& macros);
    void packmediumMacros(std::vector<Macro*>& macros);
    void packlargeMacros(std::vector<Macro*>& macros);
    bool isSpaceFree(int x, int y, int w, int h, std::vector<Macro*>& placedMacros_);
    void smallTreeSimulatedAnnealing(MacroBinaryTree& tree);
    void mediumTreeSimulatedAnnealing(MacroBinaryTree& tree);
    void largeTreeSimulatedAnnealing(MacroBinaryTree& tree);

    int show(int& argc, char* argv[]);

    // Getters
    std::vector<Macro*>& macros() { return macroPtrs_; }
    std::vector<Net*>&     nets() { return   netPtrs_; }
    std::vector<Pin*>&     pins() { return   pinPtrs_; }

    int coreLx()  const { return coreLx_;  }
    int coreLy()  const { return coreLy_;  }
    int coreUx()  const { return coreUx_;  }
    int coreUy()  const { return coreUy_;  }
    int64_t totalWL() const { return totalWL_; }

  private:

    int coreLx_;
    int coreLy_;
    int coreUx_;
    int coreUy_;

    int64_t totalWL_;

    void updateWL();

    std::vector<Pin>   pinInsts_;
    std::vector<Pin*>  pinPtrs_;

    std::vector<Net>   netInsts_;
    std::vector<Net*>  netPtrs_;

    std::vector<Macro>  macroInsts_;
    std::vector<Macro*> macroPtrs_;

    std::unique_ptr<Painter> painter_;

    std::vector<Macro*> placedMacros_;
    int smallMacroNum;
    int mediumMacroNum;
    int largeMacroNum;
    MacroBinaryTree smallMacroTree;
    MacroBinaryTree mediumMacroTree;
    MacroBinaryTree largeMacroTree;
};

}
