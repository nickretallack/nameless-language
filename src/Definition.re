[%%debugger.chrome];
open Helpers;

type nibID = string;
type nodeID = string;
type definitionID = string;
type contentID = string;
type language = string;

/* Misc */

/* Connections */

type connectionNode =
  | GraphConnection
  | NodeConnection(nodeID);

let encodeConnectionNode = (connectionNode: connectionNode) =>
  Json.Encode.(
    object_(
      switch (connectionNode) {
      | GraphConnection => [("type", string("graph"))]
      | NodeConnection(nodeID) => [
          ("type", string("node")),
          ("nodeID", string(nodeID)),
        ]
      },
    )
  );
let connectionNodeToString = (connectionNode: connectionNode) =>
  switch (connectionNode) {
  | GraphConnection => "graph"
  | NodeConnection(nodeID) => "node-" ++ nodeID
  };

type connectionNib =
  | ValueConnection
  | NibConnection(nibID)
  | PositionalConnection(int);

let encodeConnectionNib = (connectionNib: connectionNib) =>
  Json.Encode.(
    object_(
      switch (connectionNib) {
      | ValueConnection => [("type", string("value"))]
      | NibConnection(nibID) => [
          ("type", string("nib")),
          ("nibID", string(nibID)),
        ]
      | PositionalConnection(index) => [
          ("type", string("positional")),
          ("index", int(index)),
        ]
      },
    )
  );

let connectionNibToString = (connectionNib: connectionNib) =>
  switch (connectionNib) {
  | ValueConnection => "value"
  | NibConnection(nibID) => "nib-" ++ nibID
  | PositionalConnection(index) => "index-" ++ string_of_int(index)
  };

type connectionSide = {
  node: connectionNode,
  nib: connectionNib,
};

let encodeConnectionSide = (connectionSide: connectionSide) =>
  Json.Encode.(
    object_([
      ("node", encodeConnectionNode(connectionSide.node)),
      ("nib", encodeConnectionNib(connectionSide.nib)),
    ])
  );

type explicitConnectionSide = {
  connectionSide,
  isSource: bool,
};

let connectionSideToString = (connectionSide: connectionSide) =>
  connectionNodeToString(connectionSide.node)
  ++ "-"
  ++ connectionNibToString(connectionSide.nib);

module ConnectionComparator =
  Belt.Id.MakeComparable({
    type t = connectionSide;
    let cmp = compare;
  });
type connections =
  Belt.Map.t(connectionSide, connectionSide, ConnectionComparator.identity);

let nibKey = (connectionNib: connectionNib) => {
  switch (connectionNib) {
  | ValueConnection => "value"
  | NibConnection(nibID) => nibID
  | PositionalConnection(int) => string_of_int(int)
  };
};

let nodeKey = (connectionNode: connectionNode) => {
  switch (connectionNode) {
  | GraphConnection => "graph"
  | NodeConnection(nodeID) => nodeID
  };
};

let explicitConnectionSideKey =
    (explicitConnectionSide: explicitConnectionSide) =>
  Printf.sprintf(
    "nib-%s-%s-%s",
    nodeKey(explicitConnectionSide.connectionSide.node),
    nibKey(explicitConnectionSide.connectionSide.nib),
    explicitConnectionSide.isSource ? "source" : "sink",
  );

/* Nodes */

type definedNodeKind =
  | FunctionCallNode
  | ValueNode
  | FunctionPointerCallNode
  | FunctionDefinitionNode
  | ConstructorNode
  | AccessorNode;

let definedNodeKindToString = (kind: definedNodeKind) =>
  switch (kind) {
  | ValueNode => "value"
  | FunctionCallNode => "function call"
  | FunctionPointerCallNode => "function pointer call"
  | FunctionDefinitionNode => "function definition"
  | ConstructorNode => "constructor"
  | AccessorNode => "accessor"
  };

let definedNodeKindHasValueInput = (kind: definedNodeKind): bool =>
  switch (kind) {
  | FunctionPointerCallNode => true
  | AccessorNode => true
  | _ => false
  };

let definedNodeKindHasValueOutput = (kind: definedNodeKind): bool =>
  switch (kind) {
  | ValueNode => true
  | FunctionDefinitionNode => true
  | ConstructorNode => true
  | _ => false
  };

