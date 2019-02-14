// Generated by BUCKLESCRIPT VERSION 4.0.14, PLEASE EDIT WITH CARE
'use strict';

var Block = require("bs-platform/lib/js/block.js");
var Curry = require("bs-platform/lib/js/curry.js");
var React = require("react");
var Belt_Id = require("bs-platform/lib/js/belt_Id.js");
var Belt_Map = require("bs-platform/lib/js/belt_Map.js");
var Caml_obj = require("bs-platform/lib/js/caml_obj.js");
var Belt_List = require("bs-platform/lib/js/belt_List.js");
var Belt_Array = require("bs-platform/lib/js/belt_Array.js");
var Caml_option = require("bs-platform/lib/js/caml_option.js");
var ReasonReact = require("reason-react/src/ReasonReact.js");
var Belt_MapString = require("bs-platform/lib/js/belt_MapString.js");
var Belt_SetString = require("bs-platform/lib/js/belt_SetString.js");
var Caml_primitive = require("bs-platform/lib/js/caml_primitive.js");
var SvgNib$ReactTemplate = require("./SvgNib.bs.js");
var Helpers$ReactTemplate = require("../Helpers.bs.js");
var SvgNode$ReactTemplate = require("./SvgNode.bs.js");
var NodeMenu$ReactTemplate = require("./NodeMenu.bs.js");
var Caml_builtin_exceptions = require("bs-platform/lib/js/caml_builtin_exceptions.js");
var Interface$ReactTemplate = require("./Interface.bs.js");
var Definition$ReactTemplate = require("../Definition.bs.js");
var LayoutGraph$ReactTemplate = require("../display/LayoutGraph.bs.js");
var DetectCycles$ReactTemplate = require("../edit/DetectCycles.bs.js");
var GraphActions$ReactTemplate = require("../edit/GraphActions.bs.js");
var SvgConnection$ReactTemplate = require("./SvgConnection.bs.js");
var ColumnizeNodes$ReactTemplate = require("../display/ColumnizeNodes.bs.js");
var DefinitionHeader$ReactTemplate = require("./DefinitionHeader.bs.js");
var SvgDefinitionBox$ReactTemplate = require("./SvgDefinitionBox.bs.js");

var cmp = Caml_obj.caml_compare;

var PointerComparator = Belt_Id.MakeComparable(/* module */[/* cmp */cmp]);

var $$document = document;

function preventDefault($$event) {
  $$event.preventDefault();
  return /* () */0;
}

var component = ReasonReact.reducerComponent("Graph");

