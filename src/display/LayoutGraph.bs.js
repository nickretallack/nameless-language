// Generated by BUCKLESCRIPT VERSION 4.0.14, PLEASE EDIT WITH CARE
'use strict';

var Belt_Map = require("bs-platform/lib/js/belt_Map.js");
var Caml_obj = require("bs-platform/lib/js/caml_obj.js");
var Belt_List = require("bs-platform/lib/js/belt_List.js");
var Belt_Array = require("bs-platform/lib/js/belt_Array.js");
var Belt_MapString = require("bs-platform/lib/js/belt_MapString.js");
var Belt_SetString = require("bs-platform/lib/js/belt_SetString.js");
var Caml_primitive = require("bs-platform/lib/js/caml_primitive.js");
var Helpers$ReactTemplate = require("../Helpers.bs.js");
var Caml_builtin_exceptions = require("bs-platform/lib/js/caml_builtin_exceptions.js");
var Definition$ReactTemplate = require("../Definition.bs.js");

function getNodeIDsConnectedToInternalInputs(definitionNodeID, connections) {
  return Belt_Map.reduce(connections, Belt_SetString.empty, (function (nodeIDs, sink, source) {
                var match = source[/* node */0];
                if (match) {
                  var match$1 = definitionNodeID !== match[0];
                  if (match$1) {
                    return nodeIDs;
                  } else {
                    var match$2 = source[/* nib */1];
                    if (typeof match$2 === "number") {
                      return nodeIDs;
                    } else if (match$2.tag) {
                      throw Caml_builtin_exceptions.not_found;
                    } else {
                      var match$3 = sink[/* node */0];
                      if (match$3) {
                        return Belt_SetString.add(nodeIDs, match$3[0]);
                      } else {
                        return nodeIDs;
                      }
                    }
                  }
                } else {
                  return nodeIDs;
                }
              }));
}

function getMaxColumnFromNodeIDs(nodeIDs, nodeColumns) {
  return Belt_SetString.reduce(nodeIDs, 0, (function (maxColumn, nodeID) {
                return Caml_primitive.caml_int_max(maxColumn, Belt_MapString.getExn(nodeColumns, nodeID));
              }));
}

function getMaxColumn(definitionNodeID, connections, childNodeIDs, nodeColumns) {
  return getMaxColumnFromNodeIDs(Belt_SetString.union(getNodeIDsConnectedToInternalInputs(definitionNodeID, connections), childNodeIDs), nodeColumns);
}

