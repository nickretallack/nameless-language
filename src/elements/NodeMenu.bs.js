// Generated by BUCKLESCRIPT VERSION 4.0.14, PLEASE EDIT WITH CARE
'use strict';

var Block = require("bs-platform/lib/js/block.js");
var Curry = require("bs-platform/lib/js/curry.js");
var React = require("react");
var Caml_obj = require("bs-platform/lib/js/caml_obj.js");
var Belt_List = require("bs-platform/lib/js/belt_List.js");
var Belt_Array = require("bs-platform/lib/js/belt_Array.js");
var ReasonReact = require("reason-react/src/ReasonReact.js");
var Belt_MapString = require("bs-platform/lib/js/belt_MapString.js");
var Caml_builtin_exceptions = require("bs-platform/lib/js/caml_builtin_exceptions.js");
var Definition$ReactTemplate = require("../Definition.bs.js");
var SimpleNode$ReactTemplate = require("./SimpleNode.bs.js");

function isNumberConstant(definition) {
  var match = definition[/* implementation */0];
  if (match.tag) {
    return false;
  } else {
    switch (match[0].tag | 0) {
      case 1 : 
          return true;
      case 0 : 
      case 2 : 
          return false;
      
    }
  }
}

function getScope(nib, nodes) {
  var match = nib[/* connectionSide */0][/* node */0];
  if (match) {
    var nodeID = match[0];
    var node = Belt_MapString.getExn(nodes, nodeID);
    var match$1 = Definition$ReactTemplate.isFunctionDefinitionNode(node) && !Definition$ReactTemplate.isValueNib(nib[/* connectionSide */0][/* nib */1]);
    if (match$1) {
      return /* NodeScope */[nodeID];
    } else {
      return node[/* scope */0];
    }
  } else {
    return /* GraphScope */0;
  }
}

function maybeNameless(string) {
  var match = string.length === 0;
  if (match) {
    return "(nameless)";
  } else {
    return string;
  }
}

function canConnectToNib(definition, isSource) {
  if (isSource) {
    if (Belt_List.length(definition[/* display */2][/* inputOrdering */0]) !== 0) {
      var match = definition[/* implementation */0];
      if (match.tag) {
        return true;
      } else {
        return false;
      }
    } else {
      return false;
    }
  } else {
    return true;
  }
}

var component = ReasonReact.reducerComponent("NodeMenu");