function make(definitionID, definitions, implementation, definition, display, documentation, emit, error, _children) {
  return /* record */[
          /* debugName */component[/* debugName */0],
          /* reactClassInternal */component[/* reactClassInternal */1],
          /* handedOffState */component[/* handedOffState */2],
          /* willReceiveProps */component[/* willReceiveProps */3],
          /* didMount */(function (param) {
              $$document.addEventListener("touchmove", preventDefault, {
                    passive: false,
                    capture: true,
                    once: false
                  });
              return /* () */0;
            }),
          /* didUpdate */component[/* didUpdate */5],
          /* willUnmount */(function (param) {
              $$document.removeEventListener("touchmove", preventDefault);
              return /* () */0;
            }),
          /* willUpdate */component[/* willUpdate */7],
          /* shouldUpdate */component[/* shouldUpdate */8],
          /* render */(function (self) {
              console.log(Definition$ReactTemplate.encodeGraphImplementation(implementation));
              var columnizedNodes = ColumnizeNodes$ReactTemplate.columnizeNodes(implementation[/* nodes */2], implementation[/* connections */1]);
              var scopedNodeIDs = Belt_MapString.reduce(implementation[/* nodes */2], Belt_Map.make(Definition$ReactTemplate.ScopeComparator), (function (scopes, id, node) {
                      return Belt_Map.update(scopes, node[/* scope */0], (function (nodeIDs) {
                                    return Caml_option.some(Belt_SetString.add(nodeIDs !== undefined ? Caml_option.valFromOption(nodeIDs) : Belt_SetString.empty, id));
                                  }));
                    }));
              var match = LayoutGraph$ReactTemplate.layoutGraph(scopedNodeIDs, columnizedNodes, definitions, implementation[/* connections */1]);
              var graphSize = match[1];
              var nodeLayouts = match[0];
              var graphSize_000 = /* columns */graphSize[/* columns */0];
              var graphSize_001 = /* rows */Caml_primitive.caml_int_max(graphSize[/* rows */1], Caml_primitive.caml_int_max(Belt_List.length(definition[/* display */2][/* inputOrdering */0]), Belt_List.length(definition[/* display */2][/* outputOrdering */1])));
              var graphSize$1 = /* record */[
                graphSize_000,
                graphSize_001
              ];
              var columnWidth = 120.0 + 60.0;
              var getNodePosition = function (nodeID) {
                var position = Belt_MapString.getExn(nodeLayouts, nodeID)[/* position */0];
                return /* record */[
                        /* x */(position[/* columns */0] + 1 | 0) * columnWidth,
                        /* y */position[/* rows */1] * 20.0
                      ];
              };
              var sizeToPixels = function (size) {
                return /* record */[
                        /* x */size[/* columns */0] * columnWidth - 60.0,
                        /* y */size[/* rows */1] * 20.0
                      ];
              };
              var graphSizePixels = sizeToPixels(graphSize$1);
              var getNodeSize = function (nodeID) {
                return sizeToPixels(Belt_MapString.getExn(nodeLayouts, nodeID)[/* size */1]);
              };
              var isNibInternal = function (node, nib) {
                var match = node[/* kind */1];
                if (typeof match === "number" || !(match.tag && !(match[0][/* kind */0] !== 3 || typeof nib === "number" || nib.tag))) {
                  return false;
                } else {
                  return true;
                }
              };
              var nibPositionFormula = function (isInternal, isSink, nodePosition, nodeSize, nibIndex) {
                var rightSide = isInternal ? !isSink : isSink;
                return /* record */[
                        /* x */nodePosition[/* x */0] + (
                          isInternal ? (
                              rightSide ? nodeSize[/* x */0] - 120.0 : 120.0
                            ) : (
                              rightSide ? nodeSize[/* x */0] : 0.0
                            )
                        ),
                        /* y */nibIndex * 20.0 + 20.0 / 2.0 + nodePosition[/* y */1]
                      ];
              };
              var getNibPosition = function (connectionSide, isSink) {
                var match = connectionSide[/* node */0];
                if (match) {
                  var nodeID = match[0];
                  var nodePosition = getNodePosition(nodeID);
                  var nodeSize = getNodeSize(nodeID);
                  var node = Belt_MapString.getExn(implementation[/* nodes */2], nodeID);
                  var isInternal = isNibInternal(node, connectionSide[/* nib */1]);
                  var nibIndex = Definition$ReactTemplate.getNodeNibIndex(node, definitions, connectionSide[/* nib */1], isSink);
                  return nibPositionFormula(isInternal, isSink, nodePosition, nodeSize, nibIndex);
                } else {
                  var nibIndex$1 = Helpers$ReactTemplate.findByIndexExn(isSink ? definition[/* display */2][/* outputOrdering */1] : definition[/* display */2][/* inputOrdering */0], (function (nibID) {
                          return Caml_obj.caml_equal(/* NibConnection */Block.__(0, [nibID]), connectionSide[/* nib */1]);
                        }));
                  return nibPositionFormula(true, isSink, /* record */[
                              /* x */0.0,
                              /* y */20.0
                            ], graphSizePixels, nibIndex$1);
                }
              };
              var getNibNudge = function (source) {
                var match = source[/* node */0];
                if (match) {
                  var node = Belt_MapString.getExn(implementation[/* nodes */2], match[0]);
                  return Definition$ReactTemplate.getOutputIndex(node, definitions, source[/* nib */1]);
                } else {
                  var match$1 = source[/* nib */1];
                  if (typeof match$1 === "number") {
                    throw Caml_builtin_exceptions.not_found;
                  } else if (match$1.tag) {
                    throw Caml_builtin_exceptions.not_found;
                  } else {
                    return Helpers$ReactTemplate.findIndexExn(display[/* inputOrdering */0], match$1[0]);
                  }
                }
              };
              var allNibs = Definition$ReactTemplate.collectAllGraphNibs(definition, definitions);
              var renderedConnections = Helpers$ReactTemplate.renderMap((function (param) {
                      var source = param[1];
                      var sink = param[0];
                      return ReasonReact.element(Definition$ReactTemplate.connectionSideToString(sink), undefined, SvgConnection$ReactTemplate.make(getNibPosition(source, false), getNibPosition(sink, true), Caml_obj.caml_equal(self[/* state */1][/* selection */2], /* SelectedConnection */Block.__(0, [sink])), getNibNudge(source), undefined, (function (_event) {
                                        return Curry._1(self[/* send */3], /* SelectConnection */Block.__(1, [sink]));
                                      }), /* array */[]));
                    }), implementation[/* connections */1]);
              var renderedNibs = Belt_Array.map(Belt_List.toArray(allNibs), (function (param) {
                      var explicitConnectionSide = param[/* explicitConnectionSide */1];
                      var isSource = explicitConnectionSide[/* isSource */1];
                      var connectionSide = explicitConnectionSide[/* connectionSide */0];
                      var match = self[/* state */1][/* selection */2];
                      var tmp;
                      tmp = typeof match === "number" || match.tag !== 1 ? false : Caml_obj.caml_equal(match[0], explicitConnectionSide);
                      return ReasonReact.element(Definition$ReactTemplate.explicitConnectionSideKey(explicitConnectionSide), undefined, SvgNib$ReactTemplate.make(isSource, connectionSide, getNibPosition(connectionSide, !isSource), param[/* name */0], self[/* send */3], tmp, /* array */[]));
                    }));
              var renderedSides = ReasonReact.element(undefined, undefined, SvgDefinitionBox$ReactTemplate.make(Definition$ReactTemplate.getDisplayName(definition, "en"), /* record */[
                        /* x */0.0,
                        /* y */0.0
                      ], graphSizePixels, 120.0, 20.0, undefined, undefined, undefined, undefined, undefined, undefined, undefined, /* array */[]));
              var renderedNodes = Belt_List.toArray(Belt_List.map(Helpers$ReactTemplate.sortBy(Belt_MapString.toList(implementation[/* nodes */2]), (function (param) {
                              return Belt_MapString.getExn(nodeLayouts, param[0])[/* depth */2];
                            })), (function (param) {
                          var nodeID = param[0];
                          var match = self[/* state */1][/* selection */2];
                          var tmp;
                          tmp = typeof match === "number" || match.tag !== 2 ? false : Belt_SetString.has(match[0], nodeID);
                          return ReasonReact.element(nodeID, undefined, SvgNode$ReactTemplate.make(param[1], definitions, getNodePosition(nodeID), getNodeSize(nodeID), 120.0, 20.0, tmp, (function ($$event) {
                                            return Curry._1(self[/* send */3], /* SelectNode */Block.__(2, [/* record */[
                                                            /* nodeID */nodeID,
                                                            /* additive */$$event.shiftKey
                                                          ]]));
                                          }), (function (param) {
                                            return Curry._1(self[/* send */3], /* PointerAction */Block.__(0, [/* record */[
                                                            /* pointerID : Mouse */0,
                                                            /* action : StartDragging */Block.__(3, [nodeID])
                                                          ]]));
                                          }), undefined, (function (param) {
                                            return Curry._1(self[/* send */3], /* PointerAction */Block.__(0, [/* record */[
                                                            /* pointerID : Mouse */0,
                                                            /* action : FinishDragging */Block.__(4, [/* NodeScope */[nodeID]])
                                                          ]]));
                                          }), undefined, /* array */[]));
                        })));
              var renderedDrawingConnections = Helpers$ReactTemplate.renderMap((function (param) {
                      var pointerState = param[1];
                      if (pointerState.tag) {
                        return null;
                      } else {
                        var match = pointerState[0];
                        var point = match[/* point */1];
                        var match$1 = match[/* explicitConnectionSide */0];
                        var startIsSource = match$1[/* isSource */1];
                        var connectionSide = match$1[/* connectionSide */0];
                        var adjustedPoint_000 = /* x */point[/* x */0];
                        var adjustedPoint_001 = /* y */point[/* y */1] - 18.0;
                        var adjustedPoint = /* record */[
                          adjustedPoint_000,
                          adjustedPoint_001
                        ];
                        return ReasonReact.element(GraphActions$ReactTemplate.pointerIDToString(param[0]), undefined, SvgConnection$ReactTemplate.make(startIsSource ? getNibPosition(connectionSide, false) : adjustedPoint, startIsSource ? adjustedPoint : getNibPosition(connectionSide, true), undefined, undefined, undefined, undefined, /* array */[]));
                      }
                    }), self[/* state */1][/* pointers */0]);
              var match$1 = self[/* state */1][/* error */1];
              var match$2 = self[/* state */1][/* selection */2];
              var tmp;
              if (typeof match$2 === "number") {
                tmp = null;
              } else {
                switch (match$2.tag | 0) {
                  case 0 : 
                      var connectionSide = match$2[0];
                      tmp = React.createElement("button", {
                            onClick: (function (_event) {
                                return Curry._1(emit, /* RemoveConnection */Block.__(6, [connectionSide]));
                              })
                          }, "Remove connection");
                      break;
                  case 1 : 
                      tmp = ReasonReact.element(undefined, undefined, NodeMenu$ReactTemplate.make(definitions, implementation[/* nodes */2], match$2[0], emit, /* array */[]));
                      break;
                  case 2 : 
                      tmp = React.createElement("button", {
                            onClick: (function (_event) {
                                return Curry._1(self[/* send */3], /* RemoveSelectedNodes */0);
                              })
                          }, "Remove Node(s)");
                      break;
                  
                }
              }
              return React.createElement("div", undefined, React.createElement("svg", {
                              height: Helpers$ReactTemplate.pixels(graphSizePixels[/* y */1]),
                              width: Helpers$ReactTemplate.pixels(graphSizePixels[/* x */0]),
                              onMouseMove: (function ($$event) {
                                  $$event.preventDefault();
                                  return Curry._1(self[/* send */3], /* PointerAction */Block.__(0, [/* record */[
                                                  /* pointerID : Mouse */0,
                                                  /* action : ContinueDrawing */Block.__(1, [Helpers$ReactTemplate.pointFromMouse($$event)])
                                                ]]));
                                }),
                              onMouseUp: (function (param) {
                                  return Curry._1(self[/* send */3], /* PointerAction */Block.__(0, [/* record */[
                                                  /* pointerID : Mouse */0,
                                                  /* action : StopDrawing */0
                                                ]]));
                                }),
                              onTouchEnd: (function ($$event) {
                                  return Helpers$ReactTemplate.iterateTouches($$event, (function (touch) {
                                                return Curry._1(self[/* send */3], /* PointerAction */Block.__(0, [/* record */[
                                                                /* pointerID : Touch */[touch.identifier],
                                                                /* action : StopDrawing */0
                                                              ]]));
                                              }));
                                }),
                              onTouchMove: (function ($$event) {
                                  return Helpers$ReactTemplate.iterateTouches($$event, (function (touch) {
                                                return Curry._1(self[/* send */3], /* PointerAction */Block.__(0, [/* record */[
                                                                /* pointerID : Touch */[touch.identifier],
                                                                /* action : ContinueDrawing */Block.__(1, [/* record */[
                                                                      /* x */touch.clientX - $$event.currentTarget.offsetLeft,
                                                                      /* y */touch.clientY - $$event.currentTarget.offsetTop
                                                                    ]])
                                                              ]]));
                                              }));
                                })
                            }, renderedSides, renderedNodes, renderedConnections, renderedDrawingConnections, renderedNibs), ReasonReact.element(undefined, undefined, DefinitionHeader$ReactTemplate.make(definitionID, definitions, documentation, "(nameless graph)", emit, error, /* array */[])), match$1 !== undefined ? React.createElement("div", {
                                className: "error-message"
                              }, match$1) : null, tmp, React.createElement("h2", undefined, "Interface"), ReasonReact.element(undefined, undefined, Interface$ReactTemplate.make(definitions, implementation[/* interface */0], documentation, display, emit, /* array */[])));
            }),
          /* initialState */(function (param) {
              return /* record */[
                      /* pointers */Belt_Map.make(PointerComparator),
                      /* error */undefined,
                      /* selection : NoSelection */0
                    ];
            }),
          /* retainedProps */component[/* retainedProps */11],
          /* reducer */(function (action, state) {
              if (typeof action === "number") {
                var match = state[/* selection */2];
                if (typeof match === "number" || match.tag !== 2) {
                  return /* NoUpdate */0;
                } else {
                  var nodeIDs = match[0];
                  return /* UpdateWithSideEffects */Block.__(2, [
                            /* record */[
                              /* pointers */state[/* pointers */0],
                              /* error */state[/* error */1],
                              /* selection : NoSelection */0
                            ],
                            (function (param) {
                                return Curry._1(emit, /* RemoveNodes */Block.__(7, [nodeIDs]));
                              })
                          ]);
                }
              } else {
                switch (action.tag | 0) {
                  case 0 : 
                      var match$1 = action[0];
                      var action$1 = match$1[/* action */1];
                      var pointerID = match$1[/* pointerID */0];
                      if (typeof action$1 === "number") {
                        var match$2 = Belt_Map.get(state[/* pointers */0], pointerID);
                        if (match$2 !== undefined) {
                          var match$3 = match$2;
                          if (match$3.tag) {
                            var nodeID = match$3[0];
                            return /* UpdateWithSideEffects */Block.__(2, [
                                      /* record */[
                                        /* pointers */Belt_Map.remove(state[/* pointers */0], pointerID),
                                        /* error */state[/* error */1],
                                        /* selection */state[/* selection */2]
                                      ],
                                      (function (param) {
                                          return Curry._1(emit, /* ChangeNodeScope */Block.__(8, [/* record */[
                                                          /* nodeID */nodeID,
                                                          /* nodeScope : GraphScope */0
                                                        ]]));
                                        })
                                    ]);
                          } else {
                            return /* Update */Block.__(0, [/* record */[
                                        /* pointers */Belt_Map.remove(state[/* pointers */0], pointerID),
                                        /* error */state[/* error */1],
                                        /* selection */state[/* selection */2]
                                      ]]);
                          }
                        } else {
                          return /* NoUpdate */0;
                        }
                      } else {
                        switch (action$1.tag | 0) {
                          case 0 : 
                              return /* Update */Block.__(0, [/* record */[
                                          /* pointers */Belt_Map.set(state[/* pointers */0], pointerID, /* DrawingConnection */Block.__(0, [action$1[0]])),
                                          /* error */state[/* error */1],
                                          /* selection */state[/* selection */2]
                                        ]]);
                          case 1 : 
                              var match$4 = Belt_Map.get(state[/* pointers */0], pointerID);
                              if (match$4 !== undefined) {
                                var match$5 = match$4;
                                if (match$5.tag) {
                                  return /* NoUpdate */0;
                                } else {
                                  return /* Update */Block.__(0, [/* record */[
                                              /* pointers */Belt_Map.set(state[/* pointers */0], pointerID, /* DrawingConnection */Block.__(0, [/* record */[
                                                        /* explicitConnectionSide */match$5[0][/* explicitConnectionSide */0],
                                                        /* point */action$1[0]
                                                      ]])),
                                              /* error */state[/* error */1],
                                              /* selection */state[/* selection */2]
                                            ]]);
                                }
                              } else {
                                return /* NoUpdate */0;
                              }
                          case 2 : 
                              var match$6 = action$1[0];
                              var endNib = match$6[/* connectionSide */0];
                              var match$7 = Belt_Map.get(state[/* pointers */0], pointerID);
                              if (match$7 !== undefined) {
                                var match$8 = match$7;
                                if (match$8.tag) {
                                  return /* NoUpdate */0;
                                } else {
                                  var match$9 = match$8[0][/* explicitConnectionSide */0];
                                  var startIsSource = match$9[/* isSource */1];
                                  var startNib = match$9[/* connectionSide */0];
                                  if (startIsSource === match$6[/* isSource */1]) {
                                    if (Caml_obj.caml_equal(startNib, endNib)) {
                                      return /* Update */Block.__(0, [/* record */[
                                                  /* pointers */state[/* pointers */0],
                                                  /* error */undefined,
                                                  /* selection : SelectedNib */Block.__(1, [/* record */[
                                                        /* connectionSide */startNib,
                                                        /* isSource */startIsSource
                                                      ]])
                                                ]]);
                                    } else {
                                      return /* Update */Block.__(0, [/* record */[
                                                  /* pointers */state[/* pointers */0],
                                                  /* error */startIsSource ? "Can't connect a source to a source" : "Can't connect a sink to a sink",
                                                  /* selection */state[/* selection */2]
                                                ]]);
                                    }
                                  } else {
                                    var match$10 = startIsSource ? /* tuple */[
                                        startNib,
                                        endNib
                                      ] : /* tuple */[
                                        endNib,
                                        startNib
                                      ];
                                    var sink = match$10[1];
                                    var source = match$10[0];
                                    if (DetectCycles$ReactTemplate.checkScopes(source, sink, implementation[/* nodes */2])) {
                                      if (DetectCycles$ReactTemplate.detectCycles(Belt_Map.set(implementation[/* connections */1], sink, source), implementation[/* nodes */2])) {
                                        return /* Update */Block.__(0, [/* record */[
                                                    /* pointers */state[/* pointers */0],
                                                    /* error */"Can't create cycles",
                                                    /* selection */state[/* selection */2]
                                                  ]]);
                                      } else {
                                        return /* UpdateWithSideEffects */Block.__(2, [
                                                  /* record */[
                                                    /* pointers */Belt_Map.remove(state[/* pointers */0], pointerID),
                                                    /* error */undefined,
                                                    /* selection */state[/* selection */2]
                                                  ],
                                                  (function (param) {
                                                      return Curry._1(emit, /* CreateConnection */Block.__(0, [/* record */[
                                                                      /* source */source,
                                                                      /* sink */sink
                                                                    ]]));
                                                    })
                                                ]);
                                      }
                                    } else {
                                      return /* Update */Block.__(0, [/* record */[
                                                  /* pointers */state[/* pointers */0],
                                                  /* error */"When crossing scopes, you can only connect a source in a parent scope to a sink in a child scope.",
                                                  /* selection */state[/* selection */2]
                                                ]]);
                                    }
                                  }
                                }
                              } else {
                                return /* NoUpdate */0;
                              }
                          case 3 : 
                              return /* Update */Block.__(0, [/* record */[
                                          /* pointers */Belt_Map.set(state[/* pointers */0], pointerID, /* DraggingNode */Block.__(1, [action$1[0]])),
                                          /* error */state[/* error */1],
                                          /* selection */state[/* selection */2]
                                        ]]);
                          case 4 : 
                              var nodeScope = action$1[0];
                              var match$11 = Belt_Map.get(state[/* pointers */0], pointerID);
                              if (match$11 !== undefined) {
                                var match$12 = match$11;
                                if (match$12.tag) {
                                  var nodeID$1 = match$12[0];
                                  return /* SideEffects */Block.__(1, [(function (param) {
                                                return Curry._1(emit, /* ChangeNodeScope */Block.__(8, [/* record */[
                                                                /* nodeID */nodeID$1,
                                                                /* nodeScope */nodeScope
                                                              ]]));
                                              })]);
                                } else {
                                  return /* NoUpdate */0;
                                }
                              } else {
                                return /* NoUpdate */0;
                              }
                          
                        }
                      }
                  case 1 : 
                      return /* Update */Block.__(0, [/* record */[
                                  /* pointers */state[/* pointers */0],
                                  /* error */state[/* error */1],
                                  /* selection : SelectedConnection */Block.__(0, [action[0]])
                                ]]);
                  case 2 : 
                      var match$13 = action[0];
                      var nodeID$2 = match$13[/* nodeID */0];
                      var match$14 = state[/* selection */2];
                      var tmp;
                      if (typeof match$14 === "number") {
                        tmp = /* SelectedNodes */Block.__(2, [Belt_SetString.fromArray(/* array */[nodeID$2])]);
                      } else if (match$14.tag === 2) {
                        var nodeIDs$1 = match$14[0];
                        if (match$13[/* additive */1]) {
                          if (Belt_SetString.has(nodeIDs$1, nodeID$2)) {
                            var newNodeIDs = Belt_SetString.remove(nodeIDs$1, nodeID$2);
                            tmp = Belt_SetString.isEmpty(newNodeIDs) ? /* NoSelection */0 : /* SelectedNodes */Block.__(2, [newNodeIDs]);
                          } else {
                            tmp = /* SelectedNodes */Block.__(2, [Belt_SetString.add(nodeIDs$1, nodeID$2)]);
                          }
                        } else {
                          tmp = /* SelectedNodes */Block.__(2, [Belt_SetString.fromArray(/* array */[nodeID$2])]);
                        }
                      } else {
                        tmp = /* SelectedNodes */Block.__(2, [Belt_SetString.fromArray(/* array */[nodeID$2])]);
                      }
                      return /* Update */Block.__(0, [/* record */[
                                  /* pointers */state[/* pointers */0],
                                  /* error */state[/* error */1],
                                  /* selection */tmp
                                ]]);
                  
                }
              }
            }),
          /* jsElementWrapped */component[/* jsElementWrapped */13]
        ];
}

exports.PointerComparator = PointerComparator;
exports.$$document = $$document;
exports.preventDefault = preventDefault;
exports.component = component;
exports.make = make;
/* PointerComparator Not a pure module */