type definedNode = {
  kind: definedNodeKind,
  definitionID,
};

let encodeDefinedNode = (definedNode: definedNode) =>
  Json.Encode.(
    object_([
      ("type", string("defined")),
      ("definitionID", string(definedNode.definitionID)),
      ("kind", string(definedNodeKindToString(definedNode.kind))),
    ])
  );

type nodeKind =
  | ReferenceNode
  | ListNode(int)
  | DefinedNode(definedNode);

let encodeNodeKind = (nodeKind: nodeKind) =>
  Json.Encode.(
    switch (nodeKind) {
    | ReferenceNode => object_([("type", string("reference"))])
    | ListNode(length) =>
      object_([("type", string("list")), ("length", int(length))])
    | DefinedNode(definedNode) => encodeDefinedNode(definedNode)
    }
  );

type nodeScope =
  | GraphScope
  | NodeScope(nodeID);

let encodeNodeScope = (nodeScope: nodeScope) =>
  Json.Encode.(
    object_(
      switch (nodeScope) {
      | GraphScope => [("type", string("graph"))]
      | NodeScope(nodeID) => [
          ("type", string("node")),
          ("nodeID", string(nodeID)),
        ]
      },
    )
  );

module ScopeComparator =
  Belt.Id.MakeComparable({
    type t = nodeScope;
    let cmp = compare;
  });
type nodeScopes('a) = Belt.Map.t(nodeScope, 'a, ScopeComparator.identity);
type nodeScopeSet = Belt.Set.t(nodeScope, ScopeComparator.identity);

type node = {
  scope: nodeScope,
  kind: nodeKind,
};

let encodeNode = (node: node) =>
  Json.Encode.(
    object_([
      ("scope", encodeNodeScope(node.scope)),
      ("kind", encodeNodeKind(node.kind)),
    ])
  );

type nodeWithID = {
  id: nodeID,
  node,
};

let isFunctionDefinitionNode = (node: node) =>
  switch (node.kind) {
  | DefinedNode({kind: FunctionDefinitionNode}) => true
  | _ => false
  };

let isValueNib = (nib: connectionNib) =>
  switch (nib) {
  | ValueConnection => true
  | _ => false
  };

let isKeywordNib = (nib: connectionNib) =>
  switch (nib) {
  | NibConnection(_) => true
  | _ => false
  };

type nodes = Belt.Map.String.t(node);

/* Interface */

type primitiveValueType =
  | IntegerType
  | NumberType
  | TextType;

let primitiveValueTypes = [|TextType, NumberType|];

let primitiveValueTypeToString =
    (primitiveValueType: primitiveValueType): string =>
  switch (primitiveValueType) {
  | IntegerType => "integer"
  | NumberType => "number"
  | TextType => "text"
  };

let stringToPrimitiveValueType = (string: string): primitiveValueType =>
  switch (string) {
  | "integer" => IntegerType
  | "number" => NumberType
  | "text" => TextType
  | _ => raise(Not_found)
  };

type boolean =
  | True
  | False;

/* type definedValueKind =
   | RecordType
   | UnionType
   | FunctionPointerType; */

type publishingValueType =
  | PublishingPrimitiveValueType(primitiveValueType)
  | PublishingDefinedValueType(contentID)
  | PublishingAnyType;

type valueType =
  | PrimitiveValueType(primitiveValueType)
  | DefinedValueType(definitionID)
  | AnyType;

type typedFields = Belt.Map.String.t(valueType);

let changeTypedFields =
    (typedFields: typedFields, nibID: nibID, valueType: valueType) =>
  Belt.Map.String.set(typedFields, nibID, valueType);

type interface = {
  inputTypes: typedFields,
  outputTypes: typedFields,
};

let changeInterface =
    (interface: interface, isInput: bool, nibID: nibID, valueType: valueType) =>
  isInput ?
    {
      ...interface,
      inputTypes: changeTypedFields(interface.inputTypes, nibID, valueType),
    } :
    {
      ...interface,
      outputTypes: changeTypedFields(interface.outputTypes, nibID, valueType),
    };

type graphImplementation = {
  interface,
  connections,
  nodes,
};

