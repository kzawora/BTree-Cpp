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
    ss << "<table border=\"0\" cellborder=\"1\" cellspacing=\"0\">";
    if (node->leaf)
        ss << "<tr><td colspan=\"" << rowspan << "\"><b>Leaf Node " << node->index << "</b></td></tr><tr>";
    else
        ss << "<tr><td colspan=\"" << rowspan << "\"><b>Node " << node->index << "</b></td></tr><tr>";

    for (auto cell : node->cells) {
        if (cell->child != MAX_SIZE) {
            ss << "<td port=\"c" << cell->child << "\"><sub><u>" << cell->child << "</u></sub></td>";
        } else
            ss << "<td>•</td>";
        if (cell->getKey() != MAX_SIZE) {
            ss << "<td><b>" << cell->getKey() << "</b></td>";
        }
    }
    ss << "</tr></table>";
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

void graphViz(std::shared_ptr<BTreeNS::BTree> tree, int root) {
    std::stringstream label;
    if (root == -1) root = tree->metadata[3];
    label << "Elements: " << tree->metadata[0] << " "\
                "/ Nodes: " << tree->metadata[1] << " "\
                "/ Height: " << tree->metadata[2] << " "\
                "/ Root: " << tree->metadata[3] << " ";
    auto str = createDot(tree, label.str(), root);
    std::ofstream file("gv.dot");
    file << str;
//    system("\"C:\\Program Files (x86)\\Graphviz2.38\\bin\\dot.exe\" gv.dot -Tpng -o file.png | file.png");
//    system("file.png");
    system("gv.bat");

}
