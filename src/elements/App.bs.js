// Generated by BUCKLESCRIPT VERSION 4.0.14, PLEASE EDIT WITH CARE
'use strict';

var List = require("bs-platform/lib/js/list.js");
var Block = require("bs-platform/lib/js/block.js");
var Curry = require("bs-platform/lib/js/curry.js");
var React = require("react");
var Belt_Map = require("bs-platform/lib/js/belt_Map.js");
var Belt_Array = require("bs-platform/lib/js/belt_Array.js");
var ReasonReact = require("reason-react/src/ReasonReact.js");
var Belt_MapString = require("bs-platform/lib/js/belt_MapString.js");
var Graph$ReactTemplate = require("./Graph.bs.js");
var Helpers$ReactTemplate = require("../Helpers.bs.js");
var Caml_builtin_exceptions = require("bs-platform/lib/js/caml_builtin_exceptions.js");
var Definition$ReactTemplate = require("../Definition.bs.js");
var DefinitionList$ReactTemplate = require("./DefinitionList.bs.js");
var SimpleDefinition$ReactTemplate = require("./SimpleDefinition.bs.js");

var makers = /* array */[
  /* tuple */[
    "Function",
    (function (param) {
        return Definition$ReactTemplate.makeGraph(undefined, undefined, undefined, /* array */[/* tuple */[
                      Helpers$ReactTemplate.randomID(/* () */0),
                      ""
                    ]], undefined, undefined, /* () */0);
      })
  ],
  /* tuple */[
    "Constant",
    (function (param) {
        return Definition$ReactTemplate.makeDefinition(undefined, undefined, undefined, undefined, /* ConstantImplementation */Block.__(0, [/* TextValue */Block.__(2, [""])]), /* () */0);
      })
  ],
  /* tuple */[
    "Interface",
    (function (param) {
        var outputID = Helpers$ReactTemplate.randomID(/* () */0);
        return Definition$ReactTemplate.makeDefinition(undefined, undefined, undefined, /* array */[/* tuple */[
                      outputID,
                      ""
                    ]], /* InterfaceImplementation */Block.__(1, [/* record */[
                        /* inputTypes */Belt_MapString.empty,
                        /* outputTypes */Belt_MapString.fromArray(/* array */[/* tuple */[
                                outputID,
                                /* PrimitiveValueType */Block.__(0, [/* TextType */2])
                              ]])
                      ]]), /* () */0);
      })
  ],
  /* tuple */[
    "Record",
    (function (param) {
        var inputID1 = Helpers$ReactTemplate.randomID(/* () */0);
        var inputID2 = Helpers$ReactTemplate.randomID(/* () */0);
        return Definition$ReactTemplate.makeDefinition(undefined, undefined, /* array */[
                    /* tuple */[
                      inputID1,
                      ""
                    ],
                    /* tuple */[
                      inputID2,
                      ""
                    ]
                  ], undefined, /* RecordTypeImplementation */Block.__(4, [Belt_MapString.fromArray(/* array */[
                            /* tuple */[
                              inputID1,
                              /* PrimitiveValueType */Block.__(0, [/* TextType */2])
                            ],
                            /* tuple */[
                              inputID2,
                              /* PrimitiveValueType */Block.__(0, [/* TextType */2])
                            ]
                          ])]), /* () */0);
      })
  ]
];

var component = ReasonReact.reducerComponent("App");