let encodeMap = (map, encode) =>
  Json.Encode.object_(
    Belt.List.map(Belt.Map.String.toList(map), ((key, item)) =>
      (key, encode(item))
    ),
  );

let encodeGraphImplementation = (graphImplementation: graphImplementation) =>
  Json.Encode.(
    object_([
      ("nodes", encodeMap(graphImplementation.nodes, encodeNode)),
      (
        "connections",
        list(
          ((sink, source)) =>
            object_([
              ("sink", encodeConnectionSide(sink)),
              ("source", encodeConnectionSide(source)),
            ]),
          Belt.Map.toList(graphImplementation.connections),
        ),
      ),
    ])
  );

type publishingInterface = {
  inputs: Belt.List.t(publishingValueType),
  outputs: Belt.List.t(publishingValueType),
};

type externalImplementation = {
  name: string,
  interface,
};

type publishingExternal = {
  name: string,
  interface: publishingInterface,
};

/* Value */

type primitiveValue =
  | IntegerValue(int)
  | NumberValue(float)
  | TextValue(string);

let primitiveValueToType =
    (primitiveValue: primitiveValue): primitiveValueType =>
  switch (primitiveValue) {
  | IntegerValue(_) => IntegerType
  | NumberValue(_) => NumberType
  | TextValue(_) => TextType
  };

let primitiveValueToString = (primitiveValue: primitiveValue) =>
  switch (primitiveValue) {
  | IntegerValue(value) => string_of_int(value)
  | NumberValue(value) => floatToString(value)
  | TextValue(value) => value
  };

let displayPrimitiveValue = (primitiveValue: primitiveValue) =>
  switch (primitiveValue) {
  | TextValue(value) => "\"" ++ value ++ "\""
  | _ => primitiveValueToString(primitiveValue)
  };

let primitiveValueToTypeString = x =>
  x |> primitiveValueToType |> primitiveValueTypeToString;

type definedValue = {
  definitionID,
  values: Belt.List.t(unit => value),
}
and value =
  | PrimitiveValue(primitiveValue)
  | DefinedValue(definedValue);

/* Implementation */

type implementation =
  | ConstantImplementation(primitiveValue)
  | InterfaceImplementation(interface)
  | ExternalImplementation(externalImplementation)
  | GraphImplementation(graphImplementation)
  | RecordTypeImplementation(typedFields)
  | LabeledTypeImplementation(valueType)
  | UnionTypeImplementation(typedFields);

let implementationName = (implementation: implementation): string =>
  switch (implementation) {
  | ConstantImplementation(_) => "constant"
  | InterfaceImplementation(_) => "interface"
  | ExternalImplementation(_) => "external"
  | GraphImplementation(_) => "function"
  | RecordTypeImplementation(_) => "record type"
  | LabeledTypeImplementation(_) => "labeled type"
  | UnionTypeImplementation(_) => "union type"
  };

/* Documentation */

type vettable = {
  text: string,
  vetted: bool,
  authorID: option(string),
};

type translatable = {
  sourceLanguage: language,
  translations: Belt.Map.String.t(vettable),
};

let getTranslated = (translatable: translatable, language: language) =>
  Belt.Map.String.getExn(translatable.translations, language).text;

let setTranslated =
    (translatable: translatable, language: language, text: string) => {
  ...translatable,
  translations:
    Belt.Map.String.update(
      translatable.translations, language, (maybeVettable: option(vettable)) =>
      switch (maybeVettable) {
      | Some(vettable) => Some({...vettable, text})
      | None => Some({text, vetted: false, authorID: None})
      }
    ),
};

let makeTranslatable = (text: string, language: language) => {
  sourceLanguage: language,
  translations:
    Belt.Map.String.fromArray([|
      (language, {text, vetted: true, authorID: None}),
    |]),
};

let emptyTranslatable = makeTranslatable("", "en");

type documentation = {
  name: translatable,
  description: translatable,
  inputs: Belt.Map.String.t(translatable),
  outputs: Belt.Map.String.t(translatable),
};

/* Definition */

type display = {
  inputOrdering: Belt.List.t(string),
  outputOrdering: Belt.List.t(string),
};

type definition = {
  implementation,
  documentation,
  display,
};

