// Generated by BUCKLESCRIPT VERSION 4.0.14, PLEASE EDIT WITH CARE
'use strict';

var Block = require("bs-platform/lib/js/block.js");
var Belt_Id = require("bs-platform/lib/js/belt_Id.js");
var Caml_obj = require("bs-platform/lib/js/caml_obj.js");
var Belt_List = require("bs-platform/lib/js/belt_List.js");
var Belt_MapString = require("bs-platform/lib/js/belt_MapString.js");
var Caml_exceptions = require("bs-platform/lib/js/caml_exceptions.js");
var Helpers$ReactTemplate = require("./Helpers.bs.js");
var Caml_builtin_exceptions = require("bs-platform/lib/js/caml_builtin_exceptions.js");

function connectionNodeToString(connectionNode) {
  if (connectionNode) {
    return "node-" + connectionNode[0];
  } else {
    return "graph";
  }
}

function connectionNibToString(connectionNib) {
  if (typeof connectionNib === "number") {
    return "value";
  } else if (connectionNib.tag) {
    return "index-" + String(connectionNib[0]);
  } else {
    return "nib-" + connectionNib[0];
  }
}

function connectionSideToString(connectionSide) {
  return connectionNodeToString(connectionSide[/* node */0]) + ("-" + connectionNibToString(connectionSide[/* nib */1]));
}

var cmp = Caml_obj.caml_compare;

var ConnectionComparator = Belt_Id.MakeComparable(/* module */[/* cmp */cmp]);

function definedNodeKindHasValueInput(kind) {
  if (kind !== 2) {
    return kind >= 5;
  } else {
    return true;
  }
}

function definedNodeKindHasValueOutput(kind) {
  switch (kind) {
    case 1 : 
    case 3 : 
    case 4 : 
        return true;
    case 0 : 
    case 2 : 
    case 5 : 
        return false;
    
  }
}

var primitiveValueTypes = /* array */[
  /* TextType */2,
  /* NumberType */1
];

function primitiveValueTypeToString(primitiveValueType) {
  switch (primitiveValueType) {
    case 0 : 
        return "integer";
    case 1 : 
        return "number";
    case 2 : 
        return "text";
    
  }
}

function stringToPrimitiveValueType(string) {
  switch (string) {
    case "integer" : 
        return /* IntegerType */0;
    case "number" : 
        return /* NumberType */1;
    case "text" : 
        return /* TextType */2;
    default:
      throw Caml_builtin_exceptions.not_found;
  }
}

var changeTypedFields = Belt_MapString.set;

function primitiveValueToType(primitiveValue) {
  switch (primitiveValue.tag | 0) {
    case 0 : 
        return /* IntegerType */0;
    case 1 : 
        return /* NumberType */1;
    case 2 : 
        return /* TextType */2;
    
  }
}

function primitiveValueToString(primitiveValue) {
  switch (primitiveValue.tag | 0) {
    case 0 : 
        return String(primitiveValue[0]);
    case 1 : 
        return Helpers$ReactTemplate.floatToString(primitiveValue[0]);
    case 2 : 
        return primitiveValue[0];
    
  }
}

function primitiveValueToTypeString(x) {
  return primitiveValueTypeToString(primitiveValueToType(x));
}

function getTranslated(translatable, language) {
  return Belt_MapString.getExn(translatable[/* translations */1], language)[/* text */0];
}

function setTranslated(translatable, language, text) {
  return /* record */[
          /* sourceLanguage */translatable[/* sourceLanguage */0],
          /* translations */Belt_MapString.update(translatable[/* translations */1], language, (function (maybeVettable) {
                  if (maybeVettable !== undefined) {
                    var vettable = maybeVettable;
                    return /* record */[
                            /* text */text,
                            /* vetted */vettable[/* vetted */1],
                            /* authorID */vettable[/* authorID */2]
                          ];
                  } else {
                    return /* record */[
                            /* text */text,
                            /* vetted */false,
                            /* authorID */undefined
                          ];
                  }
                }))
        ];
}

