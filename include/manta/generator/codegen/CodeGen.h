#include "manta/generator/typesystem/HigherTypeSystem.h"

namespace manta::codegen {

class CppCodegen {
public:

  void WriteName(std::ostream& out, const typesystem::GeneralizedType* type, std::span<std::string> var_names = {});

  void WriteDeclaration(std::ostream& out, const typesystem::GeneralizedType* type);

private:

};

}