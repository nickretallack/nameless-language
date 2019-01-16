// Generated by BUCKLESCRIPT VERSION 4.0.14, PLEASE EDIT WITH CARE
'use strict';

var Block = require("bs-platform/lib/js/block.js");
var Curry = require("bs-platform/lib/js/curry.js");
var Belt_Map = require("bs-platform/lib/js/belt_Map.js");
var Belt_List = require("bs-platform/lib/js/belt_List.js");
var Belt_MapString = require("bs-platform/lib/js/belt_MapString.js");
var Helpers$ReactTemplate = require("./Helpers.bs.js");
var Caml_builtin_exceptions = require("bs-platform/lib/js/caml_builtin_exceptions.js");

function getNumber(value) {
  if (value.tag) {
    throw Caml_builtin_exceptions.not_found;
  } else {
    var match = value[0];
    switch (match.tag | 0) {
      case 1 : 
          return match[0];
      case 0 : 
      case 2 : 
          throw Caml_builtin_exceptions.not_found;
      
    }
  }
}

function addNumbers(inputs) {
  var left = Belt_List.getExn(inputs, 0);
  var right = Belt_List.getExn(inputs, 1);
  return /* :: */[
          /* PrimitiveValue */Block.__(0, [/* NumberValue */Block.__(1, [getNumber(Curry._1(left, /* () */0)) + getNumber(Curry._1(right, /* () */0))])]),
          /* [] */0
        ];
}

function evaluateExternal(name, outputIndex, inputs) {
  var externalFunction;
  if (name === "+") {
    externalFunction = addNumbers;
  } else {
    throw Caml_builtin_exceptions.not_found;
  }
  var outputs = Curry._1(externalFunction, inputs);
  return Belt_List.getExn(outputs, outputIndex);
}

function evaluateConnection(definitions, graphImplementation, sink) {
  var source = Belt_Map.getExn(graphImplementation[/* connections */0], sink);
  var match = source[/* node */0];
  if (match) {
    var nodeID = match[0];
    var node = Belt_MapString.getExn(graphImplementation[/* nodes */1], nodeID);
    if (typeof node === "number") {
      return /* PrimitiveValue */Block.__(0, [/* TextValue */Block.__(2, ["Reference!"])]);
    } else if (node.tag) {
      var match$1 = node[0];
      var definitionID = match$1[/* definitionID */1];
      var nodeDefinition = Belt_MapString.getExn(definitions, definitionID);
      switch (match$1[/* kind */0]) {
        case 0 : 
            var match$2 = nodeDefinition[/* implementation */0];
            if (match$2.tag === 2) {
              var match$3 = source[/* nib */1];
              if (typeof match$3 === "number") {
                throw Caml_builtin_exceptions.not_found;
              } else if (match$3.tag) {
                throw Caml_builtin_exceptions.not_found;
              } else {
                var outputIndex = Helpers$ReactTemplate.findIndexExn(nodeDefinition[/* display */2][/* outputOrdering */1], match$3[0]);
                var lazyInputs = Belt_List.map(nodeDefinition[/* display */2][/* inputOrdering */0], (function (nibID, param) {
                        return evaluateConnection(definitions, graphImplementation, /* record */[
                                    /* node : NodeConnection */[nodeID],
                                    /* nib : NibConnection */Block.__(0, [nibID])
                                  ]);
                      }));
                return evaluateExternal(match$2[0][/* name */0], outputIndex, lazyInputs);
              }
            } else {
              return /* PrimitiveValue */Block.__(0, [/* TextValue */Block.__(2, ["Value?"])]);
            }
        case 1 : 
            var match$4 = nodeDefinition[/* implementation */0];
            if (match$4.tag) {
              return /* PrimitiveValue */Block.__(0, [/* TextValue */Block.__(2, ["Value?"])]);
            } else {
              return /* PrimitiveValue */Block.__(0, [match$4[0]]);
            }
        case 2 : 
        case 3 : 
            return /* PrimitiveValue */Block.__(0, [/* TextValue */Block.__(2, ["Defined node?"])]);
        case 4 : 
            var match$5 = nodeDefinition[/* implementation */0];
            if (match$5.tag === 4) {
              return /* DefinedValue */Block.__(1, [/* record */[
                          /* definitionID */definitionID,
                          /* values */Belt_List.map(nodeDefinition[/* display */2][/* inputOrdering */0], (function (nibID, param) {
                                  return evaluateConnection(definitions, graphImplementation, /* record */[
                                              /* node : NodeConnection */[nodeID],
                                              /* nib : NibConnection */Block.__(0, [nibID])
                                            ]);
                                }))
                        ]]);
            } else {
              return /* PrimitiveValue */Block.__(0, [/* TextValue */Block.__(2, ["Accessor?"])]);
            }
        case 5 : 
            var match$6 = nodeDefinition[/* implementation */0];
            if (match$6.tag === 4) {
              var match$7 = source[/* nib */1];
              if (typeof match$7 === "number") {
                throw Caml_builtin_exceptions.not_found;
              } else if (match$7.tag) {
                throw Caml_builtin_exceptions.not_found;
              } else {
                var value = evaluateConnection(definitions, graphImplementation, /* record */[
                      /* node : NodeConnection */[nodeID],
                      /* nib : ValueConnection */0
                    ]);
                if (value.tag) {
                  var definedValue = value[0];
                  if (definedValue[/* definitionID */0] !== definitionID) {
                    throw Caml_builtin_exceptions.not_found;
                  } else {
                    var fieldIndex = Helpers$ReactTemplate.findIndexExn(nodeDefinition[/* display */2][/* inputOrdering */0], match$7[0]);
                    return Curry._1(Belt_List.getExn(definedValue[/* values */1], fieldIndex), /* () */0);
                  }
                } else {
                  throw Caml_builtin_exceptions.not_found;
                }
              }
            } else {
              return /* PrimitiveValue */Block.__(0, [/* TextValue */Block.__(2, ["Constructor?"])]);
            }
        
      }
    } else {
      return /* PrimitiveValue */Block.__(0, [/* TextValue */Block.__(2, ["List!"])]);
    }
  } else {
    return /* PrimitiveValue */Block.__(0, [/* TextValue */Block.__(2, ["Input!"])]);
  }
}

function evaluateGraphOutput(definitions, graphImplementation, outputID) {
  return evaluateConnection(definitions, graphImplementation, /* record */[
              /* node : GraphConnection */0,
              /* nib : NibConnection */Block.__(0, [outputID])
            ]);
}

exports.getNumber = getNumber;
exports.addNumbers = addNumbers;
exports.evaluateExternal = evaluateExternal;
exports.evaluateConnection = evaluateConnection;
exports.evaluateGraphOutput = evaluateGraphOutput;
/* Helpers-ReactTemplate Not a pure module */
