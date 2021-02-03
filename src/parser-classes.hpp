#ifndef __PARSER_CLASSES_HPP__MANTA__
#define __PARSER_CLASSES_HPP__MANTA__

#include "lexer-Manta.hpp"

namespace Manta {

    typedef int ProductionID;
    typedef int ActionID;
    typedef int StateID;

    //! \brief Encodes state items like A -> a * X b, etc.
    //!
    //! A state is a set of state items.
    struct Item {
        explicit Item(int);
        Item(int, int);

        //! \brief Create an empty item.
        Item();

        Item(const Item& item);

        Item(Item&& item) noexcept;

        Item& operator=(const Item& item);

        Item& operator=(Item&& item) noexcept;

        void add(int r);
        int& at(int i);
        int at(int i) const;
        int size() const;
        void endBookmark() const;

        friend bool operator < (const Item& a, const Item& b);
        friend bool operator == (const Item& a, const Item& b);
        friend ostream& operator << (ostream& out, const Item& item);
        friend string toString(const Item&);

        // --- Data items ---

        //! \brief The production this is a rule for.
        ProductionID production = 0;

        //! \brief The right hand side of the production.
        std::vector<int> rhs;

        //! \brief The location of the bookmark.
        //!
        //! Bookmark is in the place *before* bookmark, so e.g. "A -> a * X b" has bookmark=1.
        //! A -1 means no bookmark, this is used when an item is being used to encode a pure production,
        //! not a state item.
        mutable int bookmark = 0;

        //! \brief
        std::shared_ptr<class ParseNode> instructions = nullptr;
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
        std::set<Item>::iterator begin();
        std::set<Item>::iterator end();
        std::set<Item>::iterator begin() const;
        std::set<Item>::iterator end() const;
        std::set<Item>::iterator find(const Item& item);
        friend bool operator==(const State& s1, const State& s2);

        //! \brief True if there exists an Item in the set that is a null production, A -> null
        bool has_null_production = false;

        //! \brief The underlying set of items.
        std::set<Item> item_set;
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
        explicit Entry(int s);
        //! Create entry as a reduce.
        explicit Entry(Item r);
        //! \brief Create entry as an accept.
        explicit Entry(bool);

        bool isError() const;
        bool isShift() const;
        bool isReduce() const;
        bool isAccept() const;

        ActionID getAction() const { return action; }
        StateID getState() const { return state; }
        Item getRule() const { return rule; }

        string write(int length) const;

        friend ostream& operator << (ostream&, const Entry&);

    private:
        //! \brief The action.
        ActionID action = 0;

        //! \brief The state to transition true.
        StateID state = 0;

        //! \brief The reduce rule (if applicable).
        Item rule;
    };

}
#endif // __PARSER_CLASSES_HPP__MANTA__