type definitions = Belt.Map.String.t(definition);

let getDisplayName = (definition: definition, language: language) => {
  let text = getTranslated(definition.documentation.name, language);
  if (text != "") {
    text;
  } else {
    switch (definition.implementation) {
    | ConstantImplementation(value) => displayPrimitiveValue(value)
    | _ =>
      "(nameless " ++ implementationName(definition.implementation) ++ ")"
    };
  };
};

type displayNib = {
  name: string,
  nib: connectionNib,
};

let displayKeywordNibs =
    (definition: definition, language: language, isInputs: bool)
    : list(displayNib) =>
  Belt.List.map(
    isInputs ?
      definition.display.inputOrdering : definition.display.outputOrdering,
    nibID => {
      let documentation = definition.documentation;
      let nibs = isInputs ? documentation.inputs : documentation.outputs;
      let translatable = Belt.Map.String.getExn(nibs, nibID);
      let vettable =
        Belt.Map.String.getExn(translatable.translations, language);
      {nib: NibConnection(nibID), name: vettable.text};
    },
  );

let displayKeywordInputs = (definition: definition, language: language) =>
  displayKeywordNibs(definition, language, true);

let displayKeywordOutputs = (definition: definition, language: language) =>
  displayKeywordNibs(definition, language, false);

type displayNibs = {
  inputs: list(displayNib),
  outputs: list(displayNib),
  internalInputs: list(displayNib),
  internalOutputs: list(displayNib),
};

let makeDisplayNibs =
    (
      ~inputs=[],
      ~outputs=[],
      ~internalInputs=[],
      ~internalOutputs=[],
      _unit: unit,
    ) => {
  inputs,
  outputs,
  internalInputs,
  internalOutputs,
};

let displayDefinedNode =
    (definition: definition, kind: definedNodeKind, language: language)
    : displayNibs =>
  switch (kind) {
  | FunctionCallNode =>
    makeDisplayNibs(
      ~inputs=displayKeywordInputs(definition, language),
      ~outputs=displayKeywordOutputs(definition, language),
      (),
    )
  | ValueNode =>
    makeDisplayNibs(~outputs=[{nib: ValueConnection, name: ""}], ())
  | FunctionPointerCallNode =>
    makeDisplayNibs(
      ~inputs=[
        {nib: ValueConnection, name: "implementation"},
        ...displayKeywordInputs(definition, language),
      ],
      ~outputs=displayKeywordOutputs(definition, language),
      (),
    )
  | FunctionDefinitionNode =>
    makeDisplayNibs(
      ~outputs=[{nib: ValueConnection, name: ""}],
      ~internalInputs=displayKeywordInputs(definition, language),
      ~internalOutputs=displayKeywordOutputs(definition, language),
      (),
    )
  | ConstructorNode =>
    makeDisplayNibs(
      ~inputs=displayKeywordInputs(definition, language),
      ~outputs=[{nib: ValueConnection, name: ""}],
      (),
    )
  | AccessorNode =>
    makeDisplayNibs(
      ~inputs=[{nib: ValueConnection, name: ""}],
      ~outputs=displayKeywordInputs(definition, language),
      (),
    )
  };

let displayNode =
    (node: node, definitions: definitions, language: language): displayNibs =>
  switch (node.kind) {
  | ReferenceNode =>
    makeDisplayNibs(
      ~outputs=[{nib: ValueConnection, name: "Reference"}],
      (),
    )
  | ListNode(length) =>
    makeDisplayNibs(
      ~outputs=[{nib: ValueConnection, name: ""}],
      ~inputs=
        Belt.List.makeBy(length, index =>
          {nib: PositionalConnection(index), name: string_of_int(index)}
        ),
      (),
    )
  | DefinedNode({kind, definitionID}) =>
    let definition = Belt.Map.String.getExn(definitions, definitionID);
    displayDefinedNode(definition, kind, language);
  };

type explicitDisplayNib = {
  name: string,
  explicitConnectionSide,
};

let displayNibsToExplicitConnectionSides =
    (displayNibs: list(displayNib), node: connectionNode, isSource: bool)
    : list(explicitDisplayNib) =>
  Belt.List.map(displayNibs, (displayNib: displayNib) =>
    {
      name: displayNib.name,
      explicitConnectionSide: {
        connectionSide: {
          node,
          nib: displayNib.nib,
        },
        isSource,
      },
    }
  );

