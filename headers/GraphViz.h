//
// Created by thewildhealer on 21.12.2018.
//

#ifndef SBD2_GRAPHVIZ_H
#define SBD2_GRAPHVIZ_H
//
// Created by thewildhealer on 21.12.2018.
//

#include "../headers/GraphViz.h"
#include <string>
#include <memory>
namespace BTreeNS {
    class BTree;

    class BTreeNode;
}

std::string getTreeLabel(std::shared_ptr <BTreeNS::BTreeNode> node);

std::string createDot(std::shared_ptr <BTreeNS::BTree> tree, std::string label, int start);

void graphViz(std::shared_ptr <BTreeNS::BTree> tree, int root = -1);

#endif //SBD2_GRAPHVIZ_H