function make(definitions, nodes, nib, emit, _children) {
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
          /* render */(function (self) {
              var scope = getScope(nib, nodes);
              var renderCategory = function (name, category) {
                var match = Caml_obj.caml_equal(self[/* state */1][/* category */0], category);
                return React.createElement("a", {
                            className: match ? "selected" : "",
                            onClick: (function (_event) {
                                return Curry._1(self[/* send */3], /* SelectCategory */Block.__(0, [category]));
                              })
                          }, name);
              };
              var nodeSelector = function (filterFunction) {
                return React.createElement("div", {
                            className: "type-selector-choices"
                          }, React.createElement("h3", undefined, "Definitions"), Belt_Array.map(Belt_MapString.toArray(Belt_MapString.keep(definitions, (function (_definitionID, definition) {
                                          if (Curry._1(filterFunction, definition)) {
                                            return canConnectToNib(definition, nib[/* isSource */1]);
                                          } else {
                                            return false;
                                          }
                                        }))), (function (param) {
                                  var definitionID = param[0];
                                  var match = Caml_obj.caml_equal(self[/* state */1][/* definitionID */1], definitionID);
                                  return React.createElement("a", {
                                              key: definitionID,
                                              className: match ? "selected" : "",
                                              onClick: (function (_event) {
                                                  return Curry._1(self[/* send */3], /* SelectDefinition */Block.__(1, [definitionID]));
                                                })
                                            }, Definition$ReactTemplate.getDisplayName(param[1], "en"));
                                })));
              };
              var match = nib[/* isSource */1];
              var match$1 = self[/* state */1][/* category */0];
              var match$2 = self[/* state */1][/* definitionID */1];
              var tmp;
              if (match$2 !== undefined) {
                var definitionID = match$2;
                var definition = Belt_MapString.getExn(definitions, definitionID);
                var match$3 = definition[/* implementation */0];
                var tmp$1;
                var exit = 0;
                switch (match$3.tag | 0) {
                  case 1 : 
                      var match$4 = Caml_obj.caml_equal(self[/* state */1][/* definedNodeKind */2], /* FunctionPointerCallNode */2);
                      var match$5 = nib[/* isSource */1];
                      tmp$1 = React.createElement(React.Fragment, undefined, React.createElement("a", {
                                className: match$4 ? "selected" : "",
                                onClick: (function (_event) {
                                    return Curry._1(self[/* send */3], /* SetDefinedNodeKind */Block.__(2, [/* FunctionPointerCallNode */2]));
                                  })
                              }, "function pointer call"), match$5 ? null : React.createElement("a", {
                                  onClick: (function (_event) {
                                      return Curry._1(emit, /* AddNode */Block.__(4, [/* record */[
                                                      /* node : record */[
                                                        /* scope */getScope(nib, nodes),
                                                        /* kind : DefinedNode */Block.__(1, [/* record */[
                                                              /* kind : FunctionDefinitionNode */3,
                                                              /* definitionID */definitionID
                                                            ]])
                                                      ],
                                                      /* explicitConnectionSide */nib,
                                                      /* connectionNib : ValueConnection */0
                                                    ]]));
                                    })
                                }, "inline function"));
                      break;
                  case 2 : 
                  case 3 : 
                      exit = 1;
                      break;
                  default:
                    tmp$1 = React.createElement(React.Fragment, undefined, "TODO");
                }
                if (exit === 1) {
                  var match$6 = Caml_obj.caml_equal(self[/* state */1][/* definedNodeKind */2], /* FunctionCallNode */0);
                  var match$7 = nib[/* isSource */1];
                  tmp$1 = React.createElement(React.Fragment, undefined, React.createElement("a", {
                            className: match$6 ? "selected" : "",
                            onClick: (function (_event) {
                                return Curry._1(self[/* send */3], /* SetDefinedNodeKind */Block.__(2, [/* FunctionCallNode */0]));
                              })
                          }, "call"), match$7 ? null : React.createElement("a", {
                              onClick: (function (_event) {
                                  return Curry._1(self[/* send */3], /* AddValue */0);
                                })
                            }, "value"));
                }
                var match$8 = self[/* state */1][/* definedNodeKind */2];
                var tmp$2;
                if (match$8 !== undefined) {
                  var definedNodeKind = match$8;
                  var display = Definition$ReactTemplate.displayNode(/* record */[
                        /* scope */getScope(nib, nodes),
                        /* kind : DefinedNode */Block.__(1, [/* record */[
                              /* kind */definedNodeKind,
                              /* definitionID */definitionID
                            ]])
                      ], definitions, "en");
                  var match$9 = nib[/* isSource */1];
                  var match$10 = nib[/* isSource */1];
                  tmp$2 = React.createElement("div", undefined, React.createElement("h3", undefined, match$9 ? "Input" : "Output"), Belt_List.toArray(Belt_List.map(match$10 ? display[/* inputs */0] : display[/* outputs */1], (function (displayNib) {
                                  return React.createElement("a", {
                                              key: SimpleNode$ReactTemplate.nibKey(displayNib[/* nib */1]),
                                              onClick: (function (_event) {
                                                  return Curry._1(emit, /* AddNode */Block.__(4, [/* record */[
                                                                  /* node : record */[
                                                                    /* scope */getScope(nib, nodes),
                                                                    /* kind : DefinedNode */Block.__(1, [/* record */[
                                                                          /* kind */definedNodeKind,
                                                                          /* definitionID */definitionID
                                                                        ]])
                                                                  ],
                                                                  /* explicitConnectionSide */nib,
                                                                  /* connectionNib */displayNib[/* nib */1]
                                                                ]]));
                                                })
                                            }, maybeNameless(displayNib[/* name */0]));
                                }))));
                } else {
                  tmp$2 = null;
                }
                tmp = React.createElement(React.Fragment, undefined, React.createElement("div", undefined, React.createElement("h3", undefined, "Usage"), tmp$1), tmp$2);
              } else {
                tmp = null;
              }
              return React.createElement(React.Fragment, undefined, React.createElement("h2", undefined, "Create a node"), React.createElement("div", {
                              className: "type-selector-menu"
                            }, React.createElement("div", {
                                  className: "type-selector-categories"
                                }, React.createElement("h3", undefined, "Category"), renderCategory("Defined", /* AllCategory */1), match ? null : React.createElement(React.Fragment, undefined, React.createElement("a", {
                                            onClick: (function (_event) {
                                                return Curry._1(emit, /* AddNode */Block.__(4, [/* record */[
                                                                /* node : record */[
                                                                  /* scope */scope,
                                                                  /* kind : ReferenceNode */0
                                                                ],
                                                                /* explicitConnectionSide */nib,
                                                                /* connectionNib : ValueConnection */0
                                                              ]]));
                                              })
                                          }, "Reference"))), match$1 !== undefined ? (
                                match$1 ? nodeSelector((function (param) {
                                          return true;
                                        })) : nodeSelector(isNumberConstant)
                              ) : null, tmp));
            }),
          /* initialState */(function (param) {
              return /* record */[
                      /* category */undefined,
                      /* definitionID */undefined,
                      /* definedNodeKind */undefined
                    ];
            }),
          /* retainedProps */component[/* retainedProps */11],
          /* reducer */(function (action, state) {
              if (typeof action === "number") {
                if (nib[/* isSource */1]) {
                  return /* NoUpdate */0;
                } else {
                  return /* SideEffects */Block.__(1, [(function (param) {
                                var match = state[/* definitionID */1];
                                var tmp;
                                if (match !== undefined) {
                                  tmp = match;
                                } else {
                                  throw Caml_builtin_exceptions.not_found;
                                }
                                return Curry._1(emit, /* AddNode */Block.__(4, [/* record */[
                                                /* node : record */[
                                                  /* scope */getScope(nib, nodes),
                                                  /* kind : DefinedNode */Block.__(1, [/* record */[
                                                        /* kind : ValueNode */1,
                                                        /* definitionID */tmp
                                                      ]])
                                                ],
                                                /* explicitConnectionSide */nib,
                                                /* connectionNib : ValueConnection */0
                                              ]]));
                              })]);
                }
              } else {
                switch (action.tag | 0) {
                  case 0 : 
                      return /* Update */Block.__(0, [/* record */[
                                  /* category */action[0],
                                  /* definitionID */undefined,
                                  /* definedNodeKind */undefined
                                ]]);
                  case 1 : 
                      var definitionID = action[0];
                      var match = Belt_MapString.getExn(definitions, definitionID)[/* implementation */0];
                      if (match.tag) {
                        return /* Update */Block.__(0, [/* record */[
                                    /* category */state[/* category */0],
                                    /* definitionID */definitionID,
                                    /* definedNodeKind */undefined
                                  ]]);
                      } else if (nib[/* isSource */1]) {
                        return /* NoUpdate */0;
                      } else {
                        return /* SideEffects */Block.__(1, [(function (param) {
                                      return Curry._1(emit, /* AddNode */Block.__(4, [/* record */[
                                                      /* node : record */[
                                                        /* scope */getScope(nib, nodes),
                                                        /* kind : DefinedNode */Block.__(1, [/* record */[
                                                              /* kind : ValueNode */1,
                                                              /* definitionID */definitionID
                                                            ]])
                                                      ],
                                                      /* explicitConnectionSide */nib,
                                                      /* connectionNib : ValueConnection */0
                                                    ]]));
                                    })]);
                      }
                  case 2 : 
                      return /* Update */Block.__(0, [/* record */[
                                  /* category */state[/* category */0],
                                  /* definitionID */state[/* definitionID */1],
                                  /* definedNodeKind */action[0]
                                ]]);
                  
                }
              }
            }),
          /* jsElementWrapped */component[/* jsElementWrapped */13]
        ];
}

exports.isNumberConstant = isNumberConstant;
exports.getScope = getScope;
exports.maybeNameless = maybeNameless;
exports.canConnectToNib = canConnectToNib;
exports.component = component;
exports.make = make;
/* component Not a pure module */
