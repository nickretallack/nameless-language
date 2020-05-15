type t = Belt.Set.t(ValueType.t, ValueTypeComparable.C.identity);

let fromArray = Belt.Set.fromArray(~id=(module ValueTypeComparable.C));
