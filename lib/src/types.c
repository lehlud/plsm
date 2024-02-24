#include "type.h"

type_t *LIB(typeAny), *LIB(typeNum), *LIB(typeInt), *LIB(typeFloat);

void LIB(initTypes)()
{
  LIB(typeAny) = LIB(makeType)(NULL, "Any");

  LIB(typeNum) = LIB(makeType)(LIB(typeAny), "Num");
  LIB(typeInt) = LIB(makeType)(LIB(typeNum), "Int");
  LIB(typeFloat) = LIB(makeType)(LIB(typeNum), "Float");
}
