'use strict';


function f(kind) {
  switch (kind) {
    case /* FunctionCallNode */0 :
        return "function call";
    case /* ValueNode */1 :
        return "value";
    case /* FunctionPointerCallNode */2 :
        return "function pointer call";
    case /* FunctionDefinitionNode */3 :
        return "function definition";
    case /* ConstructorNode */4 :
        return "constructor";
    case /* AccessorNode */5 :
        return "accessor";
    
  }
}

exports.f = f;
/* No side effect */
