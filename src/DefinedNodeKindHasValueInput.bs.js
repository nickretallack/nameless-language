'use strict';


function f(kind) {
  if (kind !== 2) {
    return kind >= 5;
  } else {
    return true;
  }
}

exports.f = f;
/* No side effect */
