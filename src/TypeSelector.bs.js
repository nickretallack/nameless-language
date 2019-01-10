// Generated by BUCKLESCRIPT VERSION 4.0.14, PLEASE EDIT WITH CARE
'use strict';

var Block = require("bs-platform/lib/js/block.js");
var Curry = require("bs-platform/lib/js/curry.js");
var React = require("react");
var ReasonReact = require("reason-react/src/ReasonReact.js");
var Definition$ReactTemplate = require("./Definition.bs.js");

var component = ReasonReact.statelessComponent("TypeSelector");

function make(valueType, definitions, changeType, _children) {
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
              return React.createElement("div", undefined, React.createElement("div", undefined, Definition$ReactTemplate.displayValueType(valueType, definitions, "en")), React.createElement("a", {
                              onClick: (function (_event) {
                                  return Curry._1(changeType, /* PrimitiveValueType */Block.__(0, [/* TextType */2]));
                                })
                            }, "Text"), React.createElement("a", {
                              onClick: (function (_event) {
                                  return Curry._1(changeType, /* PrimitiveValueType */Block.__(0, [/* NumberType */1]));
                                })
                            }, "Number"));
            }),
          /* initialState */component[/* initialState */10],
          /* retainedProps */component[/* retainedProps */11],
          /* reducer */component[/* reducer */12],
          /* jsElementWrapped */component[/* jsElementWrapped */13]
        ];
}

exports.component = component;
exports.make = make;
/* component Not a pure module */