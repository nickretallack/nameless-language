// Generated by BUCKLESCRIPT VERSION 4.0.14, PLEASE EDIT WITH CARE
'use strict';

var React = require("react");
var Caml_option = require("bs-platform/lib/js/caml_option.js");
var ReasonReact = require("reason-react/src/ReasonReact.js");
var Helpers$ReactTemplate = require("../Helpers.bs.js");
var NibsBox$ReactTemplate = require("./NibsBox.bs.js");

var component = ReasonReact.statelessComponent("SvgDefinitionBox");

function make(name, position, size, nodeWidth, textHeight, $staropt$star, onClick, onDoubleClick, onMouseDown, onTouchStart, onMouseUp, onTouchEnd, _children) {
  var selected = $staropt$star !== undefined ? $staropt$star : false;
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
              var tmp = { };
              if (onClick !== undefined) {
                tmp.onClick = Caml_option.valFromOption(onClick);
              }
              if (onDoubleClick !== undefined) {
                tmp.onDoubleClick = Caml_option.valFromOption(onDoubleClick);
              }
              if (onMouseDown !== undefined) {
                tmp.onMouseDown = Caml_option.valFromOption(onMouseDown);
              }
              if (onMouseUp !== undefined) {
                tmp.onMouseUp = Caml_option.valFromOption(onMouseUp);
              }
              if (onTouchEnd !== undefined) {
                tmp.onTouchEnd = Caml_option.valFromOption(onTouchEnd);
              }
              if (onTouchStart !== undefined) {
                tmp.onTouchStart = Caml_option.valFromOption(onTouchStart);
              }
              return React.createElement("g", tmp, React.createElement("rect", {
                              height: Helpers$ReactTemplate.pixels(size[/* y */1] - textHeight),
                              width: Helpers$ReactTemplate.pixels(size[/* x */0] - nodeWidth * 2.0),
                              fill: selected ? "blue" : "black",
                              fillOpacity: "0.05",
                              x: Helpers$ReactTemplate.pixels(position[/* x */0] + nodeWidth),
                              y: Helpers$ReactTemplate.pixels(position[/* y */1] + textHeight)
                            }), ReasonReact.element(undefined, undefined, NibsBox$ReactTemplate.make(undefined, /* record */[
                                  /* x */position[/* x */0],
                                  /* y */position[/* y */1] + textHeight
                                ], size[/* y */1] - textHeight, nodeWidth, textHeight, selected, undefined, undefined, undefined, undefined, undefined, undefined, /* array */[])), ReasonReact.element(undefined, undefined, NibsBox$ReactTemplate.make(undefined, /* record */[
                                  /* x */position[/* x */0] + size[/* x */0] - nodeWidth,
                                  /* y */position[/* y */1] + textHeight
                                ], size[/* y */1] - textHeight, nodeWidth, textHeight, selected, undefined, undefined, undefined, undefined, undefined, undefined, /* array */[])), React.createElement("rect", {
                              height: Helpers$ReactTemplate.pixels(textHeight),
                              width: Helpers$ReactTemplate.pixels(size[/* x */0]),
                              fill: selected ? "blue" : "black",
                              fillOpacity: "0.1",
                              x: Helpers$ReactTemplate.pixels(position[/* x */0]),
                              y: Helpers$ReactTemplate.pixels(position[/* y */1])
                            }), React.createElement("text", {
                              alignmentBaseline: "central",
                              textAnchor: "middle",
                              x: Helpers$ReactTemplate.pixels(position[/* x */0] + size[/* x */0] / 2.0),
                              y: Helpers$ReactTemplate.pixels(position[/* y */1] + textHeight / 2.0)
                            }, name));
            }),
          /* initialState */component[/* initialState */10],
          /* retainedProps */component[/* retainedProps */11],
          /* reducer */component[/* reducer */12],
          /* jsElementWrapped */component[/* jsElementWrapped */13]
        ];
}

exports.component = component;
exports.make = make;
/* component Not a pure module */
