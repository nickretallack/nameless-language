// Generated by BUCKLESCRIPT VERSION 4.0.14, PLEASE EDIT WITH CARE
'use strict';

var React = require("react");
var Caml_obj = require("bs-platform/lib/js/caml_obj.js");
var ReasonReact = require("reason-react/src/ReasonReact.js");
var Nib$ReactTemplate = require("./Nib.bs.js");
var Helpers$ReactTemplate = require("../Helpers.bs.js");
var Definition$ReactTemplate = require("../Definition.bs.js");
var SimpleNode$ReactTemplate = require("./SimpleNode.bs.js");

var component = ReasonReact.statelessComponent("FunctionDefinitionNode");

function make(nodeID, definition, definitionID, position, size, depth, selectedNib, emit, _children) {
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
                          style: {
                            height: Helpers$ReactTemplate.pixels(size[/* y */1]),
                            left: Helpers$ReactTemplate.pixels(position[/* x */0]),
                            top: Helpers$ReactTemplate.pixels(position[/* y */1]),
                            width: Helpers$ReactTemplate.pixels(size[/* x */0]),
                            zIndex: String(depth)
                          },
                          onDoubleClick: (function (_event) {
                              return ReasonReact.Router[/* push */0]("#" + definitionID);
                            })
                        }, React.createElement("div", {
                              className: "output external"
                            }, ReasonReact.element(undefined, undefined, Nib$ReactTemplate.make(true, /* record */[
                                      /* node : NodeConnection */[nodeID],
                                      /* nib : ValueConnection */0
                                    ], emit, Caml_obj.caml_equal(selectedNib, /* ValueConnection */0), /* array */[]))), React.createElement("div", {
                              className: "outputs"
                            }, SimpleNode$ReactTemplate.renderNibs(Definition$ReactTemplate.displayKeywordOutputs(definition, "en"), "output internal", false, /* NodeConnection */[nodeID], emit, selectedNib)), React.createElement("div", {
                              className: "definition-body"
                            }), React.createElement("div", {
                              className: "inputs"
                            }, SimpleNode$ReactTemplate.renderNibs(Definition$ReactTemplate.displayKeywordInputs(definition, "en"), "input internal", true, /* NodeConnection */[nodeID], emit, selectedNib)));
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
