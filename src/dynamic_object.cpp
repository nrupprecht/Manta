#include "dynamic_object.hpp"

namespace Manta {

  ///

  VariableData::VariableData(const string& name, const TypeRep* t, int number) 
    : variable_name(name), type(t), number_of_entries(number) 
  {
    if (type==nullptr) throw false; // \todo Real error.
    data = new Byte[t->total_size*number_of_entries];
  }

  VariableData::~VariableData() { 
    if (data) delete [] data;
  }

  string VariableData::to_string() const {
    return variable_name + ": " + Manta::to_string(type, data);
  }

  Byte* VariableData::access(const string& entry) {
    if (data==nullptr) return nullptr;
    int location = -1;
    for(auto it=type->structure.begin(); it!=type->structure.end(); ++it)
      if (std::get<0>(*it)==entry) location = std::get<3>(*it);
    if (-1<location) return &data[location];
    // Failure.
    return nullptr;
  }

  Byte* VariableData::operator() (unsigned index) {
    if (data==nullptr) return nullptr;
    return &data[index];
  }

  void VariableData::set(const string& entry, const VariableData& value) {
    if (data==nullptr) {
      cout << "Error: Data is null.\n";
      throw false;
    }
    int location = -1;
    for(auto it=type->structure.begin(); it!=type->structure.end(); ++it)
      if (std::get<0>(*it)==entry) {
        // Make sure types match.
        if (std::get<1>(*it)->type_name!=value.type->type_name) {
          cout << "Error: Types do not match. " << std::get<1>(*it)->type_name << " vs. " << type->type_name << "\n";
          throw false;
        }
        location = std::get<3>(*it);
      }

    // If we found the entry
    if (-1<location) std::copy(
      value.data,
      value.data + value.type->total_size*value.number_of_entries,
      &data[location]
    );
    else {
      cout << "Error: Location not valid.\n";
      throw false;
    }
  }


  ///

  DynamicObject::DynamicObject() {
    push_scope();
    define_type(Int_Type);
    define_type(Float_Type);
  }

  unsigned DynamicObject::define_type(const TypeRep& type_rep) {
    if (find_type(type_rep.type_name)!=nullptr) return 1;

    TypeRep *type = new TypeRep(type_rep);
    type_def_stack.rbegin()->push_back(type);
    // Return success.
    return 0;
  }

  unsigned DynamicObject::define_type(string type_name, TypeStructure& structure) {
    // Type in use error. 
    if (find_type(type_name)!=nullptr) return 1;

    // Compute the size of the type by looking at the substructure.
    int type_size = 0;
    for (auto subpart : structure)
      type_size += std::get<1>(subpart)->total_size*std::get<2>(subpart);

    // Create a type representation.
    TypeRep *type = new TypeRep(type_name, structure, type_size);
    type_def_stack.rbegin()->push_back(type);
    // Return success.
    return 0;
  }

  unsigned DynamicObject::define_type(const structure_definer& definition) {
    // Type in use error.
    if (find_type(definition.type_name)!=nullptr) return 1;

    // Go through structure members.
    TypeStructure structure;
    int type_size = 0;
    for (auto entry : definition.members) {
      // Check that the type specifier is valid.
      auto type = find_type(std::get<1>(entry));
      // Type undefined error.
      if (type==nullptr) return 2;
      //  Name of entry, Type of entry, number of entries for the sub-structure (for arrays), starting (Byte) address.
      structure.push_back(std::make_tuple(std::get<0>(entry), type, std::get<2>(entry), type_size));
      // Update type's total size.
      type_size += type->total_size * std::get<2>(entry);
    }
    // Define the type using the newly created type structure.
    define_type(definition.type_name, structure);
    // Return success
    return 0;
  }

  unsigned DynamicObject::define_variable(const string& variable_name, const string& type_name, unsigned number_of_objects) {
    // Find the type
    TypeRep* type_ptr = find_type(type_name);
    // Type undefined error.
    if (type_ptr==nullptr) return 1;

    // See if this variable name is in use.
    if (find_variable(variable_name)) return 2;

    auto *variable = new VariableData(variable_name, type_ptr, number_of_objects);
    variable_def_stack.rbegin()->push_back(variable);
    // Return success.
    return 0;
  }

  void DynamicObject::push_scope() {
    type_def_stack.push_back(list<TypeRep*>());
    variable_def_stack.push_back(list<VariableData*>());
  }

  void DynamicObject::pop_scope() {
    if (type_def_stack.size()>1) {
      type_def_stack.pop_back();
      variable_def_stack.pop_back();
    }
  }

  TypeRep* DynamicObject::top_type() {
    // Return the most recently defined type.
    for (auto it=type_def_stack.rbegin(); it!=type_def_stack.rend(); ++it)
      for (auto q=it->rbegin(); q!=it->rend(); ++q) return *q;
    // No types have been defined.
    return nullptr;
  }

  VariableData* DynamicObject::top_variable() {
    // Return the most recently defined variable.
    for (auto it=variable_def_stack.rbegin(); it!=variable_def_stack.rend(); ++it)
      for (auto q=it->rbegin(); q!=it->rend(); ++q) return *q;
    // No variables have been defined.
    return nullptr;
  }

  inline TypeRep* DynamicObject::find_type(const string& type_name) {
    // Start looking in the most recent part of the stack.
    for (auto it=type_def_stack.rbegin(); it!=type_def_stack.rend(); ++it) {
      // Look for the type at this level of the stack.
      for (auto q : *it)
        if (q->type_name==type_name) return q;
    }
    return nullptr;
  }

  inline VariableData* DynamicObject::find_variable(const string& variable_name) {
    // Start looking in the most recent part of the stack.
    for (auto it=variable_def_stack.rbegin(); it!=variable_def_stack.rend(); ++it) {
      // Look for the type at this level of the stack.
      for (auto q : *it)
        if (q->variable_name==variable_name) return q;
      // for q in it until(q->variable_name==variable_name) pass: then return q
    }
    return nullptr;
  }

}