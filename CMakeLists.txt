cmake_minimum_required(VERSION 3.12)
project(sbd2)

set(CMAKE_CXX_STANDARD 14)

add_executable(sbd2
        .clang-format
        sources/BTree/BTree.cpp
        main.cpp
        sources/Record.cpp
        sources/Storage/Storage.cpp
        headers/CommonHeader.h headers/BTree.h headers/Record.h headers/Storage.h sources/BTree/BTreeNode.cpp sources/BTree/BTreeNodeCell.cpp sources/Storage/DataStorage.cpp sources/Storage/BTreeStorage.cpp sources/GraphViz.cpp headers/GraphViz.h)
