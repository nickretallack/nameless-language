module C = Belt.Id.MakeComparable({
  type t = ValueType.t
  let cmp = compare
})
