module C = Belt.Id.MakeComparable({
  type t = NodeScope.t
  let cmp = compare
})
