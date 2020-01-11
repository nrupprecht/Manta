#ifndef __DYNAMIC_OBJECT_HPP__MANTA__
#define __DYNAMIC_OBJECT_HPP__MANTA__

#include <list>
using std::list;

#include <tuple>

#include "parser-classes.hpp"

namespace Manta {

  template<typename T> inline string toStr(T x) {
    stringstream stream;
    stream << x;
    string str;
    stream >> str;
    return str;
  }

  // Typedef of a Byte of data.
  typedef char Byte;

  // Forward declaration.
  class TypeRep;

  // Name of entry, Type of entry, number of entries for the sub-structure (for arrays), starting (Byte) address.
  typedef vector<std::tuple<string, const TypeRep*, int, int> > TypeStructure;

  struct TypeRep {
    TypeRep(const string& name, const TypeStructure& strct, int size) 
      : type_name(name), structure(strct), total_size(size) {};

    TypeRep(const TypeRep&) = default;

    string to_string() const {
      string representation = "< " + type_name + " ";
      for (auto subpart : structure) {
        int array_size = std::get<2>(subpart);
        if (array_size>1) representation += "[" + std::get<1>(subpart)->type_name + "," + toStr(array_size) + "]";
        else representation += std::get<1>(subpart)->type_name;
        representation += " ";
      }
      representation += ">";

      return representation;
    }

    friend bool are_equal(const TypeRep *x, const TypeRep *y) {
      return x->type_name==y->type_name;
    }

    //! \brief The name of the type.
    string type_name;

    //! \brief A description of the structure.
    TypeStructure structure;

    //! \brief The size (in Bytes) of one such object.
    int total_size;
  };

  inline string to_string(const TypeRep *type_info, Byte *data) {
    if (type_info->type_name=="int") return "int=" + toStr<int>(data[0]);
    else if (type_info->type_name=="float") return "float=" + toStr<float>(data[0]);

    string representation = "< " + type_info->type_name + ": ";
    // Recursively explore structure.
    for (auto entry : type_info->structure)
      representation += std::get<0>(entry) + "::" + to_string(std::get<1>(entry), &data[std::get<3>(entry)]) + " ";

    representation += ">";
    return representation;
  }

  // Predefined basic types.
  const unsigned Int_Type_Size = sizeof(int)/sizeof(Byte);
  const unsigned Float_Type_Size = sizeof(float)/sizeof(Byte);
  const TypeRep Int_Type {"int", TypeStructure{}, Int_Type_Size };
  const TypeRep Float_Type {"float", TypeStructure{}, Float_Type_Size };

  struct VariableData {
    VariableData(const string&, const TypeRep*, int number=1);
    ~VariableData();

    string to_string() const;
    Byte* access(const string& entry);
    Byte* operator() (unsigned index);
    void set(const string& entry, const VariableData& value);

    //! \brief Name of the variable.
    string variable_name;

    //! \brief Pointer to the type representation of this variable.
    const TypeRep *type;
    //! \brief Pointer to the start of the data.
    Byte *data;
    //! \brief The number of entries. Will be > 1 for an array.
    unsigned number_of_entries;
  };

  //! \brief Return a VariableData structure representing an int.
  inline VariableData create_int(int value) {
    auto variable = VariableData("", &Int_Type);
    *reinterpret_cast<int*>(variable.data) = value;
    return variable;
  }

  inline void set_int(int value, VariableData *int_data) {
    if (int_data->type->type_name!="int") {
      cout << "Error: Trying to move an int to a " << int_data->type->type_name << endl;
      throw false;
    }
    *reinterpret_cast<int*>(int_data->data) = value;
  }

  //! \brief Return a VariableData structure representing a float.
  inline VariableData create_float(float value) {
    auto variable = VariableData("", &Float_Type);
    *reinterpret_cast<int*>(variable.data) = value;
    return variable;
  }

  inline void set_float(float value, VariableData *float_data) {
    if (float_data->type->type_name!="float") {
      cout << "Error: Trying to move a float to a " << float_data->type->type_name << endl;
      throw false;
    }
    *reinterpret_cast<float*>(float_data->data) = value;
  }

  //! \brief Move a value from one variable type to another.
  inline void move_value(VariableData& x, VariableData &&y) {
    if (!are_equal(x.type, y.type)) {
      cout << "X type: " << x.type->to_string() << ", Y type: " << y.type->to_string() << endl;
      throw false;
    }
    if (x.data) delete [] x.data;
    x.data = y.data;
    y.data = nullptr;
  }

  struct structure_definer {
    structure_definer(const string& name) : type_name(name) {};

    void add_member(string name, string type, unsigned entries=1) {
      members.push_back(std::make_tuple(name, type, entries));
    }

    vector<std::tuple<string, string, unsigned> > members;

    string type_name;
  };

  class DynamicObject {
  public:
    DynamicObject();

    //! \brief Define a type in the current scope from an existing TypeRep object.
    unsigned define_type(const TypeRep&);
    //! \brief Define a type in the current scope from a name and description.
    unsigned define_type(string, TypeStructure&);
    //! \brief Define a type in the current scope from a structure_definer structure.
    unsigned define_type(const structure_definer&);

    //! \brief Define a variable in the current scope.
    unsigned define_variable(const string&, const string&, unsigned=1);

    //! \brief Create a new scope.
    void push_scope();
    //! \brief Pop the latest scope.
    void pop_scope();

    //! \brief Get the most recently defined type.
    TypeRep* top_type();
    //! \brief Get the most recently defined variable.
    VariableData* top_variable();

  private:

    inline TypeRep* find_type(const string&);
    inline VariableData* find_variable(const string&);

    //! \brief Scoped type definitions.
    list<list<TypeRep*> > type_def_stack;
    //! \brief Scoped variable definitions.
    list<list<VariableData*> > variable_def_stack;
  };

}
#endif // __DYNAMIC_OBJECT_HPP__MANTA__