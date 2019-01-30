// Generated by BUCKLESCRIPT VERSION 4.0.14, PLEASE EDIT WITH CARE
'use strict';

var $$Array = require("bs-platform/lib/js/array.js");
var React = require("react");
var Caml_obj = require("bs-platform/lib/js/caml_obj.js");
var Belt_List = require("bs-platform/lib/js/belt_List.js");
var Belt_Option = require("bs-platform/lib/js/belt_Option.js");
var Caml_option = require("bs-platform/lib/js/caml_option.js");
var ReasonReact = require("reason-react/src/ReasonReact.js");
var Nib$ReactTemplate = require("./Nib.bs.js");
var Helpers$ReactTemplate = require("../Helpers.bs.js");

var component = ReasonReact.statelessComponent("SimpleNode");

function nibKey(connectionNib) {
  if (typeof connectionNib === "number") {
    return "value";
  } else if (connectionNib.tag) {
    return String(connectionNib[0]);
  } else {
    return connectionNib[0];
  }
}

function renderNibs(nibs, className, isSource, connectionNode, emit, selectedNib) {
  return $$Array.of_list(Belt_List.map(nibs, (function (param) {
                    var nib = param[/* nib */1];
                    var name = param[/* name */0];
                    return React.createElement("div", {
                                key: nibKey(nib),
                                className: className
                              }, isSource ? null : name, ReasonReact.element(undefined, undefined, Nib$ReactTemplate.make(isSource, /* record */[
                                        /* node */connectionNode,
                                        /* nib */nib
                                      ], emit, Caml_obj.caml_equal(nib, selectedNib), /* array */[])), isSource ? name : null);
                  })));
}

function make(nodeID, definitionID, name, inputs, outputs, position, size, selectedNib, emit, _children) {
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
          /* render */(function (_self) {
              var tmp = {
                className: "node",
                style: {
                  height: Helpers$ReactTemplate.pixels(size[/* y */1]),
                  left: Helpers$ReactTemplate.pixels(position[/* x */0]),
                  top: Helpers$ReactTemplate.pixels(position[/* y */1]),
                  width: Helpers$ReactTemplate.pixels(size[/* x */0])
                }
              };
              var tmp$1 = Belt_Option.map(definitionID, (function (definitionID, _event) {
                      return ReasonReact.Router[/* push */0]("#" + definitionID);
                    }));
              if (tmp$1 !== undefined) {
                tmp.onDoubleClick = Caml_option.valFromOption(tmp$1);
              }
              return React.createElement("div", tmp, name !== undefined ? React.createElement("div", {
                                className: "name"
                              }, name) : null, renderNibs(inputs, "input", false, /* NodeConnection */[nodeID], emit, selectedNib), renderNibs(outputs, "output", true, /* NodeConnection */[nodeID], emit, selectedNib));
            }),
          /* initialState */component[/* initialState */10],
          /* retainedProps */component[/* retainedProps */11],
          /* reducer */component[/* reducer */12],
          /* jsElementWrapped */component[/* jsElementWrapped */13]
        ];
}

exports.component = component;
exports.nibKey = nibKey;
exports.renderNibs = renderNibs;
exports.make = make;
/* component Not a pure module */