function makeTranslatable(text, language) {
  return /* record */[
          /* sourceLanguage */language,
          /* translations */Belt_MapString.fromArray(/* array */[/* tuple */[
                  language,
                  /* record */[
                    /* text */text,
                    /* vetted */true,
                    /* authorID */undefined
                  ]
                ]])
        ];
}

var emptyTranslatable = makeTranslatable("", "en");

function getDisplayName(definition, language) {
  var text = getTranslated(definition[/* documentation */1][/* name */0], language);
  if (text !== "") {
    return text;
  } else {
    var match = definition[/* implementation */0];
    if (match.tag) {
      return "(nameless)";
    } else {
      return primitiveValueToString(match[0]);
    }
  }
}

function displayKeywordNibs(definition, language, isInputs) {
  return Belt_List.map(isInputs ? definition[/* display */2][/* inputOrdering */0] : definition[/* display */2][/* outputOrdering */1], (function (nibID) {
                var documentation = definition[/* documentation */1];
                var nibs = isInputs ? documentation[/* inputs */2] : documentation[/* outputs */3];
                var translatable = Belt_MapString.getExn(nibs, nibID);
                var vettable = Belt_MapString.getExn(translatable[/* translations */1], language);
                return /* record */[
                        /* name */vettable[/* text */0],
                        /* nib : NibConnection */Block.__(0, [nibID])
                      ];
              }));
}

function displayKeywordInputs(definition, language) {
  return displayKeywordNibs(definition, language, true);
}

function displayKeywordOutputs(definition, language) {
  return displayKeywordNibs(definition, language, false);
}

function displayDefinedNode(definition, kind, language) {
  switch (kind) {
    case 0 : 
        return /* record */[
                /* inputs */displayKeywordNibs(definition, language, true),
                /* outputs */displayKeywordNibs(definition, language, false)
              ];
    case 2 : 
        return /* record */[
                /* inputs : :: */[
                  /* record */[
                    /* name */"implementation",
                    /* nib : ValueConnection */0
                  ],
                  displayKeywordNibs(definition, language, true)
                ],
                /* outputs */displayKeywordNibs(definition, language, false)
              ];
    case 1 : 
    case 3 : 
        return /* record */[
                /* inputs : [] */0,
                /* outputs : :: */[
                  /* record */[
                    /* name */"",
                    /* nib : ValueConnection */0
                  ],
                  /* [] */0
                ]
              ];
    case 4 : 
        return /* record */[
                /* inputs */displayKeywordNibs(definition, language, true),
                /* outputs : :: */[
                  /* record */[
                    /* name */"",
                    /* nib : ValueConnection */0
                  ],
                  /* [] */0
                ]
              ];
    case 5 : 
        return /* record */[
                /* inputs : :: */[
                  /* record */[
                    /* name */"",
                    /* nib : ValueConnection */0
                  ],
                  /* [] */0
                ],
                /* outputs */displayKeywordNibs(definition, language, true)
              ];
    
  }
}

function displayNode(node, definitions, language) {
  if (typeof node === "number") {
    return /* record */[
            /* inputs : [] */0,
            /* outputs : :: */[
              /* record */[
                /* name */"Reference",
                /* nib : ValueConnection */0
              ],
              /* [] */0
            ]
          ];
  } else if (node.tag) {
    var match = node[0];
    var definition = Belt_MapString.getExn(definitions, match[/* definitionID */1]);
    return displayDefinedNode(definition, match[/* kind */0], language);
  } else {
    return /* record */[
            /* inputs */Belt_List.makeBy(node[0], (function (index) {
                    return /* record */[
                            /* name */String(index),
                            /* nib : PositionalConnection */Block.__(1, [index])
                          ];
                  })),
            /* outputs : :: */[
              /* record */[
                /* name */"",
                /* nib : ValueConnection */0
              ],
              /* [] */0
            ]
          ];
  }
}

