'use strict';

var React = require("react");
var ReactDOMRe = require("reason-react/src/ReactDOMRe.js");
var AppView$ReasonReactExamples = require("./AppView.bs.js");

ReactDOMRe.renderToElementWithId(React.createElement(AppView$ReasonReactExamples.make, { }), "app");

/*  Not a pure module */
