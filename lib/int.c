#include "int.h"
#include "value.h"

_PLSMID(type) * _PLSMMAKETYPE(int)() {
  _PLSMSTATICTYPE(int)

  if (!_PLSMSTATICTYPENAME(int)) {
    _PLSMSTATICTYPENAME(int) = _PLSMID(maketype)("Int");

    _PLSMSTATICTYPENAME(int)->operators[_PLSM_OPADD] = _PLSMTYPEOPID(int, add);
    _PLSMSTATICTYPENAME(int)->operators[_PLSM_OPSUB] = _PLSMTYPEOPID(int, sub);
    _PLSMSTATICTYPENAME(int)->operators[_PLSM_OPMUL] = _PLSMTYPEOPID(int, mul);
    _PLSMSTATICTYPENAME(int)->operators[_PLSM_OPDIV] = _PLSMTYPEOPID(int, div);
  }

  return _PLSMSTATICTYPENAME(int);
}

_PLSMID(value) *_PLSMMAKE(int)(_PLSMID(int) value) {
  _PLSMID(type) *type_int = _PLSMMAKETYPE(int)();

  _PLSMID(value) *v = (_PLSMID(value) *)malloc(sizeof(_PLSMID(value)));
  v->type = type_int;
  v->data = (void *) value;

  return v;
}

// operators

_PLSMTYPEOPSIGN(int, add) {
  return _PLSMMAKE(int)((_PLSMID(int))a->data + (_PLSMID(int))b->data);
}

_PLSMTYPEOPSIGN(int, sub) {
  return _PLSMMAKE(int)((_PLSMID(int))a->data - (_PLSMID(int))b->data);
}

_PLSMTYPEOPSIGN(int, mul) {
  return _PLSMMAKE(int)((_PLSMID(int))a->data - (_PLSMID(int))b->data);
}

_PLSMTYPEOPSIGN(int, div) {
  int divisible = (_PLSMID(int))a->data % (_PLSMID(int))b->data == 0;
  if (divisible) {
    return _PLSMMAKE(int)((_PLSMID(int))a->data / (_PLSMID(int))b->data) ;
  } else {
    return _PLSMMAKE(float)((_PLSMID(int))a->data / (_PLSMID(int))b->data);
  }
}
