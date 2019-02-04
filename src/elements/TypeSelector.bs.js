// Generated by BUCKLESCRIPT VERSION 4.0.14, PLEASE EDIT WITH CARE
'use strict';

var Block = require("bs-platform/lib/js/block.js");
var Curry = require("bs-platform/lib/js/curry.js");
var React = require("react");
var Belt_Array = require("bs-platform/lib/js/belt_Array.js");
var ReasonReact = require("reason-react/src/ReasonReact.js");
var Belt_MapString = require("bs-platform/lib/js/belt_MapString.js");
var Caml_builtin_exceptions = require("bs-platform/lib/js/caml_builtin_exceptions.js");
var Definition$ReactTemplate = require("../Definition.bs.js");

function categoryFromType(valueType, definitions) {
  if (typeof valueType === "number") {
    return /* AnyCategory */4;
  } else if (valueType.tag) {
    var definition = Belt_MapString.getExn(definitions, valueType[0]);
    var match = definition[/* implementation */0];
    switch (match.tag | 0) {
      case 1 : 
          return /* FunctionCategory */3;
      case 4 : 
          return /* RecordCategory */2;
      default:
        throw Caml_builtin_exceptions.not_found;
    }
  } else if (valueType[0] >= 2) {
    return /* TextCategory */1;
  } else {
    return /* NumberCategory */0;
  }
}

function hasDefinitionID(valueType, definitionID) {
  if (typeof valueType === "number" || !valueType.tag) {
    return false;
  } else {
    return definitionID === valueType[0];
  }
}

var component = ReasonReact.reducerComponent("TypeSelector");

function isRecordType(definition) {
  var match = definition[/* implementation */0];
  if (match.tag === 4) {
    return true;
  } else {
    return false;
  }
}

function isInterface(definition) {
  var match = definition[/* implementation */0];
  if (match.tag === 1) {
    return true;
  } else {
    return false;
  }
}

function make(valueType, definitions, changeType, _children) {
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
              var renderCategory = function (name, category) {
                var match = self[/* state */1][/* category */1] === category;
                return React.createElement("a", {
                            className: match ? "selected" : "",
                            onClick: (function (_event) {
                                return Curry._1(self[/* send */3], /* SelectCategory */[category]);
                              })
                          }, name);
              };
              var definedTypeSelector = function (name, filterFunction) {
                return React.createElement("div", {
                            className: "type-selector-choices"
                          }, React.createElement("h3", undefined, name + " Types"), Belt_Array.map(Belt_MapString.toArray(Belt_MapString.keep(definitions, (function (_definitionID, definition) {
                                          return Curry._1(filterFunction, definition);
                                        }))), (function (param) {
                                  var definitionID = param[0];
                                  var match = hasDefinitionID(valueType, definitionID);
                                  return React.createElement("a", {
                                              key: definitionID,
                                              className: match ? "selected" : "",
                                              onClick: (function (_event) {
                                                  return Curry._1(changeType, /* DefinedValueType */Block.__(1, [definitionID]));
                                                })
                                            }, Definition$ReactTemplate.getDisplayName(param[1], "en"));
                                })));
              };
              var match = self[/* state */1][/* opened */0];
              var tmp;
              if (match) {
                var match$1 = self[/* state */1][/* category */1];
                var tmp$1;
                if (match$1 >= 2) {
                  switch (match$1 - 2 | 0) {
                    case 0 : 
                        tmp$1 = definedTypeSelector("Record", isRecordType);
                        break;
                    case 1 : 
                        tmp$1 = definedTypeSelector("Function", isInterface);
                        break;
                    case 2 : 
                        tmp$1 = null;
                        break;
                    
                  }
                } else {
                  tmp$1 = null;
                }
                tmp = React.createElement("div", {
                      className: "type-selector-menu"
                    }, React.createElement("div", {
                          className: "type-selector-categories"
                        }, React.createElement("h3", undefined, "Category"), renderCategory("Any", /* AnyCategory */4), renderCategory("Text", /* TextCategory */1), renderCategory("Number", /* NumberCategory */0), renderCategory("Record", /* RecordCategory */2), renderCategory("Function", /* FunctionCategory */3)), tmp$1);
              } else {
                tmp = null;
              }
              return React.createElement("div", {
                          className: "type-selector"
                        }, React.createElement("a", {
                              onClick: (function (_event) {
                                  return Curry._1(self[/* send */3], /* Toggle */0);
                                })
                            }, Definition$ReactTemplate.displayValueType(valueType, definitions, "en")), tmp);
            }),
          /* initialState */(function (param) {
              return /* record */[
                      /* opened */false,
                      /* category */categoryFromType(valueType, definitions)
                    ];
            }),
          /* retainedProps */component[/* retainedProps */11],
          /* reducer */(function (action, state) {
              if (action) {
                var category = action[0];
                return /* UpdateWithSideEffects */Block.__(2, [
                          /* record */[
                            /* opened */state[/* opened */0],
                            /* category */category
                          ],
                          (function (param) {
                              if (category !== 1) {
                                if (category !== 0) {
                                  return /* () */0;
                                } else {
                                  return Curry._1(changeType, /* PrimitiveValueType */Block.__(0, [/* NumberType */1]));
                                }
                              } else {
                                return Curry._1(changeType, /* PrimitiveValueType */Block.__(0, [/* TextType */2]));
                              }
                            })
                        ]);
              } else {
                return /* Update */Block.__(0, [/* record */[
                            /* opened */!state[/* opened */0],
                            /* category */state[/* category */1]
                          ]]);
              }
            }),
          /* jsElementWrapped */component[/* jsElementWrapped */13]
        ];
}

exports.categoryFromType = categoryFromType;
exports.hasDefinitionID = hasDefinitionID;
exports.component = component;
exports.isRecordType = isRecordType;
exports.isInterface = isInterface;
exports.make = make;
/* component Not a pure module */
