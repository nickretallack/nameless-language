'use strict';


function f(connectionNode) {
  if (connectionNode) {
    return "node-" + connectionNode[0];
  } else {
    return "graph";
  }
}

exports.f = f;
/* No side effect */
