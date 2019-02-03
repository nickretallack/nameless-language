// Generated by BUCKLESCRIPT VERSION 4.0.14, PLEASE EDIT WITH CARE
'use strict';

var $$Array = require("bs-platform/lib/js/array.js");
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
var Nib$ReactTemplate = require("./Nib.bs.js");
var Node$ReactTemplate = require("./Node.bs.js");
var SvgNib$ReactTemplate = require("./SvgNib.bs.js");
var Helpers$ReactTemplate = require("../Helpers.bs.js");
var NibsBox$ReactTemplate = require("./NibsBox.bs.js");
var SvgNode$ReactTemplate = require("./SvgNode.bs.js");
var NodeMenu$ReactTemplate = require("./NodeMenu.bs.js");
var Caml_builtin_exceptions = require("bs-platform/lib/js/caml_builtin_exceptions.js");
var Connection$ReactTemplate = require("./Connection.bs.js");
var Definition$ReactTemplate = require("../Definition.bs.js");
var SimpleNode$ReactTemplate = require("./SimpleNode.bs.js");
var LayoutGraph$ReactTemplate = require("../display/LayoutGraph.bs.js");
var DetectCycles$ReactTemplate = require("../edit/DetectCycles.bs.js");
var GraphActions$ReactTemplate = require("../edit/GraphActions.bs.js");
var SvgConnection$ReactTemplate = require("./SvgConnection.bs.js");
var ColumnizeNodes$ReactTemplate = require("../display/ColumnizeNodes.bs.js");

var cmp = Caml_obj.caml_compare;

var PointerComparator = Belt_Id.MakeComparable(/* module */[/* cmp */cmp]);

function renderNibs(nibs, className, isSource, connectionNode, emit, appEmit, selectedNib) {
  return $$Array.of_list(Belt_List.map(nibs, (function (param) {
                    var nib = param[/* nib */1];
                    var nameEditor = React.createElement("input", {
                          value: param[/* name */0],
                          onChange: (function ($$event) {
                              var tmp;
                              if (typeof nib === "number") {
                                throw Caml_builtin_exceptions.not_found;
                              } else if (nib.tag) {
                                throw Caml_builtin_exceptions.not_found;
                              } else {
                                tmp = nib[0];
                              }
                              return Curry._1(appEmit, /* NibAction */Block.__(5, [/* record */[
                                              /* nibID */tmp,
                                              /* isInput */isSource,
                                              /* action : ChangeNibName */Block.__(0, [Helpers$ReactTemplate.getEventValue($$event)])
                                            ]]));
                            })
                        });
                    return React.createElement("div", {
                                key: SimpleNode$ReactTemplate.nibKey(nib),
                                className: className
                              }, isSource ? null : nameEditor, ReasonReact.element(undefined, undefined, Nib$ReactTemplate.make(isSource, /* record */[
                                        /* node */connectionNode,
                                        /* nib */nib
                                      ], emit, Caml_obj.caml_equal(nib, selectedNib), /* array */[])), isSource ? nameEditor : null);
                  })));
}

var $$document = document;

function preventDefault($$event) {
  $$event.preventDefault();
  return /* () */0;
}

var component = ReasonReact.reducerComponent("Graph");

