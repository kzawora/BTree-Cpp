#include "headers/BTree.h"
#include "headers/Record.h"
#include "headers/Storage.h"
#include "headers/GraphViz.h"
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <ctime>

int shell_cmd(std::string cmd, std::shared_ptr<BTreeNS::BTree> db) {
    if (cmd == "") {
        std::cout << "You need to type a command." << std::endl;
        return 0;
    }
    std::istringstream iss(cmd);
    std::vector<std::string> words(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>());
    if (words[0] == "help") {
        std::cout << "help\t- displays help" << std::endl;
        std::cout << "meta\t- shows btree metadata" << std::endl;
        std::cout << "delete\t- " << std::endl;
        std::cout << "random (n)\t- adds n random records (ascending indices)" << std::endl;
        std::cout << "superrandom (n)\t- adds n random records (random indices)" << std::endl;
        std::cout << "data\t- dumps every record in btree" << std::endl;
        std::cout << "get (n)\t- gets record with key n" << std::endl;
        std::cout << "set (n) (opt: values)\t- sets record with key n with random data or specified values"
                  << std::endl;
        std::cout << "show\t- visualizes btree" << std::endl;
        std::cout << "exit\t- exits" << std::endl;


    } else if (words[0] == "set") {
        if (words.size() < 2) {
            std::cout << "[ERROR] set requires index." << std::endl;
            return 0;
        }
        std::shared_ptr<Record> r;
        auto index = std::stoi(words[1]);
        if (words.size() > 2) {
            std::vector<double> values;
            for (int i = 2; i < words.size(); i++) {
                values.push_back(std::stod(words[i]));
            }
            r = std::make_shared<Record>(index, values);
        }
        r = Record::generate();
        std::cout << "Adding " << *r << "at index " << index << "." << std::endl;
        db->set(index, r);
    } else if (words[0] == "delete") {
        if (words.size() < 2) {
            std::cout << "[ERROR] random requires a additional integer argument." << std::endl;
            return 0;
        }
        auto count = std::stoi(words[1]);
        std::cout << "Deleting record with index " << count << "." << std::endl;
    } else if (words[0] == "get") {
        if (words.size() < 2) {
            std::cout << "[ERROR] get requires a additional integer argument." << std::endl;
            return 0;
        }
        auto key = std::stoi(words[1]);
        auto rec = db->get(key);
        if (rec == nullptr) {
            std::cout << "[ERROR] key " << key << " does not exist." << std::endl;
            return 0;
        }
        rec->print();
    } else if (words[0] == "flush") {
        std::cout << "flushing..." << std::endl;
        db->flush();
    } else if (words[0] == "random") {
        if (words.size() < 2) {
            std::cout << "[ERROR] random requires a additional integer argument." << std::endl;
            return 0;
        }
        auto count = std::stoi(words[1]);
        std::cout << "Randomly generating " << count << " records." << std::endl;
        int next_key = 0;
        std::vector<int> indices;
        while (count > 0) {
            auto ptr = db->get(next_key);
            if (ptr == nullptr) {
                auto rec = Record::generate();
                rec->print();
                db->set(next_key, rec);
                count--;
                indices.push_back(next_key);
            }
            next_key++;
        }
        std::cout << "Added " << indices.size() << " elements. {";
        for (auto index : indices) {
            std::cout << index;
            if (index == indices.back())
                std::cout << "}" << std::endl;
            else
                std::cout << " ";

        }
    } else if (words[0] == "data") {
        std::cout << "dumping data..." << std::endl;
        db->flush();
        db->printData();
    } else if (words[0] == "show") {
        std::cout << "show..." << std::endl;
        db->flush();
        graphViz(db);
    } else if (words[0] == "exit") {
        std::cout << "exiting..." << std::endl;
        return -1;
    } else if (words[0] == "clear") {
        db->createTree();
        return 0;
    } else if (words[0] == "superrandom") {
        if (words.size() < 2) {
            std::cout << "[ERROR] random requires a additional integer argument." << std::endl;
            return 0;
        }
        auto count = std::stoi(words[1]);
        std::cout << "Randomly generating " << count << " records." << std::endl;
        std::vector<int> indices;
        while (count > 0) {
            int next_key = rand() % 99999 + 0;
            auto ptr = db->get(next_key);
            if (ptr == nullptr) {
                auto rec = Record::generate();
                rec->print();
                db->set(next_key, rec);
                count--;
                indices.push_back(next_key);
            }
        }
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
//    btreetest();
//    datatest();
    srand(time(0));
    auto db = std::make_shared<BTreeNS::BTree>("database");
    while (true) {
        std::cout << "> ";
        std::string input;
        getline(std::cin, input);
        int status = shell_cmd(input, db);
        if (status == -1) break;
    }
    return 0;
}