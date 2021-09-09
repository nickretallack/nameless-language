type t = Belt.Map.t<ConnectionSide.t, ConnectionSideSet.t, ConnectionSideComparable.C.identity>

let fromArray = array => Belt.Map.fromArray(array, ~id=module(ConnectionSideComparable.C))
