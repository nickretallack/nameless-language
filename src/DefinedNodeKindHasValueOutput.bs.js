'use strict';


function f(kind) {
  switch (kind) {
    case /* ValueNode */1 :
    case /* FunctionDefinitionNode */3 :
    case /* ConstructorNode */4 :
        return true;
    case /* FunctionCallNode */0 :
    case /* FunctionPointerCallNode */2 :
    case /* AccessorNode */5 :
        return false;
    
  }
}

exports.f = f;
/* No side effect */
