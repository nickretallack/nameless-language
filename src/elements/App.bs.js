// Generated by BUCKLESCRIPT VERSION 4.0.14, PLEASE EDIT WITH CARE
'use strict';

var Block = require("bs-platform/lib/js/block.js");
var Curry = require("bs-platform/lib/js/curry.js");
var React = require("react");
var Belt_List = require("bs-platform/lib/js/belt_List.js");
var Belt_Array = require("bs-platform/lib/js/belt_Array.js");
var ReasonReact = require("reason-react/src/ReasonReact.js");
var Belt_MapString = require("bs-platform/lib/js/belt_MapString.js");
var Graph$ReactTemplate = require("./Graph.bs.js");
var Helpers$ReactTemplate = require("../Helpers.bs.js");
var AppReducer$ReactTemplate = require("./AppReducer.bs.js");
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

function make(definitions, _children) {
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
              var match = self[/* state */1][/* definitionID */2];
              var tmp;
              if (match === "") {
                tmp = ReasonReact.element(undefined, undefined, DefinitionList$ReactTemplate.make(self[/* state */1][/* definitions */1], /* array */[]));
              } else {
                var definitionID = self[/* state */1][/* definitionID */2];
                var match$1 = Belt_MapString.get(self[/* state */1][/* definitions */1], definitionID);
                if (match$1 !== undefined) {
                  var definition = match$1;
                  var implementation = definition[/* implementation */0];
                  var emit = function (action) {
                    return Curry._1(self[/* send */3], /* DefinitionAction */Block.__(0, [/* record */[
                                    /* definitionID */definitionID,
                                    /* action */action
                                  ]]));
                  };
                  var match$2 = self[/* state */1][/* execution */0];
                  var stackFrame;
                  if (match$2 !== undefined) {
                    var execution = match$2;
                    var match$3 = Belt_List.headExn(execution[/* stack */1]);
                    var scope = Belt_MapString.getExn(execution[/* scopes */0], match$3[/* scopeID */0]);
                    stackFrame = /* record */[
                      /* scope */scope,
                      /* explicitConnectionSide */match$3[/* explicitConnectionSide */1],
                      /* action */match$3[/* action */2]
                    ];
                  } else {
                    stackFrame = undefined;
                  }
                  tmp = implementation.tag === 3 ? ReasonReact.element(definitionID, undefined, Graph$ReactTemplate.make(definitionID, self[/* state */1][/* definitions */1], implementation[0], definition, definition[/* display */2], definition[/* documentation */1], emit, self[/* state */1][/* error */3], stackFrame, /* array */[])) : ReasonReact.element(undefined, undefined, SimpleDefinition$ReactTemplate.make(definitionID, definition, definitions, self[/* state */1][/* error */3], emit, /* array */[]));
                } else {
                  tmp = "Not found";
                }
              }
              var match$4 = self[/* state */1][/* execution */0];
              var tmp$1;
              if (match$4 !== undefined) {
                var match$5 = match$4[/* result */2];
                tmp$1 = React.createElement("div", undefined, React.createElement("button", {
                          onClick: (function (param) {
                              return Curry._1(self[/* send */3], /* Step */0);
                            })
                        }, "step"), match$5 !== undefined ? "Result: " + Definition$ReactTemplate.displayValue(match$5) : null);
              } else {
                tmp$1 = null;
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
                              })), tmp, tmp$1);
            }),
          /* initialState */(function (param) {
              return /* record */[
                      /* execution */undefined,
                      /* definitions */definitions,
                      /* definitionID */ReasonReact.Router[/* dangerouslyGetInitialUrl */3](/* () */0)[/* hash */1],
                      /* error : NoAppError */0
                    ];
            }),
          /* retainedProps */component[/* retainedProps */11],
          /* reducer */AppReducer$ReactTemplate.reducer,
          /* jsElementWrapped */component[/* jsElementWrapped */13]
        ];
}

exports.makers = makers;
exports.component = component;
exports.make = make;
/* component Not a pure module */
