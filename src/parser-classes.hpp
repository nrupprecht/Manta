#ifndef __PARSER_CLASSES_HPP__MANTA__
#define __PARSER_CLASSES_HPP__MANTA__

#include "lexer-Manta.hpp"

namespace Manta {

  //! \brief Encodes state items like A -> a * X b, etc.
  //!
  //! A state is a set of state items.
  struct Item {
    Item(int p);
    Item(int p, int b);

    //! \brief Create an empty item.
    Item();

    void add(int r);
    int at(int i);
    int at(int i) const;
    int size() const;
    void endBookmark() const;

    friend bool operator < (const Item& a, const Item& b);
    friend bool operator == (const Item& a, const Item& b);

    friend ostream& operator<<(ostream& out, const Item& item);

    //! \brief The production this is a rule for.
    int production;

    //! \brief The right hand side of the production.
    vector<int> rhs;

    //! \brief The location of the bookmark. 
    //!
    //! Bookmark is in the place *before* bookmark, so e.g. "A -> a * X b" has bookmark=1.
    //! A -1 means no bookmark, this is used when an item is being used to encode a pure production,
    //! not a state item.
    mutable int bookmark = 0;
  };

  //! \brief Define a state to be a set of Items, with some extra features for convenience.
  struct State {

    void insert(const Item& item);

    // Set the bookmarks in all the items in this state to be fresh.
    void zero_bookmarks();

    bool contains(const Item& item) const;

    friend ostream& operator<<(ostream& out, const State& state);

    int size() const;
    bool empty() const;
    set<Item>::iterator begin();
    set<Item>::iterator end();
    set<Item>::iterator begin() const;
    set<Item>::iterator end() const;
    set<Item>::iterator find(const Item& item);
    friend bool operator==(const State& s1, const State& s2);

    //! \brief True if there exists an Item in the set that is a null production, A -> null
    bool has_null_production = false;

    //! \brief The underlying set of items.
    set<Item> item_set;
  };

  //! \brief Table entry.
  //!
  //! Actions
  //! 0 - Error.
  //! 1 - Shift.
  //! 2 - Reduce.
  //! 3 - Accept.
  struct Entry {
    //! Create entry as an error
    Entry();
    //! Create entry as a shift.
    Entry(int s);
    //! Create entry as a reduce.
    Entry(Item r);
    //! \brief Create entry as an accept.
    Entry(bool);

    bool isError() const;
    bool isShift() const;
    bool isReduce() const;
    bool isAccept() const;

    string write(int length) const;

    friend ostream& operator << (ostream&, const Entry&);

    //! \brief The action.
    int action = 0;
    //! \brief The state to transition true.
    int state  = 0;
    //! \brief The reduce rule (if applicable).
    Item rule;
  };

}
#endif // __PARSER_CLASSES_HPP__MANTA__