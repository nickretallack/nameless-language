'use strict';


function f(connectionNib) {
  if (typeof connectionNib === "number") {
    return "value";
  } else if (connectionNib.tag) {
    return String(connectionNib[0]);
  } else {
    return connectionNib[0];
  }
}

exports.f = f;
/* No side effect */