function make(size, definitions, _children) {
  return /* record */[
          /* debugName */component[/* debugName */0],
          /* reactClassInternal */component[/* reactClassInternal */1],
          /* handedOffState */component[/* handedOffState */2],
          /* willReceiveProps */component[/* willReceiveProps */3],
          /* didMount */(function (self) {
              var watcherId = ReasonReact.Router[/* watchUrl */1]((function (url) {
                      return Curry._1(self[/* send */3], /* ChangeRoute */Block.__(2, [url]));
                    }));
              return Curry._1(self[/* onUnmount */4], (function (param) {
                            return ReasonReact.Router[/* unwatchUrl */2](watcherId);
                          }));
            }),
          /* didUpdate */component[/* didUpdate */5],
          /* willUnmount */component[/* willUnmount */6],
          /* willUpdate */component[/* willUpdate */7],
          /* shouldUpdate */component[/* shouldUpdate */8],
          /* render */(function (self) {
              var match = self[/* state */1][/* definitionID */1];
              var tmp;
              if (match === "") {
                tmp = ReasonReact.element(undefined, undefined, DefinitionList$ReactTemplate.make(self[/* state */1][/* definitions */0], /* array */[]));
              } else {
                var definitionID = self[/* state */1][/* definitionID */1];
                var match$1 = Belt_MapString.get(self[/* state */1][/* definitions */0], definitionID);
                if (match$1 !== undefined) {
                  var definition = match$1;
                  var implementation = definition[/* implementation */0];
                  var emit = function (action) {
                    return Curry._1(self[/* send */3], /* DefinitionAction */Block.__(0, [/* record */[
                                    /* definitionID */definitionID,
                                    /* action */action
                                  ]]));
                  };
                  tmp = implementation.tag === 3 ? ReasonReact.element(definitionID, undefined, Graph$ReactTemplate.make(self[/* state */1][/* definitions */0], implementation[0], definition, definition[/* display */2], definition[/* documentation */1], size, emit, /* array */[])) : ReasonReact.element(undefined, undefined, SimpleDefinition$ReactTemplate.make(definition, definitions, emit, /* array */[]));
                } else {
                  tmp = "Not found";
                }
              }
              return React.createElement("div", undefined, React.createElement("a", {
                              href: "#"
                            }, "Library"), " New:", Belt_Array.mapWithIndex(makers, (function (index, param) {
                                var maker = param[1];
                                return React.createElement("a", {
                                            key: String(index),
                                            className: "maker",
                                            onClick: (function (_event) {
                                                return Curry._1(self[/* send */3], /* CreateDefinition */Block.__(1, [Curry._1(maker, /* () */0)]));
                                              })
                                          }, param[0]);
                              })), tmp);
            }),
          /* initialState */(function (param) {
              return /* record */[
                      /* definitions */definitions,
                      /* definitionID */ReasonReact.Router[/* dangerouslyGetInitialUrl */3](/* () */0)[/* hash */1]
                    ];
            }),
          /* retainedProps */component[/* retainedProps */11],
          /* reducer */(function (action, state) {
              switch (action.tag | 0) {
                case 0 : 
                    var match = action[0];
                    var action$1 = match[/* action */1];
                    var definitionID = match[/* definitionID */0];
                    var definition = Belt_MapString.getExn(state[/* definitions */0], definitionID);
                    var newDefinition;
                    if (typeof action$1 === "number") {
                      if (action$1 === 0) {
                        var nibID = Helpers$ReactTemplate.randomID(/* () */0);
                        var match$1 = definition[/* implementation */0];
                        var tmp;
                        if (match$1.tag === 1) {
                          var $$interface = match$1[0];
                          tmp = /* InterfaceImplementation */Block.__(1, [/* record */[
                                /* inputTypes */Belt_MapString.set($$interface[/* inputTypes */0], nibID, /* PrimitiveValueType */Block.__(0, [/* NumberType */1])),
                                /* outputTypes */$$interface[/* outputTypes */1]
                              ]]);
                        } else {
                          tmp = definition[/* implementation */0];
                        }
                        var init = definition[/* documentation */1];
                        var init$1 = definition[/* display */2];
                        newDefinition = /* record */[
                          /* implementation */tmp,
                          /* documentation : record */[
                            /* name */init[/* name */0],
                            /* description */init[/* description */1],
                            /* inputs */Belt_MapString.set(definition[/* documentation */1][/* inputs */2], nibID, Definition$ReactTemplate.emptyTranslatable),
                            /* outputs */init[/* outputs */3]
                          ],
                          /* display : record */[
                            /* inputOrdering */List.append(definition[/* display */2][/* inputOrdering */0], /* :: */[
                                  nibID,
                                  /* [] */0
                                ]),
                            /* outputOrdering */init$1[/* outputOrdering */1]
                          ]
                        ];
                      } else {
                        var nibID$1 = Helpers$ReactTemplate.randomID(/* () */0);
                        var match$2 = definition[/* implementation */0];
                        var tmp$1;
                        if (match$2.tag === 1) {
                          var $$interface$1 = match$2[0];
                          tmp$1 = /* InterfaceImplementation */Block.__(1, [/* record */[
                                /* inputTypes */$$interface$1[/* inputTypes */0],
                                /* outputTypes */Belt_MapString.set($$interface$1[/* outputTypes */1], nibID$1, /* PrimitiveValueType */Block.__(0, [/* NumberType */1]))
                              ]]);
                        } else {
                          tmp$1 = definition[/* implementation */0];
                        }
                        var init$2 = definition[/* documentation */1];
                        var init$3 = definition[/* display */2];
                        newDefinition = /* record */[
                          /* implementation */tmp$1,
                          /* documentation : record */[
                            /* name */init$2[/* name */0],
                            /* description */init$2[/* description */1],
                            /* inputs */init$2[/* inputs */2],
                            /* outputs */Belt_MapString.set(definition[/* documentation */1][/* outputs */3], nibID$1, Definition$ReactTemplate.emptyTranslatable)
                          ],
                          /* display : record */[
                            /* inputOrdering */init$3[/* inputOrdering */0],
                            /* outputOrdering */List.append(definition[/* display */2][/* outputOrdering */1], /* :: */[
                                  nibID$1,
                                  /* [] */0
                                ])
                          ]
                        ];
                      }
                    } else {
                      switch (action$1.tag | 0) {
                        case 0 : 
                            var match$3 = action$1[0];
                            var match$4 = definition[/* implementation */0];
                            if (match$4.tag === 3) {
                              var graphImplementation = match$4[0];
                              newDefinition = /* record */[
                                /* implementation : GraphImplementation */Block.__(3, [/* record */[
                                      /* connections */Belt_Map.set(graphImplementation[/* connections */0], match$3[/* sink */1], match$3[/* source */0]),
                                      /* nodes */graphImplementation[/* nodes */1]
                                    ]]),
                                /* documentation */definition[/* documentation */1],
                                /* display */definition[/* display */2]
                              ];
                            } else {
                              newDefinition = definition;
                            }
                            break;
                        case 1 : 
                            var init$4 = definition[/* documentation */1];
                            newDefinition = /* record */[
                              /* implementation */definition[/* implementation */0],
                              /* documentation : record */[
                                /* name */Definition$ReactTemplate.setTranslated(definition[/* documentation */1][/* name */0], "en", action$1[0]),
                                /* description */init$4[/* description */1],
                                /* inputs */init$4[/* inputs */2],
                                /* outputs */init$4[/* outputs */3]
                              ],
                              /* display */definition[/* display */2]
                            ];
                            break;
                        case 2 : 
                            var init$5 = definition[/* documentation */1];
                            newDefinition = /* record */[
                              /* implementation */definition[/* implementation */0],
                              /* documentation : record */[
                                /* name */Definition$ReactTemplate.setTranslated(definition[/* documentation */1][/* description */1], "en", action$1[0]),
                                /* description */init$5[/* description */1],
                                /* inputs */init$5[/* inputs */2],
                                /* outputs */init$5[/* outputs */3]
                              ],
                              /* display */definition[/* display */2]
                            ];
                            break;
                        case 3 : 
                            var match$5 = definition[/* implementation */0];
                            if (match$5.tag) {
                              throw Caml_builtin_exceptions.not_found;
                            } else {
                              newDefinition = /* record */[
                                /* implementation : ConstantImplementation */Block.__(0, [action$1[0]]),
                                /* documentation */definition[/* documentation */1],
                                /* display */definition[/* display */2]
                              ];
                            }
                            break;
                        case 4 : 
                            var match$6 = action$1[0];
                            var explicitConnectionSide = match$6[/* explicitConnectionSide */1];
                            var nodeID = Helpers$ReactTemplate.randomID(/* () */0);
                            var nodeConnectionSide_000 = /* node : NodeConnection */[nodeID];
                            var nodeConnectionSide_001 = /* nib */match$6[/* connectionNib */2];
                            var nodeConnectionSide = /* record */[
                              nodeConnectionSide_000,
                              nodeConnectionSide_001
                            ];
                            var match$7 = explicitConnectionSide[/* isSource */1];
                            var match$8 = match$7 ? /* tuple */[
                                explicitConnectionSide[/* connectionSide */0],
                                nodeConnectionSide
                              ] : /* tuple */[
                                nodeConnectionSide,
                                explicitConnectionSide[/* connectionSide */0]
                              ];
                            var match$9 = definition[/* implementation */0];
                            if (match$9.tag === 3) {
                              var graphImplementation$1 = match$9[0];
                              newDefinition = /* record */[
                                /* implementation : GraphImplementation */Block.__(3, [/* record */[
                                      /* connections */Belt_Map.set(graphImplementation$1[/* connections */0], match$8[1], match$8[0]),
                                      /* nodes */Belt_MapString.set(graphImplementation$1[/* nodes */1], nodeID, match$6[/* node */0])
                                    ]]),
                                /* documentation */definition[/* documentation */1],
                                /* display */definition[/* display */2]
                              ];
                            } else {
                              newDefinition = definition;
                            }
                            break;
                        case 5 : 
                            var match$10 = action$1[0];
                            var action$2 = match$10[/* action */2];
                            var isInput = match$10[/* isInput */1];
                            var nibID$2 = match$10[/* nibID */0];
                            if (action$2.tag) {
                              var valueType = action$2[0];
                              var match$11 = definition[/* implementation */0];
                              var tmp$2;
                              switch (match$11.tag | 0) {
                                case 1 : 
                                    var $$interface$2 = match$11[0];
                                    tmp$2 = /* InterfaceImplementation */Block.__(1, [isInput ? /* record */[
                                            /* inputTypes */Definition$ReactTemplate.changeTypedFields($$interface$2[/* inputTypes */0], nibID$2, valueType),
                                            /* outputTypes */$$interface$2[/* outputTypes */1]
                                          ] : /* record */[
                                            /* inputTypes */$$interface$2[/* inputTypes */0],
                                            /* outputTypes */Definition$ReactTemplate.changeTypedFields($$interface$2[/* outputTypes */1], nibID$2, valueType)
                                          ]]);
                                    break;
                                case 4 : 
                                    var tmp$3;
                                    if (isInput) {
                                      tmp$3 = Definition$ReactTemplate.changeTypedFields(match$11[0], nibID$2, valueType);
                                    } else {
                                      throw Caml_builtin_exceptions.not_found;
                                    }
                                    tmp$2 = /* RecordTypeImplementation */Block.__(4, [tmp$3]);
                                    break;
                                default:
                                  throw Caml_builtin_exceptions.not_found;
                              }
                              newDefinition = /* record */[
                                /* implementation */tmp$2,
                                /* documentation */definition[/* documentation */1],
                                /* display */definition[/* display */2]
                              ];
                            } else {
                              var nibs = isInput ? definition[/* documentation */1][/* inputs */2] : definition[/* documentation */1][/* outputs */3];
                              var nib = Belt_MapString.getExn(nibs, nibID$2);
                              var newNib = Definition$ReactTemplate.setTranslated(nib, "en", action$2[0]);
                              var newNibs = Belt_MapString.set(nibs, nibID$2, newNib);
                              var documentation;
                              if (isInput) {
                                var init$6 = definition[/* documentation */1];
                                documentation = /* record */[
                                  /* name */init$6[/* name */0],
                                  /* description */init$6[/* description */1],
                                  /* inputs */newNibs,
                                  /* outputs */init$6[/* outputs */3]
                                ];
                              } else {
                                var init$7 = definition[/* documentation */1];
                                documentation = /* record */[
                                  /* name */init$7[/* name */0],
                                  /* description */init$7[/* description */1],
                                  /* inputs */init$7[/* inputs */2],
                                  /* outputs */newNibs
                                ];
                              }
                              newDefinition = /* record */[
                                /* implementation */definition[/* implementation */0],
                                /* documentation */documentation,
                                /* display */definition[/* display */2]
                              ];
                            }
                            break;
                        
                      }
                    }
                    return /* Update */Block.__(0, [/* record */[
                                /* definitions */Belt_MapString.set(state[/* definitions */0], definitionID, newDefinition),
                                /* definitionID */state[/* definitionID */1]
                              ]]);
                case 1 : 
                    var definitionID$1 = Helpers$ReactTemplate.randomID(/* () */0);
                    return /* UpdateWithSideEffects */Block.__(2, [
                              /* record */[
                                /* definitions */Belt_MapString.set(state[/* definitions */0], definitionID$1, action[0]),
                                /* definitionID */state[/* definitionID */1]
                              ],
                              (function (param) {
                                  return ReasonReact.Router[/* push */0]("#" + definitionID$1);
                                })
                            ]);
                case 2 : 
                    return /* Update */Block.__(0, [/* record */[
                                /* definitions */state[/* definitions */0],
                                /* definitionID */action[0][/* hash */1]
                              ]]);
                
              }
            }),
          /* jsElementWrapped */component[/* jsElementWrapped */13]
        ];
}

exports.makers = makers;
exports.component = component;
exports.make = make;
/* component Not a pure module */
