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

let connectionNodeToString = (connectionNode: connectionNode) =>
  switch (connectionNode) {
  | GraphConnection => "graph"
  | NodeConnection(nodeID) => "node-" ++ nodeID
  };

type connectionNib =
  | ValueConnection
  | NibConnection(nibID)
  | PositionalConnection(int);

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

/* Nodes */

type definedNodeKind =
  | FunctionCallNode
  | ValueNode
  | FunctionPointerCallNode
  | FunctionDefinitionNode
  | ConstructorNode
  | AccessorNode;

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

type nodeKind =
  | ReferenceNode
  | ListNode(int)
  | DefinedNode(definedNode);

type nodeScope =
  | GraphScope
  | NodeScope(nodeID);

module ScopeComparator =
  Belt.Id.MakeComparable({
    type t = nodeScope;
    let cmp = compare;
  });
type nodeScopes('a) = Belt.Map.t(nodeScope, 'a, ScopeComparator.identity);

type node = {
  scope: nodeScope,
  kind: nodeKind,
};

/*
 variant Yes
 variant No
 variant Maybe

 union type boolean = Yes | No
 union type trinary = Yes | No | Maybe


 foo : () => boolean
 bar : trinary => ()

 bar(foo())

 foo : () => trinary
 bar : boolean => ()

 bar(default(foo(), boolean, Yes));

 let rec fac n =
   n <= 1 ?
     1
     : n * (fac (n - 1))


 x = Yes : trinary
 type assert x : boolean

 variant Nothing
 variant Some(x)
 union type Maybe = Nothing | Some(x)

 union type Maybe = Nothing | x

 vairant Error(x)
 variant Success(x)
 union type Result = Error(a) | Success(b)

 x : Nothing -> Nothing

 AuthorA:
 type person1 = {
   name: string,
   age: integer,
 }


 AuthorB:
 foo1 : person1 -> ()

 AuthorA
 type person2 = {
   name: string,
   age: integer,
   thuesathueath
 };

 AuthorC
 foo2 : perosn2 -> ()


 Foo (foo1 <- foo2)
 Person (person1
   <- person2a -- vetted by the community
   <- person2b -- security exploit!
 )



 type employed inherits person {
   company: string,
 }

 type hasAddress inherits person {
   address: string,
 }

 type businessman inherits person = {
   company: string,
 }


 {
   name,
   age,
   company,
   address
 }

 (,,,)


 fun : {empoyed hasAddress} -> ()




 */

type nodes = Belt.Map.String.t(node);

type graphImplementation = {
  connections,
  nodes,
};

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
  | PublishingDefinedValueType(contentID);

type valueType =
  | PrimitiveValueType(primitiveValueType)
  | DefinedValueType(definitionID);

type typedFields = Belt.Map.String.t(valueType);

let changeTypedFields =
    (typedFields: typedFields, nibID: nibID, valueType: valueType) =>
  Belt.Map.String.set(typedFields, nibID, valueType);

type interface = {
  inputTypes: typedFields,
  outputTypes: typedFields,
};

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

/* type implementationType =
    | ConstantImplementationType(primitiveValue)
    | InterfaceImplementation(interface)
    | ExternalImplementation(externalImplementation)
    | GraphImplementation(graphImplementation)
    | RecordTypeImplementation(typedFields)
    | UnionTypeImplementation(typedFields);
   */

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
    | ConstantImplementation(value) => primitiveValueToString(value)
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
};

let displayDefinedNode =
    (definition: definition, kind: definedNodeKind, language: language)
    : displayNibs =>
  switch (kind) {
  | FunctionCallNode => {
      inputs: displayKeywordInputs(definition, language),
      outputs: displayKeywordOutputs(definition, language),
    }
  | ValueNode => {inputs: [], outputs: [{nib: ValueConnection, name: ""}]}
  | FunctionPointerCallNode => {
      inputs: [
        {nib: ValueConnection, name: "implementation"},
        ...displayKeywordInputs(definition, language),
      ],
      outputs: displayKeywordOutputs(definition, language),
    }
  | FunctionDefinitionNode => {
      inputs: [],
      outputs: [{nib: ValueConnection, name: ""}],
    }
  | ConstructorNode => {
      inputs: displayKeywordInputs(definition, language),
      outputs: [{nib: ValueConnection, name: ""}],
    }
  | AccessorNode => {
      inputs: [{nib: ValueConnection, name: ""}],
      outputs: displayKeywordInputs(definition, language),
    }
  };

let displayNode = (node: node, definitions: definitions, language: language) =>
  switch (node.kind) {
  | ReferenceNode => {
      outputs: [{nib: ValueConnection, name: "Reference"}],
      inputs: [],
    }
  | ListNode(length) => {
      outputs: [{nib: ValueConnection, name: ""}],
      inputs:
        Belt.List.makeBy(length, index =>
          {nib: PositionalConnection(index), name: string_of_int(index)}
        ),
    }
  | DefinedNode({kind, definitionID}) =>
    let definition = Belt.Map.String.getExn(definitions, definitionID);
    displayDefinedNode(definition, kind, language);
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
      true,
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
        nodes: Belt.Map.String.fromArray(nodes),
        connections:
          Belt.Map.fromArray(connections, ~id=(module ConnectionComparator)),
      }),
    (),
  );

exception InvalidConnection;

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
