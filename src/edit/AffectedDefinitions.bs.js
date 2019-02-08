// Generated by BUCKLESCRIPT VERSION 4.0.14, PLEASE EDIT WITH CARE
'use strict';

var Belt_MapString = require("bs-platform/lib/js/belt_MapString.js");

function findUses(definitionID, definitions) {
  return Belt_MapString.keep(definitions, (function (param, otherDefinition) {
                var match = otherDefinition[/* implementation */0];
                if (match.tag === 3) {
                  return Belt_MapString.some(match[0][/* nodes */2], (function (param, node) {
                                var match = node[/* kind */1];
                                if (typeof match === "number" || !match.tag) {
                                  return false;
                                } else {
                                  return match[0][/* definitionID */1] === definitionID;
                                }
                              }));
                } else {
                  return false;
                }
              }));
}

exports.findUses = findUses;
/* No side effect */
