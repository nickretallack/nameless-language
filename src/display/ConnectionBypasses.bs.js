// Generated by BUCKLESCRIPT VERSION 4.0.14, PLEASE EDIT WITH CARE
'use strict';

var List = require("bs-platform/lib/js/list.js");
var Belt_Map = require("bs-platform/lib/js/belt_Map.js");
var Belt_List = require("bs-platform/lib/js/belt_List.js");
var Pervasives = require("bs-platform/lib/js/pervasives.js");
var Belt_MapString = require("bs-platform/lib/js/belt_MapString.js");
var Belt_SetString = require("bs-platform/lib/js/belt_SetString.js");

function getColumn(connectionSide, $$default, nodeLayouts) {
  var match = connectionSide[/* node */0];
  if (match) {
    return Belt_MapString.getExn(nodeLayouts, match[0])[/* position */0];
  } else {
    return $$default;
  }
}

function getParentScopes(nodeID, nodes) {
  var match = Belt_MapString.getExn(nodes, nodeID)[/* scope */0];
  if (match) {
    var nodeID$1 = match[0];
    return /* :: */[
            nodeID$1,
            getParentScopes(nodeID$1, nodes)
          ];
  } else {
    return /* [] */0;
  }
}

function collisionDetect(nodeLayout, nodePosition) {
  if (nodePosition[/* columns */0] >= nodeLayout[/* position */0][/* columns */0] && nodePosition[/* columns */0] <= (nodeLayout[/* position */0][/* columns */0] + nodeLayout[/* size */1][/* columns */0] | 0) && nodePosition[/* rows */1] >= nodeLayout[/* position */0][/* rows */1]) {
    return nodePosition[/* rows */1] <= (nodeLayout[/* position */0][/* rows */1] + nodeLayout[/* size */1][/* rows */1] | 0);
  } else {
    return false;
  }
}

function calculate(nodeLayouts, connections, nodes, maxColumn) {
  return Belt_Map.mapWithKey(connections, (function (sink, source) {
                var startPosition = getColumn(sink, /* record */[
                      /* columns */0,
                      /* rows */0
                    ], nodeLayouts);
                var endPosition = getColumn(source, /* record */[
                      /* columns */maxColumn,
                      /* rows */0
                    ], nodeLayouts);
                var length = (endPosition[/* columns */0] - startPosition[/* columns */0] | 0) - 1 | 0;
                var match = sink[/* node */0];
                var parentScopes = Belt_SetString.fromArray(Belt_List.toArray(match ? getParentScopes(match[0], nodes) : /* [] */0));
                return Belt_List.reduce(Belt_List.makeBy(length, (function (index) {
                                    return index;
                                  })), /* tuple */[
                              /* [] */0,
                              endPosition[/* rows */1]
                            ], (function (param, index) {
                                var rows = param[1];
                                var columns = endPosition[/* columns */0] - index | 0;
                                var position = /* record */[
                                  /* columns */columns,
                                  /* rows */rows
                                ];
                                var collisions = Belt_MapString.keep(nodeLayouts, (function (nodeID, layout) {
                                        if (Belt_SetString.has(parentScopes, nodeID)) {
                                          return false;
                                        } else {
                                          return collisionDetect(layout, position);
                                        }
                                      }));
                                var rows$1;
                                if (Belt_MapString.isEmpty(collisions)) {
                                  rows$1 = rows;
                                } else {
                                  var collisions$1 = Belt_List.fromArray(Belt_MapString.valuesToArray(collisions));
                                  var outermostCollision = Belt_List.reduce(List.tl(collisions$1), List.hd(collisions$1), (function (acc, layout) {
                                          var match = layout[/* depth */2] > acc[/* depth */2];
                                          if (match) {
                                            return layout;
                                          } else {
                                            return acc;
                                          }
                                        }));
                                  var top = outermostCollision[/* position */0][/* rows */1] + outermostCollision[/* size */1][/* rows */1] | 0;
                                  rows$1 = Pervasives.abs(outermostCollision[/* position */0][/* rows */1] - rows | 0) > Pervasives.abs(top - rows | 0) ? outermostCollision[/* position */0][/* rows */1] - 1 | 0 : top + 1 | 0;
                                }
                                return /* tuple */[
                                        /* :: */[
                                          rows$1,
                                          param[0]
                                        ],
                                        rows$1
                                      ];
                              }))[0];
              }));
}

exports.getColumn = getColumn;
exports.getParentScopes = getParentScopes;
exports.collisionDetect = collisionDetect;
exports.calculate = calculate;
/* No side effect */
