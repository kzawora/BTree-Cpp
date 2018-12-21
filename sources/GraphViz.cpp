//
// Created by thewildhealer on 21.12.2018.
//

#include "../headers/GraphViz.h"
#include <iostream>
#include <sstream>
#include <queue>
#include "../headers/BTree.h"
#include "../headers/Storage.h"

std::string getTreeLabel(std::shared_ptr<BTreeNS::BTreeNode> node) {
    int rowspan = (node->size() * 2) + 1;
    std::stringstream ss;
    ss << "<TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\">";
    if (node->leaf)
        ss << "<TR><TD COLSPAN=\"" << rowspan << "\"><B>Leaf Node " << node->index << "</B></TD></TR><TR>";
    else
        ss << "<TR><TD COLSPAN=\"" << rowspan << "\"><B>Node " << node->index << "</B></TD></TR><TR>";

    for (auto cell : node->cells) {
        if (cell->child != MAX_SIZE) {
            ss << "<TD PORT=\"c" << cell->child << "\"><SUB><u>" << cell->child << "</u></SUB></TD>";
        } else
            ss << "<TD>•</TD>";
        if (cell->getKey() != MAX_SIZE) {
            ss << "<TD><B>" << cell->getKey() << "</B></TD>";
        }
    }
    ss << "</TR></TABLE>";
    return ss.str();
}

std::string createDot(std::shared_ptr<BTreeNS::BTree> tree, std::string label, int start) {
    std::stringstream ss;
    ss << "digraph btree {graph [ranksep=1];node [shape=plaintext];label=\"" << label << "\";";
    std::queue<int> nodes_to_check;
    nodes_to_check.push(start);
    while (!nodes_to_check.empty()) {
        int index = nodes_to_check.front();
        nodes_to_check.pop();
        auto node = tree->btree->get(index);
        ss << "n" << node->index << "[label=<" << getTreeLabel(node) << ">];";

        if (node->parent != MAX_SIZE)
            ss << "n" << node->parent << ":c" << node->index << "-> n" << node->index << ";";
        for (auto cell : node->cells)
            if (cell->child != MAX_SIZE)
                nodes_to_check.push(cell->child);

    }
    ss << "}";
    return ss.str();
}

void graphViz(std::shared_ptr<BTreeNS::BTree> tree) {
    std::stringstream label;
    label << "Elements: " << tree->metadata[0] << " "\
                "/ Nodes: " << tree->metadata[1] << " "\
                "/ Height: " << tree->metadata[2] << " "\
                "/ Root: " << tree->metadata[3] << " ";
    auto str = createDot(tree, label.str(), tree->metadata[3]);
    std::ofstream file("gv.dot");
    file << str;
//    system("\"C:\\Program Files (x86)\\Graphviz2.38\\bin\\dot.exe\" gv.dot -Tpng -o file.png | file.png");
//    system("file.png");
    system("gv.bat");

}