function getNodeNibIndex(node, definitions, connectionNib, isSink) {
  var match = displayNode(node, definitions, "en");
  var inputs = match[/* inputs */0];
  var nibs = isSink ? inputs : match[/* outputs */1];
  var nibIndex = Helpers$ReactTemplate.findByIndexExn(nibs, (function (param) {
          return Caml_obj.caml_equal(param[/* nib */1], connectionNib);
        }));
  if (isSink) {
    return nibIndex;
  } else {
    return nibIndex + Belt_List.length(inputs) | 0;
  }
}

function getOutputIndex(node, definitions, connectionNib) {
  var match = displayNode(node, definitions, "en");
  return Helpers$ReactTemplate.findByIndexExn(match[/* outputs */1], (function (param) {
                return Caml_obj.caml_equal(param[/* nib */1], connectionNib);
              }));
}

function countNodeNibs(node, definitions) {
  if (typeof node === "number") {
    return 1;
  } else if (node.tag) {
    var match = node[0];
    var nodeDefinition = Belt_MapString.getExn(definitions, match[/* definitionID */1]);
    var nodeDisplay = displayDefinedNode(nodeDefinition, match[/* kind */0], "en");
    return Belt_List.length(nodeDisplay[/* inputs */0]) + Belt_List.length(nodeDisplay[/* outputs */1]) | 0;
  } else {
    return node[0];
  }
}

function displayDefinedType(definition, language) {
  var match = definition[/* implementation */0];
  var tmp;
  switch (match.tag | 0) {
    case 1 : 
        tmp = "Function";
        break;
    case 4 : 
        tmp = "Record";
        break;
    default:
      throw Caml_builtin_exceptions.not_found;
  }
  return getTranslated(definition[/* documentation */1][/* name */0], language) + (" " + tmp);
}

function displayPrimitiveValueType(primitiveValueType) {
  switch (primitiveValueType) {
    case 0 : 
        return "Integer";
    case 1 : 
        return "Number";
    case 2 : 
        return "Text";
    
  }
}

function displayValueType(valueType, definitions, language) {
  if (valueType.tag) {
    return displayDefinedType(Belt_MapString.getExn(definitions, valueType[0]), language);
  } else {
    return displayPrimitiveValueType(valueType[0]);
  }
}

var InvalidConnection = Caml_exceptions.create("Definition-ReactTemplate.InvalidConnection");

exports.connectionNodeToString = connectionNodeToString;
exports.connectionNibToString = connectionNibToString;
exports.connectionSideToString = connectionSideToString;
exports.ConnectionComparator = ConnectionComparator;
exports.definedNodeKindHasValueInput = definedNodeKindHasValueInput;
exports.definedNodeKindHasValueOutput = definedNodeKindHasValueOutput;
exports.primitiveValueTypes = primitiveValueTypes;
exports.primitiveValueTypeToString = primitiveValueTypeToString;
exports.stringToPrimitiveValueType = stringToPrimitiveValueType;
exports.changeTypedFields = changeTypedFields;
exports.primitiveValueToType = primitiveValueToType;
exports.primitiveValueToString = primitiveValueToString;
exports.primitiveValueToTypeString = primitiveValueToTypeString;
exports.getTranslated = getTranslated;
exports.setTranslated = setTranslated;
exports.makeTranslatable = makeTranslatable;
exports.emptyTranslatable = emptyTranslatable;
exports.getDisplayName = getDisplayName;
exports.displayKeywordNibs = displayKeywordNibs;
exports.displayKeywordInputs = displayKeywordInputs;
exports.displayKeywordOutputs = displayKeywordOutputs;
exports.displayDefinedNode = displayDefinedNode;
exports.displayNode = displayNode;
exports.getNodeNibIndex = getNodeNibIndex;
exports.getOutputIndex = getOutputIndex;
exports.countNodeNibs = countNodeNibs;
exports.displayDefinedType = displayDefinedType;
exports.displayPrimitiveValueType = displayPrimitiveValueType;
exports.displayValueType = displayValueType;
exports.InvalidConnection = InvalidConnection;
/* ConnectionComparator Not a pure module */
