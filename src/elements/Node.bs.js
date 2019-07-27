// Generated by BUCKLESCRIPT VERSION 4.0.14, PLEASE EDIT WITH CARE
'use strict';

var Block = require("bs-platform/lib/js/block.js");
var Belt_Debug = require("bs-platform/lib/js/belt_Debug.js");
var ReasonReact = require("reason-react/src/ReasonReact.js");
var Belt_MapString = require("bs-platform/lib/js/belt_MapString.js");
var NibsBox$ReactTemplate = require("./NibsBox.bs.js");
var Definition$ReactTemplate = require("../Definition.bs.js");
var DefinitionBox$ReactTemplate = require("./DefinitionBox.bs.js");

Belt_Debug.setupChromeDebugger(/* () */0);

var component = ReasonReact.statelessComponent("Node");

function make(node, definitions, position, size, nodeWidth, textHeight, $staropt$star, onClick, onMouseDown, onTouchStart, onMouseUp, onTouchEnd, _children) {
  var selected = $staropt$star !== undefined ? $staropt$star : false;
  return /* record */Block.record([
            "debugName",
            "reactClassInternal",
            "handedOffState",
            "willReceiveProps",
            "didMount",
            "didUpdate",
            "willUnmount",
            "willUpdate",
            "shouldUpdate",
            "render",
            "initialState",
            "retainedProps",
            "reducer",
            "jsElementWrapped"
          ], [
            component[/* debugName */0],
            component[/* reactClassInternal */1],
            component[/* handedOffState */2],
            component[/* willReceiveProps */3],
            component[/* didMount */4],
            component[/* didUpdate */5],
            component[/* willUnmount */6],
            component[/* willUpdate */7],
            component[/* shouldUpdate */8],
            (function (_self) {
                var makeNode = function (name, onDoubleClick) {
                  return ReasonReact.element(undefined, undefined, NibsBox$ReactTemplate.make(name, position, size[/* y */1], nodeWidth, textHeight, selected, onDoubleClick, onClick, onMouseDown, onTouchStart, onMouseUp, onTouchEnd, /* array */[]));
                };
                var match = node[/* kind */1];
                if (typeof match === "number") {
                  return makeNode("Reference", undefined);
                } else if (match.tag) {
                  var definitionID = match[0][/* definitionID */1];
                  var definition = Belt_MapString.getExn(definitions, definitionID);
                  var name = Definition$ReactTemplate.getDisplayName(definition, "en");
                  var onDoubleClick = function (_event) {
                    return ReasonReact.Router[/* push */0]("#" + definitionID);
                  };
                  if (Definition$ReactTemplate.isFunctionDefinitionNode(node)) {
                    return ReasonReact.element(undefined, undefined, DefinitionBox$ReactTemplate.make(name, position, size, nodeWidth, textHeight, selected, onClick, onDoubleClick, onMouseDown, onTouchStart, onMouseUp, onTouchEnd, /* array */[]));
                  } else {
                    return makeNode(name, onDoubleClick);
                  }
                } else {
                  return makeNode("List", undefined);
                }
              }),
            component[/* initialState */10],
            component[/* retainedProps */11],
            component[/* reducer */12],
            component[/* jsElementWrapped */13]
          ]);
}

exports.component = component;
exports.make = make;
/*  Not a pure module */
