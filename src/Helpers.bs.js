// Generated by BUCKLESCRIPT VERSION 4.0.14, PLEASE EDIT WITH CARE
'use strict';

var List = require("bs-platform/lib/js/list.js");
var $$Array = require("bs-platform/lib/js/array.js");
var Block = require("bs-platform/lib/js/block.js");
var Curry = require("bs-platform/lib/js/curry.js");
var Printf = require("bs-platform/lib/js/printf.js");
var Random = require("bs-platform/lib/js/random.js");
var $$String = require("bs-platform/lib/js/string.js");
var Belt_Map = require("bs-platform/lib/js/belt_Map.js");
var Caml_obj = require("bs-platform/lib/js/caml_obj.js");
var Belt_List = require("bs-platform/lib/js/belt_List.js");
var Belt_MapString = require("bs-platform/lib/js/belt_MapString.js");
var Caml_builtin_exceptions = require("bs-platform/lib/js/caml_builtin_exceptions.js");

function sortBy(list, func) {
  return Belt_List.map(Belt_List.sort(Belt_List.map(list, (function (item) {
                        return /* tuple */[
                                Curry._1(func, item),
                                item
                              ];
                      })), Caml_obj.caml_compare), (function (prim) {
                return prim[1];
              }));
}

function findIndexExn(list, needle) {
  if (list) {
    var match = Caml_obj.caml_equal(list[0], needle);
    if (match) {
      return 0;
    } else {
      return 1 + findIndexExn(list[1], needle) | 0;
    }
  } else {
    throw Caml_builtin_exceptions.not_found;
  }
}

function findByIndexExn(list, check) {
  if (list) {
    var match = Curry._1(check, list[0]);
    if (match) {
      return 0;
    } else {
      return 1 + findByIndexExn(list[1], check) | 0;
    }
  } else {
    throw Caml_builtin_exceptions.not_found;
  }
}

function simpleMergeMaps(map1, map2) {
  return Belt_MapString.reduce(map1, map2, Belt_MapString.set);
}

Random.init(Date.now() | 0);

function randomHex(param) {
  return Curry._1(Printf.sprintf(/* Format */[
                  /* Int */Block.__(4, [
                      /* Int_x */6,
                      /* No_padding */0,
                      /* No_precision */0,
                      /* End_of_format */0
                    ]),
                  "%x"
                ]), Random.$$int(16));
}

function randomID(param) {
  return $$String.concat("", Belt_List.makeBy(32, (function (param) {
                    return randomHex(/* () */0);
                  })));
}

function floatToString(x) {
  return Curry._1(Printf.sprintf(/* Format */[
                  /* Float */Block.__(8, [
                      /* Float_g */9,
                      /* No_padding */0,
                      /* No_precision */0,
                      /* End_of_format */0
                    ]),
                  "%g"
                ]), x);
}

function pixels(x) {
  return Curry._1(Printf.sprintf(/* Format */[
                  /* Float */Block.__(8, [
                      /* Float_g */9,
                      /* No_padding */0,
                      /* No_precision */0,
                      /* String_literal */Block.__(11, [
                          "px",
                          /* End_of_format */0
                        ])
                    ]),
                  "%gpx"
                ]), x);
}

function pointFromMouse($$event) {
  return /* record */[
          /* x */$$event.clientX,
          /* y */$$event.clientY
        ];
}

function positionStyle(position) {
  return {
          left: pixels(position[/* x */0]),
          top: pixels(position[/* y */1])
        };
}

function iterateTouches($$event, callback) {
  return $$Array.iter(callback, $$event.changedTouches);
}

function getEventValue($$event) {
  return $$event.target.value;
}

function renderMap(fn, map) {
  return $$Array.map(fn, Belt_Map.toArray(map));
}

function renderStringMap(fn, map) {
  return $$Array.map(fn, Belt_MapString.toArray(map));
}

function renderList(fn, list) {
  return $$Array.of_list(List.mapi(fn, list));
}

var renderArray = $$Array.map;

exports.sortBy = sortBy;
exports.findIndexExn = findIndexExn;
exports.findByIndexExn = findByIndexExn;
exports.simpleMergeMaps = simpleMergeMaps;
exports.randomHex = randomHex;
exports.randomID = randomID;
exports.floatToString = floatToString;
exports.pixels = pixels;
exports.pointFromMouse = pointFromMouse;
exports.positionStyle = positionStyle;
exports.iterateTouches = iterateTouches;
exports.getEventValue = getEventValue;
exports.renderMap = renderMap;
exports.renderStringMap = renderStringMap;
exports.renderList = renderList;
exports.renderArray = renderArray;
/*  Not a pure module */
