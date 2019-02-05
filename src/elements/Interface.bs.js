// Generated by BUCKLESCRIPT VERSION 4.0.14, PLEASE EDIT WITH CARE
'use strict';

var Curry = require("bs-platform/lib/js/curry.js");
var React = require("react");
var ReasonReact = require("reason-react/src/ReasonReact.js");
var TypedFields$ReactTemplate = require("./TypedFields.bs.js");

var component = ReasonReact.statelessComponent("Interface");

function make(definitions, $$interface, documentation, display, emit, _children) {
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
              return React.createElement("div", {
                          className: "interface"
                        }, React.createElement("div", undefined, React.createElement("h2", undefined, "Outputs:"), ReasonReact.element(undefined, undefined, TypedFields$ReactTemplate.make(definitions, $$interface[/* outputTypes */1], documentation[/* outputs */3], false, display[/* outputOrdering */1], emit, /* array */[])), React.createElement("a", {
                                  onClick: (function (_event) {
                                      return Curry._1(emit, /* AddOutput */1);
                                    })
                                }, "Add Output")), React.createElement("div", undefined, React.createElement("h2", undefined, "Inputs:"), ReasonReact.element(undefined, undefined, TypedFields$ReactTemplate.make(definitions, $$interface[/* inputTypes */0], documentation[/* inputs */2], true, display[/* inputOrdering */0], emit, /* array */[])), React.createElement("a", {
                                  onClick: (function (_event) {
                                      return Curry._1(emit, /* AddInput */0);
                                    })
                                }, "Add Input")));
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
