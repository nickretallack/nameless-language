type nibID = string;
type nodeID = string;
type definitionID = string;
type contentID = string;
type language = string;

/* Misc */

type point = {
  x: int,
  y: int,
};

/* Connections */

type connectionNode =
  | GraphConnection
  | NodeConnection(nodeID);

type connectionNib =
  | ValueConnection
  | NibConnection(nibID)
  | PositionalConnection(int);

type connectionSide = {
  node: connectionNode,
  nib: connectionNib,
};

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

let definedNodeKindHasValueInput = (kind: definedNodeKind) : bool =>
  switch (kind) {
  | FunctionPointerCallNode => true
  | AccessorNode => true
  | _ => false
  };

let definedNodeKindHasValueOutput = (kind: definedNodeKind) : bool =>
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

type externalImplementation = {
  name: string,
  interface,
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