let collectGraphNodeNibs =
    (nodes, definitions: definitions): list(explicitDisplayNib) =>
  Belt.List.reduce(
    Belt.Map.String.toList(nodes),
    [],
    (acc, (nodeID, node)) => {
      let {inputs, outputs, internalInputs, internalOutputs} =
        displayNode(node, definitions, "en");
      let connectionNode = NodeConnection(nodeID);
      Belt.List.concatMany([|
        acc,
        displayNibsToExplicitConnectionSides(inputs, connectionNode, false),
        displayNibsToExplicitConnectionSides(outputs, connectionNode, true),
        displayNibsToExplicitConnectionSides(
          internalInputs,
          connectionNode,
          true,
        ),
        displayNibsToExplicitConnectionSides(
          internalOutputs,
          connectionNode,
          false,
        ),
      |]);
    },
  );

let collectAllGraphNibs =
    (definition: definition, definitions: definitions)
    : list(explicitDisplayNib) => {
  switch (definition.implementation) {
  | GraphImplementation(graphImplementation) =>
    Belt.List.concatMany([|
      displayNibsToExplicitConnectionSides(
        displayKeywordOutputs(definition, "en"),
        GraphConnection,
        false,
      ),
      displayNibsToExplicitConnectionSides(
        displayKeywordInputs(definition, "en"),
        GraphConnection,
        true,
      ),
      collectGraphNodeNibs(graphImplementation.nodes, definitions),
    |])
  | _ => raise(Not_found)
  };
};

let functionDefinitionNibIndex =
    (definition: definition, connectionNib: connectionNib, isSink: bool) =>
  switch (connectionNib) {
  | ValueConnection => 0
  | PositionalConnection(_) => raise(Not_found)
  | NibConnection(_) =>
    let nibs = displayKeywordNibs(definition, "en", !isSink);
    findByIndexExn(nibs, ({nib}) => nib == connectionNib);
  };

let getNodeNibIndex =
    (
      node: node,
      definitions: definitions,
      connectionNib: connectionNib,
      isSink: bool,
    ) => {
  switch (node.kind) {
  | DefinedNode({definitionID, kind: FunctionDefinitionNode}) =>
    functionDefinitionNibIndex(
      Belt.Map.String.getExn(definitions, definitionID),
      connectionNib,
      isSink,
    )
    + 1
  | _ =>
    let {inputs, outputs} = displayNode(node, definitions, "en");
    let nibs = isSink ? inputs : outputs;
    let nibIndex = findByIndexExn(nibs, ({nib}) => nib == connectionNib);
    (isSink ? nibIndex : nibIndex + Belt.List.length(inputs)) + 1;
  };
};

let getOutputIndex =
    (node: node, definitions: definitions, connectionNib: connectionNib) => {
  switch (node.kind) {
  | DefinedNode({definitionID, kind: FunctionDefinitionNode}) =>
    functionDefinitionNibIndex(
      Belt.Map.String.getExn(definitions, definitionID),
      connectionNib,
      false,
    )
  | _ =>
    let {outputs} = displayNode(node, definitions, "en");
    findByIndexExn(outputs, ({nib}) => nib == connectionNib);
  };
};

let countNodeNibs = (node: node, definitions: definitions) =>
  switch (node.kind) {
  | ReferenceNode => 1
  | ListNode(length) => length
  | DefinedNode({kind, definitionID}) =>
    let nodeDefinition = Belt.Map.String.getExn(definitions, definitionID);
    let nodeDisplay = displayDefinedNode(nodeDefinition, kind, "en");
    Belt.List.length(nodeDisplay.inputs)
    + Belt.List.length(nodeDisplay.outputs);
  };

let displayDefinedType = (definition: definition, language: language): string =>
  getTranslated(definition.documentation.name, language)
  ++ " "
  ++ (
    switch (definition.implementation) {
    | RecordTypeImplementation(_) => "Record"
    | InterfaceImplementation(_) => "Function"
    | _ => raise(Not_found)
    }
  );

