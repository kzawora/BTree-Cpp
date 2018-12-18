#include "headers/BTree.h"
#include "headers/Record.h"
#include "headers/Storage.h"
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>

int shell_cmd(std::string cmd) {
    if (cmd == "") {
        std::cout << "You need to type a command." << std::endl;
        return 0;
    }
    std::istringstream iss(cmd);
    std::vector<std::string> words(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>());
    if (words[0] == "help") {
        std::cout << "help is on the way" << std::endl;
    } else if (words[0] == "add") {
        if (words.size() < 3) {
            std::cout << "[ERROR] add requires index and list of numbers." << std::endl;
            return 0;
        }
        auto index = std::stoi(words[1]);
        std::vector<double> values;
        for (int i = 2; i < words.size(); i++) {
            values.push_back(std::stod(words[i]));
        }
        Record r(index, values);
        std::cout << "Adding " << r << "at index " << index << "." << std::endl;
    } else if (words[0] == "delete") {
        if (words.size() < 2) {
            std::cout << "[ERROR] random requires a additional integer argument." << std::endl;
            return 0;
        }
        auto count = std::stoi(words[1]);
        std::cout << "Deleting record with index " << count << "." << std::endl;
    } else if (words[0] == "random") {
        if (words.size() < 2) {
            std::cout << "[ERROR] random requires a additional integer argument." << std::endl;
            return 0;
        }
        auto count = std::stoi(words[1]);
        std::cout << "Randomly generating " << count << " records." << std::endl;
    } else if (words[0] == "dump") {
        std::cout << "dumping..." << std::endl;
    } else if (words[0] == "show") {
        std::cout << "show..." << std::endl;
    } else if (words[0] == "exit") {
        std::cout << "exiting..." << std::endl;
        return -1;
    } else {
        std::cout << "Unrecognized command: " << words[0] << std::endl;
    }
    return 0;
}

/*
void storageTest() {
    std::string input;
    Storage * st = new Storage("file2.bin", 16);
    bytearray* data = new bytearray(16);

    //	st->clear();

    for (int i = 0; i < 30; i++)
        st->setPage(i, data);

    data->arr[3] = 'a';
    data->arr[2] = 'p';
    data->arr[1] = 'u';
    data->arr[0] = 'd';

    st->setPage(1, data);

    st->setPage(29, data);

    data->arr[0] = 'z';
    st->setPage(3, data);

    bytearray* read = st->getPage(1);
    std::cout << read->arr << std::endl;
    delete read;
    read = st->getPage(3);
    std::cout << read->arr;
    delete read;

    st->setPage(0, data);

    delete st;
    delete data;
}
*/
void btreetest() {
    std::shared_ptr<BTreeStorage> storage = std::make_shared<BTreeStorage>("file3");
    storage->clear();
    std::shared_ptr<BTreeNS::BTreeNode> node = std::make_shared<BTreeNS::BTreeNode>();
    node->parent = 1;
    std::shared_ptr<BTreeNS::BTreeNodeCell> cell = std::make_shared<BTreeNS::BTreeNodeCell>(2, 3, 4);
    node->insert(cell);
    node->insert(cell);
    storage->set(0, node);
    storage->set(1, node);
    storage->set(2, node);
    std::shared_ptr<BTreeNS::BTreeNode> node2 = storage->get(1);
}

void datatest() {
    std::shared_ptr<DataStorage> storage = std::make_shared<DataStorage>("file");
    storage->clear();
    std::vector<double> data = {1, 2, 3, 4, 5, 6};
    std::shared_ptr<Record> rec = std::make_shared<Record>(1, data);
    /*    storage->set(0, 0, rec);
        storage->set(0, RECORD_SIZE, rec);
        storage->set(0, 7 * RECORD_SIZE, rec);
        storage->set(1, 1 * RECORD_SIZE, rec);
        storage->set(2, 1 * RECORD_SIZE, rec);
            */
    auto rec1 = Record::generate(), rec2 = Record::generate(), rec3 = Record::generate();
    for (int i = 0; i < 9; i++) {
        storage->insert(Record::generate());
        storage->flush();
    }
    auto val1 = storage->get(0, 0);

    storage->flush();
    std::shared_ptr<Record> node2 = storage->get(0, RECORD_SIZE);
}

int main() {
    /*	for (int i = 0; i < 10; i++) {
        Record r = Record::generate();
        r.print();
    }*/
    /*

        */
    btreetest();
    datatest();
    /*
    while (true) {
        std::cout << "> ";
        getline(std::cin, input);
        int status = shell_cmd(input);
        if (status == -1) break;
    }
    */
    return 0;
}