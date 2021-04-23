module C = Belt.Id.MakeComparable({
  type t = PointerID.t
  let cmp = compare
})
