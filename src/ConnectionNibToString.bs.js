'use strict';


function f(connectionNib) {
  if (typeof connectionNib === "number") {
    return "value";
  } else if (connectionNib.tag) {
    return "index-" + String(connectionNib[0]);
  } else {
    return "nib-" + connectionNib[0];
  }
}

exports.f = f;
/* No side effect */
