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

function make(nodeID, node, definitions, position, selectedNib, emit, _children) {
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
              var makeNode = function (outputs, $staropt$star, definitionID, name, param) {
                var inputs = $staropt$star !== undefined ? $staropt$star : /* [] */0;
                return ReasonReact.element(undefined, undefined, SimpleNode$ReactTemplate.make(nodeID, definitionID, name, inputs, outputs, position, selectedNib, emit, /* array */[]));
              };
              var match = node[/* kind */1];
              if (typeof match === "number") {
                return makeNode(/* :: */[
                            /* record */[
                              /* name */"",
                              /* nib : ValueConnection */0
                            ],
                            /* [] */0
                          ], undefined, undefined, "Reference", /* () */0);
              } else if (match.tag) {
                var match$1 = match[0];
                var definitionID = match$1[/* definitionID */1];
                var kind = match$1[/* kind */0];
                var definition = Belt_MapString.getExn(definitions, definitionID);
                var match$2 = Definition$ReactTemplate.displayDefinedNode(definition, kind, "en");
                if (kind !== 3) {
                  return makeNode(match$2[/* outputs */1], match$2[/* inputs */0], definitionID, Definition$ReactTemplate.getDisplayName(definition, "en"), /* () */0);
                } else {
                  return ReasonReact.element(undefined, undefined, FunctionDefinitionNode$ReactTemplate.make(nodeID, definition, position, selectedNib, emit, /* array */[]));
                }
              } else {
                return makeNode(/* :: */[
                            /* record */[
                              /* name */"",
                              /* nib : ValueConnection */0
                            ],
                            /* [] */0
                          ], Belt_List.makeBy(match[0], (function (index) {
                                  return /* record */[
                                          /* name */String(index),
                                          /* nib : PositionalConnection */Block.__(1, [index])
                                        ];
                                })), undefined, "List", /* () */0);
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
