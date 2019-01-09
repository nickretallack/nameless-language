// Generated by BUCKLESCRIPT VERSION 4.0.14, PLEASE EDIT WITH CARE
'use strict';

var Block = require("bs-platform/lib/js/block.js");
var Curry = require("bs-platform/lib/js/curry.js");
var ReasonReact = require("reason-react/src/ReasonReact.js");

var component = ReasonReact.reducerComponent("WindowSize");

function make(render, _children) {
  return /* record */[
          /* debugName */component[/* debugName */0],
          /* reactClassInternal */component[/* reactClassInternal */1],
          /* handedOffState */component[/* handedOffState */2],
          /* willReceiveProps */component[/* willReceiveProps */3],
          /* didMount */(function (self) {
              Curry._1(self[/* send */3], /* Resize */[/* record */[
                      /* x */window.innerWidth,
                      /* y */window.innerHeight
                    ]]);
              window.addEventListener("resize", (function (_event) {
                      return Curry._1(self[/* send */3], /* Resize */[/* record */[
                                    /* x */window.innerWidth,
                                    /* y */window.innerHeight
                                  ]]);
                    }));
              return /* () */0;
            }),
          /* didUpdate */component[/* didUpdate */5],
          /* willUnmount */component[/* willUnmount */6],
          /* willUpdate */component[/* willUpdate */7],
          /* shouldUpdate */component[/* shouldUpdate */8],
          /* render */(function (self) {
              return Curry._1(render, self[/* state */1]);
            }),
          /* initialState */(function (param) {
              return /* record */[
                      /* x */1366.0,
                      /* y */768.0
                    ];
            }),
          /* retainedProps */component[/* retainedProps */11],
          /* reducer */(function (action, _state) {
              return /* Update */Block.__(0, [action[0]]);
            }),
          /* jsElementWrapped */component[/* jsElementWrapped */13]
        ];
}

exports.component = component;
exports.make = make;
/* component Not a pure module */
