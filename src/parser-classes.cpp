#include "parser-classes.hpp"
// Other files
#include "LALR-Manta.hpp"

namespace Manta {

    Item::Item(int p) : production(p) {};

    Item::Item(int p, int b) : production(p), bookmark(b) {};

    Item::Item() : production(-1), bookmark(-1) {};

    Item::Item(const Item& item) {
        *this = item;
    }

    Item::Item(Item&& item) noexcept {
        *this = item;
    }

    Item& Item::operator=(const Item& item) {
        production = item.production;
        rhs = item.rhs;
        bookmark = item.bookmark;
        if (item.instructions) {
            instructions = std::make_shared<ParseNode>("");
            *instructions = *item.instructions;
        }
        return *this;
    }

    Item& Item::operator=(Item&& item) noexcept {
        production = item.production;
        rhs = std::move(item.rhs);
        bookmark = item.bookmark;
        instructions = item.instructions;
        item.instructions = nullptr;
        return *this;
    }

    void Item::add(int r) {
        rhs.push_back(r);
    }

    int& Item::at(int i) {
        return rhs.at(i);
    }

    int Item::at(int i) const {
        return rhs.at(i);
    }

    int Item::size() const {
        return rhs.size();
    }

    void Item::endBookmark() const {
        bookmark = rhs.size();
    }

    bool operator<(const Item& a, const Item& b) {
        return a.production<b.production || a.bookmark<b.bookmark || less_than(a.rhs, b.rhs);
    }

    bool operator==(const Item& a, const Item& b) {
        return a.production==b.production && a.bookmark==b.bookmark && a.rhs==b.rhs;
    }

    ostream& operator<<(ostream& out, const Item& item) {
        out << item.production << " -> ";
        for (int i=0; i<item.size() ; ++i) {
            if (i==item.bookmark) out << "* ";
            out << item.rhs[i] << " ";
        }
        if (item.bookmark<0) out << "[*] ";
        else if (item.bookmark==item.size()) out << "* ";
        return out;
    }

    string toString(const Item& item) {
        stringstream out;
        string str;
        out << item.production << " -> ";
        for (int i=0; i<item.size() ; ++i) {
            if (i==item.bookmark) out << "* ";
            out << item.rhs[i] << " ";
        }
        if (item.bookmark<0) out << "[*] ";
        else if (item.bookmark==item.size()) out << "* ";
        out >> str;
        return str;
    }

    void State::insert(const Item& item) {
        // Check if the item is a null production.
        if (item.size()==0) has_null_production = true;
        // Insert the item into the set.
        item_set.insert(item);
    }

    // Set the bookmarks in all the items in this state to be fresh.
    void State::zero_bookmarks() {
        for (auto & item : item_set) {
            const int *i = &item.bookmark;
            *const_cast<int*>(i) = 0;
        }
    }

    bool State::contains(const Item& item) const {
        return item_set.find(item)!=item_set.end();
    }

    ostream& operator<<(ostream& out, const State& state) {
        if (state.empty()) out << "{}";
        else {
            out << "{";
            auto it = state.begin();
            out << *it;
            ++it;
            for (; it!=state.end(); ++it) out << ", " << *it;
            out << "}";
        }
        return out;
    }

    int State::size() const {
        return item_set.size();
    }

    bool State::empty() const {
        return item_set.empty();
    }

    set<Item>::iterator State::begin() {
        return item_set.begin();
    }

    set<Item>::iterator State::end() {
        return item_set.end();
    }

    set<Item>::iterator State::begin() const {
        return item_set.begin();
    }

    set<Item>::iterator State::end() const {
        return item_set.end();
    }

    set<Item>::iterator State::find(const Item& item) {
        return item_set.find(item);
    }

    bool operator==(const State& s1, const State& s2) {
        return s1.item_set==s2.item_set;
    }

    Entry::Entry() {};

    Entry::Entry(int s) : action(1), state(s) {};

    Entry::Entry(Item r) : action(2), state(0), rule(r) {};

    Entry::Entry(bool) : action(3) {};

    bool Entry::isError() const {
        return action==0;
    }

    bool Entry::isShift() const {
        return action==1;
    }

    bool Entry::isReduce() const {
        return action==2;
    }

    bool Entry::isAccept() const {
        return action==3;
    }

    string Entry::write(int length) const {
        string str;
        switch (action) {
            case 0: {
                str = "x";
                break;
            }
            case 1: {
                str = toString(state);
                break;
            }
            case 2: {
                str = "R";
                break;
            }
            case 3: {
                str = "A";
                break;
            }
            default: {
                cout << "Error. Bad Entry.\n";
                break;
            }
        }
        // Make sure the string is of the correct length.
        return buffered(str, 4);
    }

    ostream& operator << (ostream& out, const Entry& entry) {
        switch (entry.action) {
            case 0: {
                out << "x";
                break;
            }
            case 1: {
                out << "S: " << entry.state;
                break;
            }
            case 2: {
                out << "R: " << entry.rule;
                break;
            }
            case 3: {
                out << "A";
                break;
            }
            default: {
                cout << "[Bad Entry]";
                break;
            }
        }

        return out;
    }

}