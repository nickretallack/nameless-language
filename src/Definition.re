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

type node =
  | ReferenceNode
  | ListNode(int)
  | DefinedNode(definedNode);

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

type definedValue = {
  definitionID,
  values: Belt.List.t(value),
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
  | UnionTypeImplementation(typedFields);

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

let getName = (definition: definition, language: language) =>
  Belt.Map.String.getExn(definition.documentation.name.translations, language).
    text;

let getDescription = (definition: definition, language: language) =>
  Belt.Map.String.getExn(
    definition.documentation.description.translations,
    language,
  ).
    text;

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
  switch (node) {
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

let getNodeNibIndex =
    (
      node: node,
      definitions: definitions,
      connectionNib: connectionNib,
      isSink: bool,
    ) => {
  let {inputs, outputs} = displayNode(node, definitions, "en");
  let nibs = isSink ? inputs : outputs;
  let nibIndex = findByIndexExn(nibs, ({nib}) => nib == connectionNib);
  isSink ? nibIndex : nibIndex + Belt.List.length(inputs);
};

let getOutputIndex =
    (node: node, definitions: definitions, connectionNib: connectionNib) => {
  let {outputs} = displayNode(node, definitions, "en");
  findByIndexExn(outputs, ({nib}) => nib == connectionNib);
};

let countNodeNibs = (node: node, definitions: definitions) =>
  switch (node) {
  | ReferenceNode => 1
  | ListNode(length) => length
  | DefinedNode({kind, definitionID}) =>
    let nodeDefinition = Belt.Map.String.getExn(definitions, definitionID);
    let nodeDisplay = displayDefinedNode(nodeDefinition, kind, "en");
    Belt.List.length(nodeDisplay.inputs)
    + Belt.List.length(nodeDisplay.outputs);
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