function make(definitions, implementation, definition, display, documentation, emit, _children) {
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
              var columns = ColumnizeNodes$ReactTemplate.columnizeNodes(implementation[/* nodes */1], implementation[/* connections */0]);
              var columnizedNodes = Belt_List.map(columns, (function (nodes) {
                      return Belt_List.map(Belt_List.fromArray(Belt_MapString.toArray(nodes)), (function (param) {
                                    return /* record */[
                                            /* id */param[0],
                                            /* node */param[1]
                                          ];
                                  }));
                    }));
              var scopedNodeIDs = Belt_MapString.reduce(implementation[/* nodes */1], Belt_Map.make(Definition$ReactTemplate.ScopeComparator), (function (scopes, id, node) {
                      return Belt_Map.update(scopes, node[/* scope */0], (function (nodeIDs) {
                                    return Caml_option.some(Belt_SetString.add(nodeIDs !== undefined ? Caml_option.valFromOption(nodeIDs) : Belt_SetString.empty, id));
                                  }));
                    }));
              var match = LayoutGraph$ReactTemplate.layoutGraph(scopedNodeIDs, columnizedNodes, definitions, implementation[/* connections */0]);
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
                  var node = Belt_MapString.getExn(implementation[/* nodes */1], nodeID);
                  var isInternal = isNibInternal(node, connectionSide[/* nib */1]);
                  var nibIndex = Definition$ReactTemplate.getNodeNibIndex(node, definitions, connectionSide[/* nib */1], isSink);
                  return nibPositionFormula(isInternal, isSink, nodePosition, nodeSize, nibIndex);
                } else {
                  var nibIndex$1 = Helpers$ReactTemplate.findByIndexExn(isSink ? definition[/* display */2][/* outputOrdering */1] : definition[/* display */2][/* inputOrdering */0], (function (nibID) {
                          return Caml_obj.caml_equal(/* NibConnection */Block.__(0, [nibID]), connectionSide[/* nib */1]);
                        }));
                  return nibPositionFormula(true, isSink, /* record */[
                              /* x */0.0,
                              /* y */0.0
                            ], graphSizePixels, nibIndex$1);
                }
              };
              var getNibNudge = function (source) {
                var match = source[/* node */0];
                if (match) {
                  var node = Belt_MapString.getExn(implementation[/* nodes */1], match[0]);
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
              var changeName = function ($$event) {
                return Curry._1(emit, /* ChangeName */Block.__(1, [Helpers$ReactTemplate.getEventValue($$event)]));
              };
              var selectedGraphNib = function (isSource) {
                var match = self[/* state */1][/* selectedNib */2];
                if (match !== undefined) {
                  var match$1 = match;
                  var connectionSide = match$1[/* connectionSide */0];
                  var match$2 = isSource !== match$1[/* isSource */1];
                  if (match$2) {
                    return undefined;
                  } else {
                    var match$3 = connectionSide[/* node */0];
                    if (match$3) {
                      return undefined;
                    } else {
                      return connectionSide[/* nib */1];
                    }
                  }
                }
                
              };
              var selectedGraphInputNib = selectedGraphNib(true);
              var selectedGraphOutputNib = selectedGraphNib(false);
              var allNibs = Definition$ReactTemplate.collectAllGraphNibs(definition, definitions);
              var renderedConnections = Helpers$ReactTemplate.renderMap((function (param) {
                      var source = param[1];
                      var sink = param[0];
                      return ReasonReact.element(Definition$ReactTemplate.connectionSideToString(sink), undefined, SvgConnection$ReactTemplate.make(getNibPosition(source, false), getNibPosition(sink, true), getNibNudge(source), undefined, /* array */[]));
                    }), implementation[/* connections */0]);
              var renderedNibs = Belt_Array.map(Belt_List.toArray(allNibs), (function (param) {
                      var explicitConnectionSide = param[/* explicitConnectionSide */1];
                      var isSource = explicitConnectionSide[/* isSource */1];
                      var connectionSide = explicitConnectionSide[/* connectionSide */0];
                      return ReasonReact.element(SimpleNode$ReactTemplate.explicitConnectionSideKey(explicitConnectionSide), undefined, SvgNib$ReactTemplate.make(isSource, connectionSide, getNibPosition(connectionSide, !isSource), param[/* name */0], self[/* send */3], false, /* array */[]));
                    }));
              var renderedSides = React.createElement(React.Fragment, undefined, ReasonReact.element(undefined, undefined, NibsBox$ReactTemplate.make(undefined, /* record */[
                            /* x */0.0,
                            /* y */0.0
                          ], graphSizePixels[/* y */1], 120.0, 20.0, /* array */[])), ReasonReact.element(undefined, undefined, NibsBox$ReactTemplate.make(undefined, /* record */[
                            /* x */graphSizePixels[/* x */0] - 120.0,
                            /* y */0.0
                          ], graphSizePixels[/* y */1], 120.0, 20.0, /* array */[])));
              var renderedNodes = Belt_List.toArray(Belt_List.map(Belt_MapString.toList(implementation[/* nodes */1]), (function (param) {
                          var nodeID = param[0];
                          return ReasonReact.element(nodeID, undefined, SvgNode$ReactTemplate.make(param[1], definitions, getNodePosition(nodeID), getNodeSize(nodeID), 120.0, 20.0, /* array */[]));
                        })));
              var match$1 = self[/* state */1][/* error */1];
              var match$2 = self[/* state */1][/* selectedNib */2];
              return React.createElement("div", undefined, React.createElement("svg", {
                              height: Helpers$ReactTemplate.pixels(graphSizePixels[/* y */1]),
                              width: Helpers$ReactTemplate.pixels(graphSizePixels[/* x */0])
                            }, renderedSides, renderedNodes, renderedConnections, renderedNibs), React.createElement("input", {
                              className: "graph-name",
                              placeholder: "(nameless function)",
                              type: "text",
                              value: Definition$ReactTemplate.getTranslated(documentation[/* name */0], "en"),
                              onChange: changeName
                            }), React.createElement("div", {
                              className: "graph",
                              style: {
                                height: Helpers$ReactTemplate.pixels(graphSizePixels[/* y */1]),
                                width: Helpers$ReactTemplate.pixels(graphSizePixels[/* x */0])
                              },
                              onMouseMove: (function ($$event) {
                                  $$event.preventDefault();
                                  return Curry._1(self[/* send */3], /* record */[
                                              /* pointerID : Mouse */0,
                                              /* action : ContinueDrawing */Block.__(1, [Helpers$ReactTemplate.pointFromMouse($$event)])
                                            ]);
                                }),
                              onMouseUp: (function (param) {
                                  return Curry._1(self[/* send */3], /* record */[
                                              /* pointerID : Mouse */0,
                                              /* action : StopDrawing */0
                                            ]);
                                }),
                              onTouchEnd: (function ($$event) {
                                  return Helpers$ReactTemplate.iterateTouches($$event, (function (touch) {
                                                return Curry._1(self[/* send */3], /* record */[
                                                            /* pointerID : Touch */[touch.identifier],
                                                            /* action : StopDrawing */0
                                                          ]);
                                              }));
                                }),
                              onTouchMove: (function ($$event) {
                                  return Helpers$ReactTemplate.iterateTouches($$event, (function (touch) {
                                                return Curry._1(self[/* send */3], /* record */[
                                                            /* pointerID : Touch */[touch.identifier],
                                                            /* action : ContinueDrawing */Block.__(1, [/* record */[
                                                                  /* x */touch.clientX - $$event.currentTarget.offsetLeft,
                                                                  /* y */touch.clientY - $$event.currentTarget.offsetTop
                                                                ]])
                                                          ]);
                                              }));
                                })
                            }, Helpers$ReactTemplate.renderMap((function (param) {
                                    var source = param[1];
                                    var sink = param[0];
                                    return ReasonReact.element(Definition$ReactTemplate.connectionSideToString(sink), undefined, Connection$ReactTemplate.make(getNibPosition(source, false), getNibPosition(sink, true), getNibNudge(source), undefined, /* array */[]));
                                  }), implementation[/* connections */0]), Helpers$ReactTemplate.renderMap((function (param) {
                                    var match = param[1];
                                    var point = match[/* point */1];
                                    var match$1 = match[/* explicitConnectionSide */0];
                                    var startIsSource = match$1[/* isSource */1];
                                    var connectionSide = match$1[/* connectionSide */0];
                                    var adjustedPoint_000 = /* x */point[/* x */0];
                                    var adjustedPoint_001 = /* y */point[/* y */1] - 40.0;
                                    var adjustedPoint = /* record */[
                                      adjustedPoint_000,
                                      adjustedPoint_001
                                    ];
                                    return ReasonReact.element(GraphActions$ReactTemplate.pointerIDToString(param[0]), undefined, Connection$ReactTemplate.make(startIsSource ? getNibPosition(connectionSide, false) : adjustedPoint, startIsSource ? adjustedPoint : getNibPosition(connectionSide, true), startIsSource ? getNibNudge(connectionSide) : 0, undefined, /* array */[]));
                                  }), self[/* state */1][/* pointers */0]), React.createElement("div", {
                                  className: "outputs"
                                }, renderNibs(Definition$ReactTemplate.displayKeywordOutputs(definition, "en"), "output internal", false, /* GraphConnection */0, self[/* send */3], emit, selectedGraphOutputNib), React.createElement("a", {
                                      onClick: (function (_event) {
                                          return Curry._1(emit, /* AddOutput */1);
                                        })
                                    }, "Add Output")), React.createElement("div", {
                                  className: "inputs"
                                }, renderNibs(Definition$ReactTemplate.displayKeywordInputs(definition, "en"), "input internal", true, /* GraphConnection */0, self[/* send */3], emit, selectedGraphInputNib), React.createElement("a", {
                                      onClick: (function (_event) {
                                          return Curry._1(emit, /* AddInput */0);
                                        })
                                    }, "Add Input")), Helpers$ReactTemplate.renderStringMap((function (param) {
                                    var nodeID = param[0];
                                    var match = self[/* state */1][/* selectedNib */2];
                                    var tmp;
                                    if (match !== undefined) {
                                      var connectionSide = match[/* connectionSide */0];
                                      var match$1 = connectionSide[/* node */0];
                                      if (match$1) {
                                        var match$2 = match$1[0] === nodeID;
                                        tmp = match$2 ? connectionSide[/* nib */1] : undefined;
                                      } else {
                                        tmp = undefined;
                                      }
                                    } else {
                                      tmp = undefined;
                                    }
                                    return ReasonReact.element(nodeID, undefined, Node$ReactTemplate.make(nodeID, param[1], definitions, getNodePosition(nodeID), getNodeSize(nodeID), Belt_MapString.getExn(nodeLayouts, nodeID)[/* depth */2], tmp, self[/* send */3], /* array */[]));
                                  }), implementation[/* nodes */1])), match$1 !== undefined ? React.createElement("div", {
                                className: "error-message"
                              }, match$1) : null, match$2 !== undefined ? ReasonReact.element(undefined, undefined, NodeMenu$ReactTemplate.make(definitions, implementation[/* nodes */1], match$2, emit, /* array */[])) : null);
            }),
          /* initialState */(function (param) {
              return /* record */[
                      /* pointers */Belt_Map.make(PointerComparator),
                      /* error */undefined,
                      /* selectedNib */undefined
                    ];
            }),
          /* retainedProps */component[/* retainedProps */11],
          /* reducer */(function (param, state) {
              var action = param[/* action */1];
              var pointerID = param[/* pointerID */0];
              if (typeof action === "number") {
                var match = Belt_Map.has(state[/* pointers */0], pointerID);
                if (match) {
                  return /* Update */Block.__(0, [/* record */[
                              /* pointers */Belt_Map.remove(state[/* pointers */0], pointerID),
                              /* error */state[/* error */1],
                              /* selectedNib */state[/* selectedNib */2]
                            ]]);
                } else {
                  return /* NoUpdate */0;
                }
              } else {
                switch (action.tag | 0) {
                  case 0 : 
                      return /* Update */Block.__(0, [/* record */[
                                  /* pointers */Belt_Map.set(state[/* pointers */0], pointerID, action[0]),
                                  /* error */state[/* error */1],
                                  /* selectedNib */state[/* selectedNib */2]
                                ]]);
                  case 1 : 
                      var match$1 = Belt_Map.get(state[/* pointers */0], pointerID);
                      if (match$1 !== undefined) {
                        return /* Update */Block.__(0, [/* record */[
                                    /* pointers */Belt_Map.set(state[/* pointers */0], pointerID, /* record */[
                                          /* explicitConnectionSide */match$1[/* explicitConnectionSide */0],
                                          /* point */action[0]
                                        ]),
                                    /* error */state[/* error */1],
                                    /* selectedNib */state[/* selectedNib */2]
                                  ]]);
                      } else {
                        return /* NoUpdate */0;
                      }
                  case 2 : 
                      var match$2 = action[0];
                      var endNib = match$2[/* connectionSide */0];
                      var match$3 = Belt_Map.get(state[/* pointers */0], pointerID);
                      if (match$3 !== undefined) {
                        var match$4 = match$3[/* explicitConnectionSide */0];
                        var startIsSource = match$4[/* isSource */1];
                        var startNib = match$4[/* connectionSide */0];
                        if (startIsSource === match$2[/* isSource */1]) {
                          if (Caml_obj.caml_equal(startNib, endNib)) {
                            return /* Update */Block.__(0, [/* record */[
                                        /* pointers */state[/* pointers */0],
                                        /* error */undefined,
                                        /* selectedNib *//* record */[
                                          /* connectionSide */startNib,
                                          /* isSource */startIsSource
                                        ]
                                      ]]);
                          } else {
                            return /* Update */Block.__(0, [/* record */[
                                        /* pointers */state[/* pointers */0],
                                        /* error */startIsSource ? "Can't connect a source to a source" : "Can't connect a sink to a sink",
                                        /* selectedNib */state[/* selectedNib */2]
                                      ]]);
                          }
                        } else {
                          var match$5 = startIsSource ? /* tuple */[
                              startNib,
                              endNib
                            ] : /* tuple */[
                              endNib,
                              startNib
                            ];
                          var sink = match$5[1];
                          var source = match$5[0];
                          if (DetectCycles$ReactTemplate.checkScopes(source, sink, implementation[/* nodes */1])) {
                            if (DetectCycles$ReactTemplate.detectCycles(Belt_Map.set(implementation[/* connections */0], sink, source), implementation[/* nodes */1])) {
                              return /* Update */Block.__(0, [/* record */[
                                          /* pointers */state[/* pointers */0],
                                          /* error */"Can't create cycles",
                                          /* selectedNib */state[/* selectedNib */2]
                                        ]]);
                            } else {
                              return /* UpdateWithSideEffects */Block.__(2, [
                                        /* record */[
                                          /* pointers */Belt_Map.remove(state[/* pointers */0], pointerID),
                                          /* error */undefined,
                                          /* selectedNib */state[/* selectedNib */2]
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
                                        /* selectedNib */state[/* selectedNib */2]
                                      ]]);
                          }
                        }
                      } else {
                        return /* NoUpdate */0;
                      }
                  
                }
              }
            }),
          /* jsElementWrapped */component[/* jsElementWrapped */13]
        ];
}

exports.PointerComparator = PointerComparator;
exports.renderNibs = renderNibs;
exports.$$document = $$document;
exports.preventDefault = preventDefault;
exports.component = component;
exports.make = make;
/* PointerComparator Not a pure module */
