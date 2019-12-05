'use strict';

var Belt_Id = require("bs-platform/lib/js/belt_Id.js");
var Caml_obj = require("bs-platform/lib/js/caml_obj.js");

var cmp = Caml_obj.caml_compare;

var C = Belt_Id.MakeComparable({
      cmp: cmp
    });

exports.C = C;
/* C Not a pure module */
