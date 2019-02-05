// Generated by BUCKLESCRIPT VERSION 4.0.14, PLEASE EDIT WITH CARE
'use strict';

var Block = require("bs-platform/lib/js/block.js");
var Curry = require("bs-platform/lib/js/curry.js");
var React = require("react");
var ReasonReact = require("reason-react/src/ReasonReact.js");
var Helpers$ReactTemplate = require("../Helpers.bs.js");
var Definition$ReactTemplate = require("../Definition.bs.js");

var component = ReasonReact.statelessComponent("DefinitionHeader");

function make(documentation, placeholder, emit, _children) {
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
              var changeName = function ($$event) {
                return Curry._1(emit, /* ChangeName */Block.__(1, [Helpers$ReactTemplate.getEventValue($$event)]));
              };
              return React.createElement(React.Fragment, undefined, React.createElement("div", undefined, React.createElement("input", {
                                  className: "name",
                                  placeholder: placeholder,
                                  type: "text",
                                  value: Definition$ReactTemplate.getTranslated(documentation[/* name */0], "en"),
                                  onChange: changeName
                                })), React.createElement("button", {
                              onClick: (function (_event) {
                                  return Curry._1(emit, /* Fork */2);
                                })
                            }, "Fork"));
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
