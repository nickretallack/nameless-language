type nibID = string;
type nodeID = string;
type definitionID = string;
type contentID = string;
type language = string;

/* Connections */
type connectionNode =
  | GraphConnection
  | NodeConnection(nodeID);

type connectionNib =
  | ValueConnection
  | NibConnection(nibID);

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

type point = {
  x: int,
  y: int,
};

/* type Implementation
   = ConstantImplementation Value
   | ExternalImplementation Interface String
   | GraphImplementation GraphImplementationRecord
   | InterfaceImplementation Interface
   | RecordTypeImplementation TypeImplementation
   | UnionTypeImplementation TypeImplementation */
