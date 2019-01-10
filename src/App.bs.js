// Generated by BUCKLESCRIPT VERSION 4.0.14, PLEASE EDIT WITH CARE
'use strict';

var List = require("bs-platform/lib/js/list.js");
var Block = require("bs-platform/lib/js/block.js");
var Curry = require("bs-platform/lib/js/curry.js");
var Belt_Map = require("bs-platform/lib/js/belt_Map.js");
var ReasonReact = require("reason-react/src/ReasonReact.js");
var Belt_MapString = require("bs-platform/lib/js/belt_MapString.js");
var Graph$ReactTemplate = require("./Graph.bs.js");
var Helpers$ReactTemplate = require("./Helpers.bs.js");
var Caml_builtin_exceptions = require("bs-platform/lib/js/caml_builtin_exceptions.js");
var Definition$ReactTemplate = require("./Definition.bs.js");
var DefinitionList$ReactTemplate = require("./DefinitionList.bs.js");
var ConstantDefinition$ReactTemplate = require("./ConstantDefinition.bs.js");

var component = ReasonReact.reducerComponent("App");

function make(size, definitions, _children) {
  return /* record */[
          /* debugName */component[/* debugName */0],
          /* reactClassInternal */component[/* reactClassInternal */1],
          /* handedOffState */component[/* handedOffState */2],
          /* willReceiveProps */component[/* willReceiveProps */3],
          /* didMount */(function (self) {
              var watcherId = ReasonReact.Router[/* watchUrl */1]((function (url) {
                      return Curry._1(self[/* send */3], /* ChangeRoute */Block.__(1, [url]));
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
              if (match === "") {
                return ReasonReact.element(undefined, undefined, DefinitionList$ReactTemplate.make(self[/* state */1][/* definitions */0], /* array */[]));
              } else {
                var definitionID = self[/* state */1][/* definitionID */1];
                var match$1 = Belt_MapString.get(self[/* state */1][/* definitions */0], definitionID);
                if (match$1 !== undefined) {
                  var definition = match$1;
                  var documentation = definition[/* documentation */1];
                  var implementation = definition[/* implementation */0];
                  var emit = function (action) {
                    return Curry._1(self[/* send */3], /* DefinitionAction */Block.__(0, [/* record */[
                                    /* definitionID */definitionID,
                                    /* action */action
                                  ]]));
                  };
                  switch (implementation.tag | 0) {
                    case 0 : 
                        return ReasonReact.element(undefined, undefined, ConstantDefinition$ReactTemplate.make(implementation[0], documentation, emit, /* array */[]));
                    case 3 : 
                        return ReasonReact.element(undefined, undefined, Graph$ReactTemplate.make(self[/* state */1][/* definitions */0], implementation[0], definition[/* display */2], documentation, size, emit, /* array */[]));
                    default:
                      return "TODO";
                  }
                } else {
                  return "Not found";
                }
              }
            }),
          /* initialState */(function (param) {
              return /* record */[
                      /* definitions */definitions,
                      /* definitionID */ReasonReact.Router[/* dangerouslyGetInitialUrl */3](/* () */0)[/* hash */1]
                    ];
            }),
          /* retainedProps */component[/* retainedProps */11],
          /* reducer */(function (action, state) {
              if (action.tag) {
                return /* Update */Block.__(0, [/* record */[
                            /* definitions */state[/* definitions */0],
                            /* definitionID */action[0][/* hash */1]
                          ]]);
              } else {
                var match = action[0];
                var action$1 = match[/* action */1];
                var definitionID = match[/* definitionID */0];
                var definition = Belt_MapString.getExn(state[/* definitions */0], definitionID);
                var newDefinition;
                if (typeof action$1 === "number") {
                  var nibID = Helpers$ReactTemplate.randomId(/* () */0);
                  var init = definition[/* documentation */1];
                  newDefinition = /* record */[
                    /* implementation */definition[/* implementation */0],
                    /* documentation : record */[
                      /* name */init[/* name */0],
                      /* description */init[/* description */1],
                      /* inputs */Belt_MapString.set(definition[/* documentation */1][/* inputs */2], nibID, Definition$ReactTemplate.emptyTranslatable),
                      /* outputs */Belt_MapString.set(definition[/* documentation */1][/* inputs */2], nibID, Definition$ReactTemplate.emptyTranslatable)
                    ],
                    /* display : record */[
                      /* inputOrdering */List.append(definition[/* display */2][/* inputOrdering */0], /* :: */[
                            nibID,
                            /* [] */0
                          ]),
                      /* outputOrdering */List.append(definition[/* display */2][/* outputOrdering */1], /* :: */[
                            nibID,
                            /* [] */0
                          ])
                    ]
                  ];
                } else {
                  switch (action$1.tag | 0) {
                    case 0 : 
                        var match$1 = action$1[0];
                        var match$2 = definition[/* implementation */0];
                        if (match$2.tag === 3) {
                          var graphImplementation = match$2[0];
                          newDefinition = /* record */[
                            /* implementation : GraphImplementation */Block.__(3, [/* record */[
                                  /* connections */Belt_Map.set(graphImplementation[/* connections */0], match$1[/* sink */1], match$1[/* source */0]),
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
                        var init$1 = definition[/* documentation */1];
                        newDefinition = /* record */[
                          /* implementation */definition[/* implementation */0],
                          /* documentation : record */[
                            /* name */Definition$ReactTemplate.setTranslated(definition[/* documentation */1][/* name */0], "en", action$1[0]),
                            /* description */init$1[/* description */1],
                            /* inputs */init$1[/* inputs */2],
                            /* outputs */init$1[/* outputs */3]
                          ],
                          /* display */definition[/* display */2]
                        ];
                        break;
                    case 2 : 
                        var init$2 = definition[/* documentation */1];
                        newDefinition = /* record */[
                          /* implementation */definition[/* implementation */0],
                          /* documentation : record */[
                            /* name */Definition$ReactTemplate.setTranslated(definition[/* documentation */1][/* description */1], "en", action$1[0]),
                            /* description */init$2[/* description */1],
                            /* inputs */init$2[/* inputs */2],
                            /* outputs */init$2[/* outputs */3]
                          ],
                          /* display */definition[/* display */2]
                        ];
                        break;
                    case 3 : 
                        var match$3 = definition[/* implementation */0];
                        if (match$3.tag) {
                          throw Caml_builtin_exceptions.not_found;
                        } else {
                          newDefinition = /* record */[
                            /* implementation : ConstantImplementation */Block.__(0, [action$1[0]]),
                            /* documentation */definition[/* documentation */1],
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
              }
            }),
          /* jsElementWrapped */component[/* jsElementWrapped */13]
        ];
}

exports.component = component;
exports.make = make;
/* component Not a pure module */
