type t = Belt.Set.t<ConnectionSide.t, ConnectionSideComparable.C.identity>

let fromArray = array => Belt.Set.fromArray(array, ~id=module(ConnectionSideComparable.C))
