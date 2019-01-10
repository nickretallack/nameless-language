// Generated by BUCKLESCRIPT VERSION 4.0.14, PLEASE EDIT WITH CARE
'use strict';

var Block = require("bs-platform/lib/js/block.js");
var Curry = require("bs-platform/lib/js/curry.js");
var React = require("react");
var Caml_format = require("bs-platform/lib/js/caml_format.js");
var ReasonReact = require("reason-react/src/ReasonReact.js");
var Caml_js_exceptions = require("bs-platform/lib/js/caml_js_exceptions.js");
var Helpers$ReactTemplate = require("./Helpers.bs.js");
var Caml_builtin_exceptions = require("bs-platform/lib/js/caml_builtin_exceptions.js");
var Definition$ReactTemplate = require("./Definition.bs.js");

var component = ReasonReact.statelessComponent("DefinitionHeader");

function round(x) {
  return x + 0.5 | 0;
}

function convertPrimitiveValueType(primitiveValue, typeName) {
  switch (typeName) {
    case "integer" : 
        var tmp;
        switch (primitiveValue.tag | 0) {
          case 1 : 
              tmp = primitiveValue[0] + 0.5 | 0;
              break;
          case 0 : 
          case 2 : 
              tmp = Caml_format.caml_int_of_string(Definition$ReactTemplate.primitiveValueToString(primitiveValue));
              break;
          
        }
        return /* IntegerValue */Block.__(0, [tmp]);
    case "number" : 
        var tmp$1;
        try {
          tmp$1 = Caml_format.caml_float_of_string(Definition$ReactTemplate.primitiveValueToString(primitiveValue));
        }
        catch (raw_exn){
          var exn = Caml_js_exceptions.internalToOCamlException(raw_exn);
          if (exn[0] === Caml_builtin_exceptions.failure) {
            tmp$1 = 0.0;
          } else {
            throw exn;
          }
        }
        return /* NumberValue */Block.__(1, [tmp$1]);
    case "text" : 
        return /* TextValue */Block.__(2, [Definition$ReactTemplate.primitiveValueToString(primitiveValue)]);
    default:
      throw Caml_builtin_exceptions.not_found;
  }
}

function make(documentation, emit, _children) {
  return /* record */[
          /* debugName */component[/* debugName */0],
          /* reactClassInternal */component[/* reactClassInternal */1],
          /* handedOffState */component[/* handedOffState */2],
          /* willReceiveProps */component[/* willReceiveProps */3],
          /* didMount */component[/* didMount */4],
          /* didUpdate */component[/* didUpdate */5],
          /* willUnmount */component[/* willUnmount */6],
          /* willUpdate */component[/* willUpdate */7],
          /* shouldUpdate */component[/* shouldUpdate */8],
          /* render */(function (_self) {
              var changeName = function ($$event) {
                return Curry._1(emit, /* ChangeName */Block.__(1, [Helpers$ReactTemplate.getEventValue($$event)]));
              };
              return React.createElement("div", undefined, React.createElement("input", {
                              className: "name",
                              type: "text",
                              value: Definition$ReactTemplate.getTranslated(documentation[/* name */0], "en"),
                              onChange: changeName
                            }));
            }),
          /* initialState */component[/* initialState */10],
          /* retainedProps */component[/* retainedProps */11],
          /* reducer */component[/* reducer */12],
          /* jsElementWrapped */component[/* jsElementWrapped */13]
        ];
}

exports.component = component;
exports.round = round;
exports.convertPrimitiveValueType = convertPrimitiveValueType;
exports.make = make;
/* component Not a pure module */
