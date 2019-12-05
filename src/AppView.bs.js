'use strict';

var React = require("react");

function AppView(Props) {
  return React.createElement("div", undefined, "hello");
}

var make = AppView;

exports.make = make;
/* react Not a pure module */
