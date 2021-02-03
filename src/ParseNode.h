//
// Created by Nathaniel Rupprecht on 2/2/21.
//

#ifndef MANTACLION_PARSENODE_H
#define MANTACLION_PARSENODE_H

#include <utility>

#include "parser-classes.hpp"

namespace Manta {

    struct ParseNode : public std::enable_shared_from_this<ParseNode> {
        explicit ParseNode(string d) : designator(std::move(d)) {};

        ParseNode(string d, std::shared_ptr<ParseNode> p)
                : designator(std::move(d)), parent(std::move(p)) {};

        ParseNode(const ParseNode &node) {
            *this = node;
        }

        ParseNode &operator=(const ParseNode &node) {
            designator = node.designator;
            parent = node.parent;
            for (const auto &child : node.children) {
                auto new_child = std::make_shared<ParseNode>("");
                *new_child = *child;
                new_child->parent = shared_from_this();
                children.push_back(new_child);
            }
            return *this;
        }

        inline void add(const string &str) {
            children.push_back(std::make_shared<ParseNode>(str, shared_from_this()));
        }

        inline void add(const std::shared_ptr<ParseNode> &node) {
            node->parent = shared_from_this();
            children.push_back(node);
        }

        friend ostream &operator<<(ostream &out, const ParseNode &node) {
            // Make sure we dont print actual newlines or things like that.
            string alias = node.designator;
            if (alias == "\n") alias = "\\n";
            else if (alias == "\t") alias = "\\t";

            // Print out node.
            out << "[ " << alias;
            int size = node.children.size();
            if (0 < size) {
                out << ": {";
                for (int i = 0; i < size; ++i) {
                    out << *node.children[i];
                    if (i != 0) {
                        out << ", ";
                    }
                }
                out << "}";
            }
            out << " ]";
            // Return the stream.
            return out;
        }

        std::string printTerminals() {
            // If this is a terminal.
            if (children.empty()) {
                string alias = designator;
                if (alias == "\n") alias = "\\n";
                else if (alias == "\t") alias = "\\t";
                return alias + " ";
            }
            // If this is a production.
            string str;
            //for (auto it = children.rbegin(); it!=children.rend(); ++it)
            for (auto &it : children) {
                str += it->printTerminals();
            }
            // Return.
            return str;
        }

        string printTree(int level = 0) {
            string str;
            string alias = designator;
            if (alias == "\n") alias = "\\n";
            else if (alias == "\t") alias = "\\t";
            str += repeat('|', level) + alias;
            if (!children.empty()) {
                str += '\n';
                //for (int i=children.size()-1; 0<=i; --i) {
                for (int i = 0; i < children.size(); ++i) {
                    str += children[i]->printTree(level + 1);
                    if (i != children.size() - 1) str += "\n";
                }
            }
            // Return the string.
            return str;
        }

        //! Node label.
        std::string designator;

        std::shared_ptr<ParseNode> parent = nullptr;
        vector<std::shared_ptr<ParseNode>> children;
    };

}
#endif //MANTACLION_PARSENODE_H
