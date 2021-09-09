type t = Belt.Set.t<NodeScope.t, NodeScopeComparable.C.identity>

let fromArray = (items: array<NodeScope.t>): t =>
  Belt.Set.fromArray(items, ~id=module(NodeScopeComparable.C))
