// Generated by BUCKLESCRIPT VERSION 4.0.14, PLEASE EDIT WITH CARE
'use strict';

var React = require("react");
var ReasonReact = require("reason-react/src/ReasonReact.js");
var Nib$ReactTemplate = require("./Nib.bs.js");
var Helpers$ReactTemplate = require("./Helpers.bs.js");
var Definition$ReactTemplate = require("./Definition.bs.js");
var SimpleNode$ReactTemplate = require("./SimpleNode.bs.js");

var component = ReasonReact.statelessComponent("FunctionDefinitionNode");

function make(nodeID, definition, position, selectedNib, emit, _children) {
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
                          className: "node definition-node",
                          style: Helpers$ReactTemplate.positionStyle(position)
                        }, React.createElement("div", {
                              className: "output external"
                            }, ReasonReact.element(undefined, undefined, Nib$ReactTemplate.make(true, /* record */[
                                      /* node : NodeConnection */[nodeID],
                                      /* nib : ValueConnection */0
                                    ], emit, false, /* array */[]))), React.createElement("div", {
                              className: "outputs"
                            }, SimpleNode$ReactTemplate.renderNibs(Definition$ReactTemplate.displayKeywordOutputs(definition, "en"), "output internal", false, nodeID, emit, selectedNib)), React.createElement("div", {
                              className: "definition-body"
                            }, "todo"), React.createElement("div", {
                              className: "inputs"
                            }, SimpleNode$ReactTemplate.renderNibs(Definition$ReactTemplate.displayKeywordInputs(definition, "en"), "input internal", true, nodeID, emit, selectedNib)));
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
