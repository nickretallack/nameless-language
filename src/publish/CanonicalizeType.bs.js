// Generated by BUCKLESCRIPT VERSION 4.0.14, PLEASE EDIT WITH CARE
'use strict';

var Block = require("bs-platform/lib/js/block.js");
var Belt_List = require("bs-platform/lib/js/belt_List.js");
var Json_encode = require("@glennsl/bs-json/src/Json_encode.bs.js");
var Belt_MapString = require("bs-platform/lib/js/belt_MapString.js");

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

function encodeValueType(valueType) {
  if (typeof valueType === "number") {
    return Json_encode.object_(/* :: */[
                /* tuple */[
                  "type",
                  "any"
                ],
                /* [] */0
              ]);
  } else if (valueType.tag) {
    return Json_encode.object_(/* :: */[
                /* tuple */[
                  "type",
                  "defined"
                ],
                /* :: */[
                  /* tuple */[
                    "contentID",
                    valueType[0]
                  ],
                  /* [] */0
                ]
              ]);
  } else {
    return Json_encode.object_(/* :: */[
                /* tuple */[
                  "type",
                  "primitive"
                ],
                /* :: */[
                  /* tuple */[
                    "primitiveType",
                    primitiveValueTypeToString(valueType[0])
                  ],
                  /* [] */0
                ]
              ]);
  }
}

function encodeTypedFields(fields) {
  return Json_encode.list(encodeValueType, fields);
}

function canonicalizeType(valueType, dependencies) {
  if (typeof valueType === "number") {
    return /* PublishingAnyType */0;
  } else if (valueType.tag) {
    return /* PublishingDefinedValueType */Block.__(1, [Belt_MapString.getExn(dependencies, valueType[0])[/* contentID */0]]);
  } else {
    return /* PublishingPrimitiveValueType */Block.__(0, [valueType[0]]);
  }
}

function canonicalizeTypedFields(typedFields, dependencies, fieldOrdering) {
  return Belt_List.map(fieldOrdering, (function (nibID) {
                return canonicalizeType(Belt_MapString.getExn(typedFields, nibID), dependencies);
              }));
}

function encodeRecordType(fields) {
  return Json_encode.object_(/* :: */[
              /* tuple */[
                "type",
                "record"
              ],
              /* :: */[
                /* tuple */[
                  "fields",
                  Json_encode.list(encodeValueType, fields)
                ],
                /* [] */0
              ]
            ]);
}

function encodeUnionType(fields) {
  return Json_encode.object_(/* :: */[
              /* tuple */[
                "type",
                "union"
              ],
              /* :: */[
                /* tuple */[
                  "types",
                  Json_encode.list(encodeValueType, fields)
                ],
                /* [] */0
              ]
            ]);
}

function encodeLabeledType(id, valueType) {
  return Json_encode.object_(/* :: */[
              /* tuple */[
                "type",
                "label"
              ],
              /* :: */[
                /* tuple */[
                  "id",
                  id
                ],
                /* :: */[
                  /* tuple */[
                    "type",
                    encodeValueType(valueType)
                  ],
                  /* [] */0
                ]
              ]
            ]);
}

function encodeCanonicalRecordType(typedFields, dependencies, fieldOrdering) {
  return encodeRecordType(canonicalizeTypedFields(typedFields, dependencies, fieldOrdering));
}

function encodeCanonicalUnionType(typedFields, dependencies, fieldOrdering) {
  return encodeUnionType(canonicalizeTypedFields(typedFields, dependencies, fieldOrdering));
}

function encodeCanonicalLabeledType(id, valueType, dependencies) {
  return encodeLabeledType(id, canonicalizeType(valueType, dependencies));
}

function canonicalizeInterface($$interface, dependencies, display) {
  return /* record */[
          /* inputs */canonicalizeTypedFields($$interface[/* inputTypes */0], dependencies, display[/* inputOrdering */0]),
          /* outputs */canonicalizeTypedFields($$interface[/* outputTypes */1], dependencies, display[/* outputOrdering */1])
        ];
}

function encodeInterface($$interface) {
  return Json_encode.object_(/* :: */[
              /* tuple */[
                "type",
                "interface"
              ],
              /* :: */[
                /* tuple */[
                  "inputs",
                  Json_encode.list(encodeValueType, $$interface[/* inputs */0])
                ],
                /* :: */[
                  /* tuple */[
                    "outputs",
                    Json_encode.list(encodeValueType, $$interface[/* outputs */1])
                  ],
                  /* [] */0
                ]
              ]
            ]);
}

function encodeCanonicalInterface($$interface, dependencies, display) {
  return encodeInterface(canonicalizeInterface($$interface, dependencies, display));
}

function encodeExternal(publishingExternal) {
  return Json_encode.object_(/* :: */[
              /* tuple */[
                "type",
                "interface"
              ],
              /* :: */[
                /* tuple */[
                  "inputs",
                  Json_encode.list(encodeValueType, publishingExternal[/* interface */1][/* inputs */0])
                ],
                /* :: */[
                  /* tuple */[
                    "outputs",
                    Json_encode.list(encodeValueType, publishingExternal[/* interface */1][/* outputs */1])
                  ],
                  /* [] */0
                ]
              ]
            ]);
}

function encodeCanonicalExternal(externalImplementation, dependencies, display) {
  return encodeExternal(/* record */[
              /* name */externalImplementation[/* name */0],
              /* interface */canonicalizeInterface(externalImplementation[/* interface */1], dependencies, display)
            ]);
}

exports.primitiveValueTypeToString = primitiveValueTypeToString;
exports.encodeValueType = encodeValueType;
exports.encodeTypedFields = encodeTypedFields;
exports.canonicalizeType = canonicalizeType;
exports.canonicalizeTypedFields = canonicalizeTypedFields;
exports.encodeRecordType = encodeRecordType;
exports.encodeUnionType = encodeUnionType;
exports.encodeLabeledType = encodeLabeledType;
exports.encodeCanonicalRecordType = encodeCanonicalRecordType;
exports.encodeCanonicalUnionType = encodeCanonicalUnionType;
exports.encodeCanonicalLabeledType = encodeCanonicalLabeledType;
exports.canonicalizeInterface = canonicalizeInterface;
exports.encodeInterface = encodeInterface;
exports.encodeCanonicalInterface = encodeCanonicalInterface;
exports.encodeExternal = encodeExternal;
exports.encodeCanonicalExternal = encodeCanonicalExternal;
/* No side effect */
