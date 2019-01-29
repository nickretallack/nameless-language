// Generated by BUCKLESCRIPT VERSION 4.0.14, PLEASE EDIT WITH CARE
'use strict';

var List = require("bs-platform/lib/js/list.js");
var $$Array = require("bs-platform/lib/js/array.js");
var Block = require("bs-platform/lib/js/block.js");
var Curry = require("bs-platform/lib/js/curry.js");
var React = require("react");
var Belt_Id = require("bs-platform/lib/js/belt_Id.js");
var Belt_Map = require("bs-platform/lib/js/belt_Map.js");
var Caml_obj = require("bs-platform/lib/js/caml_obj.js");
var Belt_List = require("bs-platform/lib/js/belt_List.js");
var Caml_option = require("bs-platform/lib/js/caml_option.js");
var ReasonReact = require("reason-react/src/ReasonReact.js");
var Belt_MapString = require("bs-platform/lib/js/belt_MapString.js");
var Belt_SetString = require("bs-platform/lib/js/belt_SetString.js");
var Nib$ReactTemplate = require("./Nib.bs.js");
var Node$ReactTemplate = require("./Node.bs.js");
var Helpers$ReactTemplate = require("../Helpers.bs.js");
var Evaluate$ReactTemplate = require("../Evaluate.bs.js");
var NodeMenu$ReactTemplate = require("./NodeMenu.bs.js");
var Caml_builtin_exceptions = require("bs-platform/lib/js/caml_builtin_exceptions.js");
var Connection$ReactTemplate = require("./Connection.bs.js");
var Definition$ReactTemplate = require("../Definition.bs.js");
var LayoutGraph$ReactTemplate = require("../display/LayoutGraph.bs.js");
var DetectCycles$ReactTemplate = require("../edit/DetectCycles.bs.js");
var GraphActions$ReactTemplate = require("../edit/GraphActions.bs.js");
var ColumnizeNodes$ReactTemplate = require("../display/ColumnizeNodes.bs.js");

var cmp = Caml_obj.caml_compare;

var PointerComparator = Belt_Id.MakeComparable(/* module */[/* cmp */cmp]);

var $$document = document;

function preventDefault($$event) {
  $$event.preventDefault();
  return /* () */0;
}

var component = ReasonReact.reducerComponent("Graph");

