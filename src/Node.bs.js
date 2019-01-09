// Generated by BUCKLESCRIPT VERSION 4.0.14, PLEASE EDIT WITH CARE
'use strict';

var Block = require("bs-platform/lib/js/block.js");
var Belt_List = require("bs-platform/lib/js/belt_List.js");
var ReasonReact = require("reason-react/src/ReasonReact.js");
var Belt_MapString = require("bs-platform/lib/js/belt_MapString.js");
var Definition$ReactTemplate = require("./Definition.bs.js");
var SimpleNode$ReactTemplate = require("./SimpleNode.bs.js");
var FunctionDefinitionNode$ReactTemplate = require("./FunctionDefinitionNode.bs.js");

var component = ReasonReact.statelessComponent("Node");

function make(nodeID, node, definitions, position, emit, _children) {
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
              var makeNode = function (outputs, $staropt$star, name, param) {
                var inputs = $staropt$star !== undefined ? $staropt$star : /* [] */0;
                return ReasonReact.element(undefined, undefined, SimpleNode$ReactTemplate.make(nodeID, name, inputs, outputs, position, emit, /* array */[]));
              };
              if (typeof node === "number") {
                return makeNode(/* :: */[
                            /* record */[
                              /* name */"",
                              /* nib : ValueConnection */0
                            ],
                            /* [] */0
                          ], undefined, "Reference", /* () */0);
              } else if (node.tag) {
                var match = node[0];
                var kind = match[/* kind */0];
                var definition = Belt_MapString.getExn(definitions, match[/* definitionID */1]);
                var match$1 = Definition$ReactTemplate.displayDefinedNode(definition, kind, "en");
                if (kind !== 3) {
                  return makeNode(match$1[/* outputs */1], match$1[/* inputs */0], Definition$ReactTemplate.getDisplayName(definition, "en"), /* () */0);
                } else {
                  return ReasonReact.element(undefined, undefined, FunctionDefinitionNode$ReactTemplate.make(nodeID, definition, position, emit, /* array */[]));
                }
              } else {
                return makeNode(/* :: */[
                            /* record */[
                              /* name */"",
                              /* nib : ValueConnection */0
                            ],
                            /* [] */0
                          ], Belt_List.makeBy(node[0], (function (index) {
                                  return /* record */[
                                          /* name */String(index),
                                          /* nib : PositionalConnection */Block.__(1, [index])
                                        ];
                                })), "List", /* () */0);
              }
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