let displayPrimitiveValueType =
    (primitiveValueType: primitiveValueType): string =>
  switch (primitiveValueType) {
  | IntegerType => "Integer"
  | NumberType => "Number"
  | TextType => "Text"
  };

let displayValueType =
    (valueType: valueType, definitions: definitions, language: language)
    : string =>
  switch (valueType) {
  | PrimitiveValueType(primitiveValueType) =>
    displayPrimitiveValueType(primitiveValueType)
  | DefinedValueType(definitionID) =>
    displayDefinedType(
      Belt.Map.String.getExn(definitions, definitionID),
      language,
    )
  | AnyType => "Any"
  };

/* let getNibIndex =
     (
       node: node,
       definitions: definitions,
       connectionNib: connectionNib,
       isSink: bool,
     ) =>
   switch (node) {
   | ReferenceNode => (-1)
   | ListNode(length) =>
     switch(connectionNib) {
       | ValueConnection
       | NibConnection(nibID)
       | PositionalConnection(int);
     }
   | DefinedNode({kind, definitionID}) =>
     let nodeDefinition = Belt.Map.String.getExn(definitions, definitionID);
     let nodeDisplay = displayDefinedNode(nodeDefinition, kind, "en");
     Belt.List.length(nodeDisplay.inputs)
     + Belt.List.length(nodeDisplay.outputs);
   }; */

let makeNibDocs = (nibs: array((nibID, string))) =>
  Belt.Map.String.fromArray(
    Belt.Array.map(nibs, ((nibID, text)) =>
      (nibID, makeTranslatable(text, "en"))
    ),
  );

let makeDefinition =
    (
      ~name="",
      ~description="",
      ~inputs=[||],
      ~outputs=[||],
      ~implementation,
      _unit,
    ) => {
  documentation: {
    name: makeTranslatable(name, "en"),
    description: makeTranslatable(description, "en"),
    inputs: makeNibDocs(inputs),
    outputs: makeNibDocs(outputs),
  },
  display: {
    inputOrdering: Array.to_list(Array.map(((id, _name)) => id, inputs)),
    outputOrdering: Array.to_list(Array.map(((id, _name)) => id, outputs)),
  },
  implementation,
};

let makeGraph =
    (
      ~name="",
      ~description="",
      ~inputs=[||],
      ~outputs=[||],
      ~nodes=[||],
      ~connections=[||],
      _unit,
    ) =>
  makeDefinition(
    ~name,
    ~description,
    ~inputs,
    ~outputs,
    ~implementation=
      GraphImplementation({
        interface: {
          inputTypes:
            Belt.Map.String.fromArray(
              Belt.Array.map(inputs, ((id, _name)) => (id, AnyType)),
            ),
          outputTypes:
            Belt.Map.String.fromArray(
              Belt.Array.map(outputs, ((id, _name)) => (id, AnyType)),
            ),
        },
        nodes: Belt.Map.String.fromArray(nodes),
        connections:
          Belt.Map.fromArray(connections, ~id=(module ConnectionComparator)),
      }),
    (),
  );

exception InvalidConnection;
exception CycleDetected;

/* Publishing.
   These types are used ephemerally during publishing.  They won't be retained in the state.
   */

type publishingDependency = {
  contentID,
  inputOrdering: Belt.List.t(string),
  outputOrdering: Belt.List.t(string),
};

type publishingDependencies = Belt.Map.String.t(publishingDependency);

type publishingDefinedNode = {
  kind: definedNodeKind,
  contentID,
};

type publishingNode =
  | PublishingReferenceNode
  | PublishingListNode(int)
  | PublishingDefinedNode(publishingDefinedNode);

type publishingConnectionNode =
  | PublishingGraphConnection
  | PublishingNodeConnection(int);

type publishingConnectionNib =
  | PublishingValueConnection
  | PublishingNibConnection(int);

type publishingConnectionSide = {
  node: publishingConnectionNode,
  nib: publishingConnectionNib,
};

type publishingConnection = {
  source: publishingConnectionSide,
  sink: publishingConnectionSide,
};

type publishingGraphImplementation = {
  nodes: Belt.List.t(publishingNode),
  connections: Belt.List.t(publishingConnection),
  inputCount: int,
  outputCount: int,
};