function make(definitions, implementation, display, documentation, size, emit, _children) {
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
              var nodeLayouts = match[0];
              var columnWidth = 120.0 + 60.0;
              var yMargin = 20.0 * 4.0;
              var getNodePosition = function (nodeID) {
                var position = Belt_MapString.getExn(nodeLayouts, nodeID)[/* position */0];
                return /* record */[
                        /* x */position[/* columns */0] * columnWidth + columnWidth,
                        /* y */position[/* rows */1] * 20.0 + yMargin
                      ];
              };
              var getNodeSize = function (nodeID) {
                var size = Belt_MapString.getExn(nodeLayouts, nodeID)[/* size */1];
                return /* record */[
                        /* x */size[/* columns */0] * columnWidth - 60.0,
                        /* y */size[/* rows */1] * 20.0
                      ];
              };
              var nibPositions = function (nibIds, isInput) {
                var rowHeight = size[/* y */1] / (List.length(nibIds) + 1 | 0);
                return Belt_MapString.fromArray($$Array.of_list(List.mapi((function (index, nibID) {
                                      return /* tuple */[
                                              nibID,
                                              /* record */[
                                                /* x */isInput ? size[/* x */0] - 10.0 : 10.0,
                                                /* y */(index + 1 | 0) * rowHeight
                                              ]
                                            ];
                                    }), nibIds)));
              };
              var inputPositions = nibPositions(display[/* inputOrdering */0], true);
              var outputPositions = nibPositions(display[/* outputOrdering */1], false);
              var isNibInternal = function (node, nib) {
                var match = node[/* kind */1];
                if (typeof match === "number" || !(match.tag && !(match[0][/* kind */0] !== 3 || typeof nib === "number" || nib.tag))) {
                  return false;
                } else {
                  return true;
                }
              };
              var getNibPosition = function (connectionSide, isSink) {
                var match = connectionSide[/* node */0];
                if (match) {
                  var nodeID = match[0];
                  var nodePosition = getNodePosition(nodeID);
                  var nodeSize = getNodeSize(nodeID);
                  var node = Belt_MapString.getExn(implementation[/* nodes */1], nodeID);
                  var isInternal = isNibInternal(node, connectionSide[/* nib */1]);
                  var rightSide = isInternal ? !isSink : isSink;
                  return /* record */[
                          /* x */nodePosition[/* x */0] + (
                            isInternal ? (
                                rightSide ? nodeSize[/* x */0] - 120.0 : 120.0
                              ) : (
                                rightSide ? nodeSize[/* x */0] : 0.0
                              )
                          ),
                          /* y */Definition$ReactTemplate.getNodeNibIndex(node, definitions, connectionSide[/* nib */1], isSink) * 20.0 + 20.0 / 2.0 + nodePosition[/* y */1]
                        ];
                } else {
                  var match$1 = connectionSide[/* nib */1];
                  if (typeof match$1 === "number") {
                    throw Caml_builtin_exceptions.not_found;
                  } else if (match$1.tag) {
                    throw Caml_builtin_exceptions.not_found;
                  } else {
                    return Belt_MapString.getExn(isSink ? outputPositions : inputPositions, match$1[0]);
                  }
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
              var isGraphNibSelected = function (nibID, isSource) {
                var match = self[/* state */1][/* selectedNib */2];
                if (match !== undefined) {
                  var match$1 = match;
                  var connectionSide = match$1[/* connectionSide */0];
                  var match$2 = isSource === match$1[/* isSource */1];
                  if (match$2) {
                    var match$3 = connectionSide[/* node */0];
                    if (match$3) {
                      return false;
                    } else {
                      var match$4 = connectionSide[/* nib */1];
                      if (typeof match$4 === "number" || match$4.tag) {
                        return false;
                      } else {
                        return match$4[0] === nibID;
                      }
                    }
                  } else {
                    return false;
                  }
                } else {
                  return false;
                }
              };
              var match$1 = self[/* state */1][/* error */1];
              var match$2 = self[/* state */1][/* selectedNib */2];
              return React.createElement("div", undefined, React.createElement("input", {
                              className: "graph-name",
                              placeholder: "(nameless function)",
                              type: "text",
                              value: Definition$ReactTemplate.getTranslated(documentation[/* name */0], "en"),
                              onChange: changeName
                            }), match$1 !== undefined ? React.createElement("div", {
                                className: "error-message"
                              }, match$1) : null, React.createElement("div", {
                              className: "graph",
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
                                                                  /* x */touch.clientX,
                                                                  /* y */touch.clientY
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
                                    return ReasonReact.element(GraphActions$ReactTemplate.pointerIDToString(param[0]), undefined, Connection$ReactTemplate.make(startIsSource ? getNibPosition(connectionSide, false) : point, startIsSource ? point : getNibPosition(connectionSide, true), startIsSource ? getNibNudge(connectionSide) : 0, undefined, /* array */[]));
                                  }), self[/* state */1][/* pointers */0]), Belt_List.toArray(Belt_List.map(display[/* inputOrdering */0], (function (nibID) {
                                        var name = Definition$ReactTemplate.getTranslated(Belt_MapString.getExn(documentation[/* inputs */2], nibID), "en");
                                        return React.createElement("div", {
                                                    key: nibID,
                                                    className: "graph-input input",
                                                    style: {
                                                      right: Helpers$ReactTemplate.pixels(10.0),
                                                      top: Helpers$ReactTemplate.pixels(Belt_MapString.getExn(inputPositions, nibID)[/* y */1])
                                                    }
                                                  }, name, ReasonReact.element(undefined, undefined, Nib$ReactTemplate.make(true, /* record */[
                                                            /* node : GraphConnection */0,
                                                            /* nib : NibConnection */Block.__(0, [nibID])
                                                          ], self[/* send */3], isGraphNibSelected(nibID, true), /* array */[])));
                                      }))), Belt_List.toArray(Belt_List.map(display[/* outputOrdering */1], (function (nibID) {
                                        var name = Definition$ReactTemplate.getTranslated(Belt_MapString.getExn(documentation[/* outputs */3], nibID), "en");
                                        return React.createElement("div", {
                                                    key: nibID,
                                                    className: "graph-output output",
                                                    style: Helpers$ReactTemplate.positionStyle(Belt_MapString.getExn(outputPositions, nibID))
                                                  }, ReasonReact.element(undefined, undefined, Nib$ReactTemplate.make(false, /* record */[
                                                            /* node : GraphConnection */0,
                                                            /* nib : NibConnection */Block.__(0, [nibID])
                                                          ], self[/* send */3], isGraphNibSelected(nibID, false), /* array */[])), React.createElement("div", undefined, name), React.createElement("a", {
                                                        onClick: (function (_event) {
                                                            var outputID = nibID;
                                                            console.log(Evaluate$ReactTemplate.evaluateGraphOutput(definitions, implementation, outputID));
                                                            return /* () */0;
                                                          })
                                                      }, "Evaluate"));
                                      }))), Helpers$ReactTemplate.renderStringMap((function (param) {
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
                                    return ReasonReact.element(nodeID, undefined, Node$ReactTemplate.make(nodeID, param[1], definitions, getNodePosition(nodeID), getNodeSize(nodeID), tmp, self[/* send */3], /* array */[]));
                                  }), implementation[/* nodes */1])), match$2 !== undefined ? ReasonReact.element(undefined, undefined, NodeMenu$ReactTemplate.make(definitions, implementation[/* nodes */1], match$2, emit, /* array */[])) : null);
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
                        var match$5 = startIsSource !== match$2[/* isSource */1];
                        if (match$5) {
                          var match$6 = DetectCycles$ReactTemplate.detectCycles(Belt_Map.set(implementation[/* connections */0], startIsSource ? endNib : startNib, startIsSource ? startNib : endNib), implementation[/* nodes */1]);
                          if (match$6) {
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
                                                          /* source */startIsSource ? startNib : endNib,
                                                          /* sink */startIsSource ? endNib : startNib
                                                        ]]));
                                        })
                                    ]);
                          }
                        } else {
                          var match$7 = Caml_obj.caml_equal(startNib, endNib);
                          if (match$7) {
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
exports.$$document = $$document;
exports.preventDefault = preventDefault;
exports.component = component;
exports.make = make;
/* PointerComparator Not a pure module */