function layoutDefinition(nodeScope, scopedNodeIDs, columnizedNodes, definitions, connections) {
  var childNodeIDs = Belt_Map.getWithDefault(scopedNodeIDs, nodeScope, Belt_SetString.empty);
  var match = Belt_List.reduceWithIndex(columnizedNodes, /* tuple */[
        /* array */[],
        Belt_MapString.empty
      ], (function (acc, nodes, columns) {
          return Belt_List.reduce(Belt_List.keep(nodes, (function (node) {
                            return Belt_SetString.has(childNodeIDs, node[/* id */0]);
                          })), acc, (function (param, node) {
                        var columnsFilledness = param[0];
                        var match = node[/* node */1][/* kind */1];
                        var match$1;
                        var exit = 0;
                        if (typeof match === "number" || !(match.tag && match[0][/* kind */0] === 3)) {
                          exit = 1;
                        } else {
                          match$1 = layoutSubGraph(node, scopedNodeIDs, columnizedNodes, definitions, connections);
                        }
                        if (exit === 1) {
                          match$1 = /* tuple */[
                            /* record */[
                              /* columns */1,
                              /* rows */Definition$ReactTemplate.countNodeNibs(node[/* node */1], definitions) + 1 | 0
                            ],
                            Belt_MapString.empty
                          ];
                        }
                        var size = match$1[0];
                        var lastColumn = (columns + size[/* columns */0] | 0) - 1 | 0;
                        var rows = Belt_Array.reduce(Belt_Array.range(columns, lastColumn), 0, (function (row, column) {
                                return Caml_primitive.caml_int_max(row, Helpers$ReactTemplate.arrayGetWithDefault(columnsFilledness, column, 0));
                              }));
                        var adjustedChildren = Belt_MapString.map(match$1[1], (function (nodeLayout) {
                                var init = nodeLayout[/* position */0];
                                return /* record */[
                                        /* position : record */[
                                          /* columns */init[/* columns */0],
                                          /* rows */(nodeLayout[/* position */0][/* rows */1] + 1 | 0) + rows | 0
                                        ],
                                        /* size */nodeLayout[/* size */1]
                                      ];
                              }));
                        var newFilledness = Belt_Array.makeBy(Caml_primitive.caml_int_max(columnsFilledness.length, lastColumn + 1 | 0), (function (index) {
                                var match = index >= columns && index < (columns + size[/* columns */0] | 0);
                                if (match) {
                                  return (rows + size[/* rows */1] | 0) + 1 | 0;
                                } else {
                                  return Helpers$ReactTemplate.arrayGetWithDefault(columnsFilledness, index, 0);
                                }
                              }));
                        console.log(lastColumn);
                        return /* tuple */[
                                newFilledness,
                                Helpers$ReactTemplate.simpleMergeMaps(Belt_MapString.set(param[1], node[/* id */0], /* record */[
                                          /* position : record */[
                                            /* columns */columns,
                                            /* rows */rows
                                          ],
                                          /* size */size
                                        ]), adjustedChildren)
                              ];
                      }));
        }));
  var columnFilledness = match[0];
  console.log("filledness", columnFilledness);
  return /* tuple */[
          match[1],
          /* record */[
            /* columns */columnFilledness.length,
            /* rows */Belt_Array.reduce(columnFilledness, 0, Caml_obj.caml_max) + 1 | 0
          ]
        ];
}

function layoutSubGraph(definitionNode, scopedNodeIDs, columnizedNodes, definitions, connections) {
  var firstColumn = Helpers$ReactTemplate.findByIndexExn(columnizedNodes, (function (nodes) {
          return Belt_List.some(nodes, (function (node) {
                        return node[/* id */0] === definitionNode[/* id */0];
                      }));
        }));
  var match = definitionNode[/* node */1][/* kind */1];
  var nibRows;
  if (typeof match === "number") {
    throw Caml_builtin_exceptions.not_found;
  } else if (match.tag) {
    var display = Belt_MapString.getExn(definitions, match[0][/* definitionID */1])[/* display */2];
    nibRows = Caml_primitive.caml_int_max(Belt_List.length(display[/* inputOrdering */0]), Belt_List.length(display[/* outputOrdering */1]));
  } else {
    throw Caml_builtin_exceptions.not_found;
  }
  var match$1 = layoutDefinition(/* NodeScope */[definitionNode[/* id */0]], scopedNodeIDs, columnizedNodes, definitions, connections);
  var position = match$1[1];
  return /* tuple */[
          /* record */[
            /* columns */Caml_primitive.caml_int_max(0, (position[/* columns */0] - 1 | 0) - firstColumn | 0) + 2 | 0,
            /* rows */Caml_primitive.caml_int_max(nibRows, position[/* rows */1])
          ],
          match$1[0]
        ];
}

function layoutGraph(scopedNodeIDs, columnizedNodes, definitions, connections) {
  var match = layoutDefinition(/* GraphScope */0, scopedNodeIDs, columnizedNodes, definitions, connections);
  var dimensions = match[1];
  return /* tuple */[
          match[0],
          /* record */[
            /* columns */dimensions[/* columns */0] + 2 | 0,
            /* rows */dimensions[/* rows */1] + 1 | 0
          ]
        ];
}

exports.getNodeIDsConnectedToInternalInputs = getNodeIDsConnectedToInternalInputs;
exports.getMaxColumnFromNodeIDs = getMaxColumnFromNodeIDs;
exports.getMaxColumn = getMaxColumn;
exports.layoutDefinition = layoutDefinition;
exports.layoutSubGraph = layoutSubGraph;
exports.layoutGraph = layoutGraph;
/* Helpers-ReactTemplate Not a pure module */
