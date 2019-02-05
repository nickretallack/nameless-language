// Generated by BUCKLESCRIPT VERSION 4.0.14, PLEASE EDIT WITH CARE
'use strict';

var React = require("react");
var ReasonReact = require("reason-react/src/ReasonReact.js");
var Definition$ReactTemplate = require("../Definition.bs.js");
var DefinitionHeader$ReactTemplate = require("./DefinitionHeader.bs.js");
var ConstantImplementation$ReactTemplate = require("./ConstantImplementation.bs.js");
var InterfaceImplementation$ReactTemplate = require("./InterfaceImplementation.bs.js");
var RecordTypeImplementation$ReactTemplate = require("./RecordTypeImplementation.bs.js");

var component = ReasonReact.statelessComponent("SimpleDefinition");

function make(definition, definitions, emit, _children) {
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
              var display = definition[/* display */2];
              var documentation = definition[/* documentation */1];
              var implementation = definition[/* implementation */0];
              var tmp;
              switch (implementation.tag | 0) {
                case 0 : 
                    tmp = ReasonReact.element(undefined, undefined, ConstantImplementation$ReactTemplate.make(implementation[0], emit, /* array */[]));
                    break;
                case 1 : 
                    tmp = ReasonReact.element(undefined, undefined, InterfaceImplementation$ReactTemplate.make(definitions, implementation[0], documentation, display, emit, /* array */[]));
                    break;
                case 4 : 
                    tmp = ReasonReact.element(undefined, undefined, RecordTypeImplementation$ReactTemplate.make(definitions, implementation[0], documentation, display, emit, /* array */[]));
                    break;
                default:
                  tmp = "TODO";
              }
              return React.createElement("div", undefined, ReasonReact.element(undefined, undefined, DefinitionHeader$ReactTemplate.make(documentation, "(nameless " + (Definition$ReactTemplate.implementationName(implementation) + ")"), emit, /* array */[])), tmp);
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
