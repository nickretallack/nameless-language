// Generated by BUCKLESCRIPT VERSION 4.0.14, PLEASE EDIT WITH CARE
'use strict';

var Belt_MapString = require("bs-platform/lib/js/belt_MapString.js");
var Definition$ReactTemplate = require("../Definition.bs.js");
var ColumnizeNodes$ReactTemplate = require("../display/ColumnizeNodes.bs.js");

function detectCycles(connections, nodes) {
  try {
    ColumnizeNodes$ReactTemplate.columnizeNodes(nodes, connections);
    return false;
  }
  catch (exn){
    if (exn === Definition$ReactTemplate.CycleDetected) {
      return true;
    } else {
      throw exn;
    }
  }
}

function isParentScope(parent, _child, nodes) {
  while(true) {
    var child = _child;
    console.log("so far...");
    if (parent === child) {
      return true;
    } else {
      var match = Belt_MapString.getExn(nodes, child)[/* scope */0];
      if (match) {
        _child = match[0];
        continue ;
      } else {
        return false;
      }
    }
  };
}

function checkScopes(source, sink, nodes) {
  var match = sink[/* node */0];
  if (match) {
    var sinkNodeID = match[0];
    var match$1 = source[/* node */0];
    if (match$1) {
      var sourceNodeID = match$1[0];
      var sinkNode = Belt_MapString.getExn(nodes, sinkNodeID);
      var sourceNode = Belt_MapString.getExn(nodes, sourceNodeID);
      if (Definition$ReactTemplate.isFunctionDefinitionNode(sourceNode) && Definition$ReactTemplate.isKeywordNib(source[/* nib */1])) {
        var match$2 = sinkNode[/* scope */0];
        if (match$2) {
          return isParentScope(sourceNodeID, match$2[0], nodes);
        } else {
          return sourceNodeID === sinkNodeID;
        }
      } else {
        var match$3 = sourceNode[/* scope */0];
        if (match$3) {
          var sourceScopeNodeID = match$3[0];
          if (sourceScopeNodeID === sinkNodeID) {
            return true;
          } else {
            var match$4 = sinkNode[/* scope */0];
            if (match$4) {
              return isParentScope(match$4[0], sourceScopeNodeID, nodes);
            } else {
              return false;
            }
          }
        } else {
          return true;
        }
      }
    } else {
      return true;
    }
  } else {
    var match$5 = source[/* node */0];
    if (match$5) {
      var sourceNode$1 = Belt_MapString.getExn(nodes, match$5[0]);
      return sourceNode$1[/* scope */0] === /* GraphScope */0;
    } else {
      return true;
    }
  }
}

exports.detectCycles = detectCycles;
exports.isParentScope = isParentScope;
exports.checkScopes = checkScopes;
/* Definition-ReactTemplate Not a pure module */
