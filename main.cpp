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
        std::cout << "delete (n) \t- deletes node n" << std::endl;
        std::cout << "flush \t- flushes the cache to disk" << std::endl;
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
        } else
            r = Record::generate();
        std::cout << "Adding " << *r << "at index " << index << "." << std::endl;
        db->set(index, r);
    } else if (words[0] == "delete") {
        if (words.size() < 2) {
            std::cout << "[ERROR] random requires a additional integer argument." << std::endl;
            return 0;
        }
        auto key = std::stoi(words[1]);
        std::cout << "Deleting record with index " << key << "." << std::endl;
        db->del(key);
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

        std::clock_t start;
        double duration;
        start = std::clock();

        int next_key = 0;
        std::vector<int> indices;
        while (count > 0) {
            auto ptr = db->get(next_key);
            if (ptr == nullptr) {
                auto rec = Record::generate();
//                rec->print();
                db->set(next_key, rec);
                count--;
                indices.push_back(next_key);
            }
            next_key++;
        }
        duration = (std::clock() - start) / (double) CLOCKS_PER_SEC;

        std::cout << "Added " << indices.size() << " elements. {";
        for (auto index : indices) {
            std::cout << index;
            if (index == indices.back())
                std::cout << "}" << std::endl;
            else
                std::cout << " ";
        }
        std::cout << "Random took: " << duration << " s." << std::endl;
    } else if (words[0] == "data") {
        std::cout << "printing data..." << std::endl;
        db->flush();
        db->printData();
    } else if (words[0] == "show") {
        std::cout << "generating dot file for use with GraphViz..." << std::endl;
        db->flush();
        if (words.size() < 2) {
            graphViz(db);
        } else
            graphViz(db, std::stoi(words[1]));
    } else if (words[0] == "exit") {
        db->flush();
        std::cout << "exiting..." << std::endl;
        return -1;

    } else if (words[0] == "clear") {
        db->flush();
        db->createTree();
    } else if (words[0] == "meta") {
        std::stringstream ss;
        ss << "Elements: " << db->metadata[0] << " "\
        "/ Nodes: " << db->metadata[1] << " "\
        "/ Height: " << db->metadata[2] << " "\
        "/ Root: " << db->metadata[3] << " "\
        "/ Reads: " << Storage::reads << " "\
        "/ Writes: " << Storage::writes << " ";

        std::cout << ss.str() << std::endl;
    } else if (words[0] == "superrandom") {
        if (words.size() < 2) {
            std::cout << "[ERROR] random requires a additional integer argument." << std::endl;
            return 0;
        }
        auto count = std::stoi(words[1]);
        std::cout << "Randomly generating " << count << " records." << std::endl;
        std::clock_t start;
        double duration;
        start = std::clock();
        std::vector<int> indices;
        while (count > 0) {
            int next_key = rand() % 99999 + 0;
            auto ptr = db->get(next_key);
            if (ptr == nullptr) {
                auto rec = Record::generate();
                //   rec->print();
                db->set(next_key, rec);
                count--;
                indices.push_back(next_key);
            }
        }
        duration = (std::clock() - start) / (double) CLOCKS_PER_SEC;
        std::cout << "Added " << indices.size() << " elements. {";
        for (auto index : indices) {
            std::cout << index;
            if (index == indices.back())
                std::cout << "}" << std::endl;
            else
                std::cout << " ";
        }
        std::cout << "Superrandom took: " << duration << " s." << std::endl;
    } else {
        std::cout << "Unrecognized command: " << words[0] << std::endl;
    }
    return 0;
}

int main() {
